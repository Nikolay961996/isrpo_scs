#pragma once
#include "PxPhysicsAPI.h"
#include "CPxMaterial.h"
#include <vector>

using namespace physx;

class CPxShape
{
	// объект gPhysics для работы с модулем физики
	static PxPhysics*		gPhysics;
	CPxMaterial*			gMaterial;
	std::vector<PxShape*>	gShape;

public:
	CPxShape();
	// выбор модуля физики
	static void SetPhysicsModel(PxPhysics* Px);
	// выбор маткериала
	void SetMaterial(CPxMaterial* m);
	// добавить форму паралеллепипед
	void AddBoxShape(float x, float y, float z);
	// выдать материал
	PxMaterial* GetMaterial();
	std::vector<PxShape*>* GetShape();
};

