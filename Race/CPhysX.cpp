#include "CPhysX.h"

CPhysX::CPhysX()
{
	gDispatcher = NULL;
	gDispatcher = NULL;
	gFoundation = NULL;
	gPhysics = NULL;
	gScene = NULL;
	gCooking = NULL;
}

void CPhysX::Init()
{
	// инициализация библиотеки
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	if (gFoundation == NULL) {
		printf("PhysX initialization error - PxCreateFoundation\n");
		exit(0);
	};

	// инициализация объекта PhysX для работы с модулем PhysX
	PxTolerancesScale scale;
	scale.length = 1;
	scale.speed = 9.81;
	scale.mass = 10;
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, scale);
	if (gPhysics == NULL) {
		printf("PhysX initialization error - PxCreatePhysics\n");
		exit(0);
	};
	// инициализация объекта cooking
	gCooking = PxCreateCooking(PX_PHYSICS_VERSION,
		*gFoundation, PxCookingParams(scale));
	if (!gCooking) {
		printf("PhysX initialization error - PxCreateCooking\n");
		exit(0);
	};

	// visual debagger
	// Check if PhysX Visual Debuger is running and listening
	if (!gPhysics->getPvdConnectionManager())
	{
		std::cout << "Warning: PhysX Visual Debugger not found running!\n";
		return;
	}
	const char* pvdHostIP = "127.0.0.1";
	int port = 5425;
	unsigned int timeout = 100;
	PxVisualDebuggerConnectionFlags flags =
		PxVisualDebuggerConnectionFlag::eDEBUG
		| PxVisualDebuggerConnectionFlag::ePROFILE
		| PxVisualDebuggerConnectionFlag::eMEMORY;

	// Create connection with PhysX Visual Debugger
	physx::debugger::comm::PvdConnection* conn = physx::PxVisualDebuggerExt::createConnection(
		gPhysics->getPvdConnectionManager(),
		pvdHostIP,
		port,
		timeout,
		flags);
	if (conn)
	{
		std::cout << "Connected to PhysX Visual Debugger!\n";

		gPhysics->getVisualDebugger()->setVisualizeConstraints(true);
	}


	//	инициализация расширений
	if (!PxInitExtensions(*gPhysics)) {
		printf("PhysX initialization error - PxInitExtensions\n");
		exit(0);
	};

	// создание сцены
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	gDispatcher = PxDefaultCpuDispatcherCreate(0);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gScene = gPhysics->createScene(sceneDesc);
}

PxPhysics* CPhysX::GetPhysicsModule()
{
	return gPhysics;
}

PxScene* CPhysX::GetScene()
{
	return gScene;
}

PxCooking* CPhysX::GetCooking()
{
	return gCooking;
}

void CPhysX::AddActor(PxActor* actor)
{
	gScene->addActor(*actor);
}