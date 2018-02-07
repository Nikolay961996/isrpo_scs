#pragma once
#include "PxPhysicsAPI.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "extensions/PxDefaultAllocator.h"
#include <iostream>

using namespace physx;

class CPhysX
{
	// ПЕРЕМЕННЫЕ ДЛЯ ПРЕДСТАВЛЕНИЯ ФИЗИКИ:

	// объект для выделения памяти модулю PhysX (используется реализация по умолчанию)
	PxDefaultAllocator		gAllocator;
	// объект для вывода сообщений об ошибках модуля PhysX (используется реализация по умолчанию)
	PxDefaultErrorCallback	gErrorCallback;
	// объект для распределения заданий по доступным процессорам 
	// (реализация по умолчанию, создается динамически)
	PxDefaultCpuDispatcher*	gDispatcher;

	// объект Foundation для инициализации модуля физики
	PxFoundation*			gFoundation;
	// объект gPhysics для работы с модулем физики
	PxPhysics*				gPhysics;
	// объект для представления одной сцены
	PxScene*				gScene;
	//	объект для обработки сложной геометрии (кукинга)
	PxCooking*				gCooking;
	
public:
	CPhysX();
	void Init();
	PxPhysics* GetPhysicsModule();
	PxScene* GetScene();
	PxCooking* GetCooking();
	void AddActor(PxActor* actor);
};