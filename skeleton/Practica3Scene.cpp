#include "Practica3Scene.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "FountainParticleGenerator.h"
#include "Particle.h"
#include <iostream>

Practica3Scene::Practica3Scene(std::string name)
	: Scene(std::move(name)),
	  m_sistema(1500),
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

	auto zoneEarth = std::make_unique<GravityForceGenerator>(
		Vector3D(-36.0f, 15.0f, 0.0f),
		Vector3D(10.0f, 16.0f, 10.0f),
		Vector3D(0.0f, -9.8f, 0.0f),
		Vector4(0.0f, 0.7f, 1.0f, 1.0f)
	);
	m_gravityEarth = static_cast<GravityForceGenerator*>(m_sistema.addForceGenerator(std::move(zoneEarth)));

	auto zoneFloat = std::make_unique<GravityForceGenerator>(
		Vector3D(-12.0f, 15.0f, 0.0f),
		Vector3D(10.0f, 16.0f, 10.0f),
		Vector3D(0.0f, 9.8f, 0.0f),
		Vector4(1.0f, 0.5f, 0.1f, 1.0f)
	);
	m_gravityBuoyancy = static_cast<GravityForceGenerator*>(m_sistema.addForceGenerator(std::move(zoneFloat)));

	auto zoneWind = std::make_unique<WindForceGenerator>(
		Vector3D(12.0f, 15.0f, 0.0f),
		Vector3D(10.0f, 16.0f, 10.0f),
		Vector3D(18.0f, 0.0f, 0.0f),
		1.5f,
		Vector4(0.2f, 0.9f, 0.3f, 1.0f)
	);
	m_windZone = static_cast<WindForceGenerator*>(m_sistema.addForceGenerator(std::move(zoneWind)));

	auto zoneWindGravity = std::make_unique<GravityForceGenerator>(
		Vector3D(12.0f, 15.0f, 0.0f),
		Vector3D(10.0f, 16.0f, 10.0f),
		Vector3D(0.0f, -9.8f, 0.0f)
	);
	m_gravityWind = static_cast<GravityForceGenerator*>(m_sistema.addForceGenerator(std::move(zoneWindGravity)));

	auto zoneVortex = std::make_unique<WhirlwindForceGenerator>(
		Vector3D(36.0f, 15.0f, 0.0f),
		12.0f, 32.0f,
		2.2f,
		1.8f,
		Vector4(0.85f, 0.35f, 1.0f, 1.0f)
	);
	m_whirlwind = static_cast<WhirlwindForceGenerator*>(m_sistema.addForceGenerator(std::move(zoneVortex)));

	auto zoneVortexGravity = std::make_unique<GravityForceGenerator>(
		Vector3D(36.0f, 15.0f, 0.0f),
		Vector3D(12.0f, 16.0f, 12.0f),
		Vector3D(0.0f, -9.8f, 0.0f)
	);
	m_gravityWhirlwind = static_cast<GravityForceGenerator*>(m_sistema.addForceGenerator(std::move(zoneVortexGravity)));

	auto zoneExplosion = std::make_unique<ExplosionForceGenerator>(
		Vector3D(0.0f, 6.0f, 0.0f),
		30000.0f,
		35.0f,
		0.15f
	);
	m_explosion = static_cast<ExplosionForceGenerator*>(m_sistema.addForceGenerator(std::move(zoneExplosion)));

	if (m_gravityEarth)    m_gravityEarth->setDebug(m_debugZones);
	if (m_gravityBuoyancy) m_gravityBuoyancy->setDebug(m_debugZones);
	if (m_windZone)        m_windZone->setDebug(m_debugZones);
	if (m_whirlwind)       m_whirlwind->setDebug(m_debugZones);

	auto genEarth = std::make_unique<FountainParticleGenerator>(
		Vector3D(-36.0f, 0.0f, 0.0f), Vector3D(0.0f, 16.0f, 0.0f), Vector3D(1.8f, 1.5f, 1.8f)
	);
	genEarth->setColor(Vector4(0.1f, 0.6f, 1.0f, 1.0f));
	m_fountainEarth = m_sistema.addGenerator(std::move(genEarth));

	auto genFloat = std::make_unique<FountainParticleGenerator>(
		Vector3D(-12.0f, 0.0f, 0.0f), Vector3D(0.0f, 4.0f, 0.0f), Vector3D(1.5f, 1.0f, 1.5f)
	);
	genFloat->setColor(Vector4(1.0f, 0.5f, 0.2f, 1.0f));
	m_fountainFloat = m_sistema.addGenerator(std::move(genFloat));

	auto genWind = std::make_unique<FountainParticleGenerator>(
		Vector3D(8.0f, 0.0f, 0.0f), Vector3D(0.0f, 14.0f, 0.0f), Vector3D(1.2f, 1.2f, 1.2f)
	);
	genWind->setColor(Vector4(0.3f, 1.0f, 0.4f, 1.0f));
	m_fountainWind = m_sistema.addGenerator(std::move(genWind));

	auto genVortex = std::make_unique<FountainParticleGenerator>(
		Vector3D(36.0f, 0.0f, 0.0f), Vector3D(0.0f, 2.0f, 0.0f), Vector3D(2.5f, 1.0f, 2.5f)
	);
	genVortex->setColor(Vector4(0.9f, 0.4f, 1.0f, 1.0f));
	m_fountainVortex = m_sistema.addGenerator(std::move(genVortex));
}

void Practica3Scene::init()
{
	cleanup();
	initEffects();

	m_floorRenderItem = new RenderItem(
		CreateShape(physx::PxBoxGeometry(100.0f, 0.2f, 100.0f)),
		&m_floorPose,
		Vector4(0.25f, 0.25f, 0.25f, 1.0f)
	);

	std::cout << "PRACTICA 3:\n";
	std::cout << "  [E] Detonar explosion radial\n";
	std::cout << "  [1] Lanzar particula ligera (m = 1.0 kg)\n";
	std::cout << "  [2] Lanzar particula media  (m = 5.0 kg)\n";
	std::cout << "  [3] Lanzar particula pesada (m = 20.0 kg)\n";
	std::cout << "  [B] Toggle AABB debug de las zonas\n";
	std::cout << "  [F] Toggle emisores continuos\n";
	std::cout << "  [C] Limpiar particulas\n";
	std::cout << "  [R] Reiniciar escena\n";
}

void Practica3Scene::update(double dt)
{
	m_sistema.update(dt);
}

void Practica3Scene::spawnParticlesWithMass(float mass, const Vector3D& pos, const Vector3D& vel, const Vector4& color)
{
	m_sistema.addParticle(new Particle(mass, pos + Vector3D(-36.0f, 0.0f, 0.0f), vel, Vector3D::zero(), 0.99f, 0.35f, color, 8.0f));
	m_sistema.addParticle(new Particle(mass, pos + Vector3D(-12.0f, 0.0f, 0.0f), vel * 0.4f, Vector3D::zero(), 0.99f, 0.35f, color, 8.0f));
	m_sistema.addParticle(new Particle(mass, pos, vel * 0.5f, Vector3D::zero(), 0.99f, 0.35f, color, 8.0f));
	m_sistema.addParticle(new Particle(mass, pos + Vector3D(12.0f, 0.0f, 0.0f), vel * 0.8f, Vector3D::zero(), 0.99f, 0.35f, color, 8.0f));
	m_sistema.addParticle(new Particle(mass, pos + Vector3D(36.0f, 0.0f, 0.0f), vel * 0.15f, Vector3D::zero(), 0.99f, 0.35f, color, 8.0f));
}

void Practica3Scene::keyPress(unsigned char key, Camera* camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
	case 'E':
		if (m_explosion)
		{
			m_explosion->trigger(Vector3D(0.0f, 6.0f, 0.0f));
			std::cout << "Explosion en 0,6,0\n";
		}
		break;

	case '1':
		spawnParticlesWithMass(1.0f, Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 18.0f, 0.0f), Vector4(1.0f, 1.0f, 0.2f, 1.0f));
		std::cout << "Particula ligera (m = 1.0 kg)\n";
		break;

	case '2':
		spawnParticlesWithMass(5.0f, Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 18.0f, 0.0f), Vector4(1.0f, 0.4f, 0.1f, 1.0f));
		std::cout << "Particula media (m = 5.0 kg)\n";
		break;

	case '3':
		spawnParticlesWithMass(20.0f, Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 18.0f, 0.0f), Vector4(0.9f, 0.1f, 0.2f, 1.0f));
		std::cout << "Particula pesada (m = 20.0 kg)\n";
		break;

	case 'B':
		m_debugZones = !m_debugZones;
		if (m_gravityEarth)    m_gravityEarth->setDebug(m_debugZones);
		if (m_gravityBuoyancy) m_gravityBuoyancy->setDebug(m_debugZones);
		if (m_windZone)        m_windZone->setDebug(m_debugZones);
		if (m_whirlwind)       m_whirlwind->setDebug(m_debugZones);
		break;

	case 'F':
		if (m_fountainEarth)  m_fountainEarth->toggleActive();
		if (m_fountainFloat)  m_fountainFloat->toggleActive();
		if (m_fountainWind)   m_fountainWind->toggleActive();
		if (m_fountainVortex) m_fountainVortex->toggleActive();
		break;

	case 'C':
		m_sistema.clearParticles();
		std::cout << "Particulas eliminadas.\n";
		break;

	case 'R':
		initEffects();
		std::cout << "Escena reiniciada.\n";
		break;

	default:
		break;
	}
}

void Practica3Scene::cleanup()
{
	m_sistema.cleanup();
	m_gravityEarth     = nullptr;
	m_gravityBuoyancy  = nullptr;
	m_gravityWind      = nullptr;
	m_gravityWhirlwind = nullptr;
	m_windZone        = nullptr;
	m_whirlwind       = nullptr;
	m_explosion       = nullptr;

	m_fountainEarth   = nullptr;
	m_fountainFloat   = nullptr;
	m_fountainWind    = nullptr;
	m_fountainVortex  = nullptr;

	if (m_floorRenderItem)
	{
		m_floorRenderItem->release();
		m_floorRenderItem = nullptr;
	}
}
