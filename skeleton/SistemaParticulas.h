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

	Vector3D m_gravity = Vector3D(0.0f, -10.0f, 0.0f); // Gravedad global (-10 m/s^2)
	float m_lowerYBound = -10.0f;                       // Cota inferior antes de eliminar particulas.
	float m_maxHorizontalDist = 150.0f;                 // Radio horizontal máximo antes de eliminar particulas

	bool isWithinBounds(const Particle* p) const;

public:
	explicit SistemaParticulas(size_t initialReserve = 500);
	~SistemaParticulas();

	// Agrega un generador de partículas a la escena
	ParticleGenerator* addGenerator(std::unique_ptr<ParticleGenerator> generator);

	// Agrega un generador de fuerzas
	ForceGenerator* addForceGenerator(std::unique_ptr<ForceGenerator> generator);
	void addForceGenerator(ForceGenerator* generator);

	// Añade una partícula directamente al sistema
	void addParticle(Particle* p);

	// Actualiza todos los generadores, aplica fuerzas, integra las partículas y elimina las que expiran o salen del espacio
	void update(double dt);

	// Limpia todas las partículas activas sin eliminar los emisores ni generadores de fuerza
	void clearParticles();

	// Libera todas las partículas, generadores y fuerzas
	void cleanup();

	// Getters y Setters
	size_t getParticleCount() const { return m_particles.size(); }
	const std::vector<std::unique_ptr<ParticleGenerator>>& getGenerators() const { return m_generators; }
	std::vector<std::unique_ptr<ParticleGenerator>>& getGenerators() { return m_generators; }

	const std::vector<std::unique_ptr<ForceGenerator>>& getForceGenerators() const { return m_forceGenerators; }
	std::vector<std::unique_ptr<ForceGenerator>>& getForceGenerators() { return m_forceGenerators; }

	void setGravity(const Vector3D& g) { m_gravity = g; }
	Vector3D getGravity() const { return m_gravity; }
	void setBounds(float lowerY, float maxHorizontal) { m_lowerYBound = lowerY; m_maxHorizontalDist = maxHorizontal; }
};
