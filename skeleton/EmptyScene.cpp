#include "EmptyScene.h"

EmptyScene::EmptyScene(std::string name) : Scene(std::move(name)) {}

void EmptyScene::init()
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(2.0f));
	m_transform = physx::PxTransform(physx::PxVec3(0.0f, 10.0f, 0.0f));

	m_renderItem = new RenderItem(shape, &m_transform, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}

void EmptyScene::update(double dt)
{
}

void EmptyScene::keyPress(unsigned char key, const physx::PxTransform& camera)
{
	if (key == 'r' || key == 'R') {
		m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
	}
}

void EmptyScene::cleanup()
{
	if (m_renderItem) {
		m_renderItem->release(); // Deregistra y destruye el item
		m_renderItem = nullptr;
	}
}
