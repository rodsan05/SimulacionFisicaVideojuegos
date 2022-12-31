#include "Enemy.h"

EnemyClass::EnemyClass(physx::PxScene* scene, physx::PxPhysics* physics, MyCharacterController* player, Vector3 pos, float size, float speed, int hp, ParticleSystem* pSys) :
	RigidParticle(scene, physics, false, pos, Vector3(0), Vector3(0), 0.999f, size, Color(0.2f, 0.2f, 0.2f, 1), -1, -1, 1.5, Capsule, Vector3(0.5, 1, 0.5))
{
	_speed = speed;
	_player = player;
	_hp = hp;

	auto q = PxQuat(PxHalfPi, PxVec3(0, 0, 1));

	pose = rb->getGlobalPose();
	pose.q = q;

	rb->setGlobalPose(pose);
	
	auto p = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 0.2, Color(0.7, 0, 0, 1), 2000, 10, 1, Sphere);
	p->deregisterRender();
	_death_gen1 = new GaussianParticleGenerator(pos, { 0, 0, 0 }, { 0.001f, 0.001f, 0.001f }, { 20, 20, 20 }, 1, 30);
	_death_gen1->setParticle(p);

	auto p2 = new RigidParticle(scene, physics, false, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), 0.999f, 0.4, Color(0.3, 0, 0, 1), 5000, -1, 2, Cube);
	p2->deregisterRender();
	_death_gen2 = new GaussianParticleGenerator(pos, { 0, 0, 0 }, { 0.001f, 0.001f, 0.001f }, { 20, 20, 20 }, 1, 30, true);
	_death_gen2->setParticle(p2);

	_pSys = pSys;


	std::function<void(Particle*)> callback = [this](Particle* other) {
		if (other != nullptr && other->getType() == Proyectile)
		{
			_hp -= other->getDamage();

			if (_hp <= 0)
				setAlive(false);
		}
	};
	setCollisionCallback(callback);
}

EnemyClass::~EnemyClass()
{
	_death_gen1->setPos(rb->getGlobalPose().p);
	_death_gen2->setPos(rb->getGlobalPose().p);

	_pSys->appendParticles(_death_gen1->generateParticles());
	_pSys->appendParticlesWithGravity(_death_gen2->generateParticles(), -1000);

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
