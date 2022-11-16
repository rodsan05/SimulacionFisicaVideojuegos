#pragma once
#include "ParticleGenerator.h"
#include "ForceRegistry.hpp"
#include "Firework.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"

#include <string>

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

	void generateGravity();
	void generateWind();
	void generateWhirlwind();

	void clearAllGenerators();
	void clearForces();

protected:
	std::list<Particle*> _particles;
	std::list<Particle*> _firework_pool;

	std::list<ParticleGenerator*> _particle_generators;
	ParticleGenerator* _firework_gen;

	std::list<ForceGenerator*> _force_generators;
	ForceRegistry* _force_registry;
	GravityForceGenerator* _gravity_gen;
	WindForceGenerator* _wind_gen;
	WhirlwindForceGenerator* _whirlwind_gen;
};

