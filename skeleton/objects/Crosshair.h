#pragma once
#include "Particles/Particle.h"

using namespace physx;

class Crosshair : public Particle
{
public:
	Crosshair() : Particle()
	{
		updateAngle();

		partShape = createShape(Prism, 0.015, Vector3(1, 0.3, 0.3));
		partShape2 = createShape(Prism, 0.015, Vector3(1, 0.3, 0.3));

		color = Color(0, 0, 0, 1);

		renderItem = new RenderItem(partShape, &pose, color);

		if (partShape2 != nullptr)
			renderItem2 = new RenderItem(partShape2, &pose2, color);
	}

	virtual ~Crosshair()
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

		auto q1 = PxQuat(PxPi / 4, Vector3(0, 0, 1));

		pose = PxTransform(cam->getTransform().p + camDir * 2, cam->getTransform().q * q1);
		pose2 = PxTransform(cam->getTransform().p + camDir * 2, cam->getTransform().q * q1.getConjugate());
	}

protected:

	PxTransform pose2;
	PxShape* partShape2 = nullptr;

	RenderItem* renderItem2;
};

