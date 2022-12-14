#include "RigidGaussianParticleGen.h"

RigidGaussianParticleGen::RigidGaussianParticleGen(physx::PxScene* scene_, physx::PxPhysics* physics_, Vector3 pos, Vector3 vel, Vector3 dev_pos, Vector3 dev_vel, double gen_prob, int num, bool randomMass, Color color, float scale, float lifeTime, float lifeDist, float damping, float m, ParticleShape partShape_)
{
	partShape = partShape_;

	scene = scene_;
	physics = physics_;

	_mean_pos = pos;
	_mean_vel = vel;
	std_dev_pos = dev_pos;
	std_dev_vel = dev_vel;

	_generation_probability = gen_prob;
	_num_particles = num;

	_scale = scale;
	_m = m;
	_lifeDist = lifeDist;
	_lifeTime = lifeTime;
	_damping = damping;

	_color = color;

	_randomMass = randomMass;
}

RigidGaussianParticleGen::~RigidGaussianParticleGen()
{
}

std::list<RigidParticle*> RigidGaussianParticleGen::generateParticles()
{
	std::list<RigidParticle*> particles = std::list<RigidParticle*>();

	std::random_device rd{};
	std::mt19937 gen{ rd() };

	std::normal_distribution<> posX(_mean_pos.x, std_dev_pos.x);
	std::normal_distribution<> posY(_mean_pos.y, std_dev_pos.y);
	std::normal_distribution<> posZ(_mean_pos.z, std_dev_pos.z);

	std::normal_distribution<> velX(_mean_vel.x, std_dev_vel.x);
	std::normal_distribution<> velY(_mean_vel.y, std_dev_vel.y);
	std::normal_distribution<> velZ(_mean_vel.z, std_dev_vel.z);

	std::uniform_real_distribution<> genProbRandom(0, 1);

	for (int i = 0; i < _num_particles; i++)
	{
		if (genProbRandom(gen) < _generation_probability)
		{
			Vector3 pos(posX(gen), posY(gen), posZ(gen));
			Vector3 vel(velX(gen), velY(gen), velZ(gen));

			RigidParticle* p;

			if (_model == nullptr) p = new RigidParticle(scene, physics, false, pos + _mean_pos, vel + _mean_vel, Vector3(0, -10.0f, 0), _damping, 
				_scale, _color, _lifeTime, _lifeDist, _m, partShape);

			else
			{
				p = _model->clone();
				p->setPos(pos + _mean_pos);
				p->setVel(vel + _mean_vel);

				if (_randomMass)
				{
					auto m = rand() % 1000 + 100;

					p->setMass(m / 10.0f);
					p->setScale(m / 500.0f);
				}
			}

			particles.push_back(p);
		}
	}

	return particles;
}
