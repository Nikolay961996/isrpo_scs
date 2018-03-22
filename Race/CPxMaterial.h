#pragma once
#include "PxPhysicsAPI.h"

using namespace physx;

class CPxMaterial
{
	// объект gPhysics для работы с модулем физики
	static PxPhysics*		gPhysics;
	// объект для представления материала (задает упругость, коэффициент трения и т.д.)
	PxMaterial*		gMaterial;

	float friction;
	float slip;
	float elasticity;

public:
	CPxMaterial();
	// выбор модуля физики
	static void SetPhysicsModel(PxPhysics* Px);
	// создание маткриала
	void SetMaterial(float f, float s, float e);
	// выдать материал
	PxMaterial* GetMaterial();
};