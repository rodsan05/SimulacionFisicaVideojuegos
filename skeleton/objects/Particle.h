#pragma once
#include "../core.hpp"
#include "../RenderUtils.hpp"

using Color = Vector4;

class Particle
{
public:
	Particle();
	Particle(Vector3 Pos, Vector3 Vel, Vector3 a_, float dumping_, float scale_, Color color, float lifeTime_, float lifeDist_, float m_ = 0);
	~Particle();

	bool isAlive();
	void setAlive(bool set);

	void integrate(double t);

protected:

	void setParticle(Vector3 Pos, Vector3 Vel, Vector3 a_, float dumping_, float scale_, Color color, float lifeTime_, float lifeDist_, float m_ = 0);

	Vector3 vel;
	physx::PxTransform pose;
	RenderItem* renderItem;

	Color color;

	Vector3 a;
	float dumping;

	float m;
	
	bool alive = true;
	double lifeTime;
	double lifeDistance;

	double iniTime;
	Vector3 initPos;
};

