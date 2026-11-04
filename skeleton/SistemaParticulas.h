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
	// Partículas adoptadas por addParticle; se liberan al limpiar el sistema.
	std::vector<std::unique_ptr<Particle>> m_ownedParticles;
	std::vector<std::unique_ptr<ParticleGenerator>> m_generators;
	std::vector<std::unique_ptr<ForceGenerator>> m_forceGenerators;
	std::vector<ForceGenerator*> m_externalForceGenerators;

	float m_lowerYBound = -10.0f;
	float m_maxHorizontalDist = 500;

	bool isWithinBounds(const Particle* p) const;
	// Avisa a los generadores para que suelten punteros a la partícula.
	void notifyParticleDestroyed(Particle* p);

public:
	explicit SistemaParticulas(size_t initialReserve = 500);
	~SistemaParticulas();

	ParticleGenerator* addGenerator(std::unique_ptr<ParticleGenerator> generator);
	// También saca de m_particles las partículas de la pool antes de destruirlo.
	bool removeGenerator(ParticleGenerator* generator);

	ForceGenerator* addForceGenerator(std::unique_ptr<ForceGenerator> generator);
	// Versión no propietaria para generadores externos al sistema.
	void addForceGenerator(ForceGenerator* generator);
	bool removeForceGenerator(ForceGenerator* generator);

	void addParticle(Particle* p);

	void update(double dt);
	// Solo libera las partículas propias; las pools de los generadores se mantienen.
	void clearParticles();
	void cleanup();

	size_t getParticleCount() const { return m_particles.size(); }

	const std::vector<Particle*>& getParticles() const { return m_particles; }
	std::vector<Particle*>& getParticles() { return m_particles; }

	const std::vector<std::unique_ptr<ParticleGenerator>>& getGenerators() const { return m_generators; }
	std::vector<std::unique_ptr<ParticleGenerator>>& getGenerators() { return m_generators; }

	const std::vector<std::unique_ptr<ForceGenerator>>& getForceGenerators() const { return m_forceGenerators; }
	std::vector<std::unique_ptr<ForceGenerator>>& getForceGenerators() { return m_forceGenerators; }

	void setBounds(float lowerY, float maxHorizontal) { m_lowerYBound = lowerY; m_maxHorizontalDist = maxHorizontal; }
	float getLowerYBound() const { return m_lowerYBound; }
	float getMaxHorizontalDist() const { return m_maxHorizontalDist; }
};
