#include "CPxShape.h"

CPxShape::CPxShape()
{
	gShape.reserve(5);
	gMaterial = NULL;
}
PxPhysics*		CPxShape::gPhysics = NULL;

void CPxShape::SetPhysicsModel(PxPhysics* Px)
{
	gPhysics = Px;
}

void CPxShape::SetMaterial(CPxMaterial* m)
{
	gMaterial = m;
}

void CPxShape::AddBoxShape(float x, float y, float z)
{
	PxShape *shape = gPhysics->createShape(PxBoxGeometry(x, y, z), *gMaterial->GetMaterial());
	gShape.push_back(shape);
}

PxMaterial* CPxShape::GetMaterial()
{
	return gMaterial->GetMaterial();
}

std::vector<PxShape*>* CPxShape::GetShape()
{
	return &gShape;
}