#include "GravityForceGenerator.h"
#include "Particle.h"

GravityForceGenerator::GravityForceGenerator(const Vector3D& origin, const Vector3D& halfExtents,
                                             const Vector3D& gravity, const Vector4& debugColor)
	: PositionalForceGenerator(origin, halfExtents, debugColor),
	  m_gravity(gravity)
{
}

void GravityForceGenerator::applyForces(Particle* particle)
{
	if (!particle || !particle->isAlive())
		return;

	if (particle->getMassInverse() <= 0.0f)
		return;

	if (isInside(particle->getPosition()))
	{
		Vector3D force = m_gravity * particle->getMass();
		particle->addForce(force);
	}
}
