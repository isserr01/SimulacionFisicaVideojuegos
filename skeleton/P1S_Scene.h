
#pragma once

#include "Scene.h"


class Particle;

class P1S_Scene : public Scene
{
public:
    explicit P1S_Scene(std::string name)
        : Scene(std::move(name))
    {
    }

    void init() override;

    void update(double dt) override;

    void keyPress(unsigned char key, const physx::PxTransform& camera) override;

    void cleanup() override;

private:
    std::vector<Particle*> m_projectiles;

    double m_realMass = 0.008;
    double m_realSpeed = 1000.0;

    double m_simSpeed = 40.0;
    double m_realGravity = 9.81;

    double m_simMass = 0.0;
    double m_simGravity = 0.0;

    void calculateParameters();

    void shoot(const physx::PxTransform& camera);

    void clearProjectiles();
};
