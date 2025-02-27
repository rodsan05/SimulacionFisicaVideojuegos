#pragma once
#include "Particle.h"
#include "../../core.hpp"
#include "../../RenderUtils.hpp"
#include <functional>

using Color = Vector4;

class RigidParticle : public Particle
{
public:
	//default constructor cant exist, minimum is scene and physics
	RigidParticle(physx::PxScene* scene_, physx::PxPhysics* physics_)
	{
		scene = scene_;
		physics = physics_;

		_static = false;
		force = Vector3(0);
		a = Vector3(0);
		vel = Vector3(0);
		pose = physx::PxTransform(Vector3(0));
		scale = 1;

		damping = 0.999f;
		color = Color(1);
		m = 1;
		lifeTime = -1;
		lifeDistance = -1;
		initPos = Vector3(0);
		iniTime = glutGet(GLUT_ELAPSED_TIME);

		shape = Sphere;
		dimensions = Vector3(1);

		partShape = createShape(shape, scale, dimensions);

		rb = physics->createRigidDynamic(pose);

		rb->setLinearVelocity(vel);
		rb->setLinearDamping(damping);

		physx::PxRigidBodyExt::updateMassAndInertia(*rb, m / volume, NULL);

		rb->userData = this; //reference to this object used later to update transforms

		rb->attachShape(*partShape);

		scene->addActor(*rb);

		renderItem = new RenderItem(partShape, rb, color);

		collisionCallback = [](Particle* other) { return; };
	}

	//constructor
	RigidParticle(physx::PxScene* scene_, physx::PxPhysics* physics_, bool static_, Vector3 Pos, Vector3 Vel, Vector3 a_, float damping_, float scale_, Color color_, float lifeTime_ = -1, float lifeDist_ = -1, float m_ = 0, ParticleShape shape_ = ParticleShape::Sphere, Vector3 dimensions_ = Vector3(1), bool trail = false, int trailN = 0)
	{
		scene = scene_;
		physics = physics_;
		_static = static_;

		hasTrail = trail;
		trailNum = trailN;

		//params
		force = Vector3(0);
		scale = scale_;
		vel = Vel;

		damping = damping_;
		a = a_;
		pose = physx::PxTransform(Pos);
		color = color_;

		m = m_;

		lifeTime = lifeTime_;
		lifeDistance = lifeDist_;

		initPos = Pos;
		iniTime = glutGet(GLUT_ELAPSED_TIME);

		shape = shape_;

		dimensions = dimensions_;

		partShape = createShape(shape, scale, dimensions);

		if (_static)
		{
			rAct = physics->createRigidStatic(pose);

			rAct->userData = this; //reference to this object used later to update transforms

			rAct->attachShape(*partShape);

			scene->addActor(*rAct);

			renderItem = new RenderItem(partShape, rAct, color);
		}

		else
		{
			rb = physics->createRigidDynamic(pose);

			rb->setGlobalPose(pose);

			rb->setLinearVelocity(vel);
			rb->setLinearDamping(damping);

			physx::PxRigidBodyExt::updateMassAndInertia(*rb, m / volume, NULL);
	
			rb->userData = this; //reference to this object used later to update transforms

			rb->attachShape(*partShape);

			scene->addActor(*rb);

			renderItem = new RenderItem(partShape, rb, color);
		}

		collisionCallback = [](Particle* other) {};
	}
	virtual ~RigidParticle() 
	{
		if (_static)
		{
			scene->removeActor(*rAct);
		}
		else
		{
			scene->removeActor(*rb);
		}
	}

	void integrate(double t) override 
	{
		if (!_static)
			pose = rb->getGlobalPose();

		//comprueba si debe morir por tiempo
		if (lifeTime != -1)
		{
			auto time = glutGet(GLUT_ELAPSED_TIME);

			auto elapsedTime = time - iniTime;

			if (elapsedTime > lifeTime)
			{
				alive = false;
			}
		}

		//comprueba si debe morir por distancia
		if (lifeDistance != -1)
		{
			auto distance = (pose.p - initPos).magnitudeSquared();

			if (distance > pow(lifeDistance, 2))
			{
				alive = false;
			}
		}

		if (hasTrail)
		{
			auto n = _trail.size();

			if (n >= trailNum)
			{
				auto t = _trail.front();
				_trail.pop();
				delete t;
			}
			_trail.push(cloneParticle());
		}
	}

	void setTransform(physx::PxTransform& tr) 
	{
		pose = tr;
	}

	Particle* cloneParticle(ParticleShape _shape = None)
	{
		Particle* p;
		if (_shape != None)
			p = new Particle(pose.p, vel, a, damping, scale, color, lifeTime, lifeDistance, m, _shape);

		else 
			p = new Particle(pose.p, vel, a, damping, scale, color, lifeTime, lifeDistance, m, shape);
	
		p->setRotation(getRotation());

		return p;
	}

	virtual RigidParticle* clone(ParticleShape _shape = None) const override 
	{
		if (_shape != None)
			return new RigidParticle(scene, physics, _static, pose.p, vel, a, damping, scale, color, lifeTime, lifeDistance, m, _shape);

		return new RigidParticle(scene, physics, _static, pose.p, vel, a, damping, scale, color, lifeTime, lifeDistance, m, shape);
	}

	virtual void setPos(Vector3 pos) override
	{
		pose.p = pos;
		initPos = pos;

		rb->setGlobalPose(pose);
	}

	virtual void setRotation(physx::PxQuat q) 
	{
		pose.q = q;

		rb->setGlobalPose(pose);
	}

	virtual void clearForce() override 
	{
		if (rAct != nullptr) return;

		force *= 0;
		rb->clearForce();
	}
	virtual void addForce(const Vector3& f) override
	{
		if (rAct != nullptr) return;

		force += f;
		rb->addForce(f);
	}

	virtual void setScale(float s) override
	{
		scale = s;

		partShape = createShape(shape, scale, dimensions);

		DeregisterRenderItem(renderItem);
		delete renderItem;
		renderItem = new RenderItem(partShape, &pose, color);

		if (!_static)
		{
			rb->attachShape(*partShape);
			physx::PxRigidBodyExt::updateMassAndInertia(*rb, m / volume, NULL);
		}
	}

	void onCollisionCallback(Particle* other) 
	{
		collisionCallback(other);
	}

	void setCollisionCallback(std::function<void(Particle*)> callback)
	{
		collisionCallback = callback;
	}

protected:

	void setParticle(Vector3 Pos, Vector3 Vel, Vector3 a_, float damping_, float scale_, Color color, float lifeTime_, float lifeDist_, float m_ = 0);

	physx::PxRigidActor* rAct = nullptr;
	physx::PxScene* scene = nullptr;
	physx::PxPhysics* physics = nullptr;

	physx::PxRigidDynamic* rb = nullptr;

	std::function<void(Particle*)> collisionCallback;
};

