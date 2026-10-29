#pragma once

#include <vector>
#include <cstddef>

class Particle;
class ForceGenerator;
class ParticleGenerator;

class ParticleSystem
{
public:
    explicit ParticleSystem(std::size_t poolCapacity = 1000,
        float gravity = -10.0f);
    ~ParticleSystem();

    ParticleSystem(const ParticleSystem&) = delete;
    ParticleSystem& operator=(const ParticleSystem&) = delete;

    bool create(float x, float y, float z,
        float velx, float vely, float velz,
        float lifetime, int emitter = 0, double mass = 5.0);

    void update(float deltaTime);
    std::size_t getActiveCount() const { return activeCount; }
    std::size_t getCapacity() const { return particles.size(); }
    void clear();

    // Cada sistema decide qué generadores afectan a sus partículas.
    void addForceGenerator(ForceGenerator* generator, bool enabled = true);
    void addParticleGenerator(ParticleGenerator* generator);
    void clearParticleGenerators();
    void setParticleGeneratorEnabled(std::size_t index, bool enabled);
    bool isParticleGeneratorEnabled(std::size_t index) const;
    void clearForceGenerators();
    void setForceGeneratorEnabled(std::size_t index, bool enabled);
    bool isForceGeneratorEnabled(std::size_t index) const;
    std::size_t getForceGeneratorCount() const { return forceGenerators.size(); }

private:
    struct ForceEntry { ForceGenerator* generator; bool enabled; };
    std::vector<Particle*> particles;
    std::vector<float> vida;
    std::vector<float> edad;
    std::vector<int> emisor;
    std::vector<ForceEntry> forceGenerators;
    std::vector<ParticleGenerator*> particleGenerators;
    std::size_t activeCount;
    float gravity;

    void hideParticle(Particle* particle);
};
