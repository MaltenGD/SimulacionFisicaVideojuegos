#include "SistemaParticulas.h"
#include "Particle.h"
#include "ParticleGenerator.h"
#include "ForceGenerator.h"
#include <algorithm>
#include <cmath>

SistemaParticulas::SistemaParticulas(size_t initialReserve)
{
	m_particles.reserve(initialReserve);
}

SistemaParticulas::~SistemaParticulas()
{
	cleanup();
}

ParticleGenerator* SistemaParticulas::addGenerator(std::unique_ptr<ParticleGenerator> generator)
{
	m_generators.push_back(std::move(generator));
	return m_generators.back().get();
}

bool SistemaParticulas::removeGenerator(ParticleGenerator* generator)
{
	if (!generator)
		return false;

	auto it = m_generators.begin();
	while (it != m_generators.end() && it->get() != generator)
	{
		++it;
	}

	if (it == m_generators.end())
		return false;

	// El generador controla su propia pool de particulas, borra la pool antes de que el generador se destruya
	for (Particle* pooled : generator->getPooledParticles())
	{
		auto particleIt = std::find(m_particles.begin(), m_particles.end(), pooled);
		if (particleIt != m_particles.end())
		{
			notifyParticleDestroyed(pooled);
			m_particles.erase(particleIt);
		}
	}

	m_generators.erase(it);
	return true;
}

ForceGenerator* SistemaParticulas::addForceGenerator(std::unique_ptr<ForceGenerator> generator)
{
	m_forceGenerators.push_back(std::move(generator));
	return m_forceGenerators.back().get();
}

bool SistemaParticulas::removeForceGenerator(ForceGenerator* generator)
{
	if (!generator)
		return false;

	auto it = m_forceGenerators.begin();
	while (it != m_forceGenerators.end() && it->get() != generator)
	{
		++it;
	}

	if (it == m_forceGenerators.end())
		return false;

	m_forceGenerators.erase(it);
	return true;
}

void SistemaParticulas::addForceGenerator(ForceGenerator* generator)
{
	if (generator)
	{
		m_externalForceGenerators.push_back(generator);
	}
}

void SistemaParticulas::addParticle(Particle* p)
{
	if (p)
	{
		m_ownedParticles.emplace_back(p);
		m_particles.push_back(p);
	}
}

bool SistemaParticulas::isWithinBounds(const Particle* p) const
{
	Vector3D pos = p->getPosition();
	float horizDist = std::sqrt(pos.x * pos.x + pos.z * pos.z);
	return pos.y > m_lowerYBound && horizDist < m_maxHorizontalDist;
}

void SistemaParticulas::notifyParticleDestroyed(Particle* p)
{
	for (auto& generator : m_forceGenerators)
	{
		if (generator)
			generator->onParticleDestroyed(p);
	}

	for (ForceGenerator* generator : m_externalForceGenerators)
	{
		if (generator)
			generator->onParticleDestroyed(p);
	}
}

void SistemaParticulas::update(double dt)
{
	for (auto& gen : m_generators)
	{
		std::vector<Particle*> newParticles = gen->updateAndGenerate(dt);
		for (Particle* p : newParticles)
		{
			m_particles.push_back(p);
		}
	}

	for (auto& fg : m_forceGenerators)
	{
		fg->update(dt);
	}
	for (ForceGenerator* fg : m_externalForceGenerators)
	{
		fg->update(dt);
	}

	for (Particle* p : m_particles)
	{
		for (auto& fg : m_forceGenerators)
		{
			fg->applyForces(p);
		}
		for (ForceGenerator* fg : m_externalForceGenerators)
		{
			fg->applyForces(p);
		}
	}

	for (auto it = m_particles.begin(); it != m_particles.end(); )
	{
		Particle* p = *it;
		p->integrate(dt);

		bool alive = p->isAlive() && isWithinBounds(p);

		if (!alive)
		{
			notifyParticleDestroyed(p);
			p->deactivate();
			it = m_particles.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void SistemaParticulas::clearParticles()
{
	for (Particle* p : m_particles)
	{
		notifyParticleDestroyed(p);
		p->deactivate();
	}

	// delete only particles owned directly by the system; generator pools
	// keep ownership of the particles they created.
	m_ownedParticles.clear();
	m_particles.clear();
}

void SistemaParticulas::cleanup()
{
	clearParticles();
	m_generators.clear();
	m_forceGenerators.clear();
	m_externalForceGenerators.clear();
}
