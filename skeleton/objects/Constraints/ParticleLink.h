#pragma 
#include "../../core.hpp"
#include "../Particles/Particle.h"

#include <utility>

class ParticleLink 
{
public:
	ParticleLink(Particle* p1, Particle* p2, float max_length, float restitution_coef = 0) : _restitution_coef(restitution_coef), _max_length(max_length), particles(p1, p2)
	{
	}
	~ParticleLink();

	void setRestitution(float rest_coef) 
	{
		_restitution_coef = rest_coef;
	}

	virtual void checkLimit() 
	{
		auto p1 = particles.first;
		auto p2 = particles.second;

		if (currentLength() >= _max_length) 
		{
			auto force = -((p1->getVel() - p2->getVel()) * (_restitution_coef + 1)) / ((1 / p1->getMass()) + (1 / p2->getMass()));

			p1->addForce(force);
			p2->addForce(-force);
		}
	}

protected:
	float currentLength() const 
	{
		Vector3 relativePos = particles.first->getPos() - particles.second->getPos();
		
		return relativePos.magnitude();
	}

	std::pair<Particle*, Particle*> particles;
	float _restitution_coef, _max_length;
};

