#include "ParticleSystem.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"

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

		_particles.insert(_particles.end(), list.begin(), list.end());
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
			delete p;
			iterador = _particles.erase(iterador);
		}
	}
}

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name)
{
    return nullptr;
}

void ParticleSystem::generateFireworksSystem()
{
}
