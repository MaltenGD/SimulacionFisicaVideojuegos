#include "FogParticleGenerator.h"
#include "Vector3DDistribution.h"
#include "Particle.h"

FogParticleGenerator::FogParticleGenerator(Vector3D minBox, Vector3D maxBox)
	: ParticleGenerator("Niebla", (minBox + maxBox) * 0.5f, 2, 0.08),
	  m_minBox(minBox),
	  m_maxBox(maxBox)
{
	rebuildDistribution();

	m_color = Vector4(0.85f, 0.85f, 0.9f, 0.5f);
	m_radius = 0.2f;
	m_lifetime = 6.0f;
	m_damping = 0.98f;
}

FogParticleGenerator::~FogParticleGenerator() = default;

void FogParticleGenerator::rebuildDistribution()
{
	m_posDist = std::make_unique<UniformVectorDistribution>(m_minBox, m_maxBox);
	m_velDist = std::make_unique<UniformVectorDistribution>(
		Vector3D(-0.4f, -0.05f, -0.4f),
		Vector3D( 0.4f,  0.15f,  0.4f)
	);
}

Particle* FogParticleGenerator::generateSingleParticle(Particle* reusable)
{
	Vector3D pos = m_posDist->sample();
	Vector3D vel = m_velDist->sample();

	if (reusable)
	{
		reusable->reset(
			m_mass,
			pos,
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
		pos,
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
