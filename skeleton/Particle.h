#pragma once

#include "Vector3D.h"
#include <PxPhysics.h>

class RenderItem;

class Particle
{
public:
    Particle(Vector3D position, Vector3D velocity);
    Particle(Vector3D position, Vector3D velocity,
        Vector3D acceleration, double damping);
    ~Particle();

    Particle(const Particle&) = delete;
    Particle& operator=(const Particle&) = delete;

    void integrate(double dt);
    void reset(Vector3D position, Vector3D velocity,
        Vector3D acceleration, double damping);
    void setPosition(Vector3D position);
    Vector3D getPosition() const;

    double getMass() const { return mass; }
    void setMass(double value) { mass = value; }

private:
    Vector3D vel;
    Vector3D acc;
    double damping;
    double mass;

    physx::PxTransform pose;
    RenderItem* renderItem;
};
