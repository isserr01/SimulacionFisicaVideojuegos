#pragma once

#include <vector>
#include <cstddef>

class Particle;

class ParticleSystem
{
public:
    explicit ParticleSystem(std::size_t poolCapacity = 1000,
        float gravity = -10.0f);
    ~ParticleSystem();

    ParticleSystem(const ParticleSystem&) = delete;
    ParticleSystem& operator=(const ParticleSystem&) = delete;

    // Devuelve false si el pool está lleno.
    bool create(float x, float y, float z,
        float velx, float vely, float velz,
        float lifetime, int emitter = 0);

    void update(float deltaTime);

    std::size_t getActiveCount() const { return activeCount; }
    std::size_t getCapacity() const { return particles.size(); }
    void clear();

private:
    std::vector<Particle*> particles;
    std::vector<float> vida;
    std::vector<float> edad;
    std::vector<int> emisor;

    std::size_t activeCount;
    float gravity;

    void hideParticle(Particle* particle);
};
