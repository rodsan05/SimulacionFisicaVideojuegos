#pragma once
#include "ParticleGenerator.h"

class SphereParticleGenerator : public ParticleGenerator
{
public:
	SphereParticleGenerator(int numParticles, Vector3 pos, float radius, Vector3 vel);
	~SphereParticleGenerator();

	std::list<Particle*> generateParticles() override;

protected:

	float _radius;
};