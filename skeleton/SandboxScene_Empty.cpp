#include "SandboxScene_Empty.h"
#include "SandboxUI.h"
#include "Particle.h"
#include "Projectile.h"
#include "Render/Camera.h"
#include "SandboxControls.h"

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
	std::cout << "  [R] Reiniciar escena (recarga la demo)\n";
	std::cout << "\n";
	std::cout << "  --- DISPARO DE PROYECTILES DESDE LA CAMARA ---\n";
	std::cout << "  [ESPACIO] Bala de Pistola   [B] Bala de Canon   [N] Obus de Artilleria\n";
	std::cout << "\n";
	std::cout << "  --- GENERADORES DE FUERZA (activar/desactivar) ---\n";
	std::cout << "  [1] Gravedad  [2] Viento  [3] Torbellino  [4] Explosion  [5] Flotacion\n";
	std::cout << "  [X] Detonar explosion (requiere explosion activada)\n";
	std::cout << "\n";
	std::cout << "  --- GENERADORES DE PARTICULAS (activar/desactivar) ---\n";
	std::cout << "  [F] Fuente  [L] Fuego  [H] Niebla\n";
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

	case 'N':
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

	case '1':
		ToggleForceKey(m_sistema, ForceGeneratorType::GRAVITY, "Gravedad");
		break;

	case '2':
		ToggleForceKey(m_sistema, ForceGeneratorType::WIND, "Viento");
		break;

	case '3':
		ToggleForceKey(m_sistema, ForceGeneratorType::WHIRLWIND, "Torbellino");
		break;

	case '4':
		ToggleForceKey(m_sistema, ForceGeneratorType::EXPLOSION, "Explosion");
		break;

	case '5':
		ToggleForceKey(m_sistema, ForceGeneratorType::BUOYANCY, "Flotacion");
		break;

	case 'X':
		TriggerExplosionKey(m_sistema);
		break;

	case 'F':
		ToggleParticleGeneratorKey(m_sistema, ParticleGeneratorType::FOUNTAIN, "Fuente");
		break;

	case 'L':
		ToggleParticleGeneratorKey(m_sistema, ParticleGeneratorType::FIRE, "Fuego");
		break;

	case 'H':
		ToggleParticleGeneratorKey(m_sistema, ParticleGeneratorType::FOG, "Niebla");
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