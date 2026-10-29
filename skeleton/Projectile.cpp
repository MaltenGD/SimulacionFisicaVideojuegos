#include "Projectile.h"

Projectile::Projectile(float Mass, Vector3D Pos, Vector3D Vel, Vector3D Acc, Vector3D Gravity, float Damping, float radius, Vector4 color, float maxLife) :
	OldParticle(Mass, Pos, Vel, Acc, Gravity, Damping, radius, color, maxLife)
{
}

Projectile::~Projectile()
{
}

Projectile* Projectile::createFromCamera(const ProjectileConfig& config, Camera* camera)
{
	// Vector dirección hacia donde mira la cámara
	Vector3D forward = Vector3D(camera->getDir()).normalize();

	// Para que la bala no aparezca justo en la cámara y la atraviese al inicio:
	// El proyectil aparece ligeramente adelantado a la cámara.
	Vector3D spawnPos = Vector3D(camera->getEye()) + forward * 1.5f;

	Vector3D initVel = forward * config.simSpeed;
	Vector3D initGrav = Vector3D(0.0f, config.getSimGravity(), 0.0f);

	float simMass = config.getSimMass();

	return new Projectile(simMass, spawnPos, initVel, Vector3D(0.0f, 0.0f, 0.0f), initGrav, config.damping, config.radius, config.color, config.maxLifetime);
}
