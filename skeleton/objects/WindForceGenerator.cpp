#include "WindForceGenerator.h"

WindForceGenerator::WindForceGenerator(Vector3 vel, float k1, float k2) : DragForceGenerator(k1, k2), windVelocity(vel)
{
}

WindForceGenerator::~WindForceGenerator()
{
}

void WindForceGenerator::updateForce(Particle* particle, double t)
{
	if (fabs(1 / particle->getMass()) < 1e-10)
	{
		return;
	}

	//compute drag force
	Vector3 v = windVelocity - particle->getVel();
	float velocity_module = v.normalize();

	Vector3 dragF;
	velocity_module = _k1 * velocity_module + _k2 * velocity_module * velocity_module;
	
	dragF = -v * velocity_module;

	//apply drag force
	particle->addForce(dragF);
}
