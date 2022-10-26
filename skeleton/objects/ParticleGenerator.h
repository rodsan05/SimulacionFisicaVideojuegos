#pragma once
#include <string>
#include <list>
#include <random>
#include "Particle.h"

enum FireworkType 
{
	Circle, Sphere, Star
};

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

	double PI = std::_Pi;

	std::string _name;

	int _num_particles;
	double _generation_probability;

	Particle* _model = nullptr;
	Vector3 _mean_pos;
	Vector3 _mean_vel;
};

