#pragma once
#include "ForceRegistry.hpp"
#include "Particles/RigidParticle.h"
#include "IncludeFiles/ForceGeneratorsIncludes.h"

#include <vector>

enum ProyectileType
{
	Bullet, CannonBall, Laser, Misile, Fireball
};

class BulletSystem
{
public:
	BulletSystem(physx::PxScene* scene, physx::PxPhysics* physics);
	virtual ~BulletSystem();

	void update(double t);
	void shoot(ProyectileType ammo);

	void killAllParticles();

protected:

	const std::vector<int> SHOOT_CD = { 200, 1000, 500, 2000 };

	physx::PxScene* _scene;
	physx::PxPhysics* _physics;
	ForceRegistry* _force_registry;
	std::list<ForceGenerator*> _force_generators;
	std::list<Particle*> _particles;

	ProyectileType _currAmmo;

	std::vector<int> _shootTime = std::vector<int>(4, 0);
	std::vector<bool> _shoot_on_cd = std::vector<bool>(4, false);
};

