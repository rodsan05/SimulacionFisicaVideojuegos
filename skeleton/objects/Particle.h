#pragma once
#include "../core.hpp"
#include "../RenderUtils.hpp"
class Particle
{
public:
	Particle(Vector3 Pos, Vector3 Vel, Vector3 a_, float dumping_);
	~Particle();

	void integrate(double t);

private:
	Vector3 vel;
	physx::PxTransform pose;
	RenderItem* renderItem;

	Vector4 color;
	int colorFactor;

	Vector3 a;
	float dumping;
};

