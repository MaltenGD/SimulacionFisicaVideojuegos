#pragma once

#include "Particle.h"
#include "Vector3D.h"
#include <string>

// Identifica los distintos tipos de proyectiles balísticos disponibles.
enum class ProjectileType
{
	PISTOL_BULLET,
	CANNONBALL,
	ARTILLERY
};

// Clase Projectile: representa una particula con escalado cinético y de trayectoria propia.
// Tiene una velocidad simulada modificable
// Se escala la masa para conservar la energia cinetica
// Se escala la gravedad para mantener la misma curvatura
// No depende de generadores de fuerza (tiene su propia gravedad mediante aceleración fija)
class Projectile : public Particle
{
public:
	Projectile(ProjectileType type,
	           const Vector3D& pos,
	           const Vector3D& dir,
	           float realMass,
	           float realSpeed,
	           float simSpeed,
	           float radius = 0.25f,
	           const Vector4& color = Vector4(1.0f, 0.8f, 0.2f, 1.0f),
	           float lifetime = 12.0f,
	           float realGravity = 9.8f);

	virtual ~Projectile() override = default;

	// Calcula y actualiza el escalado de masa y gravedad a partir de las velocidades.
	void applyScaling(float realMass, float realSpeed, float simSpeed, float realGravity = 9.8f);

	// Getters de magnitudes reales y simuladas
	ProjectileType getType() const { return m_type; }
	std::string getTypeName() const;

	float getRealMass() const { return m_realMass; }
	float getRealSpeed() const { return m_realSpeed; }
	float getSimSpeed() const { return m_simSpeed; }
	float getScaledGravity() const { return m_scaledGravity; }
	float getRealGravity() const { return m_realGravity; }

	// Energía cinética simulada actual vs real teórica (deben coincidir)
	float getKineticEnergy() const;
	float getTheoreticalKineticEnergy() const;

	// Metodos factory para instanciar algunos tipos de proyectiles
	static Projectile* createPistolBullet(const Vector3D& pos, const Vector3D& dir, float simSpeed = 70.0f);
	static Projectile* createCannonball(const Vector3D& pos, const Vector3D& dir, float simSpeed = 30.0f);
	static Projectile* createArtillery(const Vector3D& pos, const Vector3D& dir, float simSpeed = 45.0f);

private:
	ProjectileType m_type;
	float m_realMass;
	float m_realSpeed;
	float m_simSpeed;
	float m_realGravity;
	float m_scaledGravity;
};
