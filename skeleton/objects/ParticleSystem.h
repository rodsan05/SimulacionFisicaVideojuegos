#pragma once
#include "ParticleGenerator.h"
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

	void generateFireworksSystem();

protected:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _particle_generators;
};

