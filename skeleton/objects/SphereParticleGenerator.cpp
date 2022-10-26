#include "SphereParticleGenerator.h"
#include <random>

SphereParticleGenerator::SphereParticleGenerator(int numParticles, Vector3 pos, float radius, Vector3 vel)
{
	_num_particles = numParticles; 
	_radius = radius; 
	_mean_vel = vel;
	_mean_pos = pos;
}

SphereParticleGenerator::~SphereParticleGenerator()
{
}

std::list<Particle*> SphereParticleGenerator::generateParticles()
{
	std::list<Particle*> particles = std::list<Particle*>();

	std::random_device rd{};
	std::mt19937 gen{ rd() };

	std::uniform_real_distribution<> rand(0, 1);

	double theta = 0, phi = 0;
	for (int i = 0; i < _num_particles; i++) {
		
		theta = 2 * PI * rand(gen);
		phi = acos(2 * rand(gen) - 1.0);
		
		float x = cos(theta) * sin(phi);
		float y = sin(theta) * sin(phi);
		float z = cos(phi);

		Vector3 vel(x, y, z);

		Particle* p;

		p = _model->clone();
		p->setPos(_mean_pos);
		p->setVel(vel.getNormalized() * 20);

		particles.push_back(p);
	}

	return particles;
}