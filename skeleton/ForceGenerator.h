#pragma once

#include "Vector3D.h"

class Particle;

// Para saber el tipo de generador sin tener que hacer dynamic cast; la UI lo usa para elegir el inspector.
enum class ForceGeneratorType
{
	UNKNOWN,
	GRAVITY,
	WIND,
	WHIRLWIND,
	EXPLOSION,
	BUOYANCY,
	ANCHORED_SPRING,
	SPRING,
	RUBBER
};

class ForceGenerator
{
public:
	ForceGenerator() = default;
	virtual ~ForceGenerator() = default;

	virtual ForceGeneratorType getType() const { return ForceGeneratorType::UNKNOWN; }

	virtual void update(double dt) { (void)dt; }
	virtual void applyForces(Particle* particle) = 0;
	virtual void updateForce(Particle* particle) { applyForces(particle); }

	// Se llama cuando una partícula deja de estar activa.
	// Los generadores que guarden punteros deben soltarlos aquí.
	virtual void onParticleDestroyed(Particle* particle) { (void)particle; }

	// Activacion/desactivacion comun a todos los generadores (teclas e ImGui).
	// SistemaParticulas ni actualiza ni aplica los generadores desactivados.
	void setEnabled(bool enabled) { m_enabled = enabled; }
	bool isEnabled() const { return m_enabled; }
	void toggleEnabled() { m_enabled = !m_enabled; }

protected:
	bool m_enabled = true;
};