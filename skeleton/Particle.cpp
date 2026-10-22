#include "Particle.h"
#include "RenderUtils.hpp"

#include <cmath>
#include <iostream>

namespace
{
    constexpr float PARTICLE_RADIUS = 0.25f;
    constexpr float PARTICLE_HIDDEN_COORD = 10000.0f;
}

Particle::Particle(Vector3D position, Vector3D velocity)
    : Particle(position, velocity, Vector3D(0.0, 0.0, 0.0), 1.0)
{
}

Particle::Particle(Vector3D position, Vector3D velocity,
    Vector3D acceleration, double dampingValue)
    : vel(velocity),
    acc(acceleration),
    damping(dampingValue),
    mass(5.0),
    pose(physx::PxVec3(
        static_cast<float>(position.x),
        static_cast<float>(position.y),
        static_cast<float>(position.z))),
    renderItem(nullptr)
{
    physx::PxShape* shape =
        CreateShape(physx::PxSphereGeometry(PARTICLE_RADIUS));

    if (shape != nullptr)
    {
        renderItem = new RenderItem(
            shape,
            &pose,
            Vector4(1.0f, 0.0f, 0.0f, 1.0f));

        shape->release();
    }
}

Particle::~Particle()
{
    if (renderItem != nullptr)
    {
        renderItem->release();
        renderItem = nullptr;
    }
}

void Particle::integrate(double dt)
{
    if (dt <= 0.0)
        return;

    static int frames = 0;

    if (++frames % 30 == 0)
    {
        std::cout << "Velocidad Y: " << vel.y
            << " | Aceleracion Y: " << acc.y
            << " | dt: " << dt << '\n';
    }


    // Primero actualizamos la velocidad con la aceleración.
    vel.x += acc.x * dt;
    vel.y += acc.y * dt;
    vel.z += acc.z * dt;

    // Después desplazamos la partícula con la velocidad actualizada.
    pose.p.x += static_cast<float>(vel.x * dt);
    pose.p.y += static_cast<float>(vel.y * dt);
    pose.p.z += static_cast<float>(vel.z * dt);

    const double factor = std::pow(damping, dt);
    vel.x *= factor;
    vel.y *= factor;
    vel.z *= factor;


}

void Particle::reset(Vector3D position, Vector3D velocity,
    Vector3D acceleration, double dampingValue)
{
    vel = velocity;
    acc = acceleration;
    this->damping = damping;
    pose.p = physx::PxVec3(position.x, position.y, position.z);


    std::cout << "RESET: velY=" << vel.y
        << " accY=" << acc.y
        << " posY=" << pose.p.y
        << '\n';
}

void Particle::setPosition(Vector3D position)
{
    pose.p.x = static_cast<float>(position.x);
    pose.p.y = static_cast<float>(position.y);
    pose.p.z = static_cast<float>(position.z);
}

Vector3D Particle::getPosition() const
{
    return Vector3D(pose.p.x, pose.p.y, pose.p.z);
}
