#pragma once

#include "ParticleGenerator.h"
#include <memory>

class GaussianVectorDistribution;

class FireParticleGenerator : public ParticleGenerator
{
private:
	std::unique_ptr<GaussianVectorDistribution> m_velDist;
	Vector3D m_meanVel;
	Vector3D m_stdDevVel;

	void rebuildDistribution();

public:
	FireParticleGenerator(Vector3D origin = Vector3D(15.0f, 0.0f, 0.0f));
	~FireParticleGenerator() override;

	Particle* generateSingleParticle(Particle* reusable = nullptr) override;
	ParticleGeneratorType getType() const override { return ParticleGeneratorType::FIRE; }

	Vector3D getMeanVelocity() const { return m_meanVel; }
	void setMeanVelocity(const Vector3D& vel) { m_meanVel = vel; rebuildDistribution(); }
	Vector3D getStdDevVelocity() const { return m_stdDevVel; }
	void setStdDevVelocity(const Vector3D& stdDev) { m_stdDevVel = stdDev; rebuildDistribution(); }
};
