#pragma once

#include "ParticleGenerator.h"
#include <memory>
#include <vector>

class Distribution;
class Vector3Distribution;

class FountainParticleGenerator : public ParticleGenerator
{
private:
	std::unique_ptr<Vector3Distribution> m_velDist;
	Vector3D m_fixedGravity = Vector3D::zero(); // Normalmente es nulo, solo se usa para la escena 2

public:
	FountainParticleGenerator(Vector3D origin = Vector3D(0.0f, 0.0f, 0.0f),
								Vector3D meanVel = Vector3D(0.0f, 15.0f, 0.0f),
								Vector3D stdDevVel = Vector3D(2.5f, 2.0f, 2.5f), Vector3D fixedGravity = Vector3D(2.5f, 2.0f, 2.5f));
	~FountainParticleGenerator() override;

	Particle* generateSingleParticle() override;
};
