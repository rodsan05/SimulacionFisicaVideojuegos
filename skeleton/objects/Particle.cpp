#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel)
{
	vel = Vel;

	pose = physx::PxTransform(Pos);

	color = { 1, 1, 1, 1 };

	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), &pose, color);

	RegisterRenderItem(renderItem);
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

void Particle::integrate(double t)
{
	// p(i+1) = p(i) + v * t
	pose.p = pose.p + vel * t;
}
