#pragma once
#include "Vector3D.h"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <queue>

class Particle
{
public:
    Particle(float Mass,Vector3D Pos, Vector3D Vel, Vector3D Acc, Vector3D Gravity = { 0.0f, -9.81f, 0.0f }, float Damping=1);
    ~Particle();

    void integrate_Direct(double t);
    void integrate_Semi_Implicit(double t);
    void cleanup();
    virtual bool isAlive() const { return pose.p.y > -100.0f; };
    void addForce(Vector3D Force);
    Vector3D getForce();

    

private:
	float massInverse;
    physx::PxTransform pose; // render item utiliza esta pose.
    Vector3D vel;
    Vector3D acc;
    Vector3D gravity;
    Vector3D force;
    float damping;

    RenderItem* renderItem;
};
