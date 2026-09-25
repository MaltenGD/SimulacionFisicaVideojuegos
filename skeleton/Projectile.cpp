#include "Projectile.h"

Projectile::Projectile(float Mass, Vector3D Pos, Vector3D Vel, Vector3D Acc, Vector3D Gravity, float Damping) :
	Particle(Mass, Pos, Vel, Acc, Gravity, Damping)
{

}

Projectile::~Projectile()
{

}
