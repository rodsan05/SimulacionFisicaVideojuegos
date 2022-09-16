#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel)
{
	vel = Vel;

	pose.p = Pos;

	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(5)), &pose, Vector4(1, 1, 1, 1));
}

Particle::~Particle()
{
	delete renderItem;
}

void Particle::integrate(double t)
{
	// p(i+1) = p(i) + v * t
	pose.p = pose.p + vel * t;
}
