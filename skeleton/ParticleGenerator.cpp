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
