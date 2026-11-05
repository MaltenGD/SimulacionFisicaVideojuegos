#include "SandboxScene_Empty.h"
#include "SandboxUI.h"
#include "Particle.h"
#include "Projectile.h"
#include "Render/Camera.h"

#include <cctype>
#include <iostream>

SandboxScene_Empty::SandboxScene_Empty(std::string name)
	: Scene(std::move(name)),
	  m_sistema(2000)
{
	m_ui = std::make_unique<SandboxUI>(m_sistema);
}

SandboxScene_Empty::~SandboxScene_Empty()
{
	cleanup();
}

void SandboxScene_Empty::init()
{
	cleanup();

	m_floorRenderItem = new RenderItem(
		CreateShape(physx::PxBoxGeometry(60.0f, 0.2f, 40.0f)),
		&m_floorPose,
		Vector4(0.25f, 0.25f, 0.25f, 1.0f));

	m_ui->seedDemo();

	std::cout << "  Usa la ventana Dear ImGui para crear, editar y borrar generadores.\n";
	std::cout << "  [P] Pausar / reanudar simulacion\n";
	std::cout << "  [R] Reiniciar escena de demostracion\n";
	std::cout << "  [,] Vaciar escena   [.] Cargar escena de ejemplo\n";
	std::cout << "  --- DISPARO DE PROYECTILES DESDE LA CAMARA ---\n";
	std::cout << "  [ESPACIO] Disparar Bala de Pistola ligera\n";
	std::cout << "  [B] Disparar Bala de Canon pesada\n";
	std::cout << "  [C] Disparar Obus de Artilleria\n";
}

void SandboxScene_Empty::update(double dt)
{
	if (!m_paused)
	{
		m_sistema.update(dt);
	}
}

void SandboxScene_Empty::keyPress(unsigned char key, Camera* camera)
{
	switch (toupper(key))
	{
	case ' ':
		if (camera)
		{
			Vector3D eye = camera->getEye();
			Vector3D dir = camera->getDir();
			Vector3D spawnPos = eye + dir * 2.0f;
			Projectile* p = Projectile::createPistolBullet(spawnPos, dir);
			m_sistema.addParticle(p);
			std::cout << "[DISPARO] Bala de Pistola desde camara: masa simulada=" << p->getMass()
			          << " kg, velocidad simulada=" << p->getSimSpeed() << " m/s, gravedad simulada=" << p->getScaledGravity() << " m/s^2\n";
		}
		break;

	case 'B':
		if (camera)
		{
			Vector3D eye = camera->getEye();
			Vector3D dir = camera->getDir();
			Vector3D spawnPos = eye + dir * 2.5f;
			Projectile* p = Projectile::createCannonball(spawnPos, dir);
			m_sistema.addParticle(p);
			std::cout << "[DISPARO] Bala de Canon desde camara: masa simulada=" << p->getMass()
			          << " kg, velocidad simulada=" << p->getSimSpeed() << " m/s, gravedad simulada=" << p->getScaledGravity() << " m/s^2\n";
		}
		break;

	case 'C':
		if (camera)
		{
			Vector3D eye = camera->getEye();
			Vector3D dir = camera->getDir();
			Vector3D spawnPos = eye + dir * 3.0f;
			Projectile* p = Projectile::createArtillery(spawnPos, dir);
			m_sistema.addParticle(p);
			std::cout << "[DISPARO] Obus de Artilleria desde camara: masa simulada=" << p->getMass()
			          << " kg, velocidad simulada=" << p->getSimSpeed() << " m/s, gravedad simulada=" << p->getScaledGravity() << " m/s^2\n";
		}
		break;

	case 'P':
		m_paused = !m_paused;
		std::cout << "Simulacion " << (m_paused ? "pausada" : "reanudada") << "\n";
		break;

	case 'R':
		cleanup();
		init();
		std::cout << "Escena reiniciada\n";
		break;

	default:
		break;
	}
}

void SandboxScene_Empty::drawIMGUI()
{
	if (m_ui)
	{
		m_ui->draw(m_paused);
	}
}

void SandboxScene_Empty::cleanup()
{
	m_sistema.cleanup();

	if (m_floorRenderItem)
	{
		m_floorRenderItem->release();
		m_floorRenderItem = nullptr;
	}
}
