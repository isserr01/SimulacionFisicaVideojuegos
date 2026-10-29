#include "Particle.h"
#include "ForceGenerator.h"
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
    : Particle(position, velocity, acceleration, dampingValue, 5.0)
{
}

Particle::Particle(Vector3D position, Vector3D velocity,
    Vector3D acceleration, double dampingValue, double massValue)
    : forceAccum(0.0, 0.0, 0.0),
    vel(velocity),
    acc(acceleration),
    damping(dampingValue),
    mass(massValue > 0.0 ? massValue : 1.0),
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

    const double invMass = (mass > 0.0) ? 1.0 / mass : 0.0;

    vel.x += (acc.x + forceAccum.x * invMass) * dt;
    vel.y += (acc.y + forceAccum.y * invMass) * dt;
    vel.z += (acc.z + forceAccum.z * invMass) * dt;

    pose.p.x += static_cast<float>(vel.x * dt);
    pose.p.y += static_cast<float>(vel.y * dt);
    pose.p.z += static_cast<float>(vel.z * dt);

    const double factor = std::pow(damping, dt);
    vel.x *= factor;
    vel.y *= factor;
    vel.z *= factor;

    clearForces();
}

void Particle::reset(Vector3D position, Vector3D velocity,
    Vector3D acceleration, double dampingValue)
{
    vel = velocity;
    acc = acceleration;
    damping = dampingValue;
    forceAccum = Vector3D(0.0, 0.0, 0.0);
    pose.p = physx::PxVec3(
        static_cast<float>(position.x),
        static_cast<float>(position.y),
        static_cast<float>(position.z));
}

void Particle::scaleVelocity(double factor)
{
    vel.x *= factor;
    vel.y *= factor;
    vel.z *= factor;
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

void Particle::applyForceGenerator(ForceGenerator& generator, double dt)
{
    generator.updateForce(*this, dt);
}

void Particle::addForce(Vector3D force)
{
    forceAccum.x += force.x;
    forceAccum.y += force.y;
    forceAccum.z += force.z;
}

void Particle::clearForces()
{
    forceAccum = Vector3D(0.0, 0.0, 0.0);
}