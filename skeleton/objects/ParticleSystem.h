#pragma once
#include "ParticleGenerators/ParticleGenerator.h"
#include "ForceRegistry.hpp"
#include "Particles/Firework.h"
#include "IncludeFiles/ForceGeneratorsIncludes.h"
#include "Constraints/ParticleLink.h"

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
	void generateSlinky();
	void generateFloatingDemo();
	void generateRopeDemo();

	void clearAllGenerators();
	void clearForces();

	void killAllParticles();

	void incrementAllSprings(double increment);
	void decrementAllSprings(double decrement);

protected:
	std::list<Particle*> _particles;
	std::list<Particle*> _firework_pool;

	std::list<ParticleGenerator*> _particle_generators;
	ParticleGenerator* _firework_gen;

	std::list<ForceGenerator*> _force_generators;
	std::list<SpringForceGenerator*> _spring_generators;
	std::list<ParticleLink*> _particle_links;
	ForceRegistry* _force_registry;
	GravityForceGenerator* _gravity_gen;
	GravityForceGenerator* _reverse_gravity_gen;
	WindForceGenerator* _wind_gen;
	WhirlwindForceGenerator* _whirlwind_gen;
	ExplosionForceGenerator* _explosion_gen;
};

