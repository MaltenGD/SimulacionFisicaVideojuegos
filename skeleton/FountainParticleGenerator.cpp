#include "FountainParticleGenerator.h"
#include "Vector3DDistribution.h"
#include "Particle.h"

FountainParticleGenerator::FountainParticleGenerator(Vector3D origin, Vector3D meanVel, Vector3D stdDevVel, float particleMass)
	: ParticleGenerator("FuenteDeAgua", origin, 3, 0.03),
	  m_meanVel(meanVel),
	  m_stdDevVel(stdDevVel)
{
	m_stdDevVel = clampComponents(m_stdDevVel, 0.001f);
	rebuildDistribution();
	m_color = Vector4(0.2f, 0.6f, 1.0f, 1.0f);
	m_colorMin = m_color;
	m_colorMax = m_color;
	m_radius = 0.25f;
	m_lifetime = 6.0f;
	m_damping = 0.99f;
	m_mass = particleMass;
}

FountainParticleGenerator::~FountainParticleGenerator() = default;

void FountainParticleGenerator::rebuildDistribution()
{

	m_velDist = std::make_unique<GaussianVectorDistribution>(m_meanVel, m_stdDevVel);
}

Particle* FountainParticleGenerator::generateSingleParticle(Particle* reusable)
{
	Vector3D vel = m_velDist->sample();
	Vector4 color = sampleColor();
	if (reusable)
	{
		reusable->reset(
			m_mass,
			m_origin,
			vel,
			Vector3D::zero(),
			m_damping,
			m_radius,
			color,
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
		color,
		m_lifetime
	);
	particle->setScaleWithLifetime(m_scaleWithLifetime);
	return particle;
}
