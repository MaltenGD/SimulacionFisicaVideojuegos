#pragma once

// Funciones auxiliares de teclado compartidas por las escenas sandbox.
#include "SistemaParticulas.h"
#include "ForceGenerator.h"
#include "ParticleGenerator.h"
#include "ExplosionForceGenerator.h"
#include <iostream>

// Activa/desactiva (alterna) todos los generadores de fuerza de un tipo y lo comunica por consola.
inline void ToggleForceKey(SistemaParticulas& sistema, ForceGeneratorType type, const char* name)
{
	const int state = sistema.toggleForceGeneratorType(type);
	if (state < 0)
		std::cout << "[FUERZA] No hay generador de " << name << " en esta escena\n";
	else
		std::cout << "[FUERZA] " << name << (state ? ": ACTIVADO" : ": DESACTIVADO") << "\n";
}

// Activa/desactiva (alterna) todos los generadores de particulas de un tipo.
inline void ToggleParticleGeneratorKey(SistemaParticulas& sistema, ParticleGeneratorType type, const char* name)
{
	const int state = sistema.toggleGeneratorType(type);
	if (state < 0)
		std::cout << "[GENERADOR] No hay generador de " << name << " en esta escena\n";
	else
		std::cout << "[GENERADOR] " << name << (state ? ": ACTIVADO" : ": DESACTIVADO") << "\n";
}

// Dispara la explosion (fuerza que varia con el tiempo). Solo actua si el generador esta activado (tecla 4).
inline void TriggerExplosionKey(SistemaParticulas& sistema)
{
	bool found = false;
	for (const auto& fg : sistema.getForceGenerators())
	{
		if (!fg || fg->getType() != ForceGeneratorType::EXPLOSION)
			continue;
		found = true;
		if (!fg->isEnabled())
		{
			std::cout << "[FUERZA] La explosion esta desactivada (pulsa 4 para activarla)\n";
			continue;
		}
		static_cast<ExplosionForceGenerator*>(fg.get())->trigger();
		std::cout << "[FUERZA] Explosion disparada\n";
	}
	if (!found)
		std::cout << "[FUERZA] No hay generador de Explosion en esta escena\n";
}