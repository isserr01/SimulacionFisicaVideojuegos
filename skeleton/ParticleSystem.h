#pragma once

#include <vector>
#include <random>
#include <cmath>

class Particle;

class ParticleSystem {

	//object pool de partículas
	struct Particles {
		std::vector<float> posX, posY, posZ;
		std::vector<float> velX, velY, velZ;
		std::vector<float> colorR, colorG, colorB;
		std::vector<float> vida;
	};

	std::default_random_engine generator;
	std::normal_distribution<float> lifetime_dist{ 3.0f, 0.5f };
	float particle_lifetime = lifetime_dist(generator);

	ParticleSystem();
	~ParticleSystem();

	void create(float x, float y, float z, float velx, float vely, float velz, float lifetime);
	void update(float deltatime);

	
};

