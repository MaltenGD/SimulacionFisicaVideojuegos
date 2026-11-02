#include "ExplosionForceGenerator.h"
#include "Particle.h"
#include <cmath>

ExplosionForceGenerator::ExplosionForceGenerator(const Vector3D& center, float K, float radius,
                                                 float tau, bool expandingWave, float expansionSpeed)
	: m_center(center),
	  m_K(K),
	  m_radius(radius),
	  m_tau(tau),
	  m_timeElapsed(0.0f),
	  m_active(false),
	  m_expandingWave(expandingWave),
	  m_expansionSpeed(expansionSpeed)
{
}

void ExplosionForceGenerator::trigger(const Vector3D& center)
{
	m_center = center;
	m_timeElapsed = 0.0f;
	m_active = true;
}

void ExplosionForceGenerator::trigger()
{
	m_timeElapsed = 0.0f;
	m_active = true;
}

void ExplosionForceGenerator::update(double dt)
{
	if (!m_active)
		return;

	m_timeElapsed += static_cast<float>(dt);

	if (m_timeElapsed > 4.0f * m_tau)
	{
		m_active = false;
	}
}

void ExplosionForceGenerator::applyForces(Particle* particle)
{
	if (!m_active || !particle || !particle->isAlive())
		return;

	Vector3D diff = particle->getPosition() - m_center;
	float r = diff.magnitude();

	if (r < 0.05f)
		return;

	float currentRadius = m_radius;
	if (m_expandingWave)
	{
		currentRadius = m_expansionSpeed * m_timeElapsed;
	}

	if (r < currentRadius)
	{
		float factor = (m_K / (r * r)) * std::exp(-m_timeElapsed / m_tau);
		Vector3D force = diff * factor;
		particle->addForce(force);
	}
}
