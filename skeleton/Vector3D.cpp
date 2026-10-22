#include "Vector3D.h"
#include <cmath>

Vector3D::Vector3D(physx::PxVec3 vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

float Vector3D::magnitude() const
{
	return std::sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::normalize() const
{
	return Vector3D(x/magnitude(),y/magnitude(),z/magnitude());
}

float Vector3D::dot(const Vector3D& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3D Vector3D::cross(const Vector3D& v) const
{
	return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

Vector3D& Vector3D::operator=(const Vector3D& other)
{
	if (this != &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	return *this;
}

Vector3D Vector3D::operator+(Vector3D other)
{
	return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator-(Vector3D other)
{
	return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator*(int n)
{
	return Vector3D(x * n, y * n, z * n);
}

void Vector3D::operator+=(Vector3D other)
{
	x += other.x;
	y += other.y;
	z += other.z;
}

Vector3D::operator physx::PxVec3() const
{
	return physx::PxVec3(x,y,z);

}


