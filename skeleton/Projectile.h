#pragma once

#include "Vector3D.h"

class ParticleSystem;

class Projectile
{
public:
    enum class Type { Light, Heavy };

    explicit Projectile(Type type = Type::Light);

    // Multiplica la velocidad base por el factor solicitado.
    void scaleSpeed(double factor);
    void scaleEnergy(double factor);
    double getSpeedScale() const { return speedScale; }
    double getMass() const { return mass; }
    double getBaseSpeed() const { return baseSpeed; }
    double getKineticEnergy() const;

    // Dispara desde la posición indicada en la dirección indicada.
    bool fire(ParticleSystem& system, Vector3D position,
        Vector3D direction, float lifetime = 8.0f) const;

    // Escala una fuerza antes de aplicarla al modelo de proyectil.
    Vector3D scaleForce(Vector3D force) const;

private:
    Type type;
    double mass;
    double baseSpeed;
    double speedScale;
    double forceScale;
};
