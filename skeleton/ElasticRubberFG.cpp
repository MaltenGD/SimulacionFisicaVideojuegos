#include "ElasticRubberFG.h"
#include "Particle.h"

ElasticRubberFG::ElasticRubberFG(double k, double resting_length, Particle* other, Particle* particle)
	: SpringForceGenerator(k, resting_length, other, particle)
{
}

void ElasticRubberFG::updateForce(Particle* particle)
{

	if (!m_enabled || !particle || !particle->isAlive())
		return;

	if (_particle && particle != _particle)
		return;

	if (!_other)
		return;

	if (!_other->isAlive())
		return;

	Vector3D relative_pos_vector = _other->getPos() - particle->getPos();
	float length = relative_pos_vector.magnitude();

	if (length <= _resting_length)
		return;

	float delta_x = length - _resting_length;
	Vector3D dir = relative_pos_vector.normalize();
	Vector3D force = dir * (delta_x * _k);

	particle->addForce(force);
}