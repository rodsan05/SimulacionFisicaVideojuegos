#include "Enemy.h"

EnemyClass::EnemyClass(physx::PxScene* scene, physx::PxPhysics* physics, MyCharacterController* player, Vector3 pos, float size, float speed, ParticleSystem* pSys) :
	RigidParticle(scene, physics, false, pos, Vector3(0), Vector3(0), 0.999f, size, Color(0.2f, 0.2f, 0.2f, 1), -1, -1, 2, Prism, Vector3(0.5, 1, 0.5))
{
	_speed = speed;
	_player = player;
	
	auto p = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 0.2, Color(0.7, 0, 0, 1), 2000, 10, 1, Sphere);
	p->deregisterRender();
	_death_gen1 = new GaussianParticleGenerator(pos, { 0, 0, 0 }, { 0.001f, 0.001f, 0.001f }, { 20, 20, 20 }, 1, 30);
	_death_gen1->setParticle(p);

	auto p2 = new RigidParticle(scene, physics, false, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 0.4, Color(0.3, 0, 0, 1), -1, -1, 2, Cube);
	p2->deregisterRender();
	_death_gen2 = new GaussianParticleGenerator(pos, { 0, 0, 0 }, { 0.001f, 0.001f, 0.001f }, { 20, 20, 20 }, 1, 30, true);
	_death_gen2->setParticle(p2);

	_pSys = pSys;
}

EnemyClass::~EnemyClass()
{
	_death_gen1->setPos(rb->getGlobalPose().p);
	_death_gen2->setPos(rb->getGlobalPose().p);

	_pSys->appendParticles(_death_gen1->generateParticles());
	_pSys->appendParticles(_death_gen2->generateParticles());

	delete _death_gen1;
	delete _death_gen2;
}

void EnemyClass::integrate(double t)
{
	pose = rb->getGlobalPose();

	partShape->setLocalPose(physx::PxTransform(Vector3(0)));

	auto playerPos = _player->getPos();

	auto dir = physx::PxVec2(playerPos.x - pose.p.x, playerPos.z - pose.p.z).getNormalized();
	dir *= _speed;

	auto vel = rb->getLinearVelocity();
	rb->setLinearVelocity(Vector3(dir.x, vel.y, dir.y));
}
