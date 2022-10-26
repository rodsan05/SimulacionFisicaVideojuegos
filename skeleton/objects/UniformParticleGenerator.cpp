#include "UniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(Vector3 pos, Vector3 vel, Vector3 pos_w, Vector3 vel_w, double gen_prob, int num, Color color, float scale, float lifeTime, float lifeDist, float dumping, float m)
{
	_mean_pos = pos;
	_mean_vel = vel;
	_pos_width = pos_w;
	_vel_width = vel_w;

	_generation_probability = gen_prob;
	_num_particles = num;

	_scale = scale;
	_m = m;
	_lifeDist = lifeDist;
	_lifeTime = lifeTime;
	_dumping = dumping;

	_color = color;
}

UniformParticleGenerator::~UniformParticleGenerator()
{
}

std::list<Particle*> UniformParticleGenerator::generateParticles()
{
	std::list<Particle*> particles = std::list<Particle*>();

	std::random_device rd{};
	std::mt19937 gen{ rd() };

	std::uniform_int_distribution<> posX(0, _pos_width.x);
	std::uniform_int_distribution<> posY(0, _pos_width.y);
	std::uniform_int_distribution<> posZ(0, _pos_width.z);

	std::uniform_int_distribution<> velX(0, _vel_width.x);
	std::uniform_int_distribution<> velY(0, _vel_width.y);
	std::uniform_int_distribution<> velZ(0, _vel_width.z);

	for (int i = 0; i < _num_particles; i++)
	{
		int genProbRandom = rand() % 1;

		if (genProbRandom < _generation_probability)
		{
			Vector3 pos(posX(gen), posY(gen), posZ(gen));
			Vector3 vel(velX(gen), velY(gen), velZ(gen));

			Particle* p;

			if (_model == nullptr) p = new Particle(pos + _mean_pos, vel + _mean_vel, Vector3(0, -10.0f, 0), _dumping, _scale, _color, _lifeTime, _lifeDist, _m);

			else
			{
				p = _model->clone();
				p->setPos(pos + _mean_pos);
				p->setVel(vel + _mean_vel);
			}

			particles.push_back(p);
		}
	}

	return particles;
}
