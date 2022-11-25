#pragma once
#include "ForceGenerator.h"
#include "../core.hpp"

class SpringForceGenerator : public ForceGenerator {
public:
	SpringForceGenerator(double k, double resting_length, Particle* other, double minLenght = 0.5, double maxLenght = 1000000000, bool goma = false) : 
		_resting_length(resting_length), _k(k), _other(other), _goma(goma), _max_lenght(maxLenght), _min_lenght(minLenght) {}

	virtual ~SpringForceGenerator() {}
	
	virtual void updateForce(Particle* particle, double duration) override
	{
		// Particle is the particle to apply the force 
		Vector3 force = _other->getPos() - particle->getPos();
		
		const float length = force.normalize(); 
		const float delta_x = length - _resting_length;
		
		if (length <= _min_lenght) 
		{
			particle->setVel(-particle->getVel());
		}
		else if (length >= _max_lenght)
		{
			particle->setVel(-particle->getVel());
		}

		else if (!_goma || length > _resting_length)
		{
			force *= delta_x * _k;
		
			particle->addForce(force);
		}
	}
	
	inline void setK(double k) { _k = k; }

	void incrementK(double increment) { _k += increment; }
	void decrementK(double decrement) { _k -= decrement; }

	void setGoma(bool goma) { _goma = goma; }

protected:
	double _k; // Elastic Coeff.
	double _resting_length;
	Particle* _other;

	double _max_lenght, _min_lenght;

	bool _goma;
};