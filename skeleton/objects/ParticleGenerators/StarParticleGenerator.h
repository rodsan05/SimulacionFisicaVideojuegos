#pragma once
#include "ParticleGenerator.h"

class StarParticleGenerator : public ParticleGenerator
{
public:
	StarParticleGenerator(int numParticles, Vector3 pos, float radius, Vector3 vel, float numPuntas, float pronunciamientoPuntas, float suavizadoPuntas);
	~StarParticleGenerator();

	std::list<Particle*> generateParticles() override;

protected:

	float _radius;
	float nPuntas, pronunciamiento, suavizado;
};