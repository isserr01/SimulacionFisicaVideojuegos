
#pragma once

#include "Vector3D.h"
#include <PxPhysics.h>

class RenderItem;

class Particle
{
public:

    Particle(Vector3D Pos, Vector3D Vel);
    Particle(Vector3D Pos, Vector3D Vel,
        Vector3D Acc, double Damping);

    ~Particle();

    void integrate(double t);

    double getMass() const
    {
        return mass;
    }
    void setMass(double m)
    {
        mass = m;
    }

private:
    Vector3D vel;
    Vector3D acc;
    double damping;
    double mass;

    physx::PxTransform pose;
    RenderItem* renderItem;
};
