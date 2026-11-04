#include "FireParticleGenerator.h"
#include "Vector3DDistribution.h"
#include "Particle.h"

FireParticleGenerator::FireParticleGenerator(Vector3D origin)
	: ParticleGenerator("FuegoYChispas", origin, 4, 0.02),
	  m_meanVel(0.0f, 8.0f, 0.0f),
	  m_stdDevVel(1.2f, 1.5f, 1.2f)
{
	m_stdDevVel = clampComponents(m_stdDevVel, 0.001f);
	rebuildDistribution();

	// Mantiene el color naranja/amarillo que tenía el generador original.
	m_color = Vector4(1.0f, 0.5f, 0.05f, 1.0f);
	m_colorMode = ParticleColorMode::DISTRIBUTION;
	m_colorMin = Vector4(1.0f, 0.2f, 0.05f, 1.0f);
	m_colorMax = Vector4(1.0f, 0.8f, 0.05f, 1.0f);
	m_radius = 0.2f;
	m_lifetime = 1.6f;
	m_damping = 0.96f;
}

FireParticleGenerator::~FireParticleGenerator() = default;

void FireParticleGenerator::rebuildDistribution()
{
	m_velDist = std::make_unique<GaussianVectorDistribution>(m_meanVel, m_stdDevVel);
}

Particle* FireParticleGenerator::generateSingleParticle(Particle* reusable)
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
