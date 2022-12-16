#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "objects/Particles/Particle.h"
#include "objects/Particles/Proyectile.h"
#include "objects/ParticleSystem.h"

#include <iostream>



using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;


PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxCudaContextManager* gCudaContextManager;
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;

ProyectileType proyectile;

std::vector<Particle*> particles;
ParticleSystem* ps;

void createScene()
{
	auto floor = new RigidParticle(gScene, gPhysics, true, Vector3(0, -5, 0), Vector3(0), Vector3(0), 0, 40, Color(0.9, 0.9, 0.9, 1), -1, -1, 0, Plane);
	particles.push_back(floor);

	auto block = new RigidParticle(gScene, gPhysics, true, Vector3(4, -5 + 10, 16), Vector3(0), Vector3(0), 0, 10, Color(0.9, 0, 0.8, 1), -1, -1, 0, Prism, Vector3(0.5, 1, 0.5));
	particles.push_back(block);

	block = new RigidParticle(gScene, gPhysics, true, Vector3(17, -5 + 4, 5), Vector3(0), Vector3(0), 0, 4, Color(0, 0.8, 0.9, 1), -1, -1, 0, Prism, Vector3(0.75, 1, 0.25));
	particles.push_back(block);

	block = new RigidParticle(gScene, gPhysics, true, Vector3(-12, -5 + 8, -7), Vector3(0), Vector3(0), 0, 8, Color(0.9, 0.8, 0, 1), -1, -1, 0, Prism, Vector3(0.65, 1, 0.35));
	particles.push_back(block);

	block = new RigidParticle(gScene, gPhysics, true, Vector3(-40 - 4, -5 + 5, 0), Vector3(0), Vector3(0), 0, 40, Color(0.9, 0.9, 0.9, 1), -1, -1, 0, Prism, Vector3(0.1, 0.2, 1));
	particles.push_back(block);

	block = new RigidParticle(gScene, gPhysics, true, Vector3(40 + 4, -5 + 5, 0), Vector3(0), Vector3(0), 0, 40, Color(0.9, 0.9, 0.9, 1), -1, -1, 0, Prism, Vector3(0.1, 0.2, 1));
	particles.push_back(block);

	block = new RigidParticle(gScene, gPhysics, true, Vector3(0, -5 + 5, -40 - 4), Vector3(0), Vector3(0), 0, 40, Color(0.9, 0.9, 0.9, 1), -1, -1, 0, Prism, Vector3(1.2, 0.2, 0.1));
	particles.push_back(block);

	block = new RigidParticle(gScene, gPhysics, true, Vector3(0, -5 + 5, 40 + 4), Vector3(0), Vector3(0), 0, 40, Color(0.9, 0.9, 0.9, 1), -1, -1, 0, Prism, Vector3(1.2, 0.2, 0.1));
	particles.push_back(block);
}

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
	PxCudaContextManagerDesc cudaContextManagerDesc;

	//gCudaContextManager = PxCreateCudaContextManager(*gFoundation, cudaContextManagerDesc);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(4);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	//sceneDesc.gpuDispatcher = gCudaContextManager->getGpuDispatcher();
	/*sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
	sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;*/
	sceneDesc.simulationEventCallback = &gContactReportCallback;

	gScene = gPhysics->createScene(sceneDesc);
	gScene->setFlag(PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);

	proyectile = ProyectileType::Bullet;

	ps = new ParticleSystem(gScene, gPhysics, 30);

	createScene();
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
	case '6':
		//proyectile = Misile;
		ps->createParticleGenerator(RigidDemo);
		break;
	case 'C':
		ps->generateFireworksSystem(Circle);
		break;
	case 'E':
		ps->generateFireworksSystem(SphereFirework);
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
	case 'H':
		ps->generateSpringDemo();
		break;
	case 'F':
		ps->generateFloatingDemo();
		break;
	case 'Z':
		ps->generateSlinky();
		break;
	case 'R':
		ps->generateRopeDemo();
		break;
	case '+':
		ps->incrementAllSprings(1);
	case '-':
		ps->decrementAllSprings(1);
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