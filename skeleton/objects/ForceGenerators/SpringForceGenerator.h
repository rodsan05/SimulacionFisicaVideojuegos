#pragma once
#include "ForceGenerator.h"
#include "../../core.hpp"

class SpringForceGenerator : public ForceGenerator {
public:
	SpringForceGenerator(double k, double resting_length, Particle* other, double minLenght = 0.01, double maxLenght = 100, double ruptureLenght = 150, bool goma = false) :
		_resting_length(resting_length), _k(k), _other(other), _goma(goma), _max_length(maxLenght), _min_length(minLenght), _rupture_length(ruptureLenght) {}

	virtual ~SpringForceGenerator() {}
	
	virtual void updateForce(Particle* particle, double duration) override
	{
		// Particle is the particle to apply the force 
		Vector3 force = _other->getPos() - particle->getPos();
		
		const float length = force.normalize(); 
		const float delta_x = length - _resting_length;
		
		if (length <= _min_length) 
		{
			particle->setVel(-particle->getVel() * 0.1);
		}
		else if (length >= _max_length)
		{
			if (length >= _rupture_length) 
			{
				force = Vector3(0);
				return;
			}

			force *= sqrt(delta_x) * _k;
			particle->addForce(force);
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

	double _max_length, _min_length, _rupture_length;

	bool _goma;
};