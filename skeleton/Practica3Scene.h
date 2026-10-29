#pragma once

#include "Practica0Scene.h"
#include "SistemaParticulas.h"
#include "RenderUtils.hpp"

class GravityForceGenerator;
class ParticleGenerator;

class Practica3Scene : public Practica0Scene
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

	GravityForceGenerator* m_gravityZoneEarth = nullptr;
	GravityForceGenerator* m_gravityZoneBuoyancy = nullptr;

	ParticleGenerator* m_fountainEarth = nullptr;
	ParticleGenerator* m_fountainFloating = nullptr;
	ParticleGenerator* m_fountainCenter = nullptr;

	RenderItem* m_floorRenderItem = nullptr;
	physx::PxTransform m_floorPose{ physx::PxVec3(0.0f, -0.3f, 0.0f) };

	bool m_debugZones = true;
};
