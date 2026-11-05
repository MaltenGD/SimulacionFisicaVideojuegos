#include "Projectile.h"
#include <cmath>

Projectile::Projectile(ProjectileType type,
                       const Vector3D& pos,
                       const Vector3D& dir,
                       float realMass,
                       float realSpeed,
                       float simSpeed,
                       float radius,
                       const Vector4& color,
                       float lifetime,
                       float realGravity)
	: Particle(1.0f, pos, Vector3D::zero(), Vector3D::zero(), 0.999f, radius, color, lifetime),
	  m_type(type),
	  m_realMass(realMass),
	  m_realSpeed(realSpeed),
	  m_simSpeed(simSpeed),
	  m_realGravity(realGravity),
	  m_scaledGravity(9.8f)
{
	// Dirección normalizada
	Vector3D normalizedDir = dir;
	float mag = normalizedDir.magnitude();
	if (mag > 0.0001f)
	{
		normalizedDir = normalizedDir.normalize();
	}
	else
	{
		normalizedDir = Vector3D(0.0f, 0.0f, -1.0f);
	}

	// Calcula m_scaledMass y m_scaledGravity
	applyScaling(realMass, realSpeed, simSpeed, realGravity);

	// Velocidad simulada inicial en la dirección del disparo
	vel = normalizedDir * m_simSpeed;
}

void Projectile::applyScaling(float realMass, float realSpeed, float simSpeed, float realGravity)
{
	m_realMass = realMass > 0.0001f ? realMass : 0.0001f;
	m_realSpeed = realSpeed > 0.01f ? realSpeed : 0.01f;
	m_simSpeed = simSpeed > 0.01f ? simSpeed : 0.01f;
	m_realGravity = realGravity;

	// Relación de velocidades v_real / v_sim
	const float speedRatio = m_realSpeed / m_simSpeed;

	// Se conserva la energía cinética
	const float scaledMass = m_realMass * (speedRatio * speedRatio);
	setMass(scaledMass);

	// Se conserva la trayectoria parabólica para que el arco parabólico sea idéntico en el espacio
	m_scaledGravity = m_realGravity / (speedRatio * speedRatio);

	// Se aplica la gravedad directamente como aceleración intrínseca de la partícula.
	// para que la partícula sea 100% independiente de que exista o no un generador
	// de fuerzas de gravedad externo en la escena.
	setAcceleration(Vector3D(0.0f, -m_scaledGravity, 0.0f));
}

std::string Projectile::getTypeName() const
{
	switch (m_type)
	{
	case ProjectileType::PISTOL_BULLET:
		return "Bala de Pistola";
	case ProjectileType::CANNONBALL:
		return "Bala de Canon";
	case ProjectileType::ARTILLERY:
		return "Proyectil de Artilleria";
	default:
		return "Proyectil";
	}
}

float Projectile::getKineticEnergy() const
{
	const float currentSpeed = vel.magnitude();
	return 0.5f * getMass() * (currentSpeed * currentSpeed);
}

float Projectile::getTheoreticalKineticEnergy() const
{
	return 0.5f * m_realMass * (m_realSpeed * m_realSpeed);
}

Projectile* Projectile::createPistolBullet(const Vector3D& pos, const Vector3D& dir, float simSpeed)
{
	// Masa real: 0.008 kg (8 gramos)
	// Velocidad de real: 380 m/s
	// Velocidad simulada: 70 m/s
	return new Projectile(
		ProjectileType::PISTOL_BULLET,
		pos,
		dir,
		0.008f,    // realMass (kg)
		380.0f,    // realSpeed (m/s)
		simSpeed,  // simSpeed (m/s)
		0.2f,      // radius (m)
		Vector4(1.0f, 0.85f, 0.2f, 1.0f), // amarillo
		8.0f       // lifetime (s)
	);
}

Projectile* Projectile::createCannonball(const Vector3D& pos, const Vector3D& dir, float simSpeed)
{
	// Masa real: 5.4 kg
	// Velocidad de salida real: 80 m/s
	// Velocidad simulada: 30 m/s
	return new Projectile(
		ProjectileType::CANNONBALL,
		pos,
		dir,
		5.4f,      // realMass (kg)
		80.0f,     // realSpeed (m/s)
		simSpeed,  // simSpeed (m/s)
		0.5f,      // radius (m)
		Vector4(0.2f, 0.2f, 0.25f, 1.0f), // gris plomo
		12.0f      // lifetime (s)
	);
}

Projectile* Projectile::createArtillery(const Vector3D& pos, const Vector3D& dir, float simSpeed)
{
	// Masa real: 45 kg
	// Velocidad de salida real: 250 m/s
	// Velocidad simulada: 45 m/s
	return new Projectile(
		ProjectileType::ARTILLERY,
		pos,
		dir,
		45.0f,     // realMass (kg)
		250.0f,    // realSpeed (m/s)
		simSpeed,  // simSpeed (m/s)
		0.65f,     // radius (m)
		Vector4(0.85f, 0.35f, 0.1f, 1.0f), // naranja rojizo
		15.0f      // lifetime (s)
	);
}
