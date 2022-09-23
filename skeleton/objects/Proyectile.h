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

		switch (type)
		{
		case Bullet:
			speed = 150;
			gravity = 1;
			dumping = 0.999;
			radius = 1;
			break;
		case CannonBall:
			break;
		case Laser:
			break;
		case Misile:
			break;
		case Fireball:
			break;
		default:
			break;
		}

		setParticle(pos, dir.getNormalized() * speed, { 0, -gravity, 0 }, dumping, radius, { 0.5, 0.5, 0.5, 1 });
	}
	~Proyectile() 
	{
	}
};

