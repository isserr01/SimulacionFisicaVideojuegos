#pragma once

#include "Scene.h"
#include "Particle.h"
#include "ForceGenerator.h"
#include "RenderUtils.hpp"
#include <memory>

class P4S_Scene : public Scene
{
public:
    explicit P4S_Scene(std::string name) : Scene(std::move(name)) {}

    void init() override;
    void cleanup() override;
    void update(double dt) override;
    void keyPress(unsigned char key, const physx::PxTransform& cameraTransform) override;

private:
    enum class Mode { FixedSpring, PairSpring, ElasticBand, Buoyancy };
    Mode mode = Mode::FixedSpring;

    std::unique_ptr<Particle> p1, p2;

    std::unique_ptr<SpringToAnchorGenerator> fixedSpring;
    std::unique_ptr<PairSpringGenerator> pairSpring;
    std::unique_ptr<BuoyancyGenerator> buoyancy;

    physx::PxTransform anchorTransform, waterTransform;

    RenderItem* anchorItem = nullptr;
    RenderItem* waterItem = nullptr;

    Vector3D anchor{0.0, 12.0, 0.0};

    double k = 35.0;
    double mass = 2.0;
    double volume = 0.004;
    double waterLevel = 3.0;
    double impulseTime = 0.0;
    bool explicitEuler = false;
    float groundLevel = 0.5f;
    float particleRadius = 0.5f;

    void resetParticles();
    void rebuildSpring();
};
