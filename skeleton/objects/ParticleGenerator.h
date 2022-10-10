#pragma once
#include <string>
#include <list>
#include "Particle.h"

class ParticleGenerator
{
public:
	ParticleGenerator() {};
	~ParticleGenerator() {};

	virtual std::list<Particle*> generateParticles() = 0;

	void setParticle(Particle* model) {};

protected:
	std::string _name;

	int _num_particles;
	double _generation_probability;

	Particle* _model;
	Vector3 _mean_pos;
	Vector3 _mean_vel;
};

