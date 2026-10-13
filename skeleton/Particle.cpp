#include "Particle.h"
#include <cmath>

Particle::Particle(float Mass, Vector3D Pos, Vector3D Vel, Vector3D Acc, Vector3D Gravity, float Damping, float radius, Vector4 color, float maxLife) :
    massInverse(Mass > 0.0f ? 1.0f / Mass : 0.0f),
    pose(physx::PxTransform(Pos)),
    renderItem(new RenderItem(
               CreateShape(physx::PxSphereGeometry(radius)),
                &pose,
                color)),
    vel(Vel),
    acc(Acc),
    gravity(Gravity),
    force(0.0f, 0.0f, 0.0f),
    damping(Damping),
    lifetime(0.0f),
    maxLifetime(maxLife)
{}

Particle::~Particle()
{
	cleanup();
}

void Particle::integrate_Direct(double t)
{
    Vector3D netAcc = acc + gravity + force * massInverse;
    pose.p += physx::PxVec3(vel) * t;
    vel += netAcc * t;
    if (damping > 0.0f) {
        vel = vel * std::pow(damping, t);
    } else {
        vel = Vector3D::zero();
    }
    force = Vector3D(0.0f, 0.0f, 0.0f);
    lifetime += t;
}

void Particle::integrate_Semi_Implicit(double t)
{
    // Integración Semi-implícita:
    Vector3D netAcc = acc + gravity + force * massInverse;
    vel += netAcc * t;
    if (damping > 0.0f) {
        vel = vel * std::pow(damping, t);
    } else {
        vel = Vector3D::zero();
    }
    pose.p += physx::PxVec3(vel) * t;
    force = Vector3D(0.0f, 0.0f, 0.0f);
    lifetime += t;
}


void Particle::cleanup()
{
    if (renderItem) {
        renderItem->release(); // Deregistra y destruye el item
        renderItem = nullptr;
    }
}
Vector3D Particle::getForce()
{
    return force;
}

void Particle::addForce(Vector3D Force)
{
    force += Force;
}
