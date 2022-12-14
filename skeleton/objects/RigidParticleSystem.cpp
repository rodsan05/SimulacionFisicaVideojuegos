#include "RigidParticleSystem.h"
#include "IncludeFiles/RigidParticleGeneratorsIncludes.h"

RigidParticleSystem::RigidParticleSystem()
{
}

RigidParticleSystem::~RigidParticleSystem()
{
}

void RigidParticleSystem::createParticleGenerator(ParticleGenType type)
{
}

void RigidParticleSystem::update(double t)
{
}

void RigidParticleSystem::generate()
{
}

void RigidParticleSystem::generatePerpetual()
{
}

void RigidParticleSystem::clearAllGenerators()
{
}

void RigidParticleSystem::clearForces()
{
}

void RigidParticleSystem::killAllParticles()
{
	//for (auto p : _particles) p->setAlive(false);
}
