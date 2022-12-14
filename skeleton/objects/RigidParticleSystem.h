#pragma once

#include "ParticleGenerators/RigidParticleGenerator.h"

enum ParticleGenType
{
	Hormigas, Cubo, Sangre, Humo, RandomMass
};

class RigidParticleSystem
{
	RigidParticleSystem();
	~RigidParticleSystem();

	void createParticleGenerator(ParticleGenType type);

	void update(double t);
	void generate();
	void generatePerpetual();

	void clearAllGenerators();
	void clearForces();

	void killAllParticles();

protected:
	std::list<RigidParticle*> _particles;
};

