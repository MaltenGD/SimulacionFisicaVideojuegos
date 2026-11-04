#pragma once

#include "Vector3D.h"
#include "core.hpp"
#include "Particle.h"
#include <string>
#include <memory>
#include <vector>
#include <random>

// Identifica el tipo sin RTTI; la UI lo usa para elegir el inspector.
enum class ParticleGeneratorType
{
	UNKNOWN,
	FOUNTAIN,
	FIRE,
	FOG
};

enum class ParticleColorMode
{
	SINGLE,
	DISTRIBUTION
};

class ParticleGenerator
{
protected:
	std::string m_name;
	Vector3D m_origin = Vector3D::zero();
	Vector4 m_color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	ParticleColorMode m_colorMode = ParticleColorMode::SINGLE;
	Vector4 m_colorMin = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 m_colorMax = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	std::mt19937 m_colorGen{ std::random_device{}() };
	std::uniform_real_distribution<float> m_colorUnit{ 0.0f, 1.0f };
	float m_radius = 0.3f;
	float m_mass = 1.0f;
	float m_damping = 0.99f;
	float m_lifetime = 5.0f;
	bool m_scaleWithLifetime = false;

	int m_particlesPerSpawn = 2;
	size_t m_maxParticles = 500;
	double m_spawnInterval = 0.05;
	double m_timeSinceLastSpawn = 0.0;
	bool m_active = true;
	// Pool propia del generador; SistemaParticulas no la libera directamente.
	std::vector<std::unique_ptr<Particle>> m_particlePool;

	static float clampMin(float value, float minValue)
	{
		return value >= minValue ? value : minValue;
	}

	static Vector3D clampComponents(const Vector3D& value, float minValue)
	{
		return Vector3D(
			clampMin(value.x, minValue),
			clampMin(value.y, minValue),
			clampMin(value.z, minValue));
	}

	Particle* acquireParticle();

public:
	ParticleGenerator(std::string name, Vector3D origin, int particlesPerSpawn = 2, double spawnInterval = 0.05);
	virtual ~ParticleGenerator() = default;

	// Lo llama SistemaParticulas::update; devuelve solo las partículas generadas en este frame.
	virtual std::vector<Particle*> updateAndGenerate(double dt);
	virtual Particle* generateSingleParticle(Particle* reusable = nullptr) = 0;
	virtual ParticleGeneratorType getType() const { return ParticleGeneratorType::UNKNOWN; }


	// Se usa al borrar el generador para sacar su pool de la lista activa.
	std::vector<Particle*> getPooledParticles() const
	{
		std::vector<Particle*> particles;
		particles.reserve(m_particlePool.size());
		for (const auto& particle : m_particlePool)
		{
			particles.push_back(particle.get());
		}
		return particles;
	}

	const std::string& getName() const { return m_name; }
	// Fog lo sobrescribe para mover también su caja.
	virtual void setOrigin(const Vector3D& pos) { m_origin = pos; }
	Vector3D getOrigin() const { return m_origin; }
	void setColor(const Vector4& c) { m_color = c; }
	Vector4 getColor() const { return m_color; }
	ParticleColorMode getColorMode() const { return m_colorMode; }
	void setColorMode(ParticleColorMode mode) { m_colorMode = mode; }
	Vector4 getColorMin() const { return m_colorMin; }
	Vector4 getColorMax() const { return m_colorMax; }
	void setColorRange(const Vector4& minColor, const Vector4& maxColor)
	{
		m_colorMin = minColor;
		m_colorMax = maxColor;
	}
	Vector4 sampleColor();
	// Solo afecta a las partículas que se generen o reutilicen después.
	void setRadius(float r) { m_radius = clampMin(r, 0.01f); }
	float getRadius() const { return m_radius; }
	void setMass(float m) { m_mass = m; }
	float getMass() const { return m_mass; }
	void setDamping(float d) { m_damping = d; }
	float getDamping() const { return m_damping; }
	void setLifetime(float lt) { m_lifetime = clampMin(lt, 0.01f); }
	float getLifetime() const { return m_lifetime; }
	void setScaleWithLifetime(bool enabled) { m_scaleWithLifetime = enabled; }
	bool getScaleWithLifetime() const { return m_scaleWithLifetime; }
	void setEmissionRate(int count, double interval) { m_particlesPerSpawn = count > 0 ? count : 1; setSpawnInterval(interval); }
	int getParticlesPerSpawn() const { return m_particlesPerSpawn; }
	double getSpawnInterval() const { return m_spawnInterval; }
	void setSpawnInterval(double interval) { m_spawnInterval = interval > 0.001 ? interval : 0.001; }
	// Solo reserva capacidad; no elimina partículas ya creadas.
	void setMaxParticles(size_t maxParticles) { m_maxParticles = maxParticles; m_particlePool.reserve(maxParticles); }
	size_t getMaxParticles() const { return m_maxParticles; }
	size_t getPoolSize() const { return m_particlePool.size(); }

	void setActive(bool active) { m_active = active; }
	bool isActive() const { return m_active; }
	void toggleActive() { m_active = !m_active; }
};
