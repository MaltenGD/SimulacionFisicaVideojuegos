#pragma once

#include "ParticleGenerator.h"
#include <memory>
#include <random>

class Distribution;
class Vector3Distribution;

class FireParticleGenerator : public ParticleGenerator
{
private:
	std::unique_ptr<Vector3Distribution> m_velDist;
	std::mt19937 m_colorGen;
	std::uniform_real_distribution<float> m_colorVar;

public:
	FireParticleGenerator(Vector3D origin = Vector3D(15.0f, 0.0f, 0.0f));
	~FireParticleGenerator() override;

	Particle* generateSingleParticle() override;
};
