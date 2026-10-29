
#include "P2S_Scene.h"
#include "Particle.h"
#include "Vector3D.h"

#include <random>
#include <algorithm>
#include <iostream>

namespace
{
    std::default_random_engine generator;

    float normal(float mean, float deviation)
    {
        std::normal_distribution<float> distribution(
            mean, deviation
        );
        
        return distribution(generator);
    }

    float uniform(float minimum, float maximum)
    {
        std::uniform_real_distribution<float> distribution(
            minimum, maximum
        );

        return distribution(generator);
    }
}

void P2S_Scene::init()
{
    m_emissionTimer = 0.0;
}

void P2S_Scene::update(double dt)
{
    if (dt <= 0.0)
        return;

    static int frames = 0;

    // Actualizar las partículas existentes.
    m_particleSystem.update(static_cast<float>(dt));

    // Emitir partículas periódicamente desde los tres emisores.
    m_emissionTimer += dt;

    while (m_emissionTimer >= m_emissionInterval)
    {
        m_emissionTimer -= m_emissionInterval;

        emitParticle(0);
        emitParticle(1);
        emitParticle(2);
    }
}

void P2S_Scene::keyPress(
    unsigned char key,
    const physx::PxTransform& camera)
{
    (void)camera;

    switch (key)
    {
    case ' ':
        emitParticle(0);
        emitParticle(1);
        emitParticle(2);
        break;

    case 'r':
    case 'R':
        // Reiniciar la escena limpiando las partículas activas.
        cleanup();
        break;

    default:
        break;
    }
}

void P2S_Scene::cleanup()
{
    // El pool es propiedad de ParticleSystem.
    // Sus partículas se liberan al destruir el sistema.
    // No se deben borrar individualmente desde la escena.
}

void P2S_Scene::emitParticle(int emitter)
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    float vx = 0.0f;
    float vy = 0.0f;
    float vz = 0.0f;

    float lifetime = 2.0f;

    switch (emitter)
    {
    case 0:
        x = -4.0f;
        vx = normal(0.0f, 0.8f);
        vy = normal(7.0f, 0.8f);
        vz = normal(0.0f, 0.8f);
        lifetime = uniform(1.5f, 2.5f);
        break;

    case 1:
        x = 0.0f;
        vx = normal(0.0f, 0.3f);
        vy = normal(5.0f, 0.5f);
        vz = normal(0.0f, 0.3f);
        lifetime = uniform(1.5f, 2.5f);
        break;

    case 2:
        x = 4.0f;
        vx = normal(0.0f, 1.0f);
        vy = normal(8.0f, 1.0f);
        vz = normal(0.0f, 1.0f);
        lifetime = uniform(1.0f, 1.8f);
        break;

    default:
        return;
    }

    m_particleSystem.create(
        x, y, z,
        vx, vy, vz,
        lifetime,
        emitter
    );
}
