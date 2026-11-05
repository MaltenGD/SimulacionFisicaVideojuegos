#pragma once

#include "ForceGenerator.h"
#include "Vector3D.h"

class Particle;

class SpringForceGenerator : public ForceGenerator
{
public:
	SpringForceGenerator(double k, double resting_length, Particle* other, Particle* particle = nullptr);
	virtual ~SpringForceGenerator() override = default;

	ForceGeneratorType getType() const override { return ForceGeneratorType::SPRING; }
	virtual void updateForce(Particle* particle) override;
	virtual void applyForces(Particle* particle) override { updateForce(particle); }
	virtual void onParticleDestroyed(Particle* particle) override;

	inline void setK(double k) { _k = k; }
	inline double getK() const { return _k; }
	inline void setRestingLength(double length) { _resting_length = length; }
	inline double getRestingLength() const { return _resting_length; }
	inline Particle* getOther() const { return _other; }
	inline Particle* getParticle() const { return _particle; }

protected:
	double _k;
	double _resting_length;
	Particle* _other;
	Particle* _particle;
};