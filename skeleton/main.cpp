#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "objects/Particles/Particle.h"
#include "objects/ParticleSystem.h"

#include <iostream>
#include "objects/MyCharacterController.h"
#include "objects/BulletSystem.h"



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
PxControllerManager* manager = nullptr;
ContactReportCallback gContactReportCallback;

ProyectileType proyectile;

std::vector<Particle*> particles;
ParticleSystem* ps;
BulletSystem* bs;
MyCharacterController* characterControl;

bool keys[256];  // array to track the state of each key

void createScene()
{
	auto floor = new RigidParticle(gScene, gPhysics, true, Vector3(0, -5, 0), Vector3(0), Vector3(0), 0, 40, Color(0.9, 0.9, 0.9, 1), -1, -1, 0, Plane);
	particles.push_back(floor);

	auto block = new RigidParticle(gScene, gPhysics, true, Vector3(-40 - 4, -5 + 5, 0), Vector3(0), Vector3(0), 0, 40, Color(0.9, 0.9, 0.9, 1), -1, -1, 0, Prism, Vector3(0.1, 0.2, 1));
	particles.push_back(block);

	block = new RigidParticle(gScene, gPhysics, true, Vector3(40 + 4, -5 + 5, 0), Vector3(0), Vector3(0), 0, 40, Color(0.9, 0.9, 0.9, 1), -1, -1, 0, Prism, Vector3(0.1, 0.2, 1));
	particles.push_back(block);

	block = new RigidParticle(gScene, gPhysics, true, Vector3(0, -5 + 5, -40 - 4), Vector3(0), Vector3(0), 0, 40, Color(0.9, 0.9, 0.9, 1), -1, -1, 0, Prism, Vector3(1.2, 0.2, 0.1));
	particles.push_back(block);

	block = new RigidParticle(gScene, gPhysics, true, Vector3(0, -5 + 5, 40 + 4), Vector3(0), Vector3(0), 0, 40, Color(0.9, 0.9, 0.9, 1), -1, -1, 0, Prism, Vector3(1.2, 0.2, 0.1));
	particles.push_back(block);

	/*block = new RigidParticle(gScene, gPhysics, true, Vector3(4, -5 + 10, 16), Vector3(0), Vector3(0), 0, 10, Color(0.9, 0, 0.8, 1), -1, -1, 0, Prism, Vector3(0.5, 1, 0.5));
	particles.push_back(block);

	block = new RigidParticle(gScene, gPhysics, true, Vector3(17, -5 + 4, 5), Vector3(0), Vector3(0), 0, 4, Color(0, 0.8, 0.9, 1), -1, -1, 0, Prism, Vector3(0.75, 1, 0.25));
	particles.push_back(block);

	block = new RigidParticle(gScene, gPhysics, true, Vector3(-12, -5 + 8, -7), Vector3(0), Vector3(0), 0, 8, Color(0.9, 0.8, 0, 1), -1, -1, 0, Prism, Vector3(0.65, 1, 0.35));
	particles.push_back(block);*/
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
	//sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(4);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	//sceneDesc.gpuDispatcher = gCudaContextManager->getGpuDispatcher();
	/*sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
	sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;*/
	sceneDesc.simulationEventCallback = &gContactReportCallback;

	gScene = gPhysics->createScene(sceneDesc);
	gScene->setFlag(PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);

	manager = PxCreateControllerManager(*gScene);

	characterControl = new MyCharacterController(manager, GetCamera(), Vector3(0, 10, 0), Vector3(0, -10, 0), 5, 50000, gPhysics->createMaterial(0.5f, 0.5f, 0.5f));
	ps = new ParticleSystem(gScene, gPhysics, 30);
	bs = new BulletSystem(gScene, gPhysics);

	createScene();
}

void handleKeyboard()
{
	Camera* cam = GetCamera();

	if (keys['q'])
		ps->clearAllGenerators();
	else if (keys['b'])
		bs->shoot(Bullet);
	else if (keys['1'])
		ps->createParticleGenerator(Hormigas);
	else if (keys['2'])
		ps->createParticleGenerator(Cubo);
	else if (keys['3'])
		ps->createParticleGenerator(Sangre);
	else if (keys['4'])
		ps->createParticleGenerator(Humo);
	else if (keys['5'])
		ps->createParticleGenerator(RandomMass);
	else if (keys['6'])
		ps->createParticleGenerator(RigidDemo);
	else if (keys['c'])
		ps->generateFireworksSystem(Circle);
	else if (keys['e'])
		ps->generateFireworksSystem(SphereFirework);
	else if (keys['x'])
		ps->generateFireworksSystem(Star);
	else if (keys['g'])
		ps->generate();
	else if (keys['u'])
		ps->generateGravity();
	else if (keys['j'])
		ps->generateReverseGravity();
	else if (keys['i'])
		ps->generateWind();
	else if (keys['o'])
		ps->generateWhirlwind();
	else if (keys['p'])
		ps->generateExplosion();
	else if (keys['y'])
		ps->clearForces();
	else if (keys['m'])
		ps->killAllParticles();
	else if (keys['h'])
		ps->generateSpringDemo();
	else if (keys['f'])
		ps->generateFloatingDemo();
	else if (keys['z'])
		ps->generateSlinky();
	else if (keys['r'])
		ps->generateRopeDemo();
	else if (keys['+'])
		ps->incrementAllSprings(1);
	else if (keys['-'])
		ps->decrementAllSprings(1);

	if (keys['w'])
		characterControl->setInputDirX(1);

	else if (keys['s'])
		characterControl->setInputDirX(-1);

	if (keys['a'])
		characterControl->setInputDirY(-1);

	else if (keys['d'])
		characterControl->setInputDirY(1);

	if (keys[' '])
		characterControl->jump();
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	handleKeyboard();

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
	bs->update(t);

	if (characterControl->isJumping())
		characterControl->addForce(Vector3(0, -100, 0));
	characterControl->integrate(t);
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

	keys[tolower(key)] = true;
}

void keyRelease(unsigned char key)
{
	keys[tolower(key)] = false;
}

void mousePress(int button, int state, int x, int y)
{
	if (button == 0 && state == 1) 
	{
		bs->shoot(Bullet);
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char* const*)
{
	ShowCursor(FALSE);
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