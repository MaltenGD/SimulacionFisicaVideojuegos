#include "SandboxScene_Empty.h"
#include "SandboxUI.h"
#include "Particle.h"

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

	std::cout << "SANDBOX EN TIEMPO REAL:\n";
	std::cout << "  Usa la ventana Dear ImGui para crear, editar y borrar generadores.\n";
	std::cout << "  [P] Pausar / reanudar simulacion\n";
	std::cout << "  [R] Reiniciar escena de demostracion\n";
	std::cout << "  [,] Vaciar escena   [.] Cargar escena de ejemplo\n";
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
	PX_UNUSED(camera);

	switch (toupper(key))
	{
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
