#pragma once
#include "RenderUtils.hpp"
class Practica0Axes
{
	public:
        
        Practica0Axes() {
    
        physx::PxShape* dot = CreateShape(physx::PxSphereGeometry(1.0f));
        m_originDot_transform = physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f));
        m_xDot_transform = physx::PxTransform(physx::PxVec3(10.0f, 0.0f, 0.0f));
        m_yDot_transform = physx::PxTransform(physx::PxVec3(0.0f, 10.0f, 0.0f));
        m_zDot_transform = physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 10.0f));

		m_originDot_renderItem = new RenderItem(dot, &m_originDot_transform, Vector4(1.0f, 1.0f, 1.0f, 1.0f)); // Blanco
		m_xDot_renderItem = new RenderItem(dot, &m_xDot_transform, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		m_yDot_renderItem = new RenderItem(dot, &m_yDot_transform, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		m_zDot_renderItem = new RenderItem(dot, &m_zDot_transform, Vector4(0.0f, 0.0f, 1.0f, 1.0f));



         }
        ~Practica0Axes() {

            cleanup();
        }

        void cleanup()
        {
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

    

    


};

    

