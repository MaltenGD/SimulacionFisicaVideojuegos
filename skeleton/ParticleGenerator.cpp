#include "ParticleGenerator.h"
#include <utility>

ParticleGenerator::ParticleGenerator(std::string name, Vector3D origin, int particlesPerSpawn, double spawnInterval)
	: m_name(std::move(name)),
	  m_origin(origin),
	  m_particlesPerSpawn(particlesPerSpawn),
	  m_spawnInterval(spawnInterval)
{
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
			Particle* p = generateSingleParticle();
			if (p)
			{
				generated.push_back(p);
			}
		}
	}
	return generated;
}
