#pragma once

#include "Scene.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "ForceGenerator.h"
#include "ParticleGenerator.h"
#include "Projectile.h"

#include <memory>
#include <string>

class P3S_Scene : public Scene
{
public:
    explicit P3S_Scene(std::string name) : Scene(std::move(name)) {}
    void init() override;
    void cleanup() override;
    void update(double dt) override;
    void keyPress(unsigned char key,
        const physx::PxTransform& cameraTransform) override;

private:
    ParticleSystem particleSystem{ 1000, 0.0f };

    std::unique_ptr<GravityGenerator> gravity;
    std::unique_ptr<WindGenerator> wind;
    std::unique_ptr<VortexGenerator> vortex;
    std::unique_ptr<ExplosionGenerator> explosion;

    std::unique_ptr<Particle> normalModel;
    std::unique_ptr<Particle> gaussianModel;
    std::unique_ptr<ParticleGenerator> uniformGenerator;
    std::unique_ptr<ParticleGenerator> normalGenerator;

    Projectile lightProjectile{ Projectile::Type::Light };
    Projectile heavyProjectile{ Projectile::Type::Heavy };
    bool heavySelected = false;
    double speedScale = 1.0;
};
