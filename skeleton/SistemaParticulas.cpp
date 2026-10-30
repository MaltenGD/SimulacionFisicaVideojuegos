#include "SistemaParticulas.h"
#include "Particle.h"
#include "ParticleGenerator.h"
#include "ForceGenerator.h"
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

ForceGenerator* SistemaParticulas::addForceGenerator(std::unique_ptr<ForceGenerator> generator)
{
	m_forceGenerators.push_back(std::move(generator));
	return m_forceGenerators.back().get();
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
		m_particles.push_back(p);
	}
}

bool SistemaParticulas::isWithinBounds(const Particle* p) const
{
	Vector3D pos = p->getPosition();
	float horizDist = std::sqrt(pos.x * pos.x + pos.z * pos.z);
	return pos.y > m_lowerYBound && horizDist < m_maxHorizontalDist;
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
		p->integrate_Semi_Implicit(dt);

		bool alive = p->isAlive() && isWithinBounds(p);

		if (!alive)
		{
			delete p;
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
		delete p;
	}
	m_particles.clear();
}

void SistemaParticulas::cleanup()
{
	clearParticles();
	m_generators.clear();
	m_forceGenerators.clear();
	m_externalForceGenerators.clear();
}
