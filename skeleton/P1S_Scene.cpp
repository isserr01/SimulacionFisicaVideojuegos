#include "P1S_Scene.h"
#include "Particle.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"

#include <vector>
#include <algorithm>
#include <cmath>

void P1S_Scene::init()
{
    // Valores de referencia del proyectil real
    m_realMass = 0.008;       // kg: 8 gramos
    m_realSpeed = 800.0;      // m/s

    // Velocidad reducida para visualizar el disparo
    m_simSpeed = 40.0;        // u/s

    // Gravedad real y parámetros calculados
    m_realGravity = 9.81;
    calculateParameters();
}

void P1S_Scene::update(double dt)
{

    for (Particle* p : m_projectiles)
    {
        if (p != nullptr)
            p->integrate(dt);
    }
}

void P1S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera)
{
    switch (key)
    {
    case ' ':
        shoot(camera);
        break;

    case '+':
    case '=':
        m_realMass += 0.001;  // +1 gramo
        calculateParameters();
        break;

    case '-':
    case '_':
        m_realMass -= 0.001;

        if (m_realMass < 0.001)
        {
            m_realMass = 0.001;
        }
        calculateParameters();
        break;

    case 'r':
    case 'R':
        clearProjectiles();
        break;

    default:
        break;
    }
}

void P1S_Scene::cleanup()
{
    clearProjectiles();
}

void P1S_Scene::calculateParameters()
{
    // Energía cinética real = energía cinética simulada
    m_simMass = m_realMass
        * (m_realSpeed * m_realSpeed)
        / (m_simSpeed * m_simSpeed);

    // Conservación aproximada de la forma de la trayectoria
    double scale = m_simSpeed / m_realSpeed;
    m_simGravity = m_realGravity * scale * scale;
}

void P1S_Scene::shoot(const physx::PxTransform& camera)
{
    // Posición inicial: posición de la cámara
    physx::PxVec3 position = camera.p;

    // Dirección frontal de la cámara.
    physx::PxVec3 direction =
        camera.q.rotate(physx::PxVec3(0.0f, 0.0f, -1.0f));

    direction.normalize();

    Vector3D pos(
        position.x,
        position.y,
        position.z
    );

    Vector3D velocity(
        direction.x * m_simSpeed,
        direction.y * m_simSpeed,
        direction.z * m_simSpeed
    );

    // Gravedad en el eje Y negativo.
    Vector3D acceleration(
        0.0f,
        -m_simGravity,
        0.0f
    );

    // Cada disparo crea una partícula independiente.
    Particle* projectile = new Particle(
        pos,
        velocity,
        acceleration,
        1.0
    );

    projectile->setMass(m_simMass);
    m_projectiles.push_back(projectile);
}

void P1S_Scene::clearProjectiles()
{
    for (Particle* p : m_projectiles)
        delete p;

    m_projectiles.clear();
}
