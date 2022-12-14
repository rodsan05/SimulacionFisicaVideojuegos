#pragma once
#include "Particle.h"
#include "../../core.hpp"
#include "../../RenderUtils.hpp"

using Color = Vector4;

class RigidParticle : public Particle
{
public:
	//default constructor cant exist, minimum is scene and physics
	RigidParticle(physx::PxScene* scene_, physx::PxPhysics* physics_)
	{
	}

	//constructor
	RigidParticle(physx::PxScene* scene_, physx::PxPhysics* physics_, bool static_, Vector3 Pos, Vector3 Vel, Vector3 a_, float damping_, float scale_, Color color_, float lifeTime_ = -1, float lifeDist_ = -1, float m_ = 0, ParticleShape shape_ = ParticleShape::Sphere)
	{
		scene = scene_;
		physics = physics_;
		_static = static_;

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

		//shape
		auto partShape = CreateShape(physx::PxSphereGeometry(scale_));
		volume = (4 / 3) * physx::PxPi * pow(scale_, 3);

		switch (shape_)
		{
		case Sphere:
			break;
		case Cube:
			partShape = CreateShape(physx::PxBoxGeometry(scale_, scale_, scale_));
			volume = pow(scale_, 3);
			break;
		case Capsule:
			partShape = CreateShape(physx::PxCapsuleGeometry(scale_ / 4, scale_));
			volume = (physx::PxPi * pow(scale_ / 4, 2) * scale_ * 2) + ((4 / 3) * physx::PxPi * pow(scale / 4, 3)); //cilinder area + sphere area
			break;
		case Plane:
			partShape = CreateShape(physx::PxBoxGeometry(scale_, 0.5, scale_));
			volume = pow(scale_, 2) * 0.5;
			break;
		default:
			break;
		}

		if (_static)
		{
			rb = physics->createRigidStatic(pose);
		}

		else
		{
			rb = physics->createRigidDynamic(pose);

			auto rb2 = static_cast<physx::PxRigidBody*>(rb);

			physx::PxRigidBodyExt::updateMassAndInertia(*rb2, m / volume, NULL);
		}
	
		rb->attachShape(*partShape);
		
		scene->addActor(*rb);

		renderItem = new RenderItem(partShape, rb, color);
	}
	~RigidParticle();


	virtual RigidParticle* clone() const override 
	{
		return new RigidParticle(scene, physics, _static, pose.p, vel, a, damping, scale, color, lifeTime, lifeDistance, m, shape);
	}

protected:

	void setParticle(Vector3 Pos, Vector3 Vel, Vector3 a_, float damping_, float scale_, Color color, float lifeTime_, float lifeDist_, float m_ = 0);

	physx::PxRigidActor* rb;
	physx::PxScene* scene;
	physx::PxPhysics* physics;
};

