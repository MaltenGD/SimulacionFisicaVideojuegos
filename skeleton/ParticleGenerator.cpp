#include "ParticleGenerator.h"
#include "Particle.h"
#include <utility>
#include <cmath>

namespace
{
	float SampleChannel(float a, float b, float t)
	{
		const float lo = std::fmin(a, b);
		const float hi = std::fmax(a, b);
		return lo + (hi - lo) * t;
	}
}

ParticleGenerator::ParticleGenerator(std::string name, Vector3D origin, int particlesPerSpawn, double spawnInterval)
	: m_name(std::move(name)),
	m_origin(origin),
	m_particlesPerSpawn(particlesPerSpawn > 0 ? particlesPerSpawn : 1),
	m_spawnInterval(spawnInterval >= 0.001 ? spawnInterval : 0.001)
{
	m_colorMin = m_color;
	m_colorMax = m_color;

	m_model = std::make_unique<Particle>(1.0f, Vector3D::zero(), Vector3D::zero(), Vector3D::zero(),
		0.99f, 0.3f, m_color, 5.0f);
	m_model->deactivate();
}

Particle* ParticleGenerator::spawnFromModel(const Vector3D& pos, const Vector3D& vel, Particle* reusable)
{
	const Vector4 color = sampleColor();

	if (reusable)
	{
		reusable->reset(m_model->getMass(), pos, vel, m_model->getAcceleration(),
			m_model->getDamping(), m_model->getRadius(), color, m_model->getMaxLifetime());
		reusable->setScaleWithLifetime(m_scaleWithLifetime);
		return reusable;
	}

	Particle* copy = m_model->clone(pos, vel);
	copy->setColor(color);
	copy->setScaleWithLifetime(m_scaleWithLifetime);
	return copy;
}

void ParticleGenerator::trimPool()
{
	if (m_particlePool.size() <= m_maxParticles)
	{
		return;
	}

	auto it = m_particlePool.begin();
	while (it != m_particlePool.end() && m_particlePool.size() > m_maxParticles)
	{
		if (!(*it)->isAlive())
		{
			it = m_particlePool.erase(it);
		}
		else
		{
			++it;
		}
	}
}

Vector4 ParticleGenerator::sampleColor()
{
	if (m_colorMode == ParticleColorMode::SINGLE)
	{
		return m_color;
	}

	return Vector4(
		SampleChannel(m_colorMin.x, m_colorMax.x, m_colorUnit(m_colorGen)),
		SampleChannel(m_colorMin.y, m_colorMax.y, m_colorUnit(m_colorGen)),
		SampleChannel(m_colorMin.z, m_colorMax.z, m_colorUnit(m_colorGen)),
		SampleChannel(m_colorMin.w, m_colorMax.w, m_colorUnit(m_colorGen))
	);
}

Particle* ParticleGenerator::acquireParticle()
{
	if (m_particlePool.size() > m_maxParticles)
	{
		return nullptr;
	}

	for (const auto& particle : m_particlePool)
	{
		if (!particle->isAlive())
		{
			return generateSingleParticle(particle.get());
		}
	}

	if (m_particlePool.size() >= m_maxParticles)
	{
		return nullptr;
	}

	Particle* particle = generateSingleParticle();
	if (particle)
	{
		m_particlePool.emplace_back(particle);
	}
	return particle;
}

void ParticleGenerator::setMaxParticles(size_t maxParticles)
{
	m_maxParticles = maxParticles > 0 ? maxParticles : 1;
	trimPool();

	if (m_maxParticles > m_particlePool.capacity())
	{
		m_particlePool.reserve(m_maxParticles);
	}
}

std::vector<Particle*> ParticleGenerator::updateAndGenerate(double dt)
{
	trimPool();

	std::vector<Particle*> generated;
	if (!m_active)
	{
		return generated;
	}

	m_timeSinceLastSpawn += dt;
	while (m_timeSinceLastSpawn >= m_spawnInterval)
	{
		m_timeSinceLastSpawn -= m_spawnInterval;
		for (int i = 0; i < m_particlesPerSpawn; ++i)
		{
			Particle* p = acquireParticle();
			if (p)
			{
				generated.push_back(p);
			}
		}
	}
	return generated;
}