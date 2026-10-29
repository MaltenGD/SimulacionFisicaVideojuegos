#pragma once

#include "Vector3D.h"
#include "RenderUtils.hpp"

class Particle
{
public:
    Particle(float Mass, Vector3D Pos, Vector3D Vel, Vector3D Acc,
             Vector3D Gravity = { 0.0f, -9.81f, 0.0f }, float Damping = 1.0f,
             float radius = 1.0f, Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f),
             float maxLife = 15.0f);
    virtual ~Particle();

    void integrate_Direct(double t);
    void integrate_Semi_Implicit(double t);
    void cleanup();
    virtual bool isAlive() const { return pose.p.y > -100.0f && lifetime < maxLifetime; };
    void addForce(Vector3D Force);
    Vector3D getForce();

    Vector3D getPosition() const { return pose.p; }
    Vector3D getVelocity() const { return vel; }

    void setMaxLifetime(float maxLife) { maxLifetime = maxLife; }
    float getMaxLifetime() const { return maxLifetime; }

protected:
    float massInverse;
    physx::PxTransform pose; // Lo usa RenderItem
    RenderItem* renderItem;
    Vector3D vel;
    Vector3D acc;
    Vector3D gravity;
    Vector3D force;
    float damping;
    float lifetime{ 0.0f };
    float maxLifetime{ 15.0f };
};
