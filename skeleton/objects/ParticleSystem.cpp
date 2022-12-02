#include "ParticleSystem.h"
#include "IncludeFiles/ParticleGeneratorsIncludes.h"

ParticleSystem::ParticleSystem() : _gravity_gen(nullptr), _wind_gen(nullptr), _whirlwind_gen(nullptr), _firework_gen(nullptr),
_explosion_gen(nullptr), _reverse_gravity_gen(nullptr)
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
		p = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, -10, 0), 0.999f, 1, Color(0, 0, 0, 1), -1, 20, 2);
		p->deregisterRender();
		generator = new GaussianParticleGenerator({ 0, 15, 0 }, { 0, 0, 0 }, { 3, 8, 8 }, { 10, 10, 10 }, 0.8, 20);
		generator->setParticle(p);
		generator->setPerpetual(true);
		break;
	case RandomMass:
		p = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 1, Color(0.7, 0.7, 0.2, 1), -1, 200);
		p->deregisterRender();
		generator = new GaussianParticleGenerator({ 0, 0, 0 }, { 0, 0, 0 }, { 8, 3, 8 }, { 0.001f, 0.001f, 0.001f }, 0.8, 20, true);
		generator->setParticle(p);
		generator->setPerpetual(true);
		break;
	case Cubo:
		p = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 1, Color(0.2, 0.2, 0.9, 1), -1, 1000, 1);
		p->deregisterRender();
		generator = new UniformParticleGenerator({ -15, 0, 0 }, { 0, 0,0 }, { 30, 30, 30 }, { 1, 1, 1 }, 0.8, 20);
		generator->setParticle(p);
		generator->setPerpetual(true);
		break;
	case Sangre:
		p = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, -5, 0), 0.999f, 0.5, Color(0.7, 0, 0, 1), 2000, 30, 1);
		p->deregisterRender();
		generator = new GaussianParticleGenerator({ 0, 15, 0 }, { 0, 0, 0 }, { 0.001f, 0.001f, 0.001f }, { 20, 20, 20 }, 1, 30);
		generator->setParticle(p);
		break;
	case Humo:
		p = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 5, 0), 0.999f, 0.5, Color(0.6, 0.5, 0.4, 1), -1, 200, 1);
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
	generatePerpetual();

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
		auto list = g->generateParticles();

		for (auto p : list)
		{
			if (_gravity_gen != nullptr)
				_force_registry->addRegistry(_gravity_gen, p);

			if (_reverse_gravity_gen != nullptr)
				_force_registry->addRegistry(_reverse_gravity_gen, p);

			if (_wind_gen != nullptr)
				_force_registry->addRegistry(_wind_gen, p);

			if (_whirlwind_gen != nullptr)
				_force_registry->addRegistry(_whirlwind_gen, p);
		}

		appendParticles(list);
	}
}

void ParticleSystem::generatePerpetual()
{
	for (auto g : _particle_generators)
	{
		if (g->isPerpetual())
		{
			auto list = g->generateParticles();

			for (auto p : list)
			{
				if (_gravity_gen != nullptr && p->isAffectedByGravity())
					_force_registry->addRegistry(_gravity_gen, p);

				if (_reverse_gravity_gen != nullptr)
					_force_registry->addRegistry(_reverse_gravity_gen, p);

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
	Firework* f = new Firework(this, Vector3(0, 0, 0), Vector3(0, 30, 0), Vector3(0, 0, 0), 0.999f, 1, Color(0.85, 0.03, 0.16, 1), 1000, -1, 1);
	Firework* f2 = new Firework(this, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 0.8f, Color(0.9, 0.36, 0.03, 1), 500, -1, 1);
	Firework* f3 = new Firework(this, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 0.5f, Color(0.9, 0.57, 0.03, 1), 500, -1, 1);

	f2->deregisterRender();
	f3->deregisterRender();

	ParticleGenerator* fireworkGen1;
	ParticleGenerator* fireworkGen2;

	if (ft == Circle)
	{
		fireworkGen1 = new CircleParticleGenerator(6, Vector3(0), 10, Vector3(0));
		fireworkGen2 = new CircleParticleGenerator(10, Vector3(0), 10, Vector3(0));
	}
	else if (ft == SphereFirework)
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
	if (_gravity_gen != nullptr)
		for (auto p : particles) _force_registry->addRegistry(_gravity_gen, p);

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
	if (_gravity_gen == nullptr)
	{
		_gravity_gen = new GravityForceGenerator(Vector3(0, -10, 0));

		for (auto p : _particles)
		{
			if (p->isAffectedByGravity())
				_force_registry->addRegistry(_gravity_gen, p);
		}
	}
}

void ParticleSystem::generateReverseGravity()
{
	if (_reverse_gravity_gen == nullptr)
	{
		_reverse_gravity_gen = new GravityForceGenerator(Vector3(0, 10, 0));

		for (auto p : _particles)
		{
			_force_registry->addRegistry(_reverse_gravity_gen, p);
		}
	}
}

void ParticleSystem::generateWind()
{
	if (_wind_gen == nullptr)
	{
		_wind_gen = new WindForceGenerator(Vector3(-10, 0, 0), 1, 0);

		for (auto p : _particles)
		{
			_force_registry->addRegistry(_wind_gen, p);
		}
	}
}

void ParticleSystem::generateWhirlwind()
{
	if (_whirlwind_gen == nullptr)
	{
		_whirlwind_gen = new WhirlwindForceGenerator(Vector3(0, 0, 0), 1, 1, 0, 0.15);

		for (auto p : _particles)
		{
			_force_registry->addRegistry(_whirlwind_gen, p);
		}
	}
}

void ParticleSystem::generateExplosion()
{
	if (_explosion_gen != nullptr)
	{
		_force_registry->deleteGeneratorRegistry(_explosion_gen);
		delete _explosion_gen;
	}

	_explosion_gen = new ExplosionForceGenerator(Vector3(0, 0, 0), 100, 100000);

	for (auto p : _particles)
	{
		_force_registry->addRegistry(_explosion_gen, p);
	}
}

void ParticleSystem::generateSpringDemo()
{
	// First one standard spring uniting 2 particles
	Particle* p1 = new Particle(Vector3(-12.0, 10.0, 0.0), Vector3(0), Vector3(0), 0.85, 1, Color(0.9, 0.8, 0.05, 1), -1, -1, 60);
	Particle* p2 = new Particle(Vector3(12.0, 10.0, 0.0), Vector3(0), Vector3(0), 0.85, 1, Color(0.5, 0.5, 0.5, 1), -1, -1, 60);
	p1->setMass(1.0);
	p2->setMass(1.0);

	SpringForceGenerator* f1 = new SpringForceGenerator(500, 10, p2);
	_force_registry->addRegistry(f1, p1);
	SpringForceGenerator* f2 = new SpringForceGenerator(500, 10, p1);
	_force_registry->addRegistry(f2, p2);

	_spring_generators.push_back(f1);
	_spring_generators.push_back(f2);
	_particles.push_back(p1);
	_particles.push_back(p2);

	// Then one spring with one fixed side
	Particle* p3 = new Particle(Vector3(-10.0, 20.0, 0.0), Vector3(0), Vector3(0), 0.85, 1, Color(0.9, 0.05, 0.8, 1), -1, -1, 2);
	AnchoredSpringFG* f3 = new AnchoredSpringFG(1, 10, Vector3(10.0, 20.0, 0.0));

	_force_registry->addRegistry(f3, p3);
	_spring_generators.push_back(f3);
	_particles.push_back(p3);
}

void ParticleSystem::generateSlinky()
{
	float distance = -5.0;
	float iniY = 30;

	auto p0 = new Particle(Vector3(0.0, iniY - distance, 0.0), Vector3(0), Vector3(0), 0.85, 1, Color(1, 1, 1, 1), -1, -1, 1);
	auto f0 = new AnchoredSpringFG(20, .5, Vector3(0.0, iniY - 2 * distance, 0.0));
	_force_registry->addRegistry(f0, p0);
	_spring_generators.push_back(f0);
	_particles.push_back(p0);

	for (int i = 0; i < 10; i += 2)
	{
		auto p1 = new Particle(Vector3(0.0, iniY + distance * i, 0.0), Vector3(0), Vector3(0), 0.85, 1, Color(0.9 - (i * 0.1), 0.9 - (i * 0.1), 0.9 - (i * 0.1), 1), -1, -1, 1);
		auto p2 = new Particle(Vector3(0.0, iniY + distance * (i + 1), 0.0), Vector3(0), Vector3(0), 0.85, 1, Color(0.9 - ((i + 1) * 0.1), 0.9 - ((i + 1) * 0.1), 0.9 - ((i + 1) * 0.1), 1), -1, -1, 1);

		SpringForceGenerator* f0 = new SpringForceGenerator(20, .5, p0);
		_force_registry->addRegistry(f0, p1);
		SpringForceGenerator* f1 = new SpringForceGenerator(20, .5, p2);
		_force_registry->addRegistry(f1, p1);
		SpringForceGenerator* f2 = new SpringForceGenerator(20, .5, p1);
		_force_registry->addRegistry(f2, p2);
		_force_registry->addRegistry(f2, p0);

		_spring_generators.push_back(f0);
		_spring_generators.push_back(f1);
		_spring_generators.push_back(f2);
		_particles.push_back(p1);
		_particles.push_back(p2);

		p0 = p2;
	}
}

void ParticleSystem::generateFloatingDemo()
{
	Particle* p1 = new Particle(Vector3(0.0, 10.0, 0.0), Vector3(0), Vector3(0), 0.99, 2, Color(0.9, 0.8, 0.05, 1), -1, -1, 60, Cube);
	Particle* p2 = new Particle(Vector3(0.0, 10.0, 0.0), Vector3(0), Vector3(0), 0.99, 20, Color(0.1, 0.1, 0.9, 1), -1, -1, 1, Plane);
	p2->setAffectedByGravity(false);

	BouyancyForceGenerator* forceGen = new BouyancyForceGenerator(1000, 30, 20 * 20 * 30);
	DragForceGenerator* dragGen = new DragForceGenerator(10, 0);

	_force_registry->addRegistry(forceGen, p1);
	_force_registry->addRegistry(dragGen, p1);
	_particles.push_back(p1);
	_particles.push_back(p2);
}

void ParticleSystem::clearForces()
{
	_force_registry->deleteGeneratorRegistry(_gravity_gen);
	delete _gravity_gen;
	_gravity_gen = nullptr;

	_force_registry->deleteGeneratorRegistry(_reverse_gravity_gen);
	delete _reverse_gravity_gen;
	_reverse_gravity_gen = nullptr;

	_force_registry->deleteGeneratorRegistry(_wind_gen);
	delete _wind_gen;
	_wind_gen = nullptr;

	_force_registry->deleteGeneratorRegistry(_whirlwind_gen);
	delete _whirlwind_gen;
	_whirlwind_gen = nullptr;

	_force_registry->deleteGeneratorRegistry(_explosion_gen);
	delete _explosion_gen;
	_explosion_gen = nullptr;
}

void ParticleSystem::killAllParticles()
{
	for (auto p : _particles) p->setAlive(false);

	for (auto f : _firework_pool) f->setAlive(false);
}

void ParticleSystem::incrementAllSprings(double increment)
{
	for (auto f : _spring_generators)
	{
		f->incrementK(increment);
	}
}

void ParticleSystem::decrementAllSprings(double decrement)
{
	for (auto f : _spring_generators)
	{
		f->decrementK(decrement);
	}
}
