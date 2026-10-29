#include "FountainParticleGenerator.h"
#include "Distribution.h"
#include "Particle.h"
FountainParticleGenerator::FountainParticleGenerator(Vector3D origin, Vector3D meanVel, Vector3D stdDevVel, Vector3D fixedGravity)
	: ParticleGenerator("FuenteDeAgua", origin, 3, 0.03)
{

	m_velDist = std::make_unique<Vector3Distribution>(
		new GaussianDistribution(meanVel.x, stdDevVel.x),
		new GaussianDistribution(meanVel.y, stdDevVel.y),
		new GaussianDistribution(meanVel.z, stdDevVel.z));
	m_color = Vector4(0.2f, 0.6f, 1.0f, 1.0f); // Azul	
	m_radius = 0.25f;
	m_lifetime = 4.0f;
	m_fixedGravity = fixedGravity;
}

FountainParticleGenerator::~FountainParticleGenerator() = default;

Particle* FountainParticleGenerator::generateSingleParticle()
{
	Vector3D vel = m_velDist->sample();
	return new Particle(
		m_mass,
		m_origin,
		vel,
		m_fixedGravity, // La gravedad se aplica mediante la aceleración.
		m_damping,
		m_radius,
		m_color,
		m_lifetime
	);
}
