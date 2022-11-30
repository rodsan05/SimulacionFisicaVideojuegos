#pragma once
#include <string>
#include <list>
#include <random>
#include "../Particles/Particle.h"

enum FireworkType 
{
	Circle, SphereFirework, Star
};

class ParticleGenerator
{
public:
	ParticleGenerator() {};
	ParticleGenerator(bool perpetual) : _perpetual(perpetual) {};
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

	void setPerpetual(bool set) 
	{
		_perpetual = set;
	}

	bool isPerpetual() 
	{
		return _perpetual;
	}

protected:

	double PI = std::_Pi;

	std::string _name;

	int _num_particles;
	double _generation_probability;

	Particle* _model = nullptr;
	Vector3 _mean_pos;
	Vector3 _mean_vel;

	bool _perpetual = false;
};

