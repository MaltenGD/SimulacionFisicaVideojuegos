#include "FountainParticleGenerator.h"
#include "Vector3DDistribution.h"
#include "Particle.h"

FountainParticleGenerator::FountainParticleGenerator(Vector3D origin, Vector3D meanVel, Vector3D stdDevVel)
	: ParticleGenerator("FuenteDeAgua", origin, 3, 0.03)
{
	m_velDist = std::make_unique<GaussianVectorDistribution>(meanVel, stdDevVel);
	m_color = Vector4(0.2f, 0.6f, 1.0f, 1.0f); // Azul	
	m_radius = 0.25f;
	m_lifetime = 4.0f;
	m_gravity = Vector3D(0.0f, -10.0f, 0.0f);  // g = -10 m/s^2
}

FountainParticleGenerator::~FountainParticleGenerator() = default;

Particle* FountainParticleGenerator::generateSingleParticle()
{
	Vector3D vel = m_velDist->sample();
	return new Particle(
		m_mass,
		m_origin,
		vel,
		Vector3D::zero(),
		m_gravity,
		m_damping,
		m_radius,
		m_color,
		m_lifetime
	);
}
