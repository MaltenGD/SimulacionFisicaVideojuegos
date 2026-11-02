#include "ElasticRubberFG.h"
#include "Particle.h"

ElasticRubberFG::ElasticRubberFG(double k, double resting_length, Particle* other, Particle* particle)
	: SpringForceGenerator(k, resting_length, other, particle)
{
}

void ElasticRubberFG::updateForce(Particle* particle)
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

	if (length <= static_cast<float>(_resting_length))
		return;

	float delta_x = length - static_cast<float>(_resting_length);
	Vector3D dir = relative_pos_vector.normalize();
	Vector3D force = dir * (delta_x * static_cast<float>(_k));

	particle->addForce(force);
}
