#include "Vector3DDistribution.h"

UniformVectorDistribution::UniformVectorDistribution(const Vector3D& minVal, const Vector3D& maxVal)
	: m_gen(std::random_device{}()),
	  m_distX(minVal.x, maxVal.x),
	  m_distY(minVal.y, maxVal.y),
	  m_distZ(minVal.z, maxVal.z)
{
}

Vector3D UniformVectorDistribution::sample()
{
	return Vector3D(m_distX(m_gen), m_distY(m_gen), m_distZ(m_gen));
}

void UniformVectorDistribution::setBounds(const Vector3D& minVal, const Vector3D& maxVal)
{
	m_distX = std::uniform_real_distribution<float>(minVal.x, maxVal.x);
	m_distY = std::uniform_real_distribution<float>(minVal.y, maxVal.y);
	m_distZ = std::uniform_real_distribution<float>(minVal.z, maxVal.z);
}

GaussianVectorDistribution::GaussianVectorDistribution(const Vector3D& meanVal, const Vector3D& stdDevVal)
	: m_gen(std::random_device{}()),
	  m_distX(meanVal.x, stdDevVal.x),
	  m_distY(meanVal.y, stdDevVal.y),
	  m_distZ(meanVal.z, stdDevVal.z)
{
}

Vector3D GaussianVectorDistribution::sample()
{
	return Vector3D(m_distX(m_gen), m_distY(m_gen), m_distZ(m_gen));
}

void GaussianVectorDistribution::setParameters(const Vector3D& meanVal, const Vector3D& stdDevVal)
{
	m_distX = std::normal_distribution<float>(meanVal.x, stdDevVal.x);
	m_distY = std::normal_distribution<float>(meanVal.y, stdDevVal.y);
	m_distZ = std::normal_distribution<float>(meanVal.z, stdDevVal.z);
}
