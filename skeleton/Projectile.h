#pragma once
#include "Particle.h"
class Projectile :
    public Particle
{
public:
	Projectile(float Mass, Vector3D Pos, Vector3D Vel, Vector3D Acc, Vector3D Gravity = { 0.0f, -9.81f, 0.0f }, float Damping = 1.0f);
	~Projectile();

	void shoot(const physx::PxTransform& camera);

private:
	void shoot(Vector3D initPos, Vector3D initVel, Vector3D initAcc, const physx::PxTransform& camera);
};

