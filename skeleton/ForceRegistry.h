
#pragma once

#include "ForceGenerator.h"
#include <vector>

class ForceRegistry
{
public:
    struct Registration
    {
        Particle* particle;
        ForceGenerator* generator;
    };

    void add(Particle* particle, ForceGenerator* generator)
    {
        registrations.push_back({ particle, generator });
    }

    void clear()
    {
        registrations.clear();
    }

    void updateForces(double dt)
    {
        for (const auto& registration : registrations)
        {
            if (registration.particle != nullptr &&
                registration.generator != nullptr)
            {
                registration.particle->applyForceGenerator(
                    *registration.generator, dt);
            }
        }
    }

private:
    std::vector<Registration> registrations;
};
