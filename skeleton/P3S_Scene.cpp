#include "P3S_Scene.h"

#include <iostream>

void P3S_Scene::init()
{
    cleanup();
    particleSystem.clear();
    particleSystem.clearForceGenerators();
    particleSystem.clearParticleGenerators();

    // Generadores de fuerza compartibles entre distintos sistemas.
    gravity.reset(new GravityGenerator(Vector3D(0.0, -9.8, 0.0)));
    wind.reset(new WindGenerator(Vector3D(0.0, 2.0, 0.0),
        12.0, Vector3D(5.0, 0.0, 0.0), 2.0));
    vortex.reset(new VortexGenerator(Vector3D(0.0, 2.0, 0.0),
        12.0, 1.2, 1.5));
    explosion.reset(new ExplosionGenerator(Vector3D(0.0, 2.0, 0.0),
        12.0, 120.0, 0.8));

    particleSystem.addForceGenerator(gravity.get(), true);
    particleSystem.addForceGenerator(wind.get(), false);
    particleSystem.addForceGenerator(vortex.get(), false);
    particleSystem.addForceGenerator(explosion.get(), false);

    // Los generadores almacenan una partícula modelo y emiten nuevas
    // partículas con sus propiedades físicas y posiciones aleatorias.
    normalModel.reset(new Particle(Vector3D(-3.0, 1.0, 0.0),
        Vector3D(0.0, 5.0, 0.0), Vector3D(0.0, 0.0, 0.0), 0.999, 1.0));
    gaussianModel.reset(new Particle(Vector3D(3.0, 1.0, 0.0),
        Vector3D(0.0, 7.0, 0.0), Vector3D(0.0, 0.0, 0.0), 0.999, 8.0));

    uniformGenerator.reset(new ParticleGenerator(*normalModel, 0.15, 4.0,
        1.5, ParticleGenerator::Distribution::Uniform));
    normalGenerator.reset(new ParticleGenerator(*gaussianModel, 0.25, 4.0,
        0.8, ParticleGenerator::Distribution::Gaussian));

    particleSystem.addParticleGenerator(uniformGenerator.get());
    particleSystem.addParticleGenerator(normalGenerator.get());
}

void P3S_Scene::update(double dt)
{
    if (dt <= 0.0)
        return;

    particleSystem.update(static_cast<float>(dt));
    if (explosion)
        explosion->advance(dt);
}

void P3S_Scene::keyPress(unsigned char key,
    const physx::PxTransform& cameraTransform)
{
    const physx::PxVec3 forward = cameraTransform.q.rotate(
        physx::PxVec3(0.0f, 0.0f, -1.0f));
    const Vector3D direction(forward.x, forward.y, forward.z);
    const physx::PxVec3 cameraPosition = cameraTransform.p;
    const Vector3D origin(cameraPosition.x + forward.x * 1.0,
        cameraPosition.y + forward.y * 1.0,
        cameraPosition.z + forward.z * 1.0);

    switch (key)
    {
    case ' ':
        lightProjectile.scaleSpeed(speedScale);
        lightProjectile.fire(particleSystem, origin, direction);
        break;
    case 'p':
    case 'P':
        heavyProjectile.scaleSpeed(speedScale);
        heavyProjectile.fire(particleSystem, origin, direction);
        break;
    case '+':
    case '=':
        speedScale = speedScale < 4.9 ? speedScale + 0.25 : 5.0;
        break;
    case '-':
    case '_':
        speedScale = speedScale > 0.35 ? speedScale - 0.25 : 0.1;
        break;
    case 'g':
    case 'G':
        particleSystem.setForceGeneratorEnabled(0,
            !particleSystem.isForceGeneratorEnabled(0));
        break;
    case 'w':
    case 'W':
        particleSystem.setForceGeneratorEnabled(1,
            !particleSystem.isForceGeneratorEnabled(1));
        break;
    case 't':
    case 'T':
        particleSystem.setForceGeneratorEnabled(2,
            !particleSystem.isForceGeneratorEnabled(2));
        break;
    case 'x':
    case 'X':
        particleSystem.setForceGeneratorEnabled(3,
            !particleSystem.isForceGeneratorEnabled(3));
        break;
    case 'e':
    case 'E':
        explosion->trigger();
        particleSystem.setForceGeneratorEnabled(3, true);
        break;
    case 'u':
    case 'U':
        particleSystem.setParticleGeneratorEnabled(0,
            !particleSystem.isParticleGeneratorEnabled(0));
        break;
    case 'n':
    case 'N':
        particleSystem.setParticleGeneratorEnabled(1,
            !particleSystem.isParticleGeneratorEnabled(1));
        break;
    default:
        break;
    }
}

void P3S_Scene::cleanup()
{
    particleSystem.clearForceGenerators();
    particleSystem.clearParticleGenerators();
    uniformGenerator.reset();
    normalGenerator.reset();
    normalModel.reset();
    gaussianModel.reset();
    gravity.reset();
    wind.reset();
    vortex.reset();
    explosion.reset();
}
