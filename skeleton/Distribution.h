#pragma once

#include "Vector3D.h"
#include <random>

class Distribution
{
public:
	virtual ~Distribution() = default;
	virtual float sample() = 0;
};

// Distribución Uniforme
class UniformDistribution : public Distribution
{
private:
	std::mt19937 m_gen;
	std::uniform_real_distribution<float> m_dist;

public:
	UniformDistribution(const float& minVal, const float& maxVal);
	float sample() override;
	void setBounds(const float& minVal, const float& maxVal);
};

// Distribución Gaussiana / Normal (con media y desviación típica)
class GaussianDistribution : public Distribution
{
private:
	std::mt19937 m_gen;
	std::normal_distribution<float> m_dist;

public:
	GaussianDistribution(const float& meanVal, const float& stdDevVal);
	float sample() override;
	void setParameters(const float& meanVal, const float& stdDevVal);
};
