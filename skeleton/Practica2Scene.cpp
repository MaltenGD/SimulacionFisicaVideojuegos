#include "Practica2Scene.h"
#include "FountainParticleGenerator.h"
#include "FogParticleGenerator.h"
#include "FireParticleGenerator.h"
#include <iostream>

Practica2Scene::Practica2Scene(std::string name)
	: Scene(std::move(name)),
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

	// Emisor 1 (Izquierda): Fuente de agua con distribución gaussiana en velocidad
	auto gen1 = std::make_unique<FountainParticleGenerator>(
		Vector3D(-30.0f, 0.0f, 0.0f),
		Vector3D(0.0f, 15.0f, 0.0f),
		Vector3D(2.5f, 2.0f, 2.5f)
	);
	m_fountain = m_sistema.addGenerator(std::move(gen1));

	// Emisor 2 (Centro): Banco de niebla volumétrico con distribución uniforme en posición
	auto gen2 = std::make_unique<FogParticleGenerator>(
		Vector3D(-12.0f, 0.5f, -12.0f),
		Vector3D( 12.0f, 3.5f,  12.0f)
	);
	m_fog = m_sistema.addGenerator(std::move(gen2));

	// Emisor 3 (Derecha): Fuego y chispas con convección térmica
	auto gen3 = std::make_unique<FireParticleGenerator>(
		Vector3D(30.0f, 0.0f, 0.0f)
	);
	m_fire = m_sistema.addGenerator(std::move(gen3));
}

void Practica2Scene::init()
{
	cleanup();
	initEffects();

	m_floorRenderItem = new RenderItem(
		CreateShape(physx::PxBoxGeometry(100.0f, 0.2f, 100.0f)),
		&m_floorPose,
		Vector4(0.28f, 0.38f, 0.28f, 1.0f)
	);

	std::cout << "  Practica 2: Sistemas de Particulas\n";

	std::cout << "  F para activar/desactivar la Fuente de Agua\n";
	std::cout << "  N para activar/desactivar el Banco de Niebla \n";
	std::cout << "  G para activar/desactivar las Chispas de Fuego\n";
	std::cout << "  C para  limpiar particulas vivas\n";
	std::cout << "  R para reiniciar generadores\n";
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
	case 'F':
		if (m_fountain)
		{
			m_fountain->toggleActive();
			std::cout << "[Fuente de Agua] " << (m_fountain->isActive() ? "ACTIVADA" : "DESACTIVADA") << "\n";
		}
		break;

	case 'N':
		if (m_fog)
		{
			m_fog->toggleActive();
			std::cout << "[Banco de Niebla] " << (m_fog->isActive() ? "ACTIVADO" : "DESACTIVADO") << "\n";
		}
		break;

	case 'G':
		if (m_fire)
		{
			m_fire->toggleActive();
			std::cout << "[Chispas de Fuego] " << (m_fire->isActive() ? "ACTIVADO" : "DESACTIVADO") << "\n";
		}
		break;

	case 'C':
		m_sistema.clearParticles();
		std::cout << "[Limpieza] Todas las particulas vivas han sido eliminadas.\n";
		break;

	case 'R':
		initEffects();
		std::cout << "[Reset] Generadores y particulas reiniciados.\n";
		break;

	default:
		break;
	}
}

void Practica2Scene::cleanup()
{
	m_sistema.cleanup();
	m_fountain = nullptr;
	m_fog = nullptr;
	m_fire = nullptr;

	if (m_floorRenderItem)
	{
		m_floorRenderItem->release();
		m_floorRenderItem = nullptr;
	}
}
