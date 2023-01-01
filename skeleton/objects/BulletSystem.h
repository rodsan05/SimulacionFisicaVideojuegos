#pragma once
#include "ForceRegistry.hpp"
#include "Particles/RigidParticle.h"
#include "IncludeFiles/ForceGeneratorsIncludes.h"
#include "ParticleGenerators/GaussianParticleGenerator.h"
#include "Gun.h"

#include <vector>
#include "ParticleSystem.h"

class BulletSystem
{
public:
	BulletSystem(physx::PxScene* scene, physx::PxPhysics* physics, ParticleSystem* pSys);
	virtual ~BulletSystem();

	void update(double t);
	void shoot();

	void changeAmmo(ProyectileType ammo);
	ProyectileType getCurrentAmmo();

	void killAllParticles();

protected:

	const int N_AMMO = 5;
	const std::vector<int> SHOOT_CD = { 200, 1000, 500, 2000, 1000 };

	physx::PxScene* _scene;
	physx::PxPhysics* _physics;
	ForceRegistry* _force_registry;
	std::list<ForceGenerator*> _force_generators;
	std::list<Particle*> _particles;

	ParticleSystem* _pSys;

	ProyectileType _currAmmo;

	std::vector<int> _shootTime = std::vector<int>(N_AMMO, 0);
	std::vector<bool> _shoot_on_cd = std::vector<bool>(N_AMMO, false);
};

