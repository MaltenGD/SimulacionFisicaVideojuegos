#pragma once

#include "Vector3D.h"
#include <memory>
#include <vector>

class SistemaParticulas;
class ParticleGenerator;
class ForceGenerator;

class SandboxUI
{
public:
	explicit SandboxUI(SistemaParticulas& system);
	~SandboxUI();

	void draw(bool& paused);
	void seedDemo();

private:
	void drawSceneTab(bool& paused);
	void drawGeneratorsTab();
	void drawForceGeneratorsTab();
	void drawGeneratorInspector(ParticleGenerator* generator);
	void drawForceGeneratorInspector(ForceGenerator* generator);
	void flushMutations();

	SistemaParticulas& m_system;

	std::vector<std::unique_ptr<ParticleGenerator>> m_pendingGenerators;
	std::vector<std::unique_ptr<ForceGenerator>> m_pendingForceGenerators;
	std::vector<ParticleGenerator*> m_pendingGeneratorDeletes;
	std::vector<ForceGenerator*> m_pendingForceGeneratorDeletes;

	int m_generatorTypeToAdd = 0;
	int m_forceGeneratorTypeToAdd = 0;
	Vector3D m_newGeneratorOrigin{ 0.0f, 5.0f, 0.0f };
	Vector3D m_newForceOrigin{ 0.0f, 10.0f, 0.0f };
};
