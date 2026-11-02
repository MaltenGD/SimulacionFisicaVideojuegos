#pragma once

#include "SpringForceGenerator.h"

class AnchoredSpringFG : public SpringForceGenerator
{
public:
	AnchoredSpringFG(double k, double resting, const Vector3D& anchor_pos, Particle* particle = nullptr);
	virtual ~AnchoredSpringFG() override;
};
