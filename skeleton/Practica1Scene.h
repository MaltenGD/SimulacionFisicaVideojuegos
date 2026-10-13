#pragma once

#include "Practica0Scene.h"
#include "Projectile.h"
#include <vector>

class Practica1Scene : public Practica0Scene {
public:
    explicit Practica1Scene(std::string name);
    virtual ~Practica1Scene() override;

    void init() override;
    void update(double dt) override;
    void keyPress(unsigned char key, Camera* camera) override;
    void cleanup() override;

private:
    void initConfigs();
    void shootProjectile(size_t index, Camera* camera);

    //Aqui se guardan todas las particulas de la escena
    //Tanto la particuka independiente del apartado 1 como todos los proyectiles que se vayan creando
    //TODO: preguntar si es recomendable implementar una pool (dinámica o estatica) 
    // en vez de asignar memoria en cada partícula
    std::vector<Particle*> m_Particulas;

    // Guarda todos los presets de los distintos proyectiles
    std::vector<ProjectileConfig> m_configs;
    size_t m_selectedConfigIndex{ 0 };

    RenderItem* m_floorRenderItem = nullptr;
    physx::PxTransform m_floorPose{ physx::PxVec3(0.0f, -0.5f, 0.0f) };
};