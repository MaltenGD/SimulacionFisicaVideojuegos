#pragma once

#include "Scene.h"

class Practica0Axes;

class Practica0Scene : public Scene {
public:
    explicit Practica0Scene(std::string name);
    virtual ~Practica0Scene() override;

    void init() override;
    void update(double dt) override;
    void cleanup() override;

protected:
    Practica0Axes* m_axes{ nullptr };
};