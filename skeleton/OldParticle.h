#pragma once

#include "Particle.h"
#include "Vector3D.h"

// Partícula del sistema antiguo: incluye una aceleración gravitatoria propia hardcodeada.
// Usada en Practica1Scene y como base de Projectile.
// A partir de Practica2, la gravedad es gestionada por GravityForceGenerator.
class OldParticle : public Particle
{
public:
    OldParticle(float Mass, Vector3D Pos, Vector3D Vel, Vector3D Acc,
                Vector3D Gravity = { 0.0f, -9.81f, 0.0f },
                float Damping = 1.0f,
                float radius = 1.0f,
                Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f),
                float maxLife = 15.0f);

    virtual ~OldParticle() override = default;

    void integrate_Direct(double t);
    void integrate_Semi_Implicit(double t);

    void setGravity(const Vector3D& g) { m_gravity = g; }
    Vector3D getGravity() const { return m_gravity; }

protected:
    Vector3D m_gravity;
};
