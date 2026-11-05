#pragma once

#include "Scene.h"
#include "SistemaParticulas.h"
#include "RenderUtils.hpp"

class AnchoredSpringFG;
class SpringForceGenerator;
class ElasticRubberFG;
class BuoyancyForceGenerator;
class GravityForceGenerator;

class SandboxScene_Example : public Scene
{
public:
	explicit SandboxScene_Example(std::string name);
	virtual ~SandboxScene_Example() override;

	void init() override;
	void update(double dt) override;
	void keyPress(unsigned char key, Camera* camera) override;
	void cleanup() override;

private:
	void initEffects();

	SistemaParticulas m_sistema;
	bool m_paused = false;

	Particle* m_anchoredParticle = nullptr;
	AnchoredSpringFG* m_anchoredSpring = nullptr;

	Particle* m_springP1 = nullptr;
	Particle* m_springP2 = nullptr;
	SpringForceGenerator* m_spring12 = nullptr;
	SpringForceGenerator* m_spring21 = nullptr;

	Particle* m_rubberP1 = nullptr;
	Particle* m_rubberP2 = nullptr;
	ElasticRubberFG* m_rubber12 = nullptr;
	ElasticRubberFG* m_rubber21 = nullptr;

	Particle* m_boatParticle = nullptr;
	BuoyancyForceGenerator* m_buoyancy = nullptr;

	GravityForceGenerator* m_gravity = nullptr;

	RenderItem* m_floorRenderItem = nullptr;
	physx::PxTransform m_floorPose{ physx::PxVec3(0.0f, -0.3f, 0.0f) };

	RenderItem* m_waterRenderItem = nullptr;
	physx::PxTransform m_waterPose{ physx::PxVec3(22.0f, 5.0f, 0.0f) };
};