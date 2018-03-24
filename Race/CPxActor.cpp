#include "CPxActor.h"

CPxActor::CPxActor()
{
	StaticActor = NULL;
	DynamicActor = NULL;
	gShape = NULL;
}
PxPhysics*		CPxActor::gPhysics = NULL;
PxScene*		CPxActor::gScene = NULL;

void CPxActor::SetPhysicsModel(PxPhysics* Px)
{
	gPhysics = Px;
}

void CPxActor::SetShape(CPxShape* s)
{
	gShape = s;

}

void CPxActor::CreatStaticPlane(unsigned char nx, unsigned char ny, unsigned char nz, unsigned char distance)
{
	if (StaticActor == NULL && DynamicActor == NULL)
		StaticActor = PxCreatePlane(*gPhysics, PxPlane(nx, ny, nz, distance), *gShape->GetMaterial());
	else
		std::cout << "This actor was inited only!!! \n";
}

PxActor* CPxActor::GetActor()
{
	if (DynamicActor != NULL)
		return DynamicActor;
	else
		return StaticActor;
}

void CPxActor::CreatDymaicKub(float x, float y, float z)
{
	if (StaticActor == NULL && DynamicActor == NULL)
	{
		PxTransform BoxPosition(PxVec3(x, y, z));
		DynamicActor = gPhysics->createRigidDynamic(BoxPosition);
	}
	else
		std::cout << "This actor was inited only!!! \n";

	std::vector<PxShape*> temp = *gShape->GetShape();
	for (int i = 0; i < temp.size(); i++)
		DynamicActor->attachShape(*temp[i]);
}

void CPxActor::SetMass(float m)
{
	mass = m;

	if (DynamicActor != NULL)
		DynamicActor->setMass(mass);
	else
		std::cout << "The static actor dont have mass \n";
}
const float* CPxActor::GetTransformMatrix()
{
	if (DynamicActor != NULL)
	{
		// получение позиции одного актора
		PxTransform	t = DynamicActor->getGlobalPose();
		// преобразование к матричной форме
		PxMat44	m = PxMat44(t);
		// получение указателя на значения матрицы равзернутой по столбцам
		const float	*tm = m.front();
		return tm;
	}
	else
	{
		std::cout << "The static actor dont have TransformMatrix \n";
		const float f = -1;
		return &f;
	}
}

void CPxActor::SetGraf(CObject* gr)
{
	graf = gr;
}

void CPxActor::Simulating(float time)
{
	// накопленное значение времени симуляции в мс
	static	float	mAccumulator = 0.0f;
	// минимальный шаг симуляции (1/60 секунды)
	static	float	mStepSize = 1.0f / 60.0f;
	// проверка необходимости симуляции и непосредственно симуляция если необходимо
	mAccumulator += time / 1000.0f;
	if (mAccumulator >= mStepSize)
	{
		mAccumulator -= mStepSize;
		// выполнение симуляции
		gScene->simulate(mStepSize); // mStepSize
		// ожидание получения результатов
		gScene->fetchResults(true);
	};

	// получение позиции одного актора
	PxTransform	t = DynamicActor->getGlobalPose();
	// преобразование к матричной форме
	PxMat44	m = PxMat44(t);
	// получение указателя на значения матрицы равзернутой по столбцам
	const float	*tm = m.front();
	graf->SetTransformMatrix(tm);
}

void CPxActor::SetScene(PxScene* Sx)
{
	gScene = Sx;
}

void CPxActor::AddForce(PxVec3 acceleration) {
	if (DynamicActor != NULL){
		DynamicActor->addForce(acceleration, PxForceMode::eFORCE, true);
	}
}