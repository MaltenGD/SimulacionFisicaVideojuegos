#include "FountainParticleGenerator.h"
#include "Distribution.h"
#include "Particle.h"

FountainParticleGenerator::FountainParticleGenerator(Vector3D origin, Vector3D meanVel, Vector3D stdDevVel, float particleMass)
	: ParticleGenerator("FuenteDeAgua", origin, 3, 0.03),
	  m_meanVel(meanVel),
	  m_stdDevVel(stdDevVel)
{
	rebuildDistribution();
	m_color = Vector4(0.2f, 0.6f, 1.0f, 1.0f);
	m_radius = 0.25f;
	m_lifetime = 6.0f;
	m_damping = 0.99f;
	m_mass = particleMass;
}

FountainParticleGenerator::~FountainParticleGenerator() = default;

void FountainParticleGenerator::rebuildDistribution()
{
	m_velDist = std::make_unique<Vector3Distribution>(
		new GaussianDistribution(m_meanVel.x, m_stdDevVel.x),
		new GaussianDistribution(m_meanVel.y, m_stdDevVel.y),
		new GaussianDistribution(m_meanVel.z, m_stdDevVel.z));
}

Particle* FountainParticleGenerator::generateSingleParticle(Particle* reusable)
{
	Vector3D vel = m_velDist->sample();
	if (reusable)
	{
		reusable->reset(
			m_mass,
			m_origin,
			vel,
			Vector3D::zero(),
			m_damping,
			m_radius,
			m_color,
			m_lifetime
		);
		reusable->setScaleWithLifetime(m_scaleWithLifetime);
		return reusable;
	}

	Particle* particle = new Particle(
		m_mass,
		m_origin,
		vel,
		Vector3D::zero(),
		m_damping,
		m_radius,
		m_color,
		m_lifetime
	);
	particle->setScaleWithLifetime(m_scaleWithLifetime);
	return particle;
}
