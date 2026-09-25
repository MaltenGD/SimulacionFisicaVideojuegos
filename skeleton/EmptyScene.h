#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include <vector>

class EmptyScene : public Scene {
public:
    explicit EmptyScene(std::string name);

    void init() override;

    void update(double dt) override;

    void keyPress(unsigned char key, const physx::PxTransform& camera) override;

    void cleanup() override;

private:
    physx::PxTransform m_transform;
    RenderItem* m_renderItem{ nullptr };
};