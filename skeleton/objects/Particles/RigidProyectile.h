#pragma once
#include "RigidParticle.h"

using Color = Vector4;

enum ProyectileType
{
	Bullet, CannonBall, Laser, Misile, Fireball
};

class RigidProyectile : public RigidParticle
{
public:
	RigidProyectile(physx::PxScene* scene_, physx::PxPhysics* physics_, Vector3 Pos, Vector3 Vel, Vector3 a_, ProyectileType type, ParticleShape shape_ = ParticleShape::Sphere, Vector3 dimensions_ = Vector3(1)) : RigidParticle(scene, physics)
	{
		dimensions = Vector3(1);
		float speed, gravity;
		switch (type)
		{
		case Bullet:
			speed = 150;
			gravity = 1;
			damping = 0.999;
			scale = 1;
			lifeTime = 1000;
			color = Color(0.7, 0.7, 0.7, 1);
			shape = Sphere;
			break;
		case CannonBall:
			speed = 150;
			gravity = 30;
			damping = 0.8;
			scale = 7;
			lifeTime = 2000;
			color = Color(0.2, 0.2, 0.2, 1);
			shape = Sphere;
			break;
		case Laser:
			speed = 10000;
			gravity = 0;
			damping = 0.999;
			scale = 1;
			lifeTime = 2000;
			color = Color(0.9, 0.1, 0.1, 1);
			shape = Capsule;
			break;
		case Misile:
			speed = 50;
			gravity = 0;
			damping = 0.999;
			scale = 5;
			lifeTime = 4000;
			color = Color(0.9, 0.9, 0.9, 1);
			shape = Capsule;
			break;
		case Fireball:
			break;
		default:
			break;
		}

		vel = Vel;
		a = a_;
		pose = physx::PxTransform(Pos);

		initPos = Pos;
		iniTime = glutGet(GLUT_ELAPSED_TIME);

		partShape = createShape(shape, scale, dimensions);

		rb = physics->createRigidDynamic(pose);

		rb->setLinearVelocity(vel);
		rb->setLinearDamping(damping);

		physx::PxRigidBodyExt::updateMassAndInertia(*rb, m / volume, NULL);

		rb->userData = this; //reference to this object used later to update transforms

		rb->attachShape(*partShape);

		scene->addActor(*rb);

		renderItem = new RenderItem(partShape, rb, color);
	}
	~RigidProyectile();

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
	}

	void setTransform(physx::PxTransform& tr)
	{
		pose = tr;
	}

	virtual RigidParticle* clone(ParticleShape _shape = None) const override
	{
		if (shape != None)
			return new RigidParticle(scene, physics, _static, pose.p, vel, a, damping, scale, color, lifeTime, lifeDistance, m, _shape);

		return new RigidParticle(scene, physics, _static, pose.p, vel, a, damping, scale, color, lifeTime, lifeDistance, m, shape);
	}

	virtual void setPos(Vector3 pos) override
	{
		pose.p = pos;
		initPos = pos;

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

protected:

	physx::PxRigidActor* rAct = nullptr;
	physx::PxScene* scene = nullptr;
	physx::PxPhysics* physics = nullptr;

	physx::PxRigidDynamic* rb = nullptr;
};

