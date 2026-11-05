#pragma once

#include "Vector3D.h"
#include "RenderUtils.hpp"

enum class ParticleShape { SPHERE, BOX };

class ForceGenerator;

class Particle
{
public:
    Particle(float mass, Vector3D pos, Vector3D vel, Vector3D acc = Vector3D::zero(),
        float damping = 0.99f, float radius = 0.35f,
        Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        float maxLife = 10.0f,
        ParticleShape shape = ParticleShape::SPHERE,
        Vector3D boxHalfExtents = Vector3D(0.35f, 0.35f, 0.35f));
    virtual ~Particle();

    void integrate(double dt);
    void integrate_Direct(double t);
    void integrate_Semi_Implicit(double t);
    // Reutiliza una partícula de la pool con los parámetros actuales del generador.
    void reset(float Mass, Vector3D Pos, Vector3D Vel, Vector3D Acc,
        float Damping, float radius, Vector4 color,
        float maxLife);
    // Cambia el tamaño visual; la geometría original se mantiene.
    void setRadius(float radius);
    float getRadius() const { return m_radius; }
    void deactivate();
    void setScaleWithLifetime(bool enabled);
    void cleanup();

    // La particula es quien invoca al generador de fuerza con su propia informacion.
    void receiveForcesFrom(ForceGenerator& generator);
    // Los proyectiles llevan la gravedad escalada integrada y ignoran los generadores de gravedad.
    virtual bool ignoresGravityGenerators() const { return false; }
    // Copia independiente (con su propio RenderItem) en la posicion y velocidad dadas.
    // Se usa para crear particulas a partir de la particula modelo de un generador (solo esferas).
    Particle* clone(const Vector3D& pos, const Vector3D& vel) const;
    Vector4 getColor() const;
    void setColor(const Vector4& color);
    virtual bool isAlive() const { return m_alive && pose.p.y > -100.0f && lifetime < maxLifetime; }
    void addForce(const Vector3D& forceToAdd) { force += forceToAdd; }
    Vector3D getForce() const { return force; }
    void clearForce() { force = Vector3D::zero(); }

    float getMass() const { return massInverse > 0.0f ? 1.0f / massInverse : 0.0f; }
    float getMassInverse() const { return massInverse; }
    void setMass(float mass) { massInverse = mass > 0.0f ? 1.0f / mass : 0.0f; }

    Vector3D getPosition() const { return pose.p; }
    void setPosition(const Vector3D& p) { pose.p = p; }
    Vector3D getPos() const { return pose.p; }
    void setPos(const Vector3D& p) { pose.p = p; }

    Vector3D getVelocity() const { return vel; }
    void setVelocity(const Vector3D& v) { vel = v; }

    Vector3D getAcceleration() const { return acc; }
    void setAcceleration(const Vector3D& a) { acc = a; }

    float getDamping() const { return damping; }
    void setDamping(float d) { damping = d; }

    void setMaxLifetime(float maxLife) { maxLifetime = maxLife >= 0.01f ? maxLife : 0.01f; }
    float getMaxLifetime() const { return maxLifetime; }
    float getLifetime() const { return lifetime; }

    void setIntegrationMethod(bool semiImplicit) { useSemiImplicit = semiImplicit; }
    bool isSemiImplicit() const { return useSemiImplicit; }

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
    float m_radius{ 0.35f };
    // Radio de la geometría original; el radio actual se aplica como escala.
    float m_baseRadius{ 0.35f };
    ParticleShape m_shape{ ParticleShape::SPHERE };
    bool m_alive{ true };
    bool m_scaleWithLifetime{ false };
    bool useSemiImplicit{ true };

    void updateVisualScale();
};