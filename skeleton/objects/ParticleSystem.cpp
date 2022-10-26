#include "ParticleSystem.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
#include "CircleParticleGenerator.h"
#include "SphereParticleGenerator.h"
#include "StarParticleGenerator.h"

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

void ParticleSystem::generateFireworksSystem(FireworkType ft)
{
	Firework* f = new Firework(this, Vector3(0, 0, 0), Vector3(0, 30, 0), Vector3(0, 0, 0), 0.999f, 1, Color(0.85, 0.03, 0.16, 1), 1000, -1, 0);
	Firework* f2 = new Firework(this, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 0.8f, Color(0.9, 0.36, 0.03, 1), 500, -1, 0);
	Firework* f3 = new Firework(this, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 0.5f, Color(0.9, 0.57, 0.03, 1), 500, -1, 0);

	f2->deregisterRender();
	f3->deregisterRender();

	ParticleGenerator* fireworkGen1;
	ParticleGenerator* fireworkGen2;

	if (ft == Circle)
	{
		fireworkGen1 = new CircleParticleGenerator(6, Vector3(0), 10, Vector3(0));
		fireworkGen2 = new CircleParticleGenerator(10, Vector3(0), 10, Vector3(0));
	}
	else if (ft == Sphere)
	{
		fireworkGen1 = new SphereParticleGenerator(100, Vector3(0), 10, Vector3(0));
		fireworkGen2 = new SphereParticleGenerator(5, Vector3(0), 10, Vector3(0));
	}
	else if (ft == Star) 
	{
		fireworkGen1 = new StarParticleGenerator(60, Vector3(0), 10, Vector3(0), 6, 3, 1);
		fireworkGen2 = new StarParticleGenerator(5, Vector3(0), 10, Vector3(0), 5, 3, 1);
	}

	fireworkGen1->setParticle(f2);
	f->addParticleGen(fireworkGen1);

	fireworkGen2->setParticle(f3);
	f2->addParticleGen(fireworkGen2);
		
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
