#pragma once
#include "Particle.h"
class Projectile :
    public Particle
{
	Projectile(float Mass, Vector3D Pos, Vector3D Vel, Vector3D Acc, Vector3D Gravity = { 0.0f, -9.81f, 0.0f }, float Damping = 1.0f);
	~Projectile();
};

