#pragma once
#include "ForceGenerator.h"
class BouyancyForceGenerator : public ForceGenerator
{
public:
	BouyancyForceGenerator();
	BouyancyForceGenerator(float density, float depth, float volume);
	~BouyancyForceGenerator();

	virtual void updateForce(Particle* particle, double duration) override;

protected:
	float _volume, _density, _depth;
};

