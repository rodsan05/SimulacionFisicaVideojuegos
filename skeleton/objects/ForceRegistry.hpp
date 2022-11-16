#pragma once

#include <map>
#include "ForceGenerator.h"

using FRPair = std::pair<ForceGenerator*, Particle*>;

class ForceRegistry : public std::multimap<ForceGenerator*, Particle*>
{
public:
	void updateForces(double duration)
	{
		for (auto it = begin(); it != end(); it++)
		{
			it->first->updateForce(it->second, duration);
		}
	}

	void addRegistry(ForceGenerator* fg, Particle* p) 
	{
		this->insert(FRPair(fg, p));
	}

	//posiblemente ineficiente
	void deleteParticleRegistry(Particle* p) 
	{
		for (auto it = begin(); it != end();)
		{
			if (it->second == p)
			{
				it = erase(it);
			}

			else it++;
		}
	}
};