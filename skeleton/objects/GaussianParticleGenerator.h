#pragma once
#include "ParticleGenerator.h"
#include <random>

class GaussianParticleGenerator : public ParticleGenerator
{
public:
	GaussianParticleGenerator(Vector3 pos, Vector3 vel, Vector3 dev_pos, Vector3 dev_vel, double gen_prob, int num, 
		Color color = { 1, 1, 1, 1 }, float scale = 1, float lifeTime = -1, float lifeDist = -1,
		float damping = 0.999f, float m = 0);
	virtual ~GaussianParticleGenerator();

	std::list<Particle*> generateParticles() override;

protected:

	Vector3 std_dev_pos, std_dev_vel;

	float _scale, _lifeDist, _lifeTime, _m, _damping;

	Color _color;

	double std_dev_t;
};

