#pragma once
#include <PxPhysicsAPI.h>
#include "Particles/RigidParticle.h"

using namespace physx;

class MySimulationEventCallback : public PxSimulationEventCallback
{
public:

	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override {}
	virtual void onWake(PxActor** actors, PxU32 count) override {}

	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override
	{
		// Check if the collision involves the specific object
		PxRigidActor* actor1 = pairHeader.actors[0];
		PxRigidActor* actor2 = pairHeader.actors[1];

		RigidParticle* rigidParticle1 = (RigidParticle*)actor1->userData;
		RigidParticle* rigidParticle2 = (RigidParticle*)actor2->userData;

		if (rigidParticle1->isAlive()) 
			rigidParticle1->onCollisionCallback(rigidParticle2);
		if (rigidParticle2->isAlive()) 
			rigidParticle2->onCollisionCallback(rigidParticle1);
	}

	virtual void onSleep(PxActor** actors, PxU32 count) override {}
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override {}
	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override {}
};