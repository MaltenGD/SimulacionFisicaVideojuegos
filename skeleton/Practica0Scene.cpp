#include "Practica0Scene.h"
#include "Practica0Axes.h"
#include "Vector3D.h"

Practica0Scene::Practica0Scene(std::string name) : Scene(std::move(name)) {}

Practica0Scene::~Practica0Scene()
{
	cleanup();
}

void Practica0Scene::init()
{
	m_axes = new Practica0Axes();

	//Reto A Practica 0
	Vector3D u(3, 1, 0);
	Vector3D v(3, 4, 0);

	Vector3D w = u.cross(v);

	u = u.normalize();
	v = v.normalize();
	w = w.normalize();

	u = u * 5.0f;
	v = v * 5.0f;
	w = w * 5.0f;
}

void Practica0Scene::update(double dt)
{
}


void Practica0Scene::cleanup()
{
	if (m_axes) {
		m_axes->cleanup();
		delete m_axes;
		m_axes = nullptr;
	}
}

