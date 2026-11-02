#pragma once

#include "PositionalForceGenerator.h"
#include "Vector3D.h"

class WindForceGenerator : public PositionalForceGenerator
{
protected:
	Vector3D m_windVel;
	float m_k1;

public:
	WindForceGenerator(const Vector3D& origin, const Vector3D& halfExtents,
	                   const Vector3D& windVel = Vector3D(10.0f, 0.0f, 0.0f),
	                   float k1 = 1.0f,
	                   const Vector4& debugColor = Vector4(0.2f, 0.9f, 0.3f, 1.0f));
	virtual ~WindForceGenerator() override = default;

	virtual Vector3D getWindVelocity(const Vector3D& particlePos) const;
	virtual void applyForces(Particle* particle) override;

	void setWindVelocity(const Vector3D& vel) { m_windVel = vel; }
	Vector3D getWindVelocity() const { return m_windVel; }

	void setK1(float k1) { m_k1 = k1; }
	float getK1() const { return m_k1; }
};
