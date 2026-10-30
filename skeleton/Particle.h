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

    void integrate_Direct(double dt);
    void integrate_Semi_Implicit(double dt);
    void cleanup();

    virtual bool isAlive() const { return pose.p.y > -100.0f && lifetime < maxLifetime; }

    void addForce(const Vector3D& f) { force += f; }
    Vector3D getForce() const { return force; }
    void clearForce() { force = Vector3D::zero(); }

    float getMass() const { return massInverse > 0.0f ? 1.0f / massInverse : 0.0f; }
    float getMassInverse() const { return massInverse; }
    void setMass(float m) { massInverse = (m > 0.0f ? 1.0f / m : 0.0f); }

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
    float maxLifetime{ 10.0f };
};
