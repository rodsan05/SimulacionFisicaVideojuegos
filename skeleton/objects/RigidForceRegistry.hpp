#pragma once

#include <map>
#include "ForceGenerators/ForceGenerator.h"

using FRPair = std::pair<RigidParticle*, ForceGenerator*>;

class ForceRegistry : public std::multimap<RigidParticle*, ForceGenerator*>
{
public:
	void updateForces(double duration)
	{
		for (auto it = begin(); it != end();)
		{
			if (it->second != nullptr)
			{
				it->second->updateForce(it->first, duration);
				it++;
			}

			else it = erase(it);
		}
	}

	void addRegistry(ForceGenerator* fg, Particle* p) 
	{
		this->insert(FRPair(p, fg));
	}

	void deleteGeneratorRegistry(ForceGenerator* fg) 
	{
		for (auto it = begin(); it != end();)
		{
			if (it->second == fg)
			{
				it = erase(it);
			}

			else it++;
		}
	}

	//erase optimizado
	void deleteParticleRegistry(Particle* p) 
	{
		this->erase(p);
	}
};