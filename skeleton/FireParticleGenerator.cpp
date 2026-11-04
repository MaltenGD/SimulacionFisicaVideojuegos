#include "FireParticleGenerator.h"
#include "Distribution.h"
#include "Particle.h"

FireParticleGenerator::FireParticleGenerator(Vector3D origin)
	: ParticleGenerator("FuegoYChispas", origin, 4, 0.02),
	  m_colorGen(std::random_device{}()),
	  m_colorVar(0.0f, 1.0f),
	  m_meanVel(0.0f, 8.0f, 0.0f),
	  m_stdDevVel(1.2f, 1.5f, 1.2f)
{
	rebuildDistribution();

	m_radius = 0.2f;
	m_lifetime = 1.6f;
	m_damping = 0.96f;
}

FireParticleGenerator::~FireParticleGenerator() = default;

void FireParticleGenerator::rebuildDistribution()
{
	m_velDist = std::make_unique<Vector3Distribution>(
		new GaussianDistribution(m_meanVel.x, m_stdDevVel.x),
		new GaussianDistribution(m_meanVel.y, m_stdDevVel.y),
		new GaussianDistribution(m_meanVel.z, m_stdDevVel.z));
}

Particle* FireParticleGenerator::generateSingleParticle(Particle* reusable)
{
	Vector3D vel = m_velDist->sample();

	// Para que vaya cambiando entre rojo naranja y amarillo
	float green = 0.2f + 0.6f * m_colorVar(m_colorGen);
	Vector4 fireColor(1.0f, green, 0.05f, 1.0f);

	if (reusable)
	{
		reusable->reset(
			m_mass,
			m_origin,
			vel,
			Vector3D::zero(),
			m_damping,
			m_radius,
			fireColor,
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
		fireColor,
		m_lifetime
	);
	particle->setScaleWithLifetime(m_scaleWithLifetime);
	return particle;
}
