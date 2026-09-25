#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include <vector>
#include "Vector3D.h"
#include "Particle.h"

class Practica0Scene : public Scene {
public:
    explicit Practica0Scene(std::string name) : Scene(std::move(name)) {}

    void init() override {
        physx::PxShape* dot = CreateShape(physx::PxSphereGeometry(1.0f));
        m_originDot_transform = physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f));
        m_xDot_transform = physx::PxTransform(physx::PxVec3(10.0f, 0.0f, 0.0f));
        m_yDot_transform = physx::PxTransform(physx::PxVec3(0.0f, 10.0f, 0.0f));
        m_zDot_transform = physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 10.0f));

        //Reto A Practica 0
        Vector3D u(3, 1, 0);
        Vector3D v(3, 4, 0);

		Vector3D w = u.cross(v);

		u=u.normalize();
		v=v.normalize();
		w=w.normalize();
        
		u = u * 5.0f;
		v = v * 5.0f;
		w = w * 5.0f;

		miParticula = new Particle(1.0f, Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, -9.81f, 0.0f), 0.25f);

        miParticula->addForce({ 0.0f, 20.0f, 0.0f });


        // Se registra el RenderItem exactamente como en la plantilla original
        m_originDot_renderItem = new RenderItem(dot, &m_originDot_transform, Vector4(1.0f, 1.0f, 1.0f, 1.0f)); // Blanco
		m_xDot_renderItem = new RenderItem(dot, &m_xDot_transform, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		m_yDot_renderItem = new RenderItem(dot, &m_yDot_transform, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		m_zDot_renderItem = new RenderItem(dot, &m_zDot_transform, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
    }

    void update(double dt) override {
        // Lógica/Integración del alumno (por ejemplo, movimiento simple)
        //m_transform.p.y -= static_cast<float>(9.8 * dt);
        miParticula->integrate(dt);
    }

    void keyPress(unsigned char key, const physx::PxTransform& camera) override {
        if (key == 'r' || key == 'R') {
            m_originDot_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
        }
    }

    void cleanup() override {
        if (m_originDot_renderItem) {
            m_originDot_renderItem->release(); // Deregistra y destruye el item
            m_originDot_renderItem = nullptr;
        }
        if (m_xDot_renderItem) {
            m_xDot_renderItem->release(); // Deregistra y destruye el item
            m_xDot_renderItem = nullptr;
        }
        if (m_yDot_renderItem) {
            m_yDot_renderItem->release(); // Deregistra y destruye el item
            m_yDot_renderItem = nullptr;
        }
        if (m_zDot_renderItem) {
            m_zDot_renderItem->release(); // Deregistra y destruye el item
            m_zDot_renderItem = nullptr;
        }
        if (miParticula) miParticula->cleanup();
    }

private:
    physx::PxTransform m_originDot_transform;
    physx::PxTransform m_xDot_transform;
    physx::PxTransform m_yDot_transform;
    physx::PxTransform m_zDot_transform;

    RenderItem* m_originDot_renderItem{ nullptr };
	RenderItem* m_xDot_renderItem{ nullptr };
	RenderItem* m_yDot_renderItem{ nullptr };
	RenderItem* m_zDot_renderItem{ nullptr };

    Particle* miParticula = nullptr;
};