#include "Particle.h"
#include <cmath>

Particle::Particle(float Mass,Vector3D Pos, Vector3D Vel, Vector3D Acc, Vector3D Gravity, float Damping) :
    massInverse(Mass > 0.0f ? 1.0f / Mass : 0.0f),
    pose(physx::PxTransform(Pos)),
    renderItem(new RenderItem(
        CreateShape(physx::PxSphereGeometry(1.0f)),
        &pose,
        Vector4(1.0f, 1.0f, 1.0f, 1.0f))),
    vel(Vel),
    acc(Acc),
    gravity(Gravity),
    damping(Damping)
{}

Particle::~Particle()
{
	cleanup();
}

void Particle::integrate_Direct(double t)
{

}

void Particle::integrate_Semi_Implicit(double t)
{
    //Actualiza la particula utilizando la integración de Euler
    //Velocidad constante
    acc = acc + gravity + force * massInverse ; // force: sumatorio de fuerzas en este tick
    vel += (acc * t) * std::pow(damping, t); // ? : Por que se aplica damping aqui en vez de aplicarlo justo arriba.
    pose.p += physx::PxVec3(vel) * t; // ? : por que se vuelve a aplicar deltaTime.

	force = Vector3D(0.0f, 0.0f, 0.0f); // Reiniciar el sumatorio de fuerzas
    
}

void Particle::cleanup()
{
    if (renderItem) {
        renderItem->release(); // Deregistra y destruye el item
        renderItem = nullptr;
    }
}
Vector3D Particle::getForce()
{
    return force;
}

void Particle::addForce(Vector3D Force)
{
    force += Force;
}
