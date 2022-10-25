#include "Particle.h"

Particle::Particle() : vel(), dumping(), a(), pose(), color(), renderItem(), lifeTime(), m(), scale(), iniTime(), lifeDistance()
{
}

Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 a_, float dumping_, float scale_, Color color_, float lifeTime_, float lifeDist_, float m_)
{
	scale = scale_;
	vel = Vel;

	dumping = dumping_;
	a = a_;

	pose = physx::PxTransform(Pos);

	color = color_;

	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(scale_)), &pose, color);

	//RegisterRenderItem(renderItem);

	m = m_;

	lifeTime = lifeTime_;
	lifeDistance = lifeDist_;

	initPos = Pos;
	iniTime = glutGet(GLUT_ELAPSED_TIME);
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

bool Particle::isAlive()
{
	return alive;
}
void Particle::setAlive(bool set)
{
	alive = set;
}

void Particle::integrate(double t)
{
	// p(i+1) = p(i) + v * t
	pose.p = pose.p + vel * t;

	// vf = v0*d^t + a*t
	vel = vel * pow(dumping, t) + a * t;

	//comprueba si debe morir por tiempo
	if (lifeTime != -1) 
	{
		auto time = glutGet(GLUT_ELAPSED_TIME);

		auto elapsedTime = time - iniTime;

		if (elapsedTime > lifeTime) 
		{
			alive = false;
		}
	}

	//comprueba si debe morir por distancia
	if (lifeDistance != -1) 
	{
		auto distance = (pose.p - initPos).magnitudeSquared();

		if (distance > pow(lifeDistance, 2)) 
		{
			alive = false;
		}
	}
}

Particle* Particle::clone() const
{
	return new Particle(pose.p, vel, a, dumping, scale, color, lifeTime, lifeDistance, m);
}

void Particle::setPos(Vector3 pos_)
{
	pose.p = pos_;
}

void Particle::setVel(Vector3 vel_)
{
	vel = vel_;
}

void Particle::setParticle(Vector3 Pos, Vector3 Vel, Vector3 a_, float dumping_, float scale_, Color color_, float lifeTime_, float lifeDist_, float m_)
{
	scale = scale_;
	vel = Vel;

	dumping = dumping_;
	a = a_;

	pose = physx::PxTransform(Pos);

	color = color_;

	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(scale_)), &pose, color);

	m = m_;

	lifeTime = lifeTime_;
	lifeDistance = lifeDist_;

	initPos = Pos;
	iniTime = glutGet(GLUT_ELAPSED_TIME);
}
