#pragma once
#include <PxPhysicsAPI.h>
#include <cmath>

class Vector3D
{
public:
	float x, y, z;

	// Constructores: por defecto (0,0,0), por parámetros (x,y,z) y de conversión desde physx::PxVec3
	Vector3D() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3D(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	Vector3D(const physx::PxVec3& v) : x(v.x), y(v.y), z(v.z) {}

	// Constructor de copia y asignación
	Vector3D(const Vector3D& v) = default;
	Vector3D& operator=(const Vector3D& v) = default;

	// Módulo y Normalización
	float magnitude() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	Vector3D normalize() const {
		float mag = magnitude();
		if (mag > 0.0f) {
			return { x / mag, y / mag, z / mag };
		}
		return { 0.0f, 0.0f, 0.0f };
	}

	// Operaciones algebraicas:
	// Producto escalar
	float dot(const Vector3D& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	// Producto vectorial
	Vector3D cross(const Vector3D& v) const {
		return {
			(y * v.z) - (z * v.y),
			(z * v.x) - (x * v.z),
			(x * v.y) - (y * v.x)
		};
	}

	// Sobrecarga de operadores básicos: +, -, * (por un escalar), +=
	Vector3D operator+(const Vector3D& v) const {
		return { x + v.x, y + v.y, z + v.z };
	}

	Vector3D operator-(const Vector3D& v) const {
		return { x - v.x, y - v.y, z - v.z };
	}

	Vector3D operator*(const float scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}

	friend Vector3D operator*(const float scalar, const Vector3D& v) {
		return v * scalar;
	}

	Vector3D& operator+=(const Vector3D& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	// Sobrecarga de operadores básicos: +, -, * (por un escalar), +=
	Vector3D operator+(const physx::PxVec3& v) const {
		return { x + v.x, y + v.y, z + v.z};
	}

	Vector3D operator-(const physx::PxVec3& v) const {
		return { x - v.x, y - v.y, z - v.z };
	}

	Vector3D& operator+=(const physx::PxVec3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}



	// Wrapper de conversión con PhysX: Operador de conversión
	operator physx::PxVec3() const {
		return physx::PxVec3(x, y, z);
	}
};

