#pragma once

#include "ForceGenerator.h"
#include "Vector3D.h"

class Particle;

class BuoyancyForceGenerator : public ForceGenerator
{
public:
	BuoyancyForceGenerator(float h, float V, float d, float water_height = 5.0f, Particle* liquid_particle = nullptr, Particle* particle = nullptr);
	virtual ~BuoyancyForceGenerator() override = default;

	ForceGeneratorType getType() const override { return ForceGeneratorType::BUOYANCY; }
	virtual void updateForce(Particle* particle) override;
	virtual void applyForces(Particle* particle) override { updateForce(particle); }
	virtual void onParticleDestroyed(Particle* particle) override;

	void setHeight(float h) { _height = h; }
	float getHeight() const { return _height; }

	void setVolume(float v) { _volume = v; }
	float getVolume() const { return _volume; }

	void setLiquidDensity(float d) { _liquid_density = d; }
	float getLiquidDensity() const { return _liquid_density; }

	void setWaterHeight(float h0) { _water_height = h0; }
	float getWaterHeight() const { return _water_height; }

	Particle* getParticle() const { return _particle; }
	void setParticle(Particle* particle) { _particle = particle; }

	void setEnabled(bool enabled) { _enabled = enabled; }
	bool isEnabled() const { return _enabled; }

protected:
	float _height;
	float _volume;
	float _liquid_density;
	float _gravity = 9.8f;
	float _water_height = 5.0f;
	Particle* _liquid_particle;
	Particle* _particle;
	bool _enabled = true;
};
