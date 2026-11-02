#include "Distribution.h"

UniformDistribution::UniformDistribution(const float& minVal, const float& maxVal)
	: m_gen(std::random_device{}()),
	m_dist(minVal, maxVal)
{
}

float UniformDistribution::sample()
{
	return m_dist(m_gen);
}

void UniformDistribution::setBounds(const float& minVal, const float& maxVal)
{
	m_dist = std::uniform_real_distribution<float>(minVal, maxVal);
}

GaussianDistribution::GaussianDistribution(const float& meanVal, const float& stdDevVal)
	: m_gen(std::random_device{}()),
	m_dist(meanVal, stdDevVal)
{
}

float GaussianDistribution::sample()
{
	return m_dist(m_gen);
}

void GaussianDistribution::setParameters(const float& meanVal, const float& stdDevVal)
{
	m_dist = std::normal_distribution<float>(meanVal, stdDevVal);
}
