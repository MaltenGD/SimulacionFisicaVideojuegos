#include "SpringForceGenerator.h"
#include "Particle.h"

SpringForceGenerator::SpringForceGenerator(double k, double resting_length, Particle* other, Particle* particle)
	: _k(k),
	  _resting_length(resting_length),
	  _other(other),
	  _particle(particle)
{
}

void SpringForceGenerator::updateForce(Particle* particle)
{

	if (!_enabled || !particle || !particle->isAlive())
		return;

	if (_particle && particle != _particle)
		return;

	if (!_other)
		return;

	if (!_other->isAlive())
		return;

	Vector3D relative_pos_vector = _other->getPos() - particle->getPos();
	float length = relative_pos_vector.magnitude();
	if (length <= 0.0001f)
		return;

	float delta_x = length - static_cast<float>(_resting_length);
	Vector3D dir = relative_pos_vector.normalize();
	Vector3D force = dir * (delta_x * static_cast<float>(_k));

	particle->addForce(force);
}

void SpringForceGenerator::onParticleDestroyed(Particle* particle)
{
	if (particle && (particle == _particle || particle == _other))
	{
		// The generator cannot safely operate after either endpoint is destroyed.
		_enabled = false;
		_particle = nullptr;
		_other = nullptr;
	}
}
