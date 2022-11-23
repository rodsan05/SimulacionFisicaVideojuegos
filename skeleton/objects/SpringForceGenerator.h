#pragma once
#include "ForceGenerator.h"
#include "../core.hpp"

class SpringForceGenerator : public ForceGenerator {
public:
	SpringForceGenerator(double k, double resting_length, Particle* other) : _resting_length(resting_length), _k(k), _other(other) {}

	virtual ~SpringForceGenerator() {}
	
	virtual void updateForce(Particle* particle, double duration) override
	{
		// Particle is the particle to apply the force 
		Vector3 force = _other->getPos() - particle->getPos();
		
		const float length = force.normalize(); 
		const float delta_x = length - _resting_length;
		
		force *= delta_x * _k;
		
		particle->addForce(force);
	}
	
	inline void setK(double k) { _k = k; }

protected:
	double _k; // Elastic Coeff.
	double _resting_length;
	Particle* _other;
};