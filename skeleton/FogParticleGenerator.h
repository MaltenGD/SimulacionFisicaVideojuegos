#pragma once

#include "ParticleGenerator.h"
#include <memory>

class Vector3DDistribution;

class FogParticleGenerator : public ParticleGenerator
{
private:
	std::unique_ptr<Vector3DDistribution> m_posDist;
	std::unique_ptr<Vector3DDistribution> m_velDist;

public:
	FogParticleGenerator(Vector3D minBox = Vector3D(-12.0f, 0.5f, -12.0f),
	                     Vector3D maxBox = Vector3D( 12.0f,  3.5f,  12.0f));
	~FogParticleGenerator() override;

	Particle* generateSingleParticle() override;
};
