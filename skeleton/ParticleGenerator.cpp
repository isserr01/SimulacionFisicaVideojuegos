#include "ParticleGenerator.h"
#include "ParticleSystem.h"

#include <algorithm>
#include <cmath>
#include <chrono>

ParticleGenerator::ParticleGenerator(const Particle& model, double intervalValue,
    double lifetimeValue, double spread, Distribution distributionValue)
    : particleModel(new Particle(model.getPosition(), model.getVelocity(),
        model.getAcceleration(), model.getDamping(), model.getMass())),
      interval(std::max(0.001, intervalValue)),
      lifetime(std::max(0.01, lifetimeValue)),
      positionSpread(std::max(0.0, spread)),
      elapsed(0.0),
      active(true),
      distribution(distributionValue),
      randomEngine(static_cast<unsigned int>(
          std::chrono::high_resolution_clock::now().time_since_epoch().count()))
{
}

double ParticleGenerator::sampleOffset()
{
    if (distribution == Distribution::Normal)
    {
        std::normal_distribution<double> d(0.0, positionSpread);
        return d(randomEngine);
    }

    if (distribution == Distribution::Gaussian)
    {
        std::uniform_real_distribution<double> u(1e-12, 1.0);
        const double u1 = u(randomEngine);
        const double u2 = u(randomEngine);
        const double z = std::sqrt(-2.0 * std::log(u1)) *
            std::cos(6.283185307179586 * u2);
        return z * positionSpread;
    }

    std::uniform_real_distribution<double> d(-positionSpread, positionSpread);
    return d(randomEngine);
}

void ParticleGenerator::update(ParticleSystem& system, double dt)
{
    if (!active || dt <= 0.0)
        return;

    elapsed += dt;
    int safetyCounter = 0;
    while (elapsed >= interval && safetyCounter++ < 20)
    {
        elapsed -= interval;
        const Vector3D p = particleModel->getPosition();
        const Vector3D v = particleModel->getVelocity();

        system.create(
            static_cast<float>(p.x + sampleOffset()),
            static_cast<float>(p.y + sampleOffset()),
            static_cast<float>(p.z + sampleOffset()),
            static_cast<float>(v.x),
            static_cast<float>(v.y),
            static_cast<float>(v.z),
            static_cast<float>(lifetime),
            10,
            particleModel->getMass());
    }
}
