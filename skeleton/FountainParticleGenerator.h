#pragma once

#include "ParticleGenerator.h"
#include <memory>

class Vector3Distribution;

class FountainParticleGenerator : public ParticleGenerator
{
private:
	std::unique_ptr<Vector3Distribution> m_velDist;
	Vector3D m_meanVel;
	Vector3D m_stdDevVel;

	void rebuildDistribution();

public:
	FountainParticleGenerator(Vector3D origin = Vector3D(0.0f, 0.0f, 0.0f),
	                          Vector3D meanVel = Vector3D(0.0f, 15.0f, 0.0f),
	                          Vector3D stdDevVel = Vector3D(2.5f, 2.0f, 2.5f),
	                          float particleMass = 1.0f);
	~FountainParticleGenerator() override;

	Particle* generateSingleParticle(Particle* reusable = nullptr) override;
	ParticleGeneratorType getType() const override { return ParticleGeneratorType::FOUNTAIN; }

	Vector3D getMeanVelocity() const { return m_meanVel; }
	void setMeanVelocity(const Vector3D& vel) { m_meanVel = vel; rebuildDistribution(); }
	Vector3D getStdDevVelocity() const { return m_stdDevVel; }
	void setStdDevVelocity(const Vector3D& stdDev) { m_stdDevVel = stdDev; rebuildDistribution(); }
};
