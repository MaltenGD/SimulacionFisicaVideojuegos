#pragma once

#include "Practica0Scene.h"
#include "SistemaParticulas.h"
#include "RenderUtils.hpp"

class ParticleGenerator;

class Practica2Scene : public Practica0Scene
{
public:
	explicit Practica2Scene(std::string name);
	virtual ~Practica2Scene() override;

	void init() override;
	void update(double dt) override;
	void keyPress(unsigned char key, Camera* camera) override;
	void cleanup() override;

private:
	void initEffects();

	SistemaParticulas m_sistema;

	ParticleGenerator* m_fountain = nullptr;
	ParticleGenerator* m_fire = nullptr;

	RenderItem* m_floorRenderItem = nullptr;
	physx::PxTransform m_floorPose{ physx::PxVec3(0.0f, -0.3f, 0.0f) };
};
