#pragma once

#include "Vector3D.h"

class Particle;

class ForceGenerator
{
public:
	ForceGenerator() = default;
	virtual ~ForceGenerator() = default;

	virtual void update(double dt) { (void)dt; }
	virtual void applyForces(Particle* particle) = 0;
};
