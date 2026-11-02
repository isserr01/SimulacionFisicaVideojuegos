#include "ParticleSystem.h"
#include "Particle.h"
#include "ForceGenerator.h"
#include "ParticleGenerator.h"

#include <algorithm>
#include <iostream>
#include <cmath>

namespace
{
    constexpr float HIDDEN_COORD = 10000.0f;
    constexpr float MIN_LIFETIME = 0.01f;
    constexpr float MAX_DELTA_TIME = 0.05f;
}

ParticleSystem::ParticleSystem(std::size_t poolCapacity, float gravityValue)
    : activeCount(0),
    gravity(gravityValue)
{
    particles.reserve(poolCapacity);
    vida.resize(poolCapacity, 0.0f);
    edad.resize(poolCapacity, 0.0f);
    emisor.resize(poolCapacity, -1);

    for (std::size_t i = 0; i < poolCapacity; ++i)
    {
        particles.push_back(new Particle(
            Vector3D(HIDDEN_COORD, HIDDEN_COORD, HIDDEN_COORD),
            Vector3D(0.0, 0.0, 0.0),
            Vector3D(0.0, 0.0, 0.0),
            1.0));
    }
}

ParticleSystem::~ParticleSystem()
{
    for (Particle* particle : particles)
        delete particle;

    particles.clear();
}

bool ParticleSystem::create(float x, float y, float z,
    float velx, float vely, float velz,
    float lifetime, int emitter, double mass)
{
    if (activeCount >= particles.size())
        return false;

    Particle* particle = particles[activeCount];

    particle->reset(
        Vector3D(x, y, z),
        Vector3D(velx, vely, velz),
        Vector3D(0.0, gravity, 0.0),
        1.0);
    particle->setMass(mass > 0.0 ? mass : 1.0);

    if (emitter == 20)
    {
        particle->setColor(0.0f, 0.2f, 1.0f, 1.0f);
    }
    else if (emitter == 21)
    {
        particle->setColor(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        particle->setRandomColor();
    }

    edad[activeCount] = 0.0f;
    vida[activeCount] = std::max(MIN_LIFETIME, lifetime);
    emisor[activeCount] = emitter;

    ++activeCount;
    return true;
}

void ParticleSystem::update(float deltaTime)
{
    if (deltaTime <= 0.0f)
        return;

    // Evita saltos enormes si la aplicación se pausa durante un frame.
    const float dt = std::min(deltaTime, MAX_DELTA_TIME);

    // El sistema es responsable de actualizar sus emisores.
    for (ParticleGenerator* generator : particleGenerators)
        if (generator != nullptr)
            generator->update(*this, dt);

    // Reconstruimos el registro con las partículas activas del frame.
    // Es necesario porque el pool reutiliza partículas y las intercambia
    // al retirar las que han expirado. Así evitamos registros obsoletos.
    forceRegistry.clear();
    for (std::size_t particleIndex = 0; particleIndex < activeCount; ++particleIndex)
    {
        for (const auto& entry : forceGenerators)
        {
            if (entry.enabled && entry.generator != nullptr)
                forceRegistry.add(particles[particleIndex], entry.generator);
        }
    }
    forceRegistry.updateForces(dt);

    std::size_t i = 0;
    while (i < activeCount)
    {
        Particle* particle = particles[i];
        edad[i] += dt;

        particle->integrate(dt);


        const Vector3D position = particle->getPosition();
        const bool expired = edad[i] >= vida[i];
        const bool outOfBounds =
            !std::isfinite(position.x) ||
            !std::isfinite(position.y) ||
            !std::isfinite(position.z) ||
            std::abs(position.x) > 5000.0 ||
            std::abs(position.y) > 5000.0 ||
            std::abs(position.z) > 5000.0;

        if (expired || outOfBounds)
        {
            hideParticle(particle);

            const std::size_t last = activeCount - 1;
            if (i != last)
            {
                std::swap(particles[i], particles[last]);
                std::swap(vida[i], vida[last]);
                std::swap(edad[i], edad[last]);
                std::swap(emisor[i], emisor[last]);
            }

            --activeCount;
        }
        else
        {
            ++i;
        }
    }
}

void ParticleSystem::hideParticle(Particle* particle)
{
    if (particle != nullptr)
    {
        particle->reset(
            Vector3D(HIDDEN_COORD, HIDDEN_COORD, HIDDEN_COORD),
            Vector3D(0.0, 0.0, 0.0),
            Vector3D(0.0, 0.0, 0.0),
            1.0);
    }
}

void ParticleSystem::addForceGenerator(ForceGenerator* generator, bool enabled)
{
    if (generator != nullptr)
        forceGenerators.push_back({ generator, enabled });
}

void ParticleSystem::addParticleGenerator(ParticleGenerator* generator)
{
    if (generator != nullptr)
        particleGenerators.push_back(generator);
}

void ParticleSystem::clearParticleGenerators()
{
    particleGenerators.clear();
}

void ParticleSystem::setParticleGeneratorEnabled(std::size_t index, bool enabled)
{
    if (index < particleGenerators.size() && particleGenerators[index] != nullptr)
        particleGenerators[index]->setActive(enabled);
}

bool ParticleSystem::isParticleGeneratorEnabled(std::size_t index) const
{
    return index < particleGenerators.size() && particleGenerators[index] != nullptr
        && particleGenerators[index]->isActive();
}

void ParticleSystem::clearForceGenerators()
{
    forceRegistry.clear();
    forceGenerators.clear();
}

void ParticleSystem::setForceGeneratorEnabled(std::size_t index, bool enabled)
{
    if (index < forceGenerators.size())
        forceGenerators[index].enabled = enabled;
}

bool ParticleSystem::isForceGeneratorEnabled(std::size_t index) const
{
    return index < forceGenerators.size() && forceGenerators[index].enabled;
}

void ParticleSystem::clear()
{
    for (std::size_t i = 0; i < activeCount; ++i)
    {
        hideParticle(particles[i]);
        vida[i] = 0.0f;
        edad[i] = 0.0f;
        emisor[i] = -1;
    }

    activeCount = 0;
}
