#pragma once

#include "SpringForceGenerator.h"

class ElasticRubberFG : public SpringForceGenerator
{
public:
	ElasticRubberFG(double k, double resting_length, Particle* other, Particle* particle = nullptr);
	virtual ~ElasticRubberFG() override = default;

	virtual void updateForce(Particle* particle) override;
};
