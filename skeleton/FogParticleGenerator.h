#pragma once

#include "ParticleGenerator.h"
#include <memory>

class Vector3DDistribution;

class FogParticleGenerator : public ParticleGenerator
{
private:
	std::unique_ptr<Vector3DDistribution> m_posDist;
	std::unique_ptr<Vector3DDistribution> m_velDist;
	Vector3D m_minBox;
	Vector3D m_maxBox;

	void rebuildDistribution();

public:
	FogParticleGenerator(Vector3D minBox = Vector3D(-12.0f, 0.5f, -12.0f),
	                     Vector3D maxBox = Vector3D( 12.0f,  3.5f,  12.0f));
	~FogParticleGenerator() override;

	Particle* generateSingleParticle(Particle* reusable = nullptr) override;
	ParticleGeneratorType getType() const override { return ParticleGeneratorType::FOG; }
	// A diferencia de otros setOrigin, este mueve el minBox y maxBox de la caja ya que fog no tiene origin como si tiene Fountain o Fire
	void setOrigin(const Vector3D& pos) override;

	Vector3D getMinBox() const { return m_minBox; }
	Vector3D getMaxBox() const { return m_maxBox; }
	void setBounds(const Vector3D& minBox, const Vector3D& maxBox)
	{
		m_minBox = minBox;
		m_maxBox = maxBox;
		m_origin = (minBox + maxBox) * 0.5f;
		rebuildDistribution();
	}
};
