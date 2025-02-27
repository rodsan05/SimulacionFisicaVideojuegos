#include "ParticleSystem.h"
#include "IncludeFiles/ParticleGeneratorsIncludes.h"

ParticleSystem::ParticleSystem(physx::PxScene* scene, physx::PxPhysics* physics, MyCharacterController* charControl, int maxParticles) : _gravity_gen(nullptr), _wind_gen(nullptr), _whirlwind_gen(nullptr),
_explosion_gen(nullptr), _reverse_gravity_gen(nullptr), _scene(scene), _physics(physics), _charController(charControl)
{
	_force_registry = new ForceRegistry();
	_maxParticles = maxParticles;
}

ParticleSystem::~ParticleSystem()
{
	for (auto p : _particles) 
		delete p;

	for (auto p : _firework_pool)
		delete p;

	for (auto f : _force_generators)
		delete f;

	for (auto f : _spring_generators)
		delete f;
	
	clearForces();

	delete _force_registry;
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
	case RigidDemo:
		p = new RigidParticle(_scene, _physics, false, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 2, Color(0.7, 0.9, 0.05, 1), 10000, 500, 1, Capsule);
		p->deregisterRender();
		generator = new GaussianParticleGenerator({ 0, 20, 0 }, { 0, 0, 0 }, { 10, 10, 10 }, { 0.001f, 0.001f, 0.001f }, 1, 1, true, true, true);
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
			_force_registry->deleteParticleRegistry(p);

			p->onDeath();
			delete p;
			iteradorF = _firework_pool.erase(iteradorF);
		}
	}
}

void ParticleSystem::generate()
{
	if (_particles.size() <= _maxParticles)
	{
		for (auto g : _particle_generators)
		{
			auto list = g->generateParticles();

			appendParticles(list);
		}
	}
}

void ParticleSystem::generatePerpetual()
{
	if (_particles.size() <= _maxParticles)
	{
		for (auto g : _particle_generators)
		{
			if (g->isPerpetual())
			{
				auto list = g->generateParticles();

				appendParticles(list);
			}
		}
	}
}

void ParticleSystem::generateFireworksSystem(FireworkType ft, Vector3 pos)
{
	auto random1 = rand() % 100 + 10;
	auto random2 = rand() % 100 + 10;
	auto random3 = rand() % 100 + 10;
	auto color1 = Color(random1 / 100.0f, random2 / 100.0f, random3 / 100.0f, 1);

	random1 = rand() % 100 + 10;
	random2 = rand() % 100 + 10;
	random3 = rand() % 100 + 10;
	auto color2 = Color(random1 / 100.0f, random2 / 100.0f, random3 / 100.0f, 1);

	random1 = rand() % 100 + 10;
	random2 = rand() % 100 + 10;
	random3 = rand() % 100 + 10;
	auto color3 = Color(random1 / 100.0f, random2 / 100.0f, random3 / 100.0f, 1);

	Firework* f = new Firework(this, pos, Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 0.5f, Color(0.85, 0.03, 0.16, 1), 1, -1, 1);
	Firework* f2 = new Firework(this, pos, Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 0.4f, color1, 100, -1, 1);
	Firework* f3 = new Firework(this, pos, Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 0.3f, color2, 50, -1, 1);
	Firework* f4 = new Firework(this, pos, Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 0.2f, color3, 50, -1, 1);

	f2->deregisterRender();
	f3->deregisterRender();
	f4->deregisterRender();

	ParticleGenerator* fireworkGen1;
	ParticleGenerator* fireworkGen2;
	ParticleGenerator* fireworkGen3;

	if (ft == Circle)
	{
		fireworkGen1 = new CircleParticleGenerator(6, Vector3(0), 10, Vector3(0));
		fireworkGen2 = new CircleParticleGenerator(10, Vector3(0), 10, Vector3(0));
		fireworkGen3 = new CircleParticleGenerator(8, Vector3(0), 10, Vector3(0));
	}
	else if (ft == SphereFirework)
	{
		fireworkGen1 = new SphereParticleGenerator(50, Vector3(0), 10, Vector3(0));
		fireworkGen2 = new SphereParticleGenerator(5, Vector3(0), 10, Vector3(0));
		fireworkGen3 = new SphereParticleGenerator(2, Vector3(0), 10, Vector3(0));
	}
	else if (ft == Star)
	{
		fireworkGen1 = new StarParticleGenerator(60, Vector3(0), 10, Vector3(0), 6, 3, 1);
		fireworkGen2 = new StarParticleGenerator(5, Vector3(0), 10, Vector3(0), 5, 3, 1);
		fireworkGen3 = new StarParticleGenerator(5, Vector3(0), 10, Vector3(0), 5, 3, 1);
	}

	fireworkGen1->setParticle(f2);
	f->addParticleGen(std::shared_ptr<ParticleGenerator>(fireworkGen1));

	fireworkGen2->setParticle(f3);
	f2->addParticleGen(std::shared_ptr<ParticleGenerator>(fireworkGen2));

	fireworkGen3->setParticle(f4);
	f3->addParticleGen(std::shared_ptr<ParticleGenerator>(fireworkGen3));

	_firework_pool.push_back(f);
}
void ParticleSystem::appendParticles(std::list<Particle*> particles)
{
	for (auto p : particles)
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

	_particles.insert(_particles.end(), particles.begin(), particles.end());
}

void ParticleSystem::appendParticlesWithGravity(std::list<Particle*> particles, float g)
{
	auto gravGen = new GravityForceGenerator(Vector3(0, g, 0));

	for (auto p : particles)
	{
		_force_registry->addRegistry(gravGen, p);
	}

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
		_reverse_gravity_gen = new GravityForceGenerator(Vector3(0, 100, 0));

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
		_wind_gen = new WindForceGenerator(Vector3(-10, 0, 0), 10, 0);

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
		_whirlwind_gen = new WhirlwindForceGenerator(Vector3(0, 0, 0), 10, 1, 0, 0.15);

		for (auto p : _particles)
		{
			_force_registry->addRegistry(_whirlwind_gen, p);
		}
	}
}

void ParticleSystem::generateExplosion(Vector3 pos)
{
	if (_explosion_gen != nullptr)
	{
		_force_registry->deleteGeneratorRegistry(_explosion_gen);
		delete _explosion_gen;

		_force_registry->deleteGeneratorRegistry(_player_explosion_gen);
		delete _player_explosion_gen;
	}

	_explosion_gen = new ExplosionForceGenerator(pos, 10, 100000);

	for (auto p : _particles)
	{
		_force_registry->addRegistry(_explosion_gen, p);
	}

	_player_explosion_gen = new ExplosionForceGenerator(pos, 10, 20000);
	_force_registry->addRegistry(_player_explosion_gen, _charController);
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

void ParticleSystem::generateSlinky(Vector3 iniPos, Vector3 finalPos)
{
	float distance = (finalPos.x - iniPos.x) / 12;

	float iniX = iniPos.x;
	float iniY = iniPos.y;
	float iniZ = iniPos.z;

	auto p0 = new Particle(Vector3(iniX - distance, iniY, iniZ), Vector3(0), Vector3(0), 0.85, 0.2, Color(1, 1, 1, 1), -1, -1, 1);
	auto f0 = new AnchoredSpringFG(100, .5, Vector3(iniX - 2 * distance, iniY, iniZ));
	_force_registry->addRegistry(f0, p0);
	_spring_generators.push_back(f0);
	_particles.push_back(p0);

	for (int i = 0; i < 10; i += 2)
	{
		auto p1 = new Particle(Vector3(iniX + distance * i, iniY, iniZ), Vector3(0), Vector3(0), 0.85, 0.2, Color(0.9 - (i * 0.1), 0.9 - (i * 0.1), 0.9 - (i * 0.1), 1), -1, -1, 1);
		auto p2 = new Particle(Vector3(iniX + distance * (i + 1), iniY, iniZ), Vector3(0), Vector3(0), 0.85, 0.2, Color(0.9 - ((i + 1) * 0.1), 0.9 - ((i + 1) * 0.1), 0.9 - ((i + 1) * 0.1), 1), -1, -1, 1);

		SpringForceGenerator* f0 = new SpringForceGenerator(100, .5, p0);
		_force_registry->addRegistry(f0, p1);
		SpringForceGenerator* f1 = new SpringForceGenerator(100, .5, p2);
		_force_registry->addRegistry(f1, p1);
		SpringForceGenerator* f2 = new SpringForceGenerator(100, .5, p1);
		_force_registry->addRegistry(f2, p2);
		_force_registry->addRegistry(f2, p0);

		_spring_generators.push_back(f0);
		_spring_generators.push_back(f1);
		_spring_generators.push_back(f2);
		std::list<Particle*> parts;
		parts.push_back(p1);
		parts.push_back(p2);
		appendParticles(parts);

		p0 = p2;
	}
}

void ParticleSystem::generateFloatingDemo()
{
	Particle* p1 = new Particle(Vector3(0.0, 10.0, 0.0), Vector3(0), Vector3(0), 0.999f, 2, Color(0.9, 0.8, 0.05, 1), -1, -1, 60, Cube);
	Particle* p2 = new Particle(Vector3(0.0, 10.0, 0.0), Vector3(0), Vector3(0), 0.999f, 20, Color(0.1, 0.1, 0.9, 1), -1, -1, 1, Plane);
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
