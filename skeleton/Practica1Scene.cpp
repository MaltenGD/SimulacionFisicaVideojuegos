#include "Practica1Scene.h"
#include <iostream>
#include <iomanip>

Practica1Scene::Practica1Scene(std::string name) : Practica0Scene(std::move(name)) {}

Practica1Scene::~Practica1Scene()
{
	cleanup();
}

void Practica1Scene::initConfigs()
{
	m_configs.clear();

	// Bala de pistola
	m_configs.emplace_back(
		"Bala",
		0.015f,
		380.0f,
		60.0f,
		-9.81f,
		0.999f,
		0.25f,
		Vector4(1.0f, 0.85f, 0.1f, 1.0f),
		15.0f
	);
	std::cout << "Pulsa B para disparar bala (Alta velocidad) simulada a 60 m/s" << '\n';

	// Bola de Cañón
	m_configs.emplace_back(
		"Bola de Canon",
		8.0f,
		60.0f,
		12.0f,
		-9.81f,
		0.99f,
		0.8f,
		Vector4(0.2f, 0.2f, 0.25f, 1.0f),
		20.0f
	);
	std::cout << "Pulsa C para disparar Bala de canyon (Baja velocidad velocidad) simulada a 12 m/s" << '\n';

	// Globo con helio (gravedad positiva)
	m_configs.emplace_back(
		"Globo con helio",
		0.05f,
		25.0f,
		5.0f,
		+4.81f,
		0.96f,
		0.6f,
		Vector4(0.1f, 0.8f, 1.0f, 1.0f),
		15.0f
	);
	std::cout << "Pulsa F para disparar Globo con helio (Baja velocidad y Flota) simulada a 5 m/s" << '\n';
	std::cout << "Pulsa P para disparar el utimo tipo seleccionado" << '\n';

}


void Practica1Scene::shootProjectile(size_t index, Camera* camera)
{
	if (index >= m_configs.size()) return;
	m_selectedConfigIndex = index;

	const ProjectileConfig& cfg = m_configs[index];
	Projectile* p = Projectile::createFromCamera(cfg, camera);
	if (p)
	{
		m_Particulas.push_back(p);
		std::cout << "Se dispara: " << cfg.name << '\n'
		          << "Velocidad real: " << cfg.realSpeed << " m/s" << " |||S" << "Velocidad simulada: " << cfg.simSpeed << " m/s" << '\n'
		          << "Gravedad simulada: " << cfg.getSimGravity() << " m/s^2" << '\n'
		          << "Masa simulada: " << cfg.getSimMass() << " kg\n" << '\n';
	}
}

void Practica1Scene::init()
{
	cleanup();
	Practica0Scene::init();
	initConfigs();
	// Partícula de la parte 1 con velocidad constante , aceleración y damping
	Particle* ParticulaApt1 = new Particle(1.0f, Vector3D(0.0f, 25.0f, 0.0f), Vector3D(5.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, -9.81f, 0.0f), 0.98f, 0.8f, Vector4(0.2f, 0.8f, 0.2f, 1.0f));
	m_Particulas.push_back(ParticulaApt1);


	m_floorRenderItem = new RenderItem(
		CreateShape(physx::PxBoxGeometry(80.0f, 0.2f, 80.0f)),
		&m_floorPose,
		Vector4(0.25f, 0.35f, 0.25f, 1.0f)
	);
}

void Practica1Scene::update(double dt)
{

	for (auto it = m_Particulas.begin(); it != m_Particulas.end(); )
	{
		(*it)->integrate_Semi_Implicit(dt);
		if (!(*it)->isAlive())
		{
			delete *it;
			it = m_Particulas.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Practica1Scene::keyPress(unsigned char key, Camera* camera)
{
	switch (toupper(key))
	{
	case 'B':
		shootProjectile(0, camera);
		break;
	case 'C':
		shootProjectile(1, camera);
		break;
	case 'F':
		shootProjectile(2, camera);
		break;
	case 'P':
		shootProjectile(m_selectedConfigIndex, camera);
		break;
	case 'M':
		if (m_selectedConfigIndex < m_configs.size())
		{
			ProjectileConfig& configActual = m_configs[m_selectedConfigIndex];
			configActual.changeRealSpeed(configActual.realSpeed * 1.25f);

			std::cout << "Velocidad Aumentada " << configActual.name
			          << "V_real: " << configActual.realSpeed << " m/s"
			          << "V_sim: " << configActual.simSpeed << " m/s"
			          << "G_sim: " << configActual.getSimGravity() << " m/s^2"
			          << "M_sim: " << configActual.getSimMass() << " kg\n";
		}
		break;
	case 'N':
		if (m_selectedConfigIndex < m_configs.size())
		{
			ProjectileConfig& configActual = m_configs[m_selectedConfigIndex];
			configActual.changeRealSpeed(configActual.realSpeed * 0.8f);

			std::cout << "Velocidad Reducida " << configActual.name
			          << "V_real: " << configActual.realSpeed << " m/s"
			          << "V_sim: " << configActual.simSpeed << " m/s"
			          << "G_sim: " << configActual.getSimGravity() << " m/s^2"
			          << "M_sim: " << configActual.getSimMass() << " kg\n";
		}
		break;
	case 'R': // Para reiniciar proyectiles y la partícula del apartado 1
		for (auto* p : m_Particulas)
		{
			delete p;
		}
		m_Particulas.clear();
		m_Particulas.push_back(new Particle(1.0f, Vector3D(0.0f, 25.0f, 0.0f), Vector3D(5.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, -9.81f, 0.0f), 0.98f, 0.8f, Vector4(0.2f, 0.8f, 0.2f, 1.0f)));
		break;
	default:
		break;
	}
}

void Practica1Scene::cleanup()
{
	for (auto* p : m_Particulas)
	{
		delete p;
	}
	m_Particulas.clear();

	// El suelo :3
	if (m_floorRenderItem)
	{
		m_floorRenderItem->release();
		m_floorRenderItem = nullptr;
	}

	Practica0Scene::cleanup();
}


