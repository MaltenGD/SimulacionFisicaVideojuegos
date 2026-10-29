#pragma once

#include "ForceGenerator.h"
#include "Vector3D.h"
#include "RenderUtils.hpp"

class RenderItem;

class PositionalForceGenerator : public ForceGenerator
{
protected:
	Vector3D m_origin = Vector3D::zero();
	Vector3D m_halfExtents = Vector3D(5.0f, 5.0f, 5.0f);
	physx::PxTransform m_pose{ physx::PxVec3(0.0f, 0.0f, 0.0f) };
	RenderItem* m_renderItem = nullptr;
	Vector4 m_debugColor = Vector4(0.0f, 0.8f, 1.0f, 1.0f);
	bool m_debug = false;

	void updateRenderItem();

public:
	PositionalForceGenerator(const Vector3D& origin, const Vector3D& halfExtents,
	                         const Vector4& debugColor = Vector4(0.0f, 0.8f, 1.0f, 1.0f));
	virtual ~PositionalForceGenerator() override;

	// Comprueba si una posición está dentro de la caja AABB delimitadora
	virtual bool isInside(const Vector3D& pos) const;

	// Control de visualización debug del área de efecto (renderiza los límites/aristas de la caja)
	void setDebug(bool debug);
	bool isDebug() const { return m_debug; }
	void toggleDebug();

	// Getters y Setters
	void setOrigin(const Vector3D& origin);
	Vector3D getOrigin() const { return m_origin; }

	void setHalfExtents(const Vector3D& halfExtents);
	Vector3D getHalfExtents() const { return m_halfExtents; }

	void setDebugColor(const Vector4& color);
	Vector4 getDebugColor() const { return m_debugColor; }
};
