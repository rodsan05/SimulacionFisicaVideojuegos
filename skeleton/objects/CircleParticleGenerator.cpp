#include "CircleParticleGenerator.h"

CircleParticleGenerator::CircleParticleGenerator(int numParticles, Vector3 pos, float radius, Vector3 vel)
{
	_num_particles = numParticles; 
	_radius = radius; 
	_mean_vel = vel;
	_mean_pos = pos;
}

CircleParticleGenerator::~CircleParticleGenerator()
{
}

std::list<Particle*> CircleParticleGenerator::generateParticles()
{
	std::list<Particle*> particles = std::list<Particle*>();

	float angle = 0;
	for (int i = 0; i < _num_particles; i++)
	{
		angle = 360.0 / (float)_num_particles * i;

		//Vector3 pos(cos(angle * (PI / 180)) * _radius, sin(angle * (PI / 180)) * _radius, 0);

		Vector3 vel = Vector3(cos(angle * (PI / 180)), sin(angle * (PI / 180)), 0);

		Particle* p;

		p = _model->clone();
		p->setPos(_mean_pos);
		p->setVel(vel.getNormalized() * 20);

		particles.push_back(p);
	}

	return particles;
}