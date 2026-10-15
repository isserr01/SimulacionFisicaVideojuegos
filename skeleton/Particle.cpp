
#include "Particle.h"
#include "RenderUtils.hpp"
#include <cmath>

Particle::Particle(Vector3D Pos, Vector3D Vel)
    : vel(Vel),
    acc(Vector3D(0, 0, 0)),
    damping(1.0),
    pose(physx::PxVec3(
        (float)Pos.x,
        (float)Pos.y,
        (float)Pos.z)),
    renderItem(nullptr),
    mass(5)
{
    physx::PxShape* shape =
        CreateShape(physx::PxSphereGeometry(1.0f));

    renderItem = new RenderItem(
        shape,
        &pose,
        Vector4(1.0f, 0.0f, 0.0f, 1.0f)
    );


    shape->release();
}

Particle::Particle(Vector3D Pos, Vector3D Vel,
    Vector3D Acc, double Damping)
    : vel(Vel),
    acc(Acc),
    damping(Damping),
    pose(physx::PxVec3(
        (float)Pos.x,
        (float)Pos.y,
        (float)Pos.z)),
    renderItem(nullptr),
    mass(5)
{
    physx::PxShape* shape =
        CreateShape(physx::PxSphereGeometry(1.0f));

    renderItem = new RenderItem(
        shape,
        &pose,
        Vector4(1.0f, 0.0f, 0.0f, 1.0f)
    );

    shape->release();
}


Particle::~Particle()
{
    if (renderItem != nullptr)
    {
        renderItem->release();
        renderItem = nullptr;
    }
}


void Particle::integrate(double t)
{
    if (t <= 0.0)
        return;

    pose.p.x += (float)(vel.x * t);
    pose.p.y += (float)(vel.y * t);
    pose.p.z += (float)(vel.z * t);

    vel.x += acc.x * t;
    vel.y += acc.y * t;
    vel.z += acc.z * t;

    double factor = std::pow(damping, t);

    vel.x *= factor;
    vel.y *= factor;
    vel.z *= factor;
}
