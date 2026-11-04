#pragma once

#include "Vector3D.h"

class Particle;

enum class ForceGeneratorType
{
	UNKNOWN,
	GRAVITY,
	WIND,
	WHIRLWIND,
	EXPLOSION,
	BUOYANCY,
	ANCHORED_SPRING,
	SPRING,
	RUBBER
};

class ForceGenerator
{
public:
	ForceGenerator() = default;
	virtual ~ForceGenerator() = default;

	virtual ForceGeneratorType getType() const { return ForceGeneratorType::UNKNOWN; }

	virtual void update(double dt) { (void)dt; }
	virtual void applyForces(Particle* particle) = 0;
	virtual void updateForce(Particle* particle) { applyForces(particle); }

	// Called before the particle system destroys a particle referenced by a generator.
	// Generators that keep raw particle pointers must invalidate them here.
	virtual void onParticleDestroyed(Particle* particle) { (void)particle; }
};
