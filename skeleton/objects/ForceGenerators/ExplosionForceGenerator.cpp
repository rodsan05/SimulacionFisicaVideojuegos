#include "ExplosionForceGenerator.h"
#include <cmath>

ExplosionForceGenerator::ExplosionForceGenerator()
{
}

ExplosionForceGenerator::ExplosionForceGenerator(Vector3 orig, float radius, const float k) : origen(orig), explosionRadius(radius), K(k), contT(1)
{
}

ExplosionForceGenerator::~ExplosionForceGenerator()
{
}

void ExplosionForceGenerator::updateForce(Particle* particle, double t)
{
	/*auto soundVel = 343;
	explosionRadius = soundVel * t;*/

	if (fabs(1 / particle->getMass()) < 1e-10)
	{
		return;
	}

	const double euler = std::exp(1.0);

	auto difX = particle->getPos().x - origen.x;
	auto difY = particle->getPos().y - origen.y;
	auto difZ = particle->getPos().z - origen.z;

	auto r2 = pow(difX, 2) + pow(difY, 2) + pow(difZ, 2);

	if (r2 > pow(explosionRadius, 2))
	{
		return;
	}

	auto x = (K / r2) * difX * pow(euler, (-t / contT));
	auto y = (K / r2) * difY * pow(euler, (-t / contT));
	auto z = (K / r2) * difZ * pow(euler, (-t / contT));

	Vector3 force(x, y, z);

	//apply drag force
	particle->addForce(force);

	contT += t;
}
