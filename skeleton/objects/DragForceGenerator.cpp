#include "DragForceGenerator.h"

DragForceGenerator::DragForceGenerator()
{
}

DragForceGenerator::DragForceGenerator(const float k1, const float k2) : _k1(k1), _k2(k2)
{
}

DragForceGenerator::~DragForceGenerator()
{
}

void DragForceGenerator::updateForce(Particle* particle, double t)
{
	if (fabs(1 / particle->getMass()) < 1e-10) 
	{
		return;
	}

	//compute drag force
	Vector3 v = particle->getVel();
	float velocity_module = v.normalize();
	
	Vector3 dragF;
	velocity_module = _k1 * velocity_module + _k2 * velocity_module * velocity_module;
	dragF = -v * velocity_module;

	//apply drag force
	particle->addForce(dragF);
}
