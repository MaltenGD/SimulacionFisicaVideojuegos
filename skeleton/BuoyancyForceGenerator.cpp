#include "BuoyancyForceGenerator.h"
#include "Particle.h"

BuoyancyForceGenerator::BuoyancyForceGenerator(float h, float V, float d, float water_height, Particle* liquid_particle, Particle* particle)
	: _height(h),
	  _volume(V),
	  _liquid_density(d),
	  _gravity(9.8f),
	  _water_height(water_height),
	  _liquid_particle(liquid_particle),
	  _particle(particle)
{
}

void BuoyancyForceGenerator::updateForce(Particle* p)
{
	if (!_enabled || !p || !p->isAlive())
		return;

	if (_particle && p != _particle)
		return;

	float h = p->getPos().y;
	float h0 = _liquid_particle ? _liquid_particle->getPos().y : _water_height;

	Vector3D f(0.0f, 0.0f, 0.0f);
	float immersed = 0.0f;
	if (h - h0 > _height * 0.5f)
	{
		immersed = 0.0f;
	}
	else if (h0 - h > _height * 0.5f)
	{
		immersed = 1.0f;
	}
	else
	{
		immersed = (h0 - h) / _height + 0.5f;
	}

	f.y = _liquid_density * _volume * immersed * _gravity;
	p->addForce(f);
}

void BuoyancyForceGenerator::onParticleDestroyed(Particle* particle)
{
	if (particle && (particle == _particle || particle == _liquid_particle))
	{
		_enabled = false;
		_particle = nullptr;
		_liquid_particle = nullptr;
	}
}
