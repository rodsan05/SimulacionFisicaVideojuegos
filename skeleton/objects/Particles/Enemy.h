#pragma once
#include "RigidParticle.h"
#include "../MyCharacterController.h"
#include "../ParticleSystem.h"
#include "../ParticleGenerators/GaussianParticleGenerator.h"

class EnemyClass : public RigidParticle
{
public:
	EnemyClass(physx::PxScene* scene, physx::PxPhysics* physics, MyCharacterController* player, Vector3 pos, float size, float speed, int hp, ParticleSystem* pSys, bool follows = false);
	virtual ~EnemyClass();

	virtual void integrate(double t) override;

protected:
	MyCharacterController* _player;
	float _speed;
	int _hp;

	bool _follows;

	ParticleGenerator* _death_gen1;
	ParticleGenerator* _death_gen2;
	ParticleSystem* _pSys;
};

