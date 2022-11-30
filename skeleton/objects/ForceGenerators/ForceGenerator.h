#pragma once

#include "../Particles/Particle.h"
#include <list>
#include <random>

class ForceGenerator
{
public:
	virtual void updateForce(Particle* particle, double duration) = 0;
	std::string _name;
	double t = -1;
};

