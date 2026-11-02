#pragma once

#include "Vector3D.h"
#include "RenderUtils.hpp"

class Particle
{
public:
    Particle(float mass, Vector3D pos, Vector3D vel, Vector3D acc = Vector3D::zero(),
             float damping = 0.99f, float radius = 0.35f,
             Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f),
             float maxLife = 10.0f);
    virtual ~Particle();

    void integrate_Direct(double t);
    void integrate_Semi_Implicit(double t);
    void reset(float Mass, Vector3D Pos, Vector3D Vel, Vector3D Acc,
               float Damping, float radius, Vector4 color,
               float maxLife);
    void deactivate();
    void setScaleWithLifetime(bool enabled);
    void cleanup();
    virtual bool isAlive() const { return m_alive && pose.p.y > -100.0f && lifetime < maxLifetime; }
    void addForce(const Vector3D& forceToAdd) { force += forceToAdd; }
    Vector3D getForce() const { return force; }
    void clearForce() { force = Vector3D::zero(); }

    float getMass() const { return massInverse > 0.0f ? 1.0f / massInverse : 0.0f; }
    float getMassInverse() const { return massInverse; }
    void setMass(float mass) { massInverse = mass > 0.0f ? 1.0f / mass : 0.0f; }

    Vector3D getPosition() const { return pose.p; }
    void setPosition(const Vector3D& p) { pose.p = p; }

    Vector3D getVelocity() const { return vel; }
    void setVelocity(const Vector3D& v) { vel = v; }

    Vector3D getAcceleration() const { return acc; }
    void setAcceleration(const Vector3D& a) { acc = a; }

    float getDamping() const { return damping; }
    void setDamping(float d) { damping = d; }

    void setMaxLifetime(float maxLife) { maxLifetime = maxLife; }
    float getMaxLifetime() const { return maxLifetime; }
    float getLifetime() const { return lifetime; }

protected:
    float massInverse;
    physx::PxTransform pose;
    RenderItem* renderItem{ nullptr };
    Vector3D vel;
    Vector3D acc;
    Vector3D force;
    float damping;
    float lifetime{ 0.0f };
    float maxLifetime{ 15.0f };
    bool m_alive{ true };
    bool m_scaleWithLifetime{ false };

    void updateVisualScale();
};
