#include "StarParticleGenerator.h"
#include <random>

StarParticleGenerator::StarParticleGenerator(int numParticles, Vector3 pos, float radius, Vector3 vel, float numPuntas, float pronunciamientoPuntas, float suavizadoPuntas)
{
	_num_particles = numParticles; 
	_radius = radius; 
	_mean_vel = vel;
	_mean_pos = pos;

	nPuntas = numPuntas;
	pronunciamiento = pronunciamientoPuntas;
	suavizado = suavizadoPuntas;
}

StarParticleGenerator::~StarParticleGenerator()
{
}

std::list<Particle*> StarParticleGenerator::generateParticles()
{
	std::list<Particle*> particles = std::list<Particle*>();

	float angle = 0;
	for (int i = 0; i < _num_particles; i++)
	{
		angle = 360.0 / (float)_num_particles * i;

		Vector3 vel = Vector3(cos(angle * (PI / 180)), sin(angle * (PI / 180)), 0);

		float num = cos((2 * asin(suavizado) + PI * pronunciamiento) / (2 * nPuntas));
		float den = cos((2 * asin(suavizado * cos(nPuntas * angle)) + PI * pronunciamiento) / (2 * nPuntas));

		float speed = num / den;

		Particle* p;

		p = _model->clone();
		p->setPos(_mean_pos);
		p->setVel(vel.getNormalized() * speed * 20);

		particles.push_back(p);
	}

	return particles;
}