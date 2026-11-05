#pragma once

#include "ParticleGenerator.h"
#include <memory>

class GaussianVectorDistribution;

class FountainParticleGenerator : public ParticleGenerator
{
private:
	std::unique_ptr<GaussianVectorDistribution> m_velDist;
	Vector3D m_meanVel;
	Vector3D m_stdDevVel;
	// D.2: la posicion de nacimiento sigue una distribucion gaussiana alrededor del origen.
	std::unique_ptr<GaussianVectorDistribution> m_posDist;
	Vector3D m_posStdDev;

	void rebuildDistribution();

public:
	FountainParticleGenerator(Vector3D origin = Vector3D(0.0f, 0.0f, 0.0f),
		Vector3D meanVel = Vector3D(0.0f, 15.0f, 0.0f),
		Vector3D stdDevVel = Vector3D(2.5f, 2.0f, 2.5f),
		float particleMass = 1.0f,
		Vector3D posStdDev = Vector3D(0.4f, 0.05f, 0.4f));
	~FountainParticleGenerator() override;

	Particle* generateSingleParticle(Particle* reusable = nullptr) override;
	ParticleGeneratorType getType() const override { return ParticleGeneratorType::FOUNTAIN; }

	Vector3D getMeanVelocity() const { return m_meanVel; }
	void setMeanVelocity(const Vector3D& vel) { m_meanVel = vel; rebuildDistribution(); }
	Vector3D getStdDevVelocity() const { return m_stdDevVel; }
	void setStdDevVelocity(const Vector3D& stdDev) { m_stdDevVel = clampComponents(stdDev, 0.001f); rebuildDistribution(); }
	Vector3D getPosStdDev() const { return m_posStdDev; }
	void setPosStdDev(const Vector3D& stdDev) { m_posStdDev = clampComponents(stdDev, 0.001f); rebuildDistribution(); }
};