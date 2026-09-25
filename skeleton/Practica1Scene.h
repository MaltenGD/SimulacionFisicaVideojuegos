#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include <vector>
#include "Vector3D.h"
#include "Particle.h"
#include "Projectile.h"
#include "Practica0Axes.h"

class Practica1Scene : public Scene {
public:
    explicit Practica1Scene(std::string name);

    void init() override;

    void update(double dt) override;

    void keyPress(unsigned char key, const physx::PxTransform& camera) override;

    void cleanup() override;

private:
    Practica0Axes* m_axes = nullptr;
    Particle* miParticula = nullptr;
	Projectile* miProyectil = nullptr;
};