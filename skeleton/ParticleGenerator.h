#pragma once

#include "Particle.h"
#include <memory>
#include <random>

class ParticleSystem;

class ParticleGenerator
{
public:
    enum class Distribution { Uniform, Normal, Gaussian };

    ParticleGenerator(const Particle& model, double interval,
        double lifetime, double positionSpread, Distribution distribution);

    void setActive(bool value) { active = value; }
    bool isActive() const { return active; }
    void update(ParticleSystem& system, double dt);

private:
    std::unique_ptr<Particle> particleModel;
    double interval;
    double lifetime;
    double positionSpread;
    double elapsed;
    bool active;
    Distribution distribution;
    std::mt19937 randomEngine;

    double sampleOffset();
};
