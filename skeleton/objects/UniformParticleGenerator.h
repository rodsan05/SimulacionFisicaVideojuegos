#pragma once
#include "ParticleGenerator.h"
#include <random>

class UniformParticleGenerator :
    public ParticleGenerator
{
public:
	UniformParticleGenerator(Vector3 pos, Vector3 vel, Vector3 pos_w, Vector3 vel_w, double gen_prob, int num,
		Color color = { 1, 1, 1, 1 }, float scale = 1, float lifeTime = -1, float lifeDist = -1,
		float dumping = 0.999f, float m = 0);
	virtual ~UniformParticleGenerator();

	std::list<Particle*> generateParticles() override;

protected:

	Vector3 _vel_width, _pos_width;

	float _scale, _lifeDist, _lifeTime, _m, _dumping;

	Color _color;
};

