#pragma once
#include "Particles/Particle.h"
#include "PxPhysicsAPI.h"

using namespace physx;

class MyCharacterController : public Particle
{
public:
	MyCharacterController(PxControllerManager* manager, Camera* camera, Vector3 pos, Vector3 gravity, float speed, PxMaterial* material)
	{
		_cam = camera;
		_gravity = gravity;
		_speed = speed;
		_inputDir = Vector3(0);

		force = Vector3(0);
		vel = Vector3(0);
		a = Vector3(0);
		damping = 0.15f;
		m = 1;
		_maxSpeed = 30;

		PxCapsuleControllerDesc desc;
		desc.radius = 1.0f;
		desc.height = 2;
		desc.position = PxExtendedVec3(pos.x, pos.y, pos.z);	
		desc.material = material;
		desc.maxJumpHeight = 20;
		_controller = manager->createController(desc);

		_type = Player;
	}

	~MyCharacterController() 
	{
	}

	void integrate(double t) override
	{
		if (_static) return;

		auto p = _controller->getPosition();
		pose.p = Vector3(p.x, p.y, p.z);

		Vector3 dir = Vector3(0);

		PxVec3 viewY = _cam->getDir().cross(PxVec3(0, 1, 0)).getNormalized();
		PxVec3 viewX = -viewY.cross(PxVec3(0, 1, 0)).getNormalized();

		dir = viewX * _inputDir.x + viewY * _inputDir.y;

		vel += dir.getNormalized() * _speed;

		PxVec2 horizonalVel = PxVec2(vel.x, vel.z);
		if (horizonalVel.magnitude() > _maxSpeed)
			horizonalVel = horizonalVel.getNormalized() * _maxSpeed;

		vel = Vector3(horizonalVel.x, vel.y, horizonalVel.y);

		float inverse_mass = 1.0 / m;

		if (inverse_mass <= 0.0f) return;

		Vector3 totalAccel = a;
		totalAccel += force * inverse_mass;

		//update linear vel
		vel += totalAccel * t;
		//damping
		vel *= powf(damping, t);

		clearForce();

		auto desp = vel * t;

		PxControllerCollisionFlags flags = _controller->move(desp, 0.0f, 0.0f, NULL);

		auto pos = _controller->getPosition();
		_cam->setEye(Vector3(pos.x, pos.y + 2.0f, pos.z));

		_inputDir = Vector3(0);
	}

	void setGravity(Vector3 g) 
	{
		_gravity = g;
	}

	void setInputDirX(int d) 
	{
		_inputDir.x = d;
	}
	void setInputDirY(int d)
	{
		_inputDir.y = d;
	}

protected:

	PxController* _controller;
	Camera* _cam;

	Vector3 _gravity;
	Vector3 _inputDir;
	float _speed;
	float _maxSpeed;
};

