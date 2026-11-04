#pragma once

#include "Vector3D.h"
#include "core.hpp"
#include "Particle.h"
#include <string>
#include <memory>
#include <vector>

enum class ParticleGeneratorType
{
	UNKNOWN,
	FOUNTAIN,
	FIRE,
	FOG
};

class ParticleGenerator
{
protected:
	std::string m_name;
	Vector3D m_origin = Vector3D::zero();
	Vector4 m_color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
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
	std::vector<std::unique_ptr<Particle>> m_particlePool;

	Particle* acquireParticle();

public:
	ParticleGenerator(std::string name, Vector3D origin, int particlesPerSpawn = 2, double spawnInterval = 0.05);
	virtual ~ParticleGenerator() = default;

	virtual std::vector<Particle*> updateAndGenerate(double dt);
	virtual Particle* generateSingleParticle(Particle* reusable = nullptr) = 0;
	virtual ParticleGeneratorType getType() const { return ParticleGeneratorType::UNKNOWN; }


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
	void setOrigin(const Vector3D& pos) { m_origin = pos; } // Por alguna razon el fog generator no funciona con esto. TODO: ARREGLAR
	Vector3D getOrigin() const { return m_origin; }
	void setColor(const Vector4& c) { m_color = c; }
	Vector4 getColor() const { return m_color; }
	void setRadius(float r) { m_radius = r; }
	float getRadius() const { return m_radius; }
	void setMass(float m) { m_mass = m; }
	float getMass() const { return m_mass; }
	void setDamping(float d) { m_damping = d; }
	float getDamping() const { return m_damping; }
	void setLifetime(float lt) { m_lifetime = lt; }
	float getLifetime() const { return m_lifetime; }
	void setScaleWithLifetime(bool enabled) { m_scaleWithLifetime = enabled; }
	bool getScaleWithLifetime() const { return m_scaleWithLifetime; }
	void setEmissionRate(int count, double interval) { m_particlesPerSpawn = count; m_spawnInterval = interval; }
	int getParticlesPerSpawn() const { return m_particlesPerSpawn; }
	double getSpawnInterval() const { return m_spawnInterval; }
	void setSpawnInterval(double interval) { m_spawnInterval = interval > 0.001 ? interval : 0.001; }
	void setMaxParticles(size_t maxParticles) { m_maxParticles = maxParticles; m_particlePool.reserve(maxParticles); }
	size_t getMaxParticles() const { return m_maxParticles; }
	size_t getPoolSize() const { return m_particlePool.size(); }

	void setActive(bool active) { m_active = active; }
	bool isActive() const { return m_active; }
	void toggleActive() { m_active = !m_active; }
};
