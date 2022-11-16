#pragma once
#include "DragForceGenerator.h"
class WindForceGenerator : public DragForceGenerator
{
public:
	WindForceGenerator(Vector3 vel, float k1, float k2);
	~WindForceGenerator();

	void updateForce(Particle* particle, double t) override;

protected:
	Vector3 windVelocity;
};

