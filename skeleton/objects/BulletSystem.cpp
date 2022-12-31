#include "BulletSystem.h"

BulletSystem::BulletSystem(physx::PxScene* scene, physx::PxPhysics* physics, ParticleSystem* pSys) : _scene(scene), _physics(physics), _pSys(pSys)
{
	_force_registry = new ForceRegistry();
	_currAmmo = Bullet;
}

BulletSystem::~BulletSystem()
{
	for (auto f : _force_generators)
		delete f;

	for (auto p : _particles)
		delete p;

	delete _force_registry;
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

void BulletSystem::shoot()
{
	if (_shoot_on_cd[_currAmmo]) return;

	_shootTime[_currAmmo] = glutGet(GLUT_ELAPSED_TIME);
	_shoot_on_cd[_currAmmo] = true;

	float gravity, speed, damping, scale, lifeTime, m;
	int dmg = 0, trailN = 0;
	bool trail = false;
	Color color;
	ParticleShape shape;
	switch (_currAmmo)
	{
	case Bullet:
		m = 1;
		speed = 500;
		gravity = 1;
		damping = 0.2;
		scale = .5;
		lifeTime = 1000;
		color = Color(0.5, 0.5, 0.5, 1);
		shape = Sphere;
		dmg = 2;
		break;
	case CannonBall:
		m = 10;
		speed = 150;
		gravity = 20;
		damping = 0.8;
		scale = 2;
		lifeTime = 2000;
		color = Color(0.2, 0.2, 0.2, 1);
		shape = Sphere;
		dmg = 5;
		break;
	case Laser:
		m = 0.001f;
		speed = 400;
		gravity = 0;
		damping = 0.1;
		scale = .5;
		lifeTime = 2000;
		color = Color(0.9, 0.1, 0.1, 1);
		shape = Capsule;
		dmg = 5;
		trail = true;
		trailN = 50;
		break;
	case Missile:
		m = 3;
		speed = 50;
		gravity = 0;
		damping = 0;
		scale = 1;
		lifeTime = 4000;
		color = Color(0.7, 0.7, 0.7, 1);
		shape = Capsule;
		dmg = 3;
		break;
	case Fireball:
		break;
	default:
		break;
	}
	
	auto cam = GetCamera();
	auto camDir = cam->getDir().getNormalized();

	auto p = new RigidParticle(_scene, _physics, false, cam->getEye() + camDir * 2, camDir * speed, Vector3(0), damping, scale, color, lifeTime, -1, m, shape, Vector3(1), trail, trailN);
	p->setType(Proyectile);
	p->setDamage(dmg);

	auto quat = cam->getTransform().q * PxQuat(PxPi / 2, Vector3(0, 1, 0));

	p->setRotation(quat);

	std::function<void(Particle*)> callback;

	if (_currAmmo == Missile) 
	{
		callback = [p, this](Particle* other) {
			if (other != nullptr && other->getType() != Player && other->getType() != Proyectile)
			{
				p->setAlive(false);
	
				_pSys->generateExplosion(p->getPos());

				auto p1 = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, -5, 0), 0.999f, 0.2, Color(0.1, 0.1, 0.1, 1), 2000, 10, 1);
				p1->deregisterRender();
				auto generator = new GaussianParticleGenerator({ 0, 15, 0 }, { 0, 0, 0 }, { 0.001f, 0.001f, 0.001f }, { 20, 20, 20 }, 1, 70);
				generator->setParticle(p1);

				generator->setPos(p->getPos());
				_pSys->appendParticles(generator->generateParticles());
			}
		};
	}

	else
	{
		callback = [p](Particle* other) {
			if (other != nullptr && other->getType() != Player)
				p->setAlive(false);
		};
	}

	p->setCollisionCallback(callback);

	auto gravityGen = new GravityForceGenerator(Vector3(0, -gravity, 0));
	_force_registry->addRegistry(gravityGen, p);

	//explosion arma
	if (_currAmmo != Laser)
	{
		auto p1 = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, -5, 0), 0.999f, 0.03, Color(1, 0.5, 0, 1), 2000, 0.5, 1);
		p1->deregisterRender();
		auto generator = new GaussianParticleGenerator({ 0, 15, 0 }, { 0, 0, 0 }, { 0.001f, 0.001f, 0.001f }, { 2, 2, 2 }, 1, 20);
		generator->setParticle(p1);

		generator->setPos(p->getPos() + camDir * 2 - Vector3(0, 0.3, 0));
		_pSys->appendParticles(generator->generateParticles());
	}

	_particles.push_back(p);
}

void BulletSystem::changeAmmo(ProyectileType ammo)
{
	_currAmmo = ammo;
}

ProyectileType BulletSystem::getCurrentAmmo()
{
	return _currAmmo;
}

void BulletSystem::killAllParticles()
{
	for (auto p : _particles) p->setAlive(false);
}
