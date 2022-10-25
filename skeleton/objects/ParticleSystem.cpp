#include "ParticleSystem.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
#include "CircleParticleGenerator.h"

ParticleSystem::ParticleSystem(ParticleGenType particleGen)
{
	switch (particleGen)
	{
	case GAUSSIAN:
		_particle_generators.push_back(new GaussianParticleGenerator({ 0, 50, 0 }, { 0, 0,0 }, { 3, 8, 8 }, { 10, 10, 10 }, 0.8, 20, { 0, 0, 0, 1}, 1, -1, 10));
		break;
	case UNIFORM:
		break;
	default:
		break;
	}
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::update(double t)
{
	for (auto g : _particle_generators) 
	{
		auto list = g->generateParticles();	
	}

	auto iterador = _particles.begin();

	while (iterador != _particles.end()) 
	{
		auto p = *iterador;

		if (p->isAlive())
		{
			p->integrate(t);
			iterador++;
		}
		else
		{
			p->onDeath();
			delete p;
			iterador = _particles.erase(iterador);
		}
	}

	auto iteradorF = _firework_pool.begin();

	while (iteradorF != _firework_pool.end())
	{
		auto p = *iteradorF;

		if (p->isAlive())
		{
			p->integrate(t);
			iteradorF++;
		}
		else
		{
			p->onDeath();
			delete p;
			iteradorF = _firework_pool.erase(iteradorF);
		}
	}
}

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name)
{
    return nullptr;
}

void ParticleSystem::generateFireworksSystem()
{
	Firework* f = new Firework(this, Vector3(0, 0, 0), Vector3(0, 10, 0), Vector3(0, 0, 0), 0.999f, 1, Color(0.5, 0.5, 0.5, 1), 1000, -1, 0);

	CircleParticleGenerator* fireworkGen1 = new CircleParticleGenerator(6, Vector3(0), 10, Vector3(0));
	
	fireworkGen1->setParticle(f);
	f->addParticleGen(fireworkGen1);

	_firework_pool.push_back(f);
}

void ParticleSystem::appendParticles(std::list<Particle*> particles)
{
	_particles.insert(_particles.end(), particles.begin(), particles.end());
}

void ParticleSystem::appendFireworks(std::list<Particle*> particles)
{
	_firework_pool.insert(_firework_pool.end(), particles.begin(), particles.end());
}
