#include "AnchoredSpringFG.h"
#include "Particle.h"

AnchoredSpringFG::AnchoredSpringFG(double k, double resting, const Vector3D& anchor_pos, Particle* particle)
	: SpringForceGenerator(k, resting, nullptr, particle)
{
	_other = new Particle(
		0.0f,
		anchor_pos,
		Vector3D::zero(),
		Vector3D::zero(),
		0.0f,
		0.4f,
		Vector4(0.9f, 0.2f, 0.2f, 1.0f),
		1e6f,
		ParticleShape::BOX,
		Vector3D(0.4f, 0.4f, 0.4f)
	);
}

AnchoredSpringFG::~AnchoredSpringFG()
{
	if (_other)
	{
		delete _other;
		_other = nullptr;
	}
}
