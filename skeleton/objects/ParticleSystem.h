#pragma once
#include "ParticleGenerators/ParticleGenerator.h"
#include "ForceRegistry.hpp"
#include "Particles/RigidParticle.h"
#include "Particles/Firework.h"
#include "IncludeFiles/ForceGeneratorsIncludes.h"
#include "MyCharacterController.h"

#include <string>

enum ParticleGenType
{
	Hormigas, Cubo, Sangre, Humo, RandomMass, RigidDemo
};

class ParticleSystem
{
public:
	ParticleSystem(physx::PxScene* scene, physx::PxPhysics* physics, MyCharacterController* charControl, int maxParticles = 1000);
	virtual ~ParticleSystem();

	void createParticleGenerator(ParticleGenType type);

	void update(double t);
	void generate();
	void generatePerpetual();

	void generateFireworksSystem(FireworkType ft, Vector3 pos = Vector3(0));

	void appendParticles(std::list<Particle*> particles);
	void appendParticlesWithGravity(std::list<Particle*> particles, float g);
	void appendFireworks(std::list<Particle*> particles);

	void generateGravity();
	void generateReverseGravity();
	void generateWind();
	void generateWhirlwind();
	void generateExplosion(Vector3 pos = Vector3(0));

	void generateSpringDemo();
	void generateSlinky(Vector3 iniPos, Vector3 finalPos);
	void generateFloatingDemo();

	void clearAllGenerators();
	void clearForces();

	void killAllParticles();

	void incrementAllSprings(double increment);
	void decrementAllSprings(double decrement);

protected:

	physx::PxScene* _scene; 
	physx::PxPhysics* _physics;
	int _maxParticles;

	std::list<Particle*> _particles;
	std::list<Particle*> _firework_pool;

	std::list<ParticleGenerator*> _particle_generators;

	std::list<ForceGenerator*> _force_generators;
	std::list<SpringForceGenerator*> _spring_generators;
	ForceRegistry* _force_registry;
	GravityForceGenerator* _gravity_gen;
	GravityForceGenerator* _reverse_gravity_gen;
	WindForceGenerator* _wind_gen;
	WhirlwindForceGenerator* _whirlwind_gen;
	ExplosionForceGenerator* _explosion_gen;
	ExplosionForceGenerator* _player_explosion_gen;

	MyCharacterController* _charController;
};

