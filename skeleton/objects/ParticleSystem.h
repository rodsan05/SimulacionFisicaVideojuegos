#pragma once
#include "ParticleGenerator.h"
#include "Firework.h"
#include <string>

enum ParticleGenType 
{
	GAUSSIAN, UNIFORM
};

class ParticleSystem
{
public:
	ParticleSystem(ParticleGenType particleGen);
	~ParticleSystem();

	void update(double t);

	ParticleGenerator* getParticleGenerator(std::string name);

	void generateFireworksSystem(FireworkType ft);

	void appendParticles(std::list<Particle*> particles);
	void appendFireworks(std::list<Particle*> particles);

protected:
	std::list<Particle*> _particles;
	std::list<Particle*> _firework_pool;

	std::list<ParticleGenerator*> _particle_generators;
	ParticleGenerator* _firework_gen;
};

