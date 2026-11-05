#include "FogParticleGenerator.h"
#include "Vector3DDistribution.h"
#include "Particle.h"

FogParticleGenerator::FogParticleGenerator(Vector3D minBox, Vector3D maxBox)
	: ParticleGenerator("Niebla", (minBox + maxBox) * 0.5f, 2, 0.08)
{
	setBounds(minBox, maxBox);

	m_color = Vector4(0.85f, 0.85f, 0.9f, 0.5f);
	m_colorMin = m_color;
	m_colorMax = m_color;
	setRadius(0.2f);
	setLifetime(6.0f);
	setDamping(0.98f);
}

FogParticleGenerator::~FogParticleGenerator() = default;

void FogParticleGenerator::setBounds(const Vector3D& minBox, const Vector3D& maxBox)
{
	m_minBox = minBox;
	m_maxBox = m_minBox + clampComponents(maxBox - minBox, 0.1f);
	m_origin = (m_minBox + m_maxBox) * 0.5f;
	rebuildDistribution();
}

void FogParticleGenerator::setOrigin(const Vector3D& pos)
{
	Vector3D delta = pos - m_origin;
	m_origin = pos;
	m_minBox += delta;
	m_maxBox += delta;
	rebuildDistribution();
}

void FogParticleGenerator::rebuildDistribution()
{

	m_posDist = std::make_unique<UniformVectorDistribution>(m_minBox, m_maxBox);
	m_velDist = std::make_unique<UniformVectorDistribution>(
		Vector3D(-0.4f, -0.05f, -0.4f),
		Vector3D(0.4f, 0.15f, 0.4f)
	);
}

Particle* FogParticleGenerator::generateSingleParticle(Particle* reusable)
{
	// Usa distribucion uniforme
	const Vector3D pos = m_posDist->sample();
	const Vector3D vel = m_velDist->sample();
	return spawnFromModel(pos, vel, reusable);
}