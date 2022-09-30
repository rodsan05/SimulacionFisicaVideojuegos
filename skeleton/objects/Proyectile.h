#pragma once
#include "Particle.h"

enum ProyectileType
{
	Bullet, CannonBall, Laser, Misile, Fireball
};

class Proyectile : public Particle
{
public:

	Proyectile(ProyectileType type, Vector3 pos, Vector3 dir)
	{
		float speed = 1, gravity = 1, dumping = 0.999, radius = 1;
		double lifeTime = -1, lifeDist = -1;
		switch (type)
		{
		case Bullet:
			speed = 150;
			gravity = 1;
			dumping = 0.999;
			radius = 1;
			lifeTime = 1000;
			break;
		case CannonBall:
			speed = 150;
			gravity = 30;
			dumping = 0.8;
			radius = 7;
			lifeTime = 2000;
		break;
		case Laser:
			speed = 10000;
			gravity = 0;
			dumping = 0.999;
			radius = 1;
			lifeTime = 2000;
			break;
		case Misile:
			speed = 50;
			gravity = 0;
			dumping = 0.999;
			radius = 5;
			lifeTime = 4000;
			break;
		case Fireball:
			break;
		default:
			break;
		}

		setParticle(pos, dir.getNormalized() * speed, { 0, -gravity, 0 }, dumping, radius, { 0.5, 0.5, 0.5, 1 }, lifeTime, lifeDist);
	}
	~Proyectile() 
	{
	}
};

