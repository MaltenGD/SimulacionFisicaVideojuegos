#pragma once

#include "ForceGenerator.h"
#include "Vector3D.h"

class ExplosionForceGenerator : public ForceGenerator
{
private:
	Vector3D m_center;
	float m_K;
	float m_radius;
	float m_tau;
	float m_timeElapsed;
	bool m_active;
	bool m_expandingWave;
	float m_expansionSpeed;

public:
	ExplosionForceGenerator(const Vector3D& center = Vector3D::zero(),
	                       float K = 2000.0f, float radius = 30.0f, float tau = 0.4f,
	                       bool expandingWave = false, float expansionSpeed = 340.0f);
	virtual ~ExplosionForceGenerator() override = default;

	void trigger(const Vector3D& center);
	void trigger();

	virtual void update(double dt) override;
	virtual void applyForces(Particle* particle) override;

	bool isActive() const { return m_active; }
	float getTimeElapsed() const { return m_timeElapsed; }

	void setK(float K) { m_K = K; }
	float getK() const { return m_K; }

	void setRadius(float r) { m_radius = r; }
	float getRadius() const { return m_radius; }

	void setTau(float tau) { m_tau = tau; }
	float getTau() const { return m_tau; }

	void setCenter(const Vector3D& c) { m_center = c; }
	Vector3D getCenter() const { return m_center; }

	void setExpandingWave(bool exp) { m_expandingWave = exp; }
	bool isExpandingWave() const { return m_expandingWave; }
};
