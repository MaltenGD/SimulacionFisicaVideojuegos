#include "Projectile.h"
#include "Render/Camera.h"
#include "SandboxScene_Example.h"
#include "AnchoredSpringFG.h"
#include "SpringForceGenerator.h"
#include "ElasticRubberFG.h"
#include "BuoyancyForceGenerator.h"
#include "GravityForceGenerator.h"
#include "Particle.h"
#include <iostream>
#include <algorithm>
#include "SandboxControls.h"

SandboxScene_Example::SandboxScene_Example(std::string name)
	: Scene(std::move(name)),
	m_sistema(50)
{
}

SandboxScene_Example::~SandboxScene_Example()
{
	cleanup();
}

void SandboxScene_Example::initEffects()
{
	m_sistema.cleanup();

	// Gravedad global para la escena
	auto gravity = std::make_unique<GravityForceGenerator>(
		Vector3D(0.0f, 15.0f, 0.0f),
		Vector3D(60.0f, 30.0f, 30.0f),
		Vector3D(0.0f, -9.8f, 0.0f)
	);
	m_gravity = static_cast<GravityForceGenerator*>(m_sistema.addForceGenerator(std::move(gravity)));

	// 1. Muelle anclado a posicion fija
	m_anchoredParticle = new Particle(
		2.0f,
		Vector3D(-20.0f, 12.0f, 0.0f),
		Vector3D::zero(),
		Vector3D::zero(),
		0.99f,
		0.5f,
		Vector4(0.1f, 0.7f, 1.0f, 1.0f),
		1e6f
	);
	m_sistema.addParticle(m_anchoredParticle);

	auto anchoredSpring = std::make_unique<AnchoredSpringFG>(
		40.0,
		6.0,
		Vector3D(-20.0f, 20.0f, 0.0f),
		m_anchoredParticle
	);
	m_anchoredSpring = static_cast<AnchoredSpringFG*>(m_sistema.addForceGenerator(std::move(anchoredSpring)));

	// 2. Dos particulas unidas por muelle
	m_springP1 = new Particle(2.0f, Vector3D(-6.0f, 16.0f, 0.0f), Vector3D::zero(), Vector3D::zero(), 0.99f, 0.45f, Vector4(0.2f, 0.9f, 0.3f, 1.0f), 1e6f);
	m_springP2 = new Particle(2.0f, Vector3D(-6.0f, 8.0f, 0.0f), Vector3D::zero(), Vector3D::zero(), 0.99f, 0.45f, Vector4(0.2f, 0.9f, 0.3f, 1.0f), 1e6f);
	m_sistema.addParticle(m_springP1);
	m_sistema.addParticle(m_springP2);

	auto sp12 = std::make_unique<SpringForceGenerator>(30.0, 6.0, m_springP2, m_springP1);
	auto sp21 = std::make_unique<SpringForceGenerator>(30.0, 6.0, m_springP1, m_springP2);
	m_spring12 = static_cast<SpringForceGenerator*>(m_sistema.addForceGenerator(std::move(sp12)));
	m_spring21 = static_cast<SpringForceGenerator*>(m_sistema.addForceGenerator(std::move(sp21)));

	// 2B. Dos particulas unidas por goma elastica
	m_rubberP1 = new Particle(2.0f, Vector3D(6.0f, 16.0f, 0.0f), Vector3D::zero(), Vector3D::zero(), 0.99f, 0.45f, Vector4(1.0f, 0.85f, 0.2f, 1.0f), 1e6f);
	m_rubberP2 = new Particle(2.0f, Vector3D(6.0f, 8.0f, 0.0f), Vector3D::zero(), Vector3D::zero(), 0.99f, 0.45f, Vector4(1.0f, 0.85f, 0.2f, 1.0f), 1e6f);
	m_sistema.addParticle(m_rubberP1);
	m_sistema.addParticle(m_rubberP2);

	auto rb12 = std::make_unique<ElasticRubberFG>(35.0, 6.0, m_rubberP2, m_rubberP1);
	auto rb21 = std::make_unique<ElasticRubberFG>(35.0, 6.0, m_rubberP1, m_rubberP2);
	m_rubber12 = static_cast<ElasticRubberFG*>(m_sistema.addForceGenerator(std::move(rb12)));
	m_rubber21 = static_cast<ElasticRubberFG*>(m_sistema.addForceGenerator(std::move(rb21)));

	// 3. Objeto flotante en agua
	m_boatParticle = new Particle(
		1000.0f,
		Vector3D(22.0f, 6.0f, 0.0f),
		Vector3D::zero(),
		Vector3D::zero(),
		0.7f,
		1.0f,
		Vector4(1.0f, 0.5f, 0.1f, 1.0f),
		1e6f,
		ParticleShape::BOX,
		Vector3D(1.0f, 1.0f, 1.0f)
	);
	m_sistema.addParticle(m_boatParticle);

	auto buoyancy = std::make_unique<BuoyancyForceGenerator>(
		2.0f,
		2.0f,
		1000.0f,
		5.0f,
		nullptr,
		m_boatParticle
	);
	m_buoyancy = static_cast<BuoyancyForceGenerator*>(m_sistema.addForceGenerator(std::move(buoyancy)));
}

void SandboxScene_Example::init()
{
	cleanup();
	initEffects();

	m_floorRenderItem = new RenderItem(
		CreateShape(physx::PxBoxGeometry(60.0f, 0.2f, 40.0f)),
		&m_floorPose,
		Vector4(0.25f, 0.25f, 0.25f, 1.0f)
	);

	m_waterRenderItem = new RenderItem(
		CreateShape(physx::PxBoxGeometry(8.0f, 0.05f, 8.0f)),
		&m_waterPose,
		Vector4(0.0f, 0.4f, 0.9f, 0.7f)
	);

	std::cout << "SANDBOX - ESCENA DE MUELLES Y FLOTACION\n";
	std::cout << "  [P] Pausar   [R] Reiniciar   [,] Escena principal   [.] Esta escena\n";
	std::cout << "  [ESPACIO] Pistola  [B] Canon  [N] Obus (desde la camara)\n";
	std::cout << "  Generadores de fuerza: [1] Gravedad  [5] Flotacion  [6] Muelle anclado  [7] Muelle doble  [8] Goma elastica\n";
	std::cout << "  Empujes: [I] muelle anclado  [T] muelle doble  [G] goma  [Z] barco\n";
	std::cout << "  [K]/[J] +/- k del muelle  [M] cambia integrador  [U]/[O] +/- masa del barco\n";

}

void SandboxScene_Example::update(double dt)
{
	if (!m_paused)
	{
		m_sistema.update(dt);
	}
}

void SandboxScene_Example::keyPress(unsigned char key, Camera* camera)
{
	switch (toupper(key))
	{
		// --- DISPARO DE PROYECTILES DESDE LA CAMARA (Requisito B) ---
	case ' ':
		if (camera)
		{
			Vector3D eye = camera->getEye();
			Vector3D dir = camera->getDir();
			Projectile* p = Projectile::createPistolBullet(eye + dir * 2.0f, dir);
			m_sistema.addParticle(p);
			std::cout << "[DISPARO] Bala de Pistola: m_sim=" << p->getMass()
				<< " kg, v_sim=" << p->getSimSpeed() << " m/s, g_sim=" << p->getScaledGravity() << " m/s^2\n";
		}
		break;

	case 'B':
		if (camera)
		{
			Vector3D eye = camera->getEye();
			Vector3D dir = camera->getDir();
			Projectile* p = Projectile::createCannonball(eye + dir * 2.5f, dir);
			m_sistema.addParticle(p);
			std::cout << "[DISPARO] Bala de Canon: m_sim=" << p->getMass()
				<< " kg, v_sim=" << p->getSimSpeed() << " m/s, g_sim=" << p->getScaledGravity() << " m/s^2\n";
		}
		break;

	case 'N':
		if (camera)
		{
			Vector3D eye = camera->getEye();
			Vector3D dir = camera->getDir();
			Projectile* p = Projectile::createArtillery(eye + dir * 3.0f, dir);
			m_sistema.addParticle(p);
			std::cout << "[DISPARO] Obus de Artilleria: m_sim=" << p->getMass()
				<< " kg, v_sim=" << p->getSimSpeed() << " m/s, g_sim=" << p->getScaledGravity() << " m/s^2\n";
		}
		break;

	case 'I':
		if (m_anchoredParticle)
		{
			m_anchoredParticle->addForce(Vector3D(0.0f, -300.0f, 0.0f));
			std::cout << "Impulso aplicado al muelle anclado\n";
		}
		break;

	case 'K':
		if (m_anchoredSpring)
		{
			m_anchoredSpring->setK(m_anchoredSpring->getK() + 20.0);
			std::cout << "k muelle = " << m_anchoredSpring->getK() << "\n";
		}
		break;

	case 'J':
		if (m_anchoredSpring)
		{
			double newK = m_anchoredSpring->getK() - 20.0;
			if (newK < 5.0) newK = 5.0;
			m_anchoredSpring->setK(newK);
			std::cout << "k muelle = " << m_anchoredSpring->getK() << "\n";
		}
		break;

	case 'M':
		if (m_anchoredParticle)
		{
			m_anchoredParticle->setIntegrationMethod(!m_anchoredParticle->isSemiImplicit());
			std::cout << "Integrador muelle: " << (m_anchoredParticle->isSemiImplicit() ? "Euler Semi-implicito" : "Euler Explicito") << "\n";
		}
		break;

		// --- Activacion/desactivacion de generadores de fuerza ---
	case '1':
		ToggleForceKey(m_sistema, ForceGeneratorType::GRAVITY, "Gravedad");
		break;

	case '5':
		ToggleForceKey(m_sistema, ForceGeneratorType::BUOYANCY, "Flotacion");
		break;

	case '6':
		ToggleForceKey(m_sistema, ForceGeneratorType::ANCHORED_SPRING, "Muelle anclado");
		break;

	case '7':
		ToggleForceKey(m_sistema, ForceGeneratorType::SPRING, "Muelle doble");
		break;

	case '8':
		ToggleForceKey(m_sistema, ForceGeneratorType::RUBBER, "Goma elastica");
		break;

		// --- Empujes de demostracion ---
	case 'T':
		if (m_springP1)
		{
			m_springP1->addForce(Vector3D(0.0f, 150.0f, 0.0f));
			std::cout << "Empuje a muelle doble\n";
		}
		break;

	case 'G':
		if (m_rubberP1)
		{
			m_rubberP1->addForce(Vector3D(0.0f, 150.0f, 0.0f));
			std::cout << "Empuje a goma elastica\n";
		}
		break;

	case 'U':
		if (m_boatParticle)
		{
			m_boatParticle->setMass(m_boatParticle->getMass() + 250.0f);
			std::cout << "Masa objeto flotante = " << m_boatParticle->getMass() << " kg\n";
		}
		break;

	case 'O':
		if (m_boatParticle)
		{
			float newM = m_boatParticle->getMass() - 250.0f;
			if (newM < 100.0f) newM = 100.0f;
			m_boatParticle->setMass(newM);
			std::cout << "Masa objeto flotante = " << m_boatParticle->getMass() << " kg\n";
		}
		break;

	case 'Z':
		if (m_boatParticle)
		{
			m_boatParticle->addForce(Vector3D(0.0f, -6000.0f, 0.0f));
			std::cout << "Objeto empujado hacia abajo en el agua\n";
		}
		break;

	case 'P':
		m_paused = !m_paused;
		std::cout << "Simulacion " << (m_paused ? "pausada" : "reanudada") << "\n";
		break;

	case 'R':
		initEffects();
		std::cout << "Escena reiniciada\n";
		break;

	default:
		break;
	}
}

void SandboxScene_Example::cleanup()
{
	m_sistema.cleanup();

	m_anchoredParticle = nullptr;
	m_anchoredSpring = nullptr;

	m_springP1 = nullptr;
	m_springP2 = nullptr;
	m_spring12 = nullptr;
	m_spring21 = nullptr;

	m_rubberP1 = nullptr;
	m_rubberP2 = nullptr;
	m_rubber12 = nullptr;
	m_rubber21 = nullptr;

	m_boatParticle = nullptr;
	m_buoyancy = nullptr;
	m_gravity = nullptr;

	if (m_floorRenderItem)
	{
		m_floorRenderItem->release();
		m_floorRenderItem = nullptr;
	}
	if (m_waterRenderItem)
	{
		m_waterRenderItem->release();
		m_waterRenderItem = nullptr;
	}
}