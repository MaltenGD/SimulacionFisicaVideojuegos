#include "WindForceGenerator.h"
#include "Particle.h"

WindForceGenerator::WindForceGenerator(const Vector3D& origin, const Vector3D& halfExtents,
                                       const Vector3D& windVel, float k1,
                                       const Vector4& debugColor)
	: PositionalForceGenerator(origin, halfExtents, debugColor),
	  m_windVel(windVel),
	  m_k1(k1)
{
}

Vector3D WindForceGenerator::getWindVelocity(const Vector3D& particlePos) const
{
	(void)particlePos;
	return m_windVel;
}

void WindForceGenerator::applyForces(Particle* particle)
{
	if (!particle || !particle->isAlive())
		return;

	Vector3D pos = particle->getPosition();
	if (isInside(pos))
	{
		Vector3D windVel = getWindVelocity(pos);
		Vector3D diff = windVel - particle->getVelocity();
		Vector3D force = diff * m_k1;
		particle->addForce(force);
	}
}
