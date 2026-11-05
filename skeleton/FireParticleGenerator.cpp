#include "FireParticleGenerator.h"
#include "Vector3DDistribution.h"
#include "Particle.h"

FireParticleGenerator::FireParticleGenerator(Vector3D origin)
	: ParticleGenerator("FuegoYChispas", origin, 4, 0.02),
	m_meanVel(0.0f, 8.0f, 0.0f),
	m_stdDevVel(1.2f, 1.5f, 1.2f),
	m_posStdDev(0.5f, 0.05f, 0.5f)
{
	m_stdDevVel = clampComponents(m_stdDevVel, 0.001f);
	m_posStdDev = clampComponents(m_posStdDev, 0.001f);
	rebuildDistribution();

	// Mantiene el color naranja/amarillo que tenía el generador original.
	m_color = Vector4(1.0f, 0.5f, 0.05f, 1.0f);
	m_colorMode = ParticleColorMode::DISTRIBUTION;
	m_colorMin = Vector4(1.0f, 0.2f, 0.05f, 1.0f);
	m_colorMax = Vector4(1.0f, 0.8f, 0.05f, 1.0f);
	setRadius(0.2f);
	setLifetime(1.6f);
	setDamping(0.96f);
}

FireParticleGenerator::~FireParticleGenerator() = default;

void FireParticleGenerator::rebuildDistribution()
{
	m_velDist = std::make_unique<GaussianVectorDistribution>(m_meanVel, m_stdDevVel);
	m_posDist = std::make_unique<GaussianVectorDistribution>(Vector3D::zero(), m_posStdDev);
}

Particle* FireParticleGenerator::generateSingleParticle(Particle* reusable)
{
	const Vector3D pos = m_origin + m_posDist->sample();
	const Vector3D vel = m_velDist->sample();
	return spawnFromModel(pos, vel, reusable);
}