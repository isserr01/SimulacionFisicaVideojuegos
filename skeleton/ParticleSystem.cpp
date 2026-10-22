#include "ParticleSystem.h"
#include "Particle.h"

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
    float lifetime, int emitter)
{
    if (activeCount >= particles.size())
        return false;

    Particle* particle = particles[activeCount];

    particle->reset(
        Vector3D(x, y, z),
        Vector3D(velx, vely, velz),
        Vector3D(0.0, gravity, 0.0),
        1.0);

    std::cout << "CREATE: velY=" << vely
        << " accY=" << gravity
        << " particleY=" << particle->getPosition().y
        << '\n';

    edad[activeCount] = 0.0f;
    vida[activeCount] = std::max(MIN_LIFETIME, lifetime);
    emisor[activeCount] = emitter;

    ++activeCount;
    return true;
}

void ParticleSystem::update(float deltaTime)
{
    if (deltaTime <= 0.0f || activeCount == 0)
        return;

    // Evita saltos enormes si la aplicación se pausa durante un frame.
    const float dt = std::min(deltaTime, MAX_DELTA_TIME);

    std::size_t i = 0;
    while (i < activeCount)
    {
        Particle* particle = particles[i];
        edad[i] += dt;
        const Vector3D before = particle->getPosition();

        particle->integrate(dt);

        const Vector3D after = particle->getPosition();

        if (i == 0)
        {
            std::cout << "Antes Y: " << before.y
                << " | Despues Y: " << after.y
                << '\n';
        }


        if (i == 0)
        {
            static int frames = 0;

            if (++frames % 30 == 0)
            {
                std::cout << "Posicion: "
                    << particle->getPosition().x << ", "
                    << particle->getPosition().y << ", "
                    << particle->getPosition().z << '\n';
            }
        }

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
            // No incrementamos i: el elemento intercambiado también debe revisarse.
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
