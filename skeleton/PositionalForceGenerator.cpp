#include "PositionalForceGenerator.h"

PositionalForceGenerator::PositionalForceGenerator(const Vector3D& origin, const Vector3D& halfExtents, const Vector4& debugColor)
	: m_origin(origin),
	  m_halfExtents(halfExtents),
	  m_pose(physx::PxTransform(origin)),
	  m_renderItem(nullptr),
	  m_debugColor(debugColor),
	  m_debug(false)
{
}

PositionalForceGenerator::~PositionalForceGenerator()
{
	if (m_renderItem)
	{
		m_renderItem->release();
		m_renderItem = nullptr;
	}
}

bool PositionalForceGenerator::isInside(const Vector3D& pos) const
{
	return (pos.x >= m_origin.x - m_halfExtents.x && pos.x <= m_origin.x + m_halfExtents.x) &&
	       (pos.y >= m_origin.y - m_halfExtents.y && pos.y <= m_origin.y + m_halfExtents.y) &&
	       (pos.z >= m_origin.z - m_halfExtents.z && pos.z <= m_origin.z + m_halfExtents.z);
}

void PositionalForceGenerator::updateRenderItem()
{
	if (m_renderItem)
	{
		m_renderItem->release();
		m_renderItem = nullptr;
	}

	if (m_debug)
	{
		m_pose = physx::PxTransform(m_origin);
		physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(m_halfExtents.x, m_halfExtents.y, m_halfExtents.z));
		if (shape)
		{
			// Marcamos la forma como TRIGGER para que se dibuje en modo líneas / wireframe
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
			m_renderItem = new RenderItem(shape, &m_pose, m_debugColor);
		}
	}
}

void PositionalForceGenerator::setDebug(bool debug)
{
	if (m_debug != debug)
	{
		m_debug = debug;
		updateRenderItem();
	}
}

void PositionalForceGenerator::toggleDebug()
{
	setDebug(!m_debug);
}

void PositionalForceGenerator::setOrigin(const Vector3D& origin)
{
	m_origin = origin;
	m_pose = physx::PxTransform(m_origin);
	if (m_debug)
	{
		updateRenderItem();
	}
}

void PositionalForceGenerator::setHalfExtents(const Vector3D& halfExtents)
{
	m_halfExtents = halfExtents;
	if (m_debug)
	{
		updateRenderItem();
	}
}

void PositionalForceGenerator::setDebugColor(const Vector4& color)
{
	m_debugColor = color;
	if (m_debug)
	{
		updateRenderItem();
	}
}
