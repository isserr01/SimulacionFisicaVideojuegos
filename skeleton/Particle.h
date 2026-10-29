#pragma once

#include "Vector3D.h"
#include <PxPhysics.h>

class RenderItem;
class ForceGenerator;

class Particle
{
public:
    Particle(Vector3D position, Vector3D velocity);
    Particle(Vector3D position, Vector3D velocity,
        Vector3D acceleration, double damping);
    Particle(Vector3D position, Vector3D velocity,
        Vector3D acceleration, double damping, double massValue);
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

    Vector3D getVelocity() const { return vel; }
    Vector3D getAcceleration() const { return acc; }
    double getDamping() const { return damping; }
    void setAcceleration(Vector3D value) { acc = value; }
    void scaleVelocity(double factor);

    void addForce(Vector3D force);
    void applyForceGenerator(ForceGenerator& generator, double dt);
    void clearForces();


private:
    Vector3D forceAccum;
    Vector3D vel;
    Vector3D acc;
    double damping;
    double mass;

    physx::PxTransform pose;
    RenderItem* renderItem;
};
