#include "Particle.h"

Particle::Particle() : vel(), dumping(), a(), pose(), color(), renderItem()
{
}

Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 a_, float dumping_, float scale_, Color color_, float m_)
{
	vel = Vel;

	dumping = dumping_;
	a = a_;

	pose = physx::PxTransform(Pos);

	color = color_;

	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(scale_)), &pose, color);

	RegisterRenderItem(renderItem);

	m = m_;
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

void Particle::integrate(double t)
{
	// p(i+1) = p(i) + v * t
	pose.p = pose.p + vel * t;

	// vf = v0*d^t + a*t
	vel = vel * pow(dumping, t) + a * t;
}

void Particle::setParticle(Vector3 Pos, Vector3 Vel, Vector3 a_, float dumping_, float scale_, Color color_, float m_)
{
	vel = Vel;

	dumping = dumping_;
	a = a_;

	pose = physx::PxTransform(Pos);

	color = color_;

	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(scale_)), &pose, color);

	RegisterRenderItem(renderItem);

	m = m_;
}
