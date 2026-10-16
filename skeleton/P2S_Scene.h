#pragma once

#include "Scene.h"

class Particle;

class P2S_Scene : public Scene
{
public:
    explicit P2S_Scene(std::string name)
        : Scene(std::move(name))
    {
    }

    void init() override;

private:

    std::vector<Particle*> m_particles;
    double m_realGravity = 10;


};

