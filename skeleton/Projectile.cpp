#include "Projectile.h"
#include "ParticleSystem.h"
#include <cmath>
#include <algorithm>

Projectile::Projectile(Type projectileType)
    : type(projectileType),
      mass(projectileType == Type::Light ? 1.0 : 8.0),
      baseSpeed(projectileType == Type::Light ? 18.0 : 10.0),
      speedScale(1.0),
      forceScale(projectileType == Type::Light ? 1.0 : 2.5)
{
}

void Projectile::scaleSpeed(double factor)
{
    speedScale = std::max(0.1, std::min(factor, 5.0));
}

void Projectile::scaleEnergy(double factor)
{
    // E = 1/2 m v^2: para escalar E por factor, v se escala por sqrt(factor).
    if (factor > 0.0)
        scaleSpeed(speedScale * std::sqrt(factor));
}

double Projectile::getKineticEnergy() const
{
    const double speed = baseSpeed * speedScale;
    return 0.5 * mass * speed * speed;
}

Vector3D Projectile::scaleForce(Vector3D force) const
{
    return Vector3D(force.x * forceScale,
        force.y * forceScale, force.z * forceScale);
}

bool Projectile::fire(ParticleSystem& system, Vector3D position,
    Vector3D direction, float lifetime) const
{
    const double length = std::sqrt(direction.x * direction.x +
        direction.y * direction.y + direction.z * direction.z);
    if (length <= 1e-8)
        return false;

    const double speed = baseSpeed * speedScale;
    const double vx = direction.x / length * speed;
    const double vy = direction.y / length * speed;
    const double vz = direction.z / length * speed;

    return system.create(
        static_cast<float>(position.x),
        static_cast<float>(position.y),
        static_cast<float>(position.z),
        static_cast<float>(vx),
        static_cast<float>(vy),
        static_cast<float>(vz),
        lifetime,
        type == Type::Light ? 20 : 21,
        mass);
}
