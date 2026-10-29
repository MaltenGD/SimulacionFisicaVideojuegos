#pragma once

#include "Vector3D.h"
#include <array>
#include <memory>
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


struct Vector3Distribution
{
	std::array<Distribution*, 3> v;
	Vector3Distribution(Distribution* x, Distribution* y, Distribution* z) :
		v{ x, y, z } { }

	Vector3Distribution(std::unique_ptr<Distribution> x, std::unique_ptr<Distribution> y, std::unique_ptr<Distribution> z) :
		v{ x.release(), y.release(), z.release() } { }

	~Vector3Distribution()
	{
		delete v[0];
		delete v[1];
		delete v[2];
	}

	Vector3D sample()
	{
		return Vector3D{ v[0]->sample(), v[1]->sample(), v[2]->sample() };
	}
};