
#pragma once

#include "Particle.h"
#include <cmath>
#include <algorithm>


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


// --------------------------------------------------
// MUELLE: particula unida a un punto fijo
// --------------------------------------------------
class SpringToAnchorGenerator : public ForceGenerator
{
public:
    SpringToAnchorGenerator(Vector3D anchor, double restLength, double k, double damping = 0.0)
        : anchor(anchor), restLength(restLength), k(k), damping(damping) {}
    void setK(double value) { k = value < 0.0 ? 0.0 : value; }
    double getK() const { return k; }
    void updateForce(Particle& p, double) override
    {
        Vector3D delta = p.getPosition() - anchor;
        const double length = delta.magnitude();
        if (length < 1e-6) return;
        Vector3D dir = delta.normalize();
        const double extension = length - restLength;
        const double speedAlong = p.getVelocity().dot(dir);
        const double magnitude = -k * extension - damping * speedAlong;
        p.addForce(Vector3D(dir.x * magnitude, dir.y * magnitude, dir.z * magnitude));
    }
private:
    Vector3D anchor;
    double restLength, k, damping;
};

// --------------------------------------------------
// MUELLE ENTRE DOS PARTICULAS / GOMA ELASTICA
// --------------------------------------------------

class PairSpringGenerator : public ForceGenerator
{
public:
    PairSpringGenerator(
        Particle* p1,
        Particle* p2,
        double restLength,
        double k,
        bool elasticBand,
        double damping
    )
        : p1(p1),
        p2(p2),
        restLength(restLength),
        k(k),
        elasticBand(elasticBand),
        damping(damping)
    {
    }

    void updateForce(Particle& particle, double dt) override
    {
        if (p1 == nullptr || p2 == nullptr)
            return;

        if (&particle != p1 && &particle != p2)
            return;

        Vector3D pos1 = p1->getPosition();
        Vector3D pos2 = p2->getPosition();

        Vector3D vel1 = p1->getVelocity();
        Vector3D vel2 = p2->getVelocity();

        double dx = pos2.x - pos1.x;
        double dy = pos2.y - pos1.y;
        double dz = pos2.z - pos1.z;

        double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
        if (distance < 0.0001)
            return;

        double nx = dx / distance;
        double ny = dy / distance;
        double nz = dz / distance;

        double extension = distance - restLength;
        if (elasticBand && extension <= 0.0)
            return;

        double relativeVelocity =
            (vel2.x - vel1.x) * nx +
            (vel2.y - vel1.y) * ny +
            (vel2.z - vel1.z) * nz;

        double forceMagnitude =
            k * extension + damping * relativeVelocity;

        double fx = forceMagnitude * nx;
        double fy = forceMagnitude * ny;
        double fz = forceMagnitude * nz;

        if (&particle == p1)
        {
            particle.addForce(Vector3D(fx, fy, fz));
        }
        else
        {
            particle.addForce(Vector3D(-fx, -fy, -fz));
        }
    }

private:
    Particle* p1;
    Particle* p2;

    double restLength;
    double k;
    bool elasticBand;
    double damping;
};


// --------------------------------------------------
// EMPUJE DE ARQUIMEDES PARA UN CUBO
// --------------------------------------------------
class BuoyancyGenerator : public ForceGenerator
{
public:
    BuoyancyGenerator(double waterLevel, double cubeHeight, double volume, double density = 1000.0)
        : waterLevel(waterLevel), cubeHeight(cubeHeight), volume(volume), density(density) {}
    void setVolume(double v) { volume = v > 0.0 ? v : 0.0; }
    void setDensity(double d) { density = d > 0.0 ? d : 0.0; }
    void updateForce(Particle& p, double) override
    {
        const double bottom = p.getPosition().y - cubeHeight * 0.5;
        const double immersed = (std::max)(0.0, (std::min)(1.0,
            (waterLevel - bottom) / cubeHeight));
        p.addForce(Vector3D(0.0, density * volume * immersed * 9.8, 0.0));
    }
private:
    double waterLevel, cubeHeight, volume, density;
};
