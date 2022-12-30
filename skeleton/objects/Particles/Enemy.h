#pragma once
#include "RigidParticle.h"
#include "../MyCharacterController.h"
#include "../ParticleSystem.h"
#include "../ParticleGenerators/GaussianParticleGenerator.h"

class EnemyClass : public RigidParticle
{
public:
	EnemyClass(physx::PxScene* scene, physx::PxPhysics* physics, MyCharacterController* player, Vector3 pos, float size, float speed, ParticleSystem* pSys);
	virtual ~EnemyClass();

	virtual void integrate(double t) override;

protected:
	MyCharacterController* _player;
	float _speed;

	ParticleGenerator* _death_gen1;
	ParticleGenerator* _death_gen2;
	ParticleSystem* _pSys;
};

