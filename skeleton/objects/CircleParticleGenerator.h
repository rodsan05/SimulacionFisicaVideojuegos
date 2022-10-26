#pragma once
#include "GaussianParticleGenerator.h"

class CircleParticleGenerator : public ParticleGenerator
{
public:
	CircleParticleGenerator(int numFireworks, Vector3 pos, float radius, Vector3 vel);
	~CircleParticleGenerator();

	std::list<Particle*> generateParticles() override;

protected:

	float _radius;
};