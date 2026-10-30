#include "WhirlwindForceGenerator.h"
#include <cmath>

WhirlwindForceGenerator::WhirlwindForceGenerator(const Vector3D& center, float radius, float height,
                                                 float K, float k1,
                                                 const Vector4& debugColor)
	: WindForceGenerator(center, Vector3D(radius, height * 0.5f, radius), Vector3D::zero(), k1, debugColor),
	  m_K(K),
	  m_radius(radius)
{
}

bool WhirlwindForceGenerator::isInside(const Vector3D& pos) const
{
	float dx = pos.x - m_origin.x;
	float dz = pos.z - m_origin.z;
	float horizontalDistSq = dx * dx + dz * dz;

	if (horizontalDistSq > m_radius * m_radius)
		return false;

	return (pos.y >= m_origin.y - m_halfExtents.y && pos.y <= m_origin.y + m_halfExtents.y);
}

Vector3D WhirlwindForceGenerator::getWindVelocity(const Vector3D& particlePos) const
{
	Vector3D diff = particlePos - m_origin;

	return Vector3D(
		-diff.z,
		(15.0f - diff.y) * 0.2f,
		diff.x
	) * m_K;
}
