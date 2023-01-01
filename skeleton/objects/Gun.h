#pragma once
#include "Particles/Particle.h"

using namespace physx;

enum ProyectileType
{
	Bullet, CannonBall, Laser, Missile, Firework
};

class Gun : public Particle
{
public:
	Gun(ProyectileType type) : Particle()
	{
		_type = type;

		updateAngle();

		if (_type == Bullet)
		{
			partShape = createShape(Prism, 0.2, Vector3(0.3, 0.3, 1));
			partShape2 = createShape(Prism, 0.2, Vector3(0.3, 0.3, 0.7));

			color = Color(0, 0, 0, 1);
		}
		else if (_type == Laser)
		{
			partShape = createShape(Prism, 0.4, Vector3(0.3, 0.3, 1));
			partShape2 = createShape(Prism, 0.4, Vector3(0.3, 0.3, 0.7));

			color = Color(0, 0, 0, 1);
		}
		else if (_type == Missile)
		{
			partShape = createShape(Prism, 0.6, Vector3(0.3, 0.3, 1));
			partShape2 = createShape(Prism, 0.01, Vector3(0.3, 0.3, 1));;

			color = Color(0, 0, 0, 1);
		}
		else if (_type == Firework)
		{
			partShape = createShape(Prism, 0.6, Vector3(0.3, 0.3, 1));
			partShape2 = createShape(Prism, 0.01, Vector3(0.3, 0.3, 1));;

			color = Color(0.9, 0.1, 0.1, 1);
		}

		renderItem = new RenderItem(partShape, &pose, color);

		if (partShape2 != nullptr)
			renderItem2 = new RenderItem(partShape2, &pose2, color);
	}

	virtual ~Gun()
	{
		if (renderItem2 != nullptr)
			DeregisterRenderItem(renderItem2);
	}

	virtual void deregisterRender() override 
	{
		DeregisterRenderItem(renderItem);
		DeregisterRenderItem(renderItem2);
	}

	void updateAngle()
	{
		auto cam = GetCamera();

		PxVec3 camDir = cam->getDir().getNormalized();

		pose = PxTransform(cam->getTransform().p + camDir * 2 - Vector3(0, 0.5, 0), cam->getTransform().q);
		pose2 = PxTransform(cam->getTransform().p + camDir * 2 - Vector3(0, 0.6, 0), cam->getTransform().q);
	}

protected:

	ProyectileType _type;
	PxTransform pose2;
	PxShape* partShape2 = nullptr;

	RenderItem* renderItem2;
};

