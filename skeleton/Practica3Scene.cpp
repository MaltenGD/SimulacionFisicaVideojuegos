#include "Practica3Scene.h"
#include "GravityForceGenerator.h"
#include "FountainParticleGenerator.h"
#include "Particle.h"
#include <iostream>

Practica3Scene::Practica3Scene(std::string name)
	: Practica0Scene(std::move(name)),
	  m_sistema(1000),
	  m_debugZones(true)
{
}

Practica3Scene::~Practica3Scene()
{
	cleanup();
}

void Practica3Scene::initEffects()
{
	m_sistema.cleanup();

	// Zona 1: Gravedad terrestre (-9.8 m/s^2), color cian
	auto zone1 = std::make_unique<GravityForceGenerator>(
		Vector3D(-25.0f, 15.0f, 0.0f),
		Vector3D(15.0f, 16.0f, 15.0f),
		Vector3D(0.0f, -9.8f, 0.0f),
		Vector4(0.0f, 0.8f, 1.0f, 1.0f)
	);
	m_gravityZoneEarth = static_cast<GravityForceGenerator*>(m_sistema.addForceGenerator(std::move(zone1)));

	// Zona 2: Flotabilidad ascendente (+4.0 m/s^2), color naranja
	auto zone2 = std::make_unique<GravityForceGenerator>(
		Vector3D(25.0f, 15.0f, 0.0f),
		Vector3D(15.0f, 16.0f, 15.0f),
		Vector3D(0.0f, 4.0f, 0.0f),
		Vector4(1.0f, 0.5f, 0.0f, 1.0f)
	);
	m_gravityZoneBuoyancy = static_cast<GravityForceGenerator*>(m_sistema.addForceGenerator(std::move(zone2)));

	if (m_gravityZoneEarth)   m_gravityZoneEarth->setDebug(m_debugZones);
	if (m_gravityZoneBuoyancy) m_gravityZoneBuoyancy->setDebug(m_debugZones);

	// Emisor izquierda (Zona gravedad terrestre) – azul
	auto genEarth = std::make_unique<FountainParticleGenerator>(
		Vector3D(-25.0f, 0.0f, 0.0f),
		Vector3D(0.0f, 16.0f, 0.0f),
		Vector3D(2.0f, 2.0f, 2.0f)
	);
	genEarth->setColor(Vector4(0.2f, 0.6f, 1.0f, 1.0f));
	m_fountainEarth = m_sistema.addGenerator(std::move(genEarth));

	// Emisor derecha (Zona flotabilidad) – naranja
	auto genBuoyancy = std::make_unique<FountainParticleGenerator>(
		Vector3D(25.0f, 0.0f, 0.0f),
		Vector3D(0.0f, 5.0f, 0.0f),
		Vector3D(2.0f, 1.5f, 2.0f)
	);
	genBuoyancy->setColor(Vector4(1.0f, 0.5f, 0.2f, 1.0f));
	m_fountainFloating = m_sistema.addGenerator(std::move(genBuoyancy));

	// Emisor central (Sin zona de fuerza → partículas flotan en línea recta) – verde
	auto genCenter = std::make_unique<FountainParticleGenerator>(
		Vector3D(0.0f, 0.0f, 0.0f),
		Vector3D(0.0f, 16.0f, 0.0f),
		Vector3D(2.0f, 2.0f, 2.0f)
	);
	genCenter->setColor(Vector4(0.2f, 1.0f, 0.4f, 1.0f));
	m_fountainCenter = m_sistema.addGenerator(std::move(genCenter));
}

void Practica3Scene::init()
{
	cleanup();
	Practica0Scene::init();
	initEffects();

	m_floorRenderItem = new RenderItem(
		CreateShape(physx::PxBoxGeometry(80.0f, 0.2f, 80.0f)),
		&m_floorPose,
		Vector4(0.25f, 0.25f, 0.25f, 1.0f)
	);

	std::cout << "\n======================================================\n";
	std::cout << "           PRACTICA 3 - ACTIVIDAD 1\n";
	std::cout << "======================================================\n";
	std::cout << " [B]/[D] : Toggle debug AABB de zonas de fuerza\n";
	std::cout << " [Z]     : Spawnar particulas ligeras  (m = 1 kg)\n";
	std::cout << " [X]     : Spawnar particulas medias   (m = 5 kg)\n";
	std::cout << " [M]     : Spawnar particulas pesadas  (m = 20 kg)\n";
	std::cout << " [N]     : Spawnar particula neutra (flota en el centro)\n";
	std::cout << " [F]     : Toggle emisores continuos\n";
	std::cout << " [C]     : Limpiar particulas\n";
	std::cout << " [R]     : Reiniciar\n";
	std::cout << "======================================================\n\n";
}

void Practica3Scene::update(double dt)
{
	m_sistema.update(dt);
}

void Practica3Scene::spawnParticlesWithMass(float mass, const Vector3D& pos, const Vector3D& vel, const Vector4& color)
{
	// Zona gravedad terrestre – solo fuerza del generador, sin gravedad interna
	Particle* p1 = new Particle(mass, pos + Vector3D(-25.0f, 0.0f, 0.0f), vel, Vector3D::zero(), 0.99f, 0.35f, color, 8.0f);
	m_sistema.addParticle(p1);

	// Zona flotabilidad – solo fuerza del generador
	Particle* p2 = new Particle(mass, pos + Vector3D(25.0f, 0.0f, 0.0f), vel * 0.5f, Vector3D::zero(), 0.99f, 0.35f, color, 8.0f);
	m_sistema.addParticle(p2);

	// Centro – sin generador de fuerza: flota en línea recta
	Particle* p3 = new Particle(mass, pos, vel, Vector3D::zero(), 0.99f, 0.35f, color, 8.0f);
	m_sistema.addParticle(p3);
}

void Practica3Scene::keyPress(unsigned char key, Camera* camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
	case 'B':
	case 'D':
		m_debugZones = !m_debugZones;
		if (m_gravityZoneEarth)   m_gravityZoneEarth->setDebug(m_debugZones);
		if (m_gravityZoneBuoyancy) m_gravityZoneBuoyancy->setDebug(m_debugZones);
		std::cout << "[DEBUG] AABB debug: " << (m_debugZones ? "ON" : "OFF") << "\n";
		break;

	case 'Z':
		spawnParticlesWithMass(1.0f, Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 18.0f, 0.0f), Vector4(1.0f, 1.0f, 0.2f, 1.0f));
		std::cout << "[SPAWN] m = 1.0 kg\n";
		break;

	case 'X':
		spawnParticlesWithMass(5.0f, Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 18.0f, 0.0f), Vector4(1.0f, 0.3f, 0.8f, 1.0f));
		std::cout << "[SPAWN] m = 5.0 kg\n";
		break;

	case 'M':
		spawnParticlesWithMass(20.0f, Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 18.0f, 0.0f), Vector4(0.8f, 0.2f, 0.2f, 1.0f));
		std::cout << "[SPAWN] m = 20.0 kg\n";
		break;

	case 'N':
	{
		// Particula neutra fuera de toda zona de fuerza: debe flotar en linea recta
		Particle* p = new Particle(1.0f, Vector3D(0.0f, 12.0f, 0.0f), Vector3D(6.0f, 0.0f, 0.0f), Vector3D::zero(), 1.0f, 0.4f, Vector4(1.0f, 1.0f, 1.0f, 1.0f), 10.0f);
		m_sistema.addParticle(p);
		std::cout << "[TEST] Particula neutra: avanza en linea recta sin caer.\n";
		break;
	}

	case 'F':
		if (m_fountainEarth)   m_fountainEarth->toggleActive();
		if (m_fountainFloating) m_fountainFloating->toggleActive();
		if (m_fountainCenter)  m_fountainCenter->toggleActive();
		std::cout << "[EMISORES] Toggled.\n";
		break;

	case 'C':
		m_sistema.clearParticles();
		std::cout << "[LIMPIEZA] Particulas eliminadas.\n";
		break;

	case 'R':
		initEffects();
		std::cout << "[RESET] Reiniciado.\n";
		break;

	default:
		break;
	}
}

void Practica3Scene::cleanup()
{
	m_sistema.cleanup();
	m_gravityZoneEarth    = nullptr;
	m_gravityZoneBuoyancy = nullptr;
	m_fountainEarth   = nullptr;
	m_fountainFloating = nullptr;
	m_fountainCenter  = nullptr;

	if (m_floorRenderItem)
	{
		m_floorRenderItem->release();
		m_floorRenderItem = nullptr;
	}

	Practica0Scene::cleanup();
}
