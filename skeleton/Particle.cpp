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
      maxLifetime(maxLife),
      m_alive(true)
{}

Particle::~Particle()
{
    cleanup();
}

void Particle::integrate_Direct(double dt)
{
    if (!isAlive())
    {
        deactivate();
        return;
    }

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
    updateVisualScale();
}

void Particle::integrate_Semi_Implicit(double dt)
{
    if (!isAlive())
    {
        deactivate();
        return;
    }

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
    updateVisualScale();
}

void Particle::reset(float Mass, Vector3D Pos, Vector3D Vel, Vector3D Acc,
                     float Damping, float radius, Vector4 color,
                     float maxLife)
{
    massInverse = Mass > 0.0f ? 1.0f / Mass : 0.0f;
    pose = physx::PxTransform(Pos);
    vel = Vel;
    acc = Acc;
    force = Vector3D::zero();
    damping = Damping;
    lifetime = 0.0f;
    maxLifetime = maxLife;
    m_alive = true;
    if (renderItem)
    {
        renderItem->visible = true;
    }

    if (renderItem)
    {
        renderItem->color = color;
    }
    updateVisualScale();
}

void Particle::deactivate()
{
    m_alive = false;
    if (renderItem)
    {
        renderItem->visible = false;
        renderItem->scale = 0.0f;
    }
}

void Particle::setScaleWithLifetime(bool enabled)
{
    m_scaleWithLifetime = enabled;
    updateVisualScale();
}

void Particle::updateVisualScale()
{
    if (!renderItem)
    {
        return;
    }

    if (!m_alive)
    {
        renderItem->visible = false;
        renderItem->scale = 0.0f;
        return;
    }

    renderItem->visible = true;
    if (!m_scaleWithLifetime || maxLifetime <= 0.0f)
    {
        renderItem->scale = 1.0f;
        return;
    }

    const float lifeRatio = lifetime / maxLifetime;
    renderItem->scale = std::fmax(0.0f, std::fmin(1.0f, 1.0f - lifeRatio));
}

void Particle::cleanup()
{
    if (renderItem) {
        renderItem->release();
        renderItem = nullptr;
    }
}
