#pragma once

#include "Vector3D.h"
#include "RenderUtils.hpp"
#include <vector>

class Particle
{
public:
    Particle(float Mass, Vector3D Pos, Vector3D Vel, Vector3D Acc,
             float Damping = 1.0f,
             float radius = 1.0f, Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f),
             float maxLife = 15.0f); // fixedGravity solo se usa para la practica 2
    virtual ~Particle();

    void integrate_Direct(double t);
    void integrate_Semi_Implicit(double t);
    void cleanup();
    virtual bool isAlive() const { return pose.p.y > -100.0f && lifetime < maxLifetime; };
    void addForce(Vector3D Force);
    Vector3D getForce();

    float getMass() const { return massInverse > 0.0f ? 1.0f / massInverse : 0.0f; }
    float getMassInverse() const { return massInverse; }
    void setMass(float mass) { massInverse = (mass > 0.0f ? 1.0f / mass : 0.0f); }

    Vector3D getPosition() const { return pose.p; }
    Vector3D getVelocity() const { return vel; }

    void setMaxLifetime(float maxLife) { maxLifetime = maxLife; }
    float getMaxLifetime() const { return maxLifetime; }

    virtual std::vector<Particle*> explode() { return {}; }

protected:
    float massInverse;
    physx::PxTransform pose; // Lo usa RenderItem
    RenderItem* renderItem;
    Vector3D vel;
    Vector3D acc;
    Vector3D force;
    float damping;
    float lifetime{ 0.0f };
    float maxLifetime{ 15.0f };
    Vector3D m_fixedGravity;
};
