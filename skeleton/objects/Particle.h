#pragma once
#include "../core.hpp"
#include "../RenderUtils.hpp"

#include <functional>

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

	virtual Particle* clone() const;

	void setPos(Vector3 pos);
	void setVel(Vector3 vel);

	void setLifeTime(float time);
	void setLifeDist(float dist);
	void setColor(Color c);
	void setScale(float s);
	void setMass(float m);

	virtual void onDeath() {};

	void deregisterRender();

protected:

	void setParticle(Vector3 Pos, Vector3 Vel, Vector3 a_, float dumping_, float scale_, Color color, float lifeTime_, float lifeDist_, float m_ = 0);

	Vector3 vel;
	physx::PxTransform pose;
	RenderItem* renderItem;

	Color color;

	Vector3 a;
	float dumping;

	float scale;
	float m;
	
	bool alive = true;
	double lifeTime;
	double lifeDistance;

	double iniTime;
	Vector3 initPos;
};

