#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const Vector3& g)
{
	_gravity = g;
}

GravityForceGenerator::~GravityForceGenerator()
{
}

void GravityForceGenerator::updateForce(Particle* particle, double t)
{
	if (fabs(1 / particle->getMass()) < 1e-10)
	{
		return;
	}

	particle->addForce(_gravity * particle->getMass());
}
