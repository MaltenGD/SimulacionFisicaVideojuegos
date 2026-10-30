#include "Particle.h"
#include <cmath>

Particle::Particle(float mass, Vector3D pos, Vector3D vel, Vector3D acc,
                   float damping, float radius, Vector4 color, float maxLife)
    : massInverse(mass > 0.0f ? 1.0f / mass : 0.0f),
      pose(physx::PxTransform(pos)),
      renderItem(new RenderItem(
          CreateShape(physx::PxSphereGeometry(radius)),
          &pose,
          color)),
      vel(vel),
      acc(acc),
      force(0.0f, 0.0f, 0.0f),
      damping(damping),
      lifetime(0.0f),
      maxLifetime(maxLife)
{
}

Particle::~Particle()
{
    cleanup();
}

void Particle::integrate_Direct(double dt)
{
    Vector3D netAcc = acc + force * massInverse;
    pose.p += physx::PxVec3(vel) * static_cast<float>(dt);
    vel += netAcc * static_cast<float>(dt);
    if (damping > 0.0f) {
        vel = vel * std::pow(damping, static_cast<float>(dt));
    } else {
        vel = Vector3D::zero();
    }
    force = Vector3D::zero();
    lifetime += static_cast<float>(dt);
}

void Particle::integrate_Semi_Implicit(double dt)
{
    Vector3D netAcc = acc + force * massInverse;
    vel += netAcc * static_cast<float>(dt);
    if (damping > 0.0f) {
        vel = vel * std::pow(damping, static_cast<float>(dt));
    } else {
        vel = Vector3D::zero();
    }
    pose.p += physx::PxVec3(vel) * static_cast<float>(dt);
    force = Vector3D::zero();
    lifetime += static_cast<float>(dt);
}

void Particle::cleanup()
{
    if (renderItem) {
        renderItem->release();
        renderItem = nullptr;
    }
}
