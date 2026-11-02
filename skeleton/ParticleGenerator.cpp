#include "ParticleGenerator.h"
#include "Particle.h"
#include <utility>

ParticleGenerator::ParticleGenerator(std::string name, Vector3D origin, int particlesPerSpawn, double spawnInterval)
	: m_name(std::move(name)),
	  m_origin(origin),
	  m_particlesPerSpawn(particlesPerSpawn),
	  m_spawnInterval(spawnInterval)
{
}

Particle* ParticleGenerator::acquireParticle()
{
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

std::vector<Particle*> ParticleGenerator::updateAndGenerate(double dt)
{
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
