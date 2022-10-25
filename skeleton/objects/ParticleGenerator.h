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

	void setParticle(Particle* model) 
	{
		_model = model;
	};

	void setPos(Vector3 pos)
	{
		_mean_pos = pos;
	}

protected:
	std::string _name;

	int _num_particles;
	double _generation_probability;

	Particle* _model = nullptr;
	Vector3 _mean_pos;
	Vector3 _mean_vel;
};

