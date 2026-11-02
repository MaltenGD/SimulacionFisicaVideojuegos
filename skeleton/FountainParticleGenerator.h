#pragma once

#include "ParticleGenerator.h"
#include <memory>

class Vector3Distribution;

class FountainParticleGenerator : public ParticleGenerator
{
private:
	std::unique_ptr<Vector3Distribution> m_velDist;

public:
	FountainParticleGenerator(Vector3D origin = Vector3D(0.0f, 0.0f, 0.0f),
	                          Vector3D meanVel = Vector3D(0.0f, 15.0f, 0.0f),
	                          Vector3D stdDevVel = Vector3D(2.5f, 2.0f, 2.5f),
	                          float particleMass = 1.0f);
	~FountainParticleGenerator() override;

	Particle* generateSingleParticle() override;
};
