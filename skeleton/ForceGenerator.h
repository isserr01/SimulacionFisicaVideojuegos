
#pragma once

#include "Particle.h"
#include <cmath>


class ForceGenerator
{
public:
    virtual ~ForceGenerator() = default;

    virtual void updateForce(Particle& particle, double dt) = 0;
};

// --------------------------------------------------
// GRAVEDAD
// --------------------------------------------------

class GravityGenerator : public ForceGenerator
{
public:
    explicit GravityGenerator(Vector3D gravity)
        : gravity(gravity)
    {
    }

    void updateForce(Particle& particle, double) override
    {
        const double mass = particle.getMass();

        if (mass <= 0.0)
            return;

        particle.addForce(Vector3D(
            gravity.x * mass,
            gravity.y * mass,
            gravity.z * mass
        ));
    }

private:
    Vector3D gravity;
};

// --------------------------------------------------
// VIENTO LINEAL
// --------------------------------------------------

class WindGenerator : public ForceGenerator
{
public:
    WindGenerator(Vector3D center,
        double radius,
        Vector3D windVelocity,
        double k1)
        : center(center),
        radius(radius),
        windVelocity(windVelocity),
        k1(k1)
    {
    }

    void updateForce(Particle& particle, double) override
    {
        Vector3D p = particle.getPosition();

        const double dx = p.x - center.x;
        const double dy = p.y - center.y;
        const double dz = p.z - center.z;

        if (dx * dx + dy * dy + dz * dz > radius * radius)
            return;

        Vector3D v = particle.getVelocity();

        particle.addForce(Vector3D(
            k1 * (windVelocity.x - v.x),
            k1 * (windVelocity.y - v.y),
            k1 * (windVelocity.z - v.z)
        ));
    }

private:
    Vector3D center;
    double radius;
    Vector3D windVelocity;
    double k1;
};

// --------------------------------------------------
// TORBELLINO
// --------------------------------------------------

class VortexGenerator : public ForceGenerator
{
public:
    VortexGenerator(Vector3D center,
        double radius,
        double strength,
        double k1)
        : center(center),
        radius(radius),
        strength(strength),
        k1(k1)
    {
    }

    void updateForce(Particle& particle, double) override
    {
        Vector3D p = particle.getPosition();

        const double dx = p.x - center.x;
        const double dy = p.y - center.y;
        const double dz = p.z - center.z;

        if (dx * dx + dy * dy + dz * dz > radius * radius)
            return;

        Vector3D vortexVelocity(
            -strength * dy,
            strength * dx,
            0.0
        );

        Vector3D v = particle.getVelocity();

        particle.addForce(Vector3D(
            k1 * (vortexVelocity.x - v.x),
            k1 * (vortexVelocity.y - v.y),
            k1 * (vortexVelocity.z - v.z)
        ));
    }

private:
    Vector3D center;
    double radius;
    double strength;
    double k1;
};

// --------------------------------------------------
// EXPLOSION
// --------------------------------------------------

class ExplosionGenerator : public ForceGenerator
{
public:
    ExplosionGenerator(Vector3D center,
        double radius,
        double strength,
        double tau)
        : center(center),
        radius(radius),
        strength(strength),
        tau(tau),
        elapsed(0.0),
        active(false)
    {
    }

    void trigger()
    {
        elapsed = 0.0;
        active = true;
    }

    void updateForce(Particle& particle, double dt) override
    {
        if (!active)
            return;

        Vector3D p = particle.getPosition();

        const double dx = p.x - center.x;
        const double dy = p.y - center.y;
        const double dz = p.z - center.z;

        const double r2 = dx * dx + dy * dy + dz * dz;

        if (r2 < radius * radius && r2 > 1e-8)
        {
            const double r = std::sqrt(r2);
            const double magnitude =
                strength / r2 * std::exp(-elapsed / tau);

            particle.addForce(Vector3D(
                magnitude * dx / r,
                magnitude * dy / r,
                magnitude * dz / r
            ));
        }
    }

    void advance(double dt)
    {
        if (!active)
            return;

        elapsed += dt;

        if (tau <= 0.0 || elapsed >= 4.0 * tau)
            active = false;
    }

private:
    Vector3D center;
    double radius;
    double strength;
    double tau;
    double elapsed;
    bool active;
};
