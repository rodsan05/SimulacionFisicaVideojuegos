#include "BulletSystem.h"

BulletSystem::BulletSystem(physx::PxScene* scene, physx::PxPhysics* physics) : _scene(scene), _physics(physics)
{
	_force_registry = new ForceRegistry();
}

BulletSystem::~BulletSystem()
{
}

void BulletSystem::update(double t)
{
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

	//cooldown
	auto currTime = glutGet(GLUT_ELAPSED_TIME);

	for (int i = 0; i < 4; i++)
	{
		if (_shoot_on_cd[i] && currTime - _shootTime[i] > SHOOT_CD[i])
		{
			_shoot_on_cd[i] = false;
		}
	}
}

void BulletSystem::shoot(ProyectileType ammo)
{
	if (_shoot_on_cd[ammo]) return;

	_shootTime[ammo] = glutGet(GLUT_ELAPSED_TIME);
	_shoot_on_cd[ammo] = true;

	float gravity, speed, damping, scale, lifeTime, m;
	Color color;
	ParticleShape shape;
	switch (ammo)
	{
	case Bullet:
		m = 1;
		speed = 500;
		gravity = 1;
		damping = 0.999;
		scale = .2;
		lifeTime = 1000;
		color = Color(0.5, 0.5, 0.5, 1);
		shape = Sphere;
		break;
	case CannonBall:
		m = 10;
		speed = 150;
		gravity = 30;
		damping = 0.2;
		scale = 2;
		lifeTime = 2000;
		color = Color(0.2, 0.2, 0.2, 1);
		shape = Sphere;
		break;
	case Laser:
		m = 0.001f;
		speed = 1000;
		gravity = 0;
		damping = 0.999;
		scale = .5;
		lifeTime = 2000;
		color = Color(0.9, 0.1, 0.1, 1);
		shape = Capsule;
		break;
	case Misile:
		m = 3;
		speed = 200;
		gravity = 0;
		damping = 0.999;
		scale = 2;
		lifeTime = 4000;
		color = Color(0.9, 0.9, 0.9, 1);
		shape = Capsule;
		break;
	case Fireball:
		break;
	default:
		break;
	}
	
	auto cam = GetCamera();
	auto p = new RigidParticle(_scene, _physics, false, cam->getEye() + cam->getDir().getNormalized(), cam->getDir().getNormalized() * speed, Vector3(0), damping, scale, color, lifeTime, -1, m, shape);
	
	std::function<void()> callback = [p](){
		p->setAlive(false);
	};
	p->setCollisionCallback(callback);

	auto gravityGen = new GravityForceGenerator(Vector3(0, -gravity, 0));
	_force_registry->addRegistry(gravityGen, p);

	_particles.push_back(p);
}

void BulletSystem::killAllParticles()
{
	for (auto p : _particles) p->setAlive(false);
}
