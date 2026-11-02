#pragma once

#include "Vector3D.h"
#include <vector>
#include <memory>

class Particle;
class ParticleGenerator;
class ForceGenerator;

class SistemaParticulas
{
private:
	std::vector<Particle*> m_particles;
	std::vector<std::unique_ptr<ParticleGenerator>> m_generators;
	std::vector<std::unique_ptr<ForceGenerator>> m_forceGenerators;
	std::vector<ForceGenerator*> m_externalForceGenerators;

	float m_lowerYBound = -10.0f;
	float m_maxHorizontalDist = 150.0f;

	bool isWithinBounds(const Particle* p) const;

public:
	explicit SistemaParticulas(size_t initialReserve = 500);
	~SistemaParticulas();

	ParticleGenerator* addGenerator(std::unique_ptr<ParticleGenerator> generator);

	ForceGenerator* addForceGenerator(std::unique_ptr<ForceGenerator> generator);
	void addForceGenerator(ForceGenerator* generator);

	void addParticle(Particle* p);

	void update(double dt);
	void clearParticles();
	void cleanup();

	size_t getParticleCount() const { return m_particles.size(); }
	const std::vector<std::unique_ptr<ParticleGenerator>>& getGenerators() const { return m_generators; }
	std::vector<std::unique_ptr<ParticleGenerator>>& getGenerators() { return m_generators; }

	const std::vector<std::unique_ptr<ForceGenerator>>& getForceGenerators() const { return m_forceGenerators; }
	std::vector<std::unique_ptr<ForceGenerator>>& getForceGenerators() { return m_forceGenerators; }

	void setBounds(float lowerY, float maxHorizontal) { m_lowerYBound = lowerY; m_maxHorizontalDist = maxHorizontal; }
};
