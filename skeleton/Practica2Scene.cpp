#include "Practica2Scene.h"
#include "FountainParticleGenerator.h"
#include "FireParticleGenerator.h"
#include "GravityForceGenerator.h"
#include <iostream>

Practica2Scene::Practica2Scene(std::string name)
	: Practica0Scene(std::move(name)),
	  m_sistema(1000)
{
}

Practica2Scene::~Practica2Scene()
{
	cleanup();
}

void Practica2Scene::initEffects()
{
	m_sistema.cleanup();

	auto gen1 = std::make_unique<FountainParticleGenerator>(
		Vector3D(0.0f, 0.0f, 0.0f),
		Vector3D(0.0f, 15.0f, 0.0f),
		Vector3D(2.5f, 2.0f, 2.5f), Vector3D(0.0f, -9.8f, 0.0f) // tiene fixedGravity.
	);
	m_fountain = m_sistema.addGenerator(std::move(gen1));

	auto gen2 = std::make_unique<FireParticleGenerator>(
		Vector3D(15.0f, 0.0f, 0.0f)
	);
	m_fire = m_sistema.addGenerator(std::move(gen2));
}


void Practica2Scene::init()
{
	cleanup();
	Practica0Scene::init();
	initEffects();

	m_floorRenderItem = new RenderItem(
		CreateShape(physx::PxBoxGeometry(80.0f, 0.2f, 80.0f)),
		&m_floorPose,
		Vector4(0.28f, 0.38f, 0.28f, 1.0f)
	);
}

void Practica2Scene::update(double dt)
{
	m_sistema.update(dt);
}

void Practica2Scene::keyPress(unsigned char key, Camera* camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
	case 'F': // Fuente de agua
		if (m_fountain)
		{
			m_fountain->toggleActive();
			std::cout << "Fuente de Agua " << (m_fountain->isActive() ? "ACTIVADA" : "DESACTIVADA") << "\n";
		}
		break;

	case 'G': // Fuego / Chispas
		if (m_fire)
		{
			m_fire->toggleActive();
			std::cout << "Chispas de Fuego " << (m_fire->isActive() ? "ACTIVADO" : "DESACTIVADO") << "\n";
		}
		break;

	case 'C': // Limpiar partículas
		m_sistema.clearParticles();
		std::cout << "Todas las particulas vivas han sido eliminadas.\n";
		break;

	case 'R': // Reiniciar
		initEffects();
		std::cout << "Generadores y particulas reiniciados.\n";
		break;

	default:
		break;
	}
}

void Practica2Scene::cleanup()
{
	m_sistema.cleanup();
	m_fountain = nullptr;
	m_fire = nullptr;

	if (m_floorRenderItem)
	{
		m_floorRenderItem->release();
		m_floorRenderItem = nullptr;
	}

	Practica0Scene::cleanup();
}
