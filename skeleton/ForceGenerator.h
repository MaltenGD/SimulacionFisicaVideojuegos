#pragma once
#include <vector>
#include "Vector3D.h"

class Particle;

class ForceGenerator
{
protected:
	std::vector<Vector3D> forces;

public:
	ForceGenerator() = default;
	virtual ~ForceGenerator() = default;

	void addForce(const Vector3D& force);

	virtual void applyForces(Particle* particle) = 0;
};
