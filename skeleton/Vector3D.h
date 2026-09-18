#pragma once
class Vector3D
{
public:
	float x, y, z;
	Vector3D() : x(), y(),z() {}
	Vector3D(const float x, const float y, const float z) : x(x), y(y),z(z) {}


	float magnitude() const;
	Vector3D normalize() const; 

	float dot(const Vector3D& v) const { return x * v.x + y * v.y + z * v.z; }
	Vector3D cross(const Vector3D& v) const 
	{ return {(y*v.z)-(z * v.y), (z * v.x) - (x * v.z) , (x * v.y) - (y * v.x) }; };
	Vector3D operator+(const Vector3D& v) const { return { x + v.x, y + v.y, z + v.z }; }
	Vector3D operator-(const Vector3D& v) const { return { x - v.x, y - v.y, z - v.z }; }
	Vector3D operator*(const float scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Vector3D opearator(const Vector3D & v) { x += v.x; y += v.y; z += v.z; return *this; }
	operator physx::PxVec3() const
	{
		return physx::PxVec3(x, y, z);
	}


};

