#pragma once

#include <PxPhysicsAPI.h>

class Vector3D
{
public:
	float x, y, z;

	//constructors
	Vector3D() : x(0), y(0), z(0) {};
	Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
	Vector3D(physx::PxVec3 vec);

	float magnitude() const;
	Vector3D normalize() const;

	//algebraic operations
	float dot(const Vector3D& v) const;
	Vector3D cross(const Vector3D& v) const;

	//operators
	bool operator=(Vector3D other);
	Vector3D operator+(Vector3D other);
	Vector3D operator-(Vector3D other);
	Vector3D operator*(int n);
	void operator+=(Vector3D other);
	operator physx::PxVec3() const;

};

