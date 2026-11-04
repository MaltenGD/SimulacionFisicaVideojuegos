#pragma once

#include "Vector3D.h"
#include <memory>
#include <vector>

class SistemaParticulas;
class ParticleGenerator;
class ForceGenerator;

// Clase que encapsula todos las llamadas a los elementos de IMGUI y tambien la modificación de los elementos de la escena

class SandboxUI
{
public:
	// La UI requiere que exista un sistema de particulas
	explicit SandboxUI(SistemaParticulas& system);
	~SandboxUI();

	void draw(bool& paused);
	void seedDemo();

private:
	// Dibuja la pestaña de la escena
	void drawSceneTab(bool& paused);
	// Dibuja la pestaña de los generadores de particulas (en la cual se puede modificar todo lo relacionado con los generadores de particulas)
	void drawGeneratorsTab();
	// Dibuja la pestaña de los generadores de fuerzas (en la cual se puede modificar todo lo relacionado con los generadores de fuerzas)
	void drawForceGeneratorsTab();
	// Dibuja los elementos de la UI relacionados con un generador de particulas (se llama solo cuando se despliega el tab del generador en sí)
	void drawGeneratorInspector(ParticleGenerator* generator);
	// Dibuja los elementos de la UI relacionados con un generador de fuerzas (se llama solo cuando se despliega el tab del generador en sí)
	void drawForceGeneratorInspector(ForceGenerator* generator);
	// Elimina los generadores que deben ser eliminados (cuando se pulsa en el boton delete se agrega el generador a su vector pending Deletes)
	void flushMutations();

	// m_system maneja todo los generadores de particulas y fuerzas de la escena
	SistemaParticulas& m_system;

	// Vectores que almacenan generadores que han sido modificados o creados en el anterior frame
	std::vector<std::unique_ptr<ParticleGenerator>> m_pendingGenerators;
	std::vector<std::unique_ptr<ForceGenerator>> m_pendingForceGenerators;
	// Vectores que almacenan generadores que deben ser eliminados (flushMutations() lo elimina)
	std::vector<ParticleGenerator*> m_pendingGeneratorDeletes;
	std::vector<ForceGenerator*> m_pendingForceGeneratorDeletes;

	// Tipo elegido en los desplegables de creación (lo cambia ImGui).
	int m_generatorTypeToAdd = 0;
	int m_forceGeneratorTypeToAdd = 0;
	//Variables para el proximo generador de particulas creado (Los cambia IMGUI)
	Vector3D m_newGeneratorOrigin{ 0.0f, 5.0f, 0.0f };
	//Variables para el proximo generador de fuerzas creado (Los cambia IMGUI)
	Vector3D m_newForceOrigin{ 0.0f, 10.0f, 0.0f };
};
