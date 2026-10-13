#pragma once
#include "Particle.h"
#include <string>

#include <utility>

// Struct que guarda la información de cada tipo de proyectil
// Permite calcular los valores simulados (masa y gravedad)
// dependiendo de la velocidad real y la simulada
struct ProjectileConfig
{
    std::string name = "Default";
    float realMass = 1.0f;       // kg
    float realSpeed = 10.0f;     // m/s
    float simSpeed = 10.0f;      // m/s
    float realGravity = -9.81f;  // m/s^2
    float damping = 0.99f;
    float radius = 0.5f;         // radio para el RenderItem
    Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // Color de la partícula
    float maxLifetime = 15.0f;   // Tiempo de vida máximo en segundos
    // Ratio entre la velocidad real y la simulada
    float ratio = 1.0f;

    ProjectileConfig() = default;

    ProjectileConfig(std::string name_, float realMass_, float realSpeed_, float simSpeed_,
                     float realGravity_, float damping_, float radius_, Vector4 color_,
                     float maxLifetime_ = 15.0f)
        : name(std::move(name_)), realMass(realMass_), realSpeed(realSpeed_), simSpeed(simSpeed_),
          realGravity(realGravity_), damping(damping_), radius(radius_), color(color_),
          maxLifetime(maxLifetime_)
    {
        ratio = (simSpeed > 0.0f) ? (realSpeed / simSpeed) : 1.0f;
    }

    // Calcula el ratio actual entre velocidad real y simulada
    float getRatio() const
    {
        if (simSpeed <= 0.0f) return 1.0f;
        return realSpeed / simSpeed;
    }

    float getSimMass() const
    {
        float r = getRatio();
        return realMass * (r * r);
    }

    float getSimGravity() const
    {
        float r = getRatio();
        if (r <= 0.0f) return realGravity;
        return realGravity / (r * r);
    }

    void changeRealSpeed(float newSpeed)
    {
        realSpeed = newSpeed;
        ratio = (simSpeed > 0.0f) ? (realSpeed / simSpeed) : 1.0f;
    }

    void changeSimSpeed(float newSimSpeed)
    {
        simSpeed = newSimSpeed;
        ratio = (simSpeed > 0.0f) ? (realSpeed / simSpeed) : 1.0f;
    }
};

class Projectile : public Particle
{
public:
    Projectile(float Mass, Vector3D Pos, Vector3D Vel, Vector3D Acc,
               Vector3D Gravity = { 0.0f, -9.81f, 0.0f }, float Damping = 1.0f,
               float radius = 0.5f, Vector4 color = Vector4(1.0f, 1.0f, 0.0f, 1.0f),
               float maxLife = 15.0f);
    virtual ~Projectile() override;

    // Crea y dispara un proyectil desde la posición y orientación de la cámara 
    static Projectile* createFromCamera(const ProjectileConfig& config, Camera* camera);
};

