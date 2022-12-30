#pragma once
#include "../../core.hpp"
#include "../../RenderUtils.hpp"

using Color = Vector4;

#define NumOfShapes 5;

enum ParticleType
{ Default, Enemy, Proyectile, Player };

enum ParticleShape 
{ Sphere, Cube, Capsule, Plane, Prism, None };

class Particle
{
public:
	Particle();
	Particle(Vector3 Pos, Vector3 Vel, Vector3 a_, float damping_, float scale_, Color color_, float lifeTime_, float lifeDist_, float m_ = 0, ParticleShape shape_ = ParticleShape::Sphere, Vector3 dimensions_ = Vector3(1));
	virtual ~Particle();

	bool isAlive();
	void setAlive(bool set);

	virtual void integrate(double t);

	virtual Particle* clone(ParticleShape _shape = None) const;

	virtual void setPos(Vector3 pos);
	void setVel(Vector3 vel);

	void setLifeTime(float time);
	void setLifeDist(float dist);
	void setColor(Color c);
	virtual void setScale(float s);
	void setMass(float m);

	physx::PxShape* createShape(ParticleShape shape_, float scale_, Vector3 dimensions);

	virtual void onDeath() {};

	void deregisterRender();

	virtual void clearForce();
	virtual void addForce(const Vector3& f);

	void setAffectedByGravity(bool set) { affectedByGravity = set; }
	bool isAffectedByGravity() { return affectedByGravity; }

	float getMass() { return m; };
	Vector3 getVel() { return vel; };
	Vector3 getPos() { return pose.p; };

	void setType(ParticleType t)
	{
		_type = t;
	}

	ParticleType getType()
	{
		return _type;
	}

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

	float getVolume() 
	{
		return volume;
	}

	void setVolume(float vol) 
	{
		volume = vol;
	}

protected:

	void setParticle(Vector3 Pos, Vector3 Vel, Vector3 a_, float damping_, float scale_, Color color, float lifeTime_, float lifeDist_, float m_ = 0);

	Vector3 vel;
	physx::PxTransform pose;
	RenderItem* renderItem;
	ParticleShape shape;
	physx::PxShape* partShape = nullptr;
	Color color;
	Vector3 dimensions;

	Vector3 a;
	float damping;

	float scale;
	float m;
	float volume;
	
	bool alive = true;
	double lifeTime;
	double lifeDistance;

	double iniTime;
	Vector3 initPos;

	Vector3 force;

	bool affectedByGravity = true;
	bool eulerSemiImplicito = true;
	bool _static = false;

	ParticleType _type;
};

