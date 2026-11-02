#pragma once

#include "WindForceGenerator.h"

class WhirlwindForceGenerator : public WindForceGenerator
{
private:
	float m_K;
	float m_radius;

public:
	WhirlwindForceGenerator(const Vector3D& center, float radius, float height = 40.0f,
	                        float K = 2.0f, float k1 = 1.5f,
	                        const Vector4& debugColor = Vector4(0.8f, 0.4f, 1.0f, 1.0f));
	virtual ~WhirlwindForceGenerator() override = default;

	virtual Vector3D getWindVelocity(const Vector3D& particlePos) const override;
	virtual bool isInside(const Vector3D& pos) const override;

	void setK(float K) { m_K = K; }
	float getK() const { return m_K; }

	void setRadius(float r) { m_radius = r; }
	float getRadius() const { return m_radius; }
};
