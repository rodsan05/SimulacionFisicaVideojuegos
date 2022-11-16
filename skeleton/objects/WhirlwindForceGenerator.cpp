#include "WhirlwindForceGenerator.h"

WhirlwindForceGenerator::WhirlwindForceGenerator(Vector3 orig, float k_, float k1, float k2, float dragToCenter) :
	WindForceGenerator(Vector3(0, 0, 0), k1, k2), origen(orig), K(k_), dragToCenterCoef(dragToCenter)
{
}

WhirlwindForceGenerator::~WhirlwindForceGenerator()
{
}

void WhirlwindForceGenerator::updateForce(Particle* particle, double t)
{
	if (fabs(1 / particle->getMass()) < 1e-10)
	{
		return;
	}

	//compute drag force
	auto x = -(particle->getPos().z - origen.z);
	auto y = 50 - (particle->getPos().y - origen.y);
	auto z = -(particle->getPos().x - origen.x);
	windVelocity = K * Vector3(x, y, z);

	Vector3 v = windVelocity - particle->getVel();
	float velocity_module = v.normalize();

	Vector3 dragF;
	velocity_module = _k1 * velocity_module + _k2 * velocity_module * velocity_module;

	dragF = -v * velocity_module;

	//apply drag force
	particle->addForce(dragF);
}
