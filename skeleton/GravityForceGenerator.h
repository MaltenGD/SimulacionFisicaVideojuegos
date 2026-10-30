#pragma once

#include "PositionalForceGenerator.h"
#include "Vector3D.h"

class GravityForceGenerator : public PositionalForceGenerator
{
private:
	Vector3D m_gravity;

public:
	GravityForceGenerator(const Vector3D& origin, const Vector3D& halfExtents,
	                      const Vector3D& gravity = Vector3D(0.0f, -9.8f, 0.0f),
	                      const Vector4& debugColor = Vector4(0.2f, 0.7f, 1.0f, 1.0f));
	virtual ~GravityForceGenerator() override = default;

	virtual void applyForces(Particle* particle) override;

	void setGravity(const Vector3D& g) { m_gravity = g; }
	Vector3D getGravity() const { return m_gravity; }
};
