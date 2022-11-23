#pragma once
#include "ParticleGenerator.h"
#include "ForceRegistry.hpp"
#include "Firework.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "SpringForceGenerator.h"
#include "AnchoredSpringFG.h"

#include <string>

enum ParticleGenType
{
	Hormigas, Cubo, Sangre, Humo, RandomMass
};

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void createParticleGenerator(ParticleGenType type);

	void update(double t);
	void generate();
	void generatePerpetual();

	ParticleGenerator* getParticleGenerator(std::string name);

	void generateFireworksSystem(FireworkType ft);

	void appendParticles(std::list<Particle*> particles);
	void appendFireworks(std::list<Particle*> particles);

	void generateGravity();
	void generateReverseGravity();
	void generateWind();
	void generateWhirlwind();
	void generateExplosion();

	void generateSpringDemo();

	void clearAllGenerators();
	void clearForces();

	void killAllParticles();

protected:
	std::list<Particle*> _particles;
	std::list<Particle*> _firework_pool;

	std::list<ParticleGenerator*> _particle_generators;
	ParticleGenerator* _firework_gen;

	std::list<ForceGenerator*> _force_generators;
	ForceRegistry* _force_registry;
	GravityForceGenerator* _gravity_gen;
	GravityForceGenerator* _reverse_gravity_gen;
	WindForceGenerator* _wind_gen;
	WhirlwindForceGenerator* _whirlwind_gen;
	ExplosionForceGenerator* _explosion_gen;
};

