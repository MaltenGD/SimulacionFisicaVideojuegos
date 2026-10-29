#pragma once

#include "Vector3D.h"
#include "core.hpp"
#include <string>
#include <vector>

class Particle;

class ParticleGenerator
{
protected:
	std::string m_name;
	Vector3D m_origin = Vector3D::zero();
	Vector3D m_gravity = Vector3D(0.0f, -10.0f, 0.0f);
	Vector4 m_color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	float m_radius = 0.3f;
	float m_mass = 1.0f;
	float m_damping = 0.99f;
	float m_lifetime = 5.0f;

	int m_particlesPerSpawn = 2;
	double m_spawnInterval = 0.05;
	double m_timeSinceLastSpawn = 0.0;
	bool m_active = true;

public:
	ParticleGenerator(std::string name, Vector3D origin, int particlesPerSpawn = 2, double spawnInterval = 0.05);
	virtual ~ParticleGenerator() = default;

	virtual std::vector<Particle*> updateAndGenerate(double dt);
	virtual Particle* generateSingleParticle() = 0;

	const std::string& getName() const { return m_name; }
	void setOrigin(const Vector3D& pos) { m_origin = pos; }
	Vector3D getOrigin() const { return m_origin; }
	void setGravity(const Vector3D& g) { m_gravity = g; }
	Vector3D getGravity() const { return m_gravity; }
	void setColor(const Vector4& c) { m_color = c; }
	Vector4 getColor() const { return m_color; }
	void setRadius(float r) { m_radius = r; }
	void setDamping(float d) { m_damping = d; }
	void setLifetime(float lt) { m_lifetime = lt; }
	void setEmissionRate(int count, double interval) { m_particlesPerSpawn = count; m_spawnInterval = interval; }

	void setActive(bool active) { m_active = active; }
	bool isActive() const { return m_active; }
	void toggleActive() { m_active = !m_active; }
};
