#pragma once

#include "Vector3D.h"
#include <random>

// Clase abstracta para generar distribuciones independientes en 3 dimensiones (no tienen por que)

// TODO: Preguntar si es necesario implementar la posiblidad de elegir el tipo de distribución en cada dimensión
// Ejemplo: 
// Posición X : Distribución uniforme
// Posición Y : Distribución uniforme
// Posición Z : Distribución normal

class Vector3DDistribution
{
public:
	virtual ~Vector3DDistribution() = default;
	virtual Vector3D sample() = 0;
};

// Distribución Uniforme [a, b] 
class UniformVectorDistribution : public Vector3DDistribution
{
private:
	std::mt19937 m_gen;
	std::uniform_real_distribution<float> m_distX;
	std::uniform_real_distribution<float> m_distY;
	std::uniform_real_distribution<float> m_distZ;

public:
	UniformVectorDistribution(const Vector3D& minVal, const Vector3D& maxVal);
	Vector3D sample() override;
	void setBounds(const Vector3D& minVal, const Vector3D& maxVal);
};

// Distribución Gaussiana / Normal (con media y desviación típica)
class GaussianVectorDistribution : public Vector3DDistribution
{
private:
	std::mt19937 m_gen;
	std::normal_distribution<float> m_distX;
	std::normal_distribution<float> m_distY;
	std::normal_distribution<float> m_distZ;

public:
	GaussianVectorDistribution(const Vector3D& meanVal, const Vector3D& stdDevVal);
	Vector3D sample() override;
	void setParameters(const Vector3D& meanVal, const Vector3D& stdDevVal);
};
