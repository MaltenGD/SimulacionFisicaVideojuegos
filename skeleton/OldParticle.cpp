#include "OldParticle.h"
#include <cmath>

OldParticle::OldParticle(float Mass, Vector3D Pos, Vector3D Vel, Vector3D Acc,
                         Vector3D Gravity, float Damping, float radius, Vector4 color, float maxLife)
    : Particle(Mass, Pos, Vel, Acc, Damping, radius, color, maxLife),
      m_gravity(Gravity)
{
}

void OldParticle::integrate_Direct(double t)
{
    Vector3D netAcc = acc + m_gravity + force * massInverse;
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

void OldParticle::integrate_Semi_Implicit(double t)
{
    // Integración Semi-implícita:
    Vector3D netAcc = acc + m_gravity + force * massInverse;
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
