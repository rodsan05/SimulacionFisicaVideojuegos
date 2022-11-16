#pragma once

#include <map>
#include "ForceGenerator.h"

using FRPair = std::pair<Particle*, ForceGenerator*>;

class ForceRegistry : public std::multimap<Particle*, ForceGenerator*>
{
public:
	void updateForces(double duration)
	{
		for (auto it = begin(); it != end(); it++)
		{
			it->second->updateForce(it->first, duration);
		}
	}

	void addRegistry(ForceGenerator* fg, Particle* p) 
	{
		this->insert(FRPair(p, fg));
	}

	void deleteParticleRegistry(Particle* p) 
	{
		this->erase(p);
	}
};