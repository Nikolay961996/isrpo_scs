#include "CPxMaterial.h"

CPxMaterial::CPxMaterial()
{
	gMaterial = NULL;
}
PxPhysics*	CPxMaterial::gPhysics = NULL;

void CPxMaterial::SetPhysicsModel(PxPhysics* Px)
{
	gPhysics = Px;
}

void CPxMaterial::SetMaterial(float f, float s, float e)
{
	friction = f;
	slip = s;
	elasticity = e;

	gMaterial = gPhysics->createMaterial(friction, slip, elasticity);
}

PxMaterial* CPxMaterial::GetMaterial()
{
	return gMaterial;
}