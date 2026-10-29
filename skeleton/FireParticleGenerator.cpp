#include "FireParticleGenerator.h"
#include "Distribution.h"
#include "Particle.h"

FireParticleGenerator::FireParticleGenerator(Vector3D origin)
	: ParticleGenerator("FuegoYChispas", origin, 4, 0.02),
	  m_colorGen(std::random_device{}()),
	  m_colorVar(0.0f, 1.0f)
{
	/*m_velDist = std::make_unique<GaussianVectorDistribution>(
		Vector3D(0.0f, 8.0f, 0.0f),
		Vector3D(1.2f, 1.5f, 1.2f)
	);*/

	m_velDist = std::make_unique<Vector3Distribution>(
		new GaussianDistribution(0.0f, 1.2f),
		new GaussianDistribution(8.0f, 1.5f),
		new GaussianDistribution(0.0f, 1.2f));

	m_radius = 0.2f;
	m_lifetime = 1.6f;
	m_damping = 0.96f;
}

FireParticleGenerator::~FireParticleGenerator() = default;

Particle* FireParticleGenerator::generateSingleParticle()
{
	Vector3D vel = m_velDist->sample();

	// Para que vaya cambiando entre rojo naranja y amarillo
	float green = 0.2f + 0.6f * m_colorVar(m_colorGen);
	Vector4 fireColor(1.0f, green, 0.05f, 1.0f);

	return new Particle(
		m_mass,
		m_origin,
		vel,
		Vector3D::zero(),
		m_damping,
		m_radius,
		fireColor,
		m_lifetime
	);
}
