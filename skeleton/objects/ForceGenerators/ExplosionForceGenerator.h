#pragma once
#include "ForceGenerator.h"

class ExplosionForceGenerator : public ForceGenerator
{
public:
	ExplosionForceGenerator();
	ExplosionForceGenerator(Vector3 orig, float radius, const float k);
	~ExplosionForceGenerator();

	virtual void updateForce(Particle* particle, double t);

protected:
	float K;//Coef for velocity
	Vector3 origen;
	float explosionRadius;

	int contT;
};