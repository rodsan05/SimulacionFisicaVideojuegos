#pragma once
#include "../../core.hpp"
#include "../../RenderUtils.hpp"

using Color = Vector4;

enum ParticleShape 
{ Sphere, Cube, Capsule, Plane };

class Particle
{
public:
	Particle();
	Particle(Vector3 Pos, Vector3 Vel, Vector3 a_, float damping_, float scale_, Color color, float lifeTime_, float lifeDist_, float m_ = 0, ParticleShape shape_ = ParticleShape::Sphere);
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

	void clearForce();
	void addForce(const Vector3& f);

	void setAffectedByGravity(bool set) { affectedByGravity = set; }
	bool isAffectedByGravity() { return affectedByGravity; }

	float getMass() { return m; };
	Vector3 getVel() { return vel; };
	Vector3 getPos() { return pose.p; };

	float getHeight() 
	{
		return scale;
	}

	void setStatic(bool s) 
	{
		_static = s;
	}

	bool isStatic() 
	{
		return _static;
	}

protected:

	void setParticle(Vector3 Pos, Vector3 Vel, Vector3 a_, float damping_, float scale_, Color color, float lifeTime_, float lifeDist_, float m_ = 0);

	Vector3 vel;
	physx::PxTransform pose;
	RenderItem* renderItem;

	Color color;

	Vector3 a;
	float damping;

	float scale;
	float m;
	
	bool alive = true;
	double lifeTime;
	double lifeDistance;

	double iniTime;
	Vector3 initPos;

	Vector3 force;

	bool affectedByGravity = true;
	bool eulerSemiImplicito = true;
	bool _static = false;
};

