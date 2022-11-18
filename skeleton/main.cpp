#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "objects/Particle.h"
#include "objects/Proyectile.h"
#include "objects/ParticleSystem.h"

#include <iostream>



using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;


PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;

ProyectileType proyectile;

std::vector<Particle*> particles;
ParticleSystem* ps;


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	proyectile = ProyectileType::Bullet;

	ps = new ParticleSystem();
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	for (int i = 0; i < particles.size(); i++)
	{
		auto p = particles[i];

		if (p->isAlive())
			p->integrate(t);

		else
		{
			delete p;
			particles.erase(particles.begin() + i);
		}
	}

	ps->update(t);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();

	for (auto p : particles)
		delete p;
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	Camera* cam = GetCamera();

	switch (toupper(key))
	{
	case 'Q':
		ps->clearAllGenerators();
		break;
	case 'B':
		particles.push_back(new Proyectile(proyectile, cam->getTransform().p + cam->getDir() * 5, cam->getDir()));
		break;
	case '1':
		//proyectile = Bullet;
		ps->createParticleGenerator(Hormigas);
		break;
	case '2':
		//proyectile = CannonBall;
		ps->createParticleGenerator(Cubo);
		break;
	case '3':
		//proyectile = Laser;
		ps->createParticleGenerator(Sangre);
		break;
	case '4':
		//proyectile = Misile;
		ps->createParticleGenerator(Humo);
		break;
	case '5':
		//proyectile = Misile;
		ps->createParticleGenerator(RandomMass);
		break;
	case 'C':
		ps->generateFireworksSystem(Circle);
		break;
	case 'E':
		ps->generateFireworksSystem(Sphere);
		break;
	case 'X':
		ps->generateFireworksSystem(Star);
		break;
	case 'G':
		ps->generate();
		break;
	case 'U':
		ps->generateGravity();
		break;
	case 'J':
		ps->generateReverseGravity();
		break;
	case 'I':
		ps->generateWind();
		break;
	case 'O':
		ps->generateWhirlwind();
		break;
	case 'P':
		ps->generateExplosion();
		break;
	case 'Y':
		ps->clearForces();
		break;
	case 'M':
		ps->killAllParticles();
		break;
	case ' ':
	{
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char* const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i < frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}