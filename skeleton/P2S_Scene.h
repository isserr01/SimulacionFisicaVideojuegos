
#pragma once

#include "Scene.h"
#include "ParticleSystem.h"

class P2S_Scene : public Scene
{
public:
    explicit P2S_Scene(std::string name)
        : Scene(std::move(name))
    {
    }

    void init() override;
    void update(double dt) override;
    void keyPress(
        unsigned char key,
        const physx::PxTransform& camera
    ) override;
    void cleanup() override;

private:
    ParticleSystem m_particleSystem{ 1000, -10.0f };

    double m_realGravity = 10.0;

    double m_emissionTimer = 0.0;
    double m_emissionInterval = 0.05;

    void emitParticle(int emitter);
};
