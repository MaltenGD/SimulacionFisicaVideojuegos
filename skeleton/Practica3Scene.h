#pragma once

#include "Scene.h"
#include "SistemaParticulas.h"
#include "RenderUtils.hpp"

class GravityForceGenerator;
class WindForceGenerator;
class WhirlwindForceGenerator;
class ExplosionForceGenerator;
class ParticleGenerator;

class Practica3Scene : public Scene
{
public:
	explicit Practica3Scene(std::string name);
	virtual ~Practica3Scene() override;

	void init() override;
	void update(double dt) override;
	void keyPress(unsigned char key, Camera* camera) override;
	void cleanup() override;

private:
	void initEffects();
	void spawnParticlesWithMass(float mass, const Vector3D& pos, const Vector3D& vel, const Vector4& color);

	SistemaParticulas m_sistema;

	// Generador de fuerzas de gravedad terrestre
	GravityForceGenerator*   m_gravityEarth     = nullptr; 
	// Generador de fuerza de gravedad invertida (hacia arriba)
	GravityForceGenerator*   m_gravityBuoyancy  = nullptr;

	// Generador de fuerza de viento
	WindForceGenerator*      m_windZone         = nullptr;
	// Generador de gravedad extra para el generador de fuerzas de viento
	GravityForceGenerator*   m_gravityWind      = nullptr;


	// Generador de fuerza de viento
	WhirlwindForceGenerator* m_whirlwind        = nullptr;

	// Generador de gravedad extra para el generador de fuerzas de torbellino
	GravityForceGenerator*   m_gravityWhirlwind = nullptr;

	ExplosionForceGenerator* m_explosion        = nullptr;

	// Emisores
	ParticleGenerator* m_fountainEarth = nullptr;
	ParticleGenerator* m_fountainFloat = nullptr;
	ParticleGenerator* m_fountainWind  = nullptr;
	ParticleGenerator* m_fountainVortex= nullptr;

	RenderItem* m_floorRenderItem = nullptr;
	physx::PxTransform m_floorPose{ physx::PxVec3(0.0f, -0.3f, 0.0f) };

	bool m_debugZones = true;
};
