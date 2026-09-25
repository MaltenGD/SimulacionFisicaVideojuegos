#include "Practica1Scene.h"

Practica1Scene::Practica1Scene(std::string name) : Scene(std::move(name)) {}

void Practica1Scene::init()
{
	m_axes = new Practica0Axes();

	miParticula = new Particle(1.0f, Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, -9.81f, 0.0f), 0.25f);

	miParticula->addForce({ 0.0f, 20.0f, 0.0f });
}

void Practica1Scene::update(double dt)
{
	miParticula->integrate(dt);
}

void Practica1Scene::keyPress(unsigned char key, const physx::PxTransform& camera)
{
}

void Practica1Scene::cleanup()
{
	if (m_axes) {
		m_axes->cleanup();
		m_axes = nullptr;
	}
	if (miParticula)
	{
		miParticula->cleanup();
		miParticula = nullptr;
	}
}
