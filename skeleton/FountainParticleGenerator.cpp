#include "FountainParticleGenerator.h"
#include "Vector3DDistribution.h"
#include "Particle.h"

FountainParticleGenerator::FountainParticleGenerator(Vector3D origin, Vector3D meanVel, Vector3D stdDevVel, float particleMass, Vector3D posStdDev)
	: ParticleGenerator("FuenteDeAgua", origin, 3, 0.03),
	m_meanVel(meanVel),
	m_stdDevVel(stdDevVel),
	m_posStdDev(posStdDev)
{
	m_stdDevVel = clampComponents(m_stdDevVel, 0.001f);
	m_posStdDev = clampComponents(m_posStdDev, 0.001f);
	rebuildDistribution();
	m_color = Vector4(0.2f, 0.6f, 1.0f, 1.0f);
	m_colorMin = m_color;
	m_colorMax = m_color;
	setRadius(0.25f);
	setLifetime(6.0f);
	setDamping(0.99f);
	setMass(particleMass);
}

FountainParticleGenerator::~FountainParticleGenerator() = default;

void FountainParticleGenerator::rebuildDistribution()
{
	m_velDist = std::make_unique<GaussianVectorDistribution>(m_meanVel, m_stdDevVel);
	m_posDist = std::make_unique<GaussianVectorDistribution>(Vector3D::zero(), m_posStdDev);
}

Particle* FountainParticleGenerator::generateSingleParticle(Particle* reusable)
{
	const Vector3D pos = m_origin + m_posDist->sample();
	const Vector3D vel = m_velDist->sample();
	return spawnFromModel(pos, vel, reusable);
}