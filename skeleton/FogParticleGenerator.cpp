#include "FogParticleGenerator.h"
#include "Vector3DDistribution.h"
#include "Particle.h"

FogParticleGenerator::FogParticleGenerator(Vector3D minBox, Vector3D maxBox)
	: ParticleGenerator("Niebla", (minBox + maxBox) * 0.5f, 2, 0.08)
{
	m_posDist = std::make_unique<UniformVectorDistribution>(minBox, maxBox);
	m_velDist = std::make_unique<UniformVectorDistribution>(
		Vector3D(-0.4f, -0.05f, -0.4f),
		Vector3D( 0.4f,  0.15f,  0.4f)
	);

	m_color = Vector4(0.85f, 0.85f, 0.9f, 0.5f);
	m_radius = 0.2f;
	m_lifetime = 6.0f;
	m_damping = 0.98f;
	m_gravity = Vector3D(0.0f, -0.3f, 0.0f);
}

FogParticleGenerator::~FogParticleGenerator() = default;

Particle* FogParticleGenerator::generateSingleParticle()
{
	Vector3D pos = m_posDist->sample();
	Vector3D vel = m_velDist->sample();

	return new Particle(
		m_mass,
		pos,
		vel,
		Vector3D::zero(),
		m_gravity,
		m_damping,
		m_radius,
		m_color,
		m_lifetime
	);
}
