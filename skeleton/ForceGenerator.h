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
	virtual void updateForce(Particle* particle) { applyForces(particle); }

	// Called before the particle system destroys a particle referenced by a generator.
	// Generators that keep raw particle pointers must invalidate them here.
	virtual void onParticleDestroyed(Particle* particle) { (void)particle; }
};
