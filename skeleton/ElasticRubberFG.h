#pragma once

#include "SpringForceGenerator.h"

class ElasticRubberFG : public SpringForceGenerator
{
public:
	ElasticRubberFG(double k, double resting_length, Particle* other, Particle* particle = nullptr);
	virtual ~ElasticRubberFG() override = default;

	ForceGeneratorType getType() const override { return ForceGeneratorType::RUBBER; }
	virtual void updateForce(Particle* particle) override;
};
