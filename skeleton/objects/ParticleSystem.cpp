#include "ParticleSystem.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
#include "CircleParticleGenerator.h"
#include "SphereParticleGenerator.h"
#include "StarParticleGenerator.h"

ParticleSystem::ParticleSystem() : _gravity_gen(nullptr), _wind_gen(nullptr), _whirlwind_gen(nullptr), _firework_gen(nullptr)
{
	_force_registry = new ForceRegistry();
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::createParticleGenerator(ParticleGenType type)
{
	ParticleGenerator* generator;
	Particle* p;

	switch (type)
	{
	case Hormigas:
		p = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, -10, 0), 0.999f, 1, Color(0, 0, 0, 1), -1, 20);
		p->deregisterRender();
		generator = new GaussianParticleGenerator({ 0, 15, 0 }, { 0, 0, 0 }, { 3, 8, 8 }, { 10, 10, 10 }, 0.8, 20);
		generator->setParticle(p);
		generator->setPerpetual(true);
		break;
	case Cubo:
		p = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 1, Color(0.2, 0.2, 0.9, 1), -1, 50, 1);
		p->deregisterRender();
		generator = new UniformParticleGenerator({ -15, 30, 0 }, { 0, 0,0 }, { 30, 30, 30 }, { 1, 1, 1 }, 0.8, 20);
		generator->setParticle(p);
		generator->setPerpetual(true);
		break;
	case Sangre:
		p = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, -5, 0), 0.999f, 0.5, Color(0.7, 0, 0, 1), 2000, 30);
		p->deregisterRender();
		generator = new GaussianParticleGenerator({ 0, 15, 0 }, { 0, 0, 0 }, { 0.001f, 0.001f, 0.001f }, { 20, 20, 20 }, 1, 30);
		generator->setParticle(p);
		break;
	case Humo:
		p = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 5, 0), 0.999f, 0.5, Color(0.6, 0.5, 0.4, 1), -1, 100);
		p->deregisterRender();
		generator = new GaussianParticleGenerator({ 0, 0, 0 }, { 0, 0, 0 }, { 2.5, 5, 0.001f }, { 0.001f, 0.001f, 0.001f }, 1, 5);
		generator->setParticle(p);
		generator->setPerpetual(true);
		break;
	default:
		break;
	}

	_particle_generators.push_back(generator);
}

void ParticleSystem::update(double t)
{
	generate();

	_force_registry->updateForces(t);

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
			_force_registry->deleteParticleRegistry(p);

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

void ParticleSystem::generate()
{
	for (auto g : _particle_generators)
	{
		if (g->isPerpetual())
		{
			auto list = g->generateParticles();

			for (auto p : list)
			{
				if (_gravity_gen != nullptr)
					_force_registry->addRegistry(_gravity_gen, p);

				if (_wind_gen != nullptr)
					_force_registry->addRegistry(_wind_gen, p);

				if (_whirlwind_gen != nullptr)
					_force_registry->addRegistry(_whirlwind_gen, p);
			}

			appendParticles(list);

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

void ParticleSystem::clearAllGenerators()
{
	auto it = _particle_generators.begin();

	while (it != _particle_generators.end())
	{
		delete* it;
		it = _particle_generators.erase(it);
	}

	_particle_generators.clear();
}

void ParticleSystem::generateGravity()
{
}

void ParticleSystem::generateWind()
{
}

void ParticleSystem::generateWhirlwind()
{
}

void ParticleSystem::clearForces()
{
	
}
