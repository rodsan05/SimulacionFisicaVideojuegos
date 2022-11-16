#pragma once
#include "ParticleGenerator.h"
#include "ForceRegistry.hpp"
#include "Firework.h"
#include "GravityForceGenerator.h"
#include <string>
#include "WindForceGenerator.h"

enum ParticleGenType
{
	Hormigas, Cubo, Sangre, Humo
};

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void createParticleGenerator(ParticleGenType type);

	void update(double t);
	void generate();

	ParticleGenerator* getParticleGenerator(std::string name);

	void generateFireworksSystem(FireworkType ft);

	void appendParticles(std::list<Particle*> particles);
	void appendFireworks(std::list<Particle*> particles);

	void clearAllGenerators();

protected:
	std::list<Particle*> _particles;
	std::list<Particle*> _firework_pool;

	std::list<ParticleGenerator*> _particle_generators;
	ParticleGenerator* _firework_gen;

	ForceRegistry* _force_registry;
	GravityForceGenerator* _gravity_gen;
	WindForceGenerator* _wind_gen;
};

