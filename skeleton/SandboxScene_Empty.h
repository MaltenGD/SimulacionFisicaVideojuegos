#pragma once

#include "Scene.h"
#include "SistemaParticulas.h"
#include "RenderUtils.hpp"

#include <memory>

class SandboxUI;

class SandboxScene_Empty : public Scene
{
public:
	explicit SandboxScene_Empty(std::string name);
	virtual ~SandboxScene_Empty() override;

	void init() override;
	void update(double dt) override;
	void keyPress(unsigned char key, Camera* camera) override;
	void drawIMGUI() override;
	void cleanup() override;

private:
	SistemaParticulas m_sistema;
	std::unique_ptr<SandboxUI> m_ui;

	bool m_paused = false;

	RenderItem* m_floorRenderItem = nullptr;
	physx::PxTransform m_floorPose{ physx::PxVec3(0.0f, -0.3f, 0.0f) };
};
