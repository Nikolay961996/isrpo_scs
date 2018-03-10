#pragma once
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "CShader.h"
#include "CCamera.h"
#include "CMaterial.h"
#include "PxPhysicsAPI.h"
#include "CSourceLight.h"

using namespace physx;

class CPlane
{
	// указатель на камеру, используемую для вывода всех объектов
	static CCameraBase* Camera;
	static CCameraBase* CameraLight;
	static CSourceLight* Light;
	// используемый шейдер
	static CShader* Shader;
	// материал
	CMaterial* Material;
	
	// индекс VAO-буфера
	GLuint	VAO_Index;
	// индекс VBO-буфера вершин
	GLuint	VBO_ArrayIndex;
	// индекс VBO-буфера индексов
	GLuint	VBO_EllementArrayIndex;
	// количество вершин для построения полигонов (количество индексов)
	int		VertexCount;
	GLubyte*	DataHight;
	int			Hight;
	float       WidthCenter;
public:
	CPlane(void);
	//	Задать используемый шейдер
	static void	SetShader(CShader* shader);
	// задать материал
	void	SetMaterial(CMaterial* Mat);
	//	Установить используемую камеру
	static void	SetCamera(CCameraBase *camera);
	static void	SetCameraLight(CCameraBase *camera);
	static void	SetLight(CSourceLight* l);

	// создаем плоскость нужного размера (width)
	// с заданным количество точек в длину и ширину
	void	CreatePlane(PxScene* gS, PxCooking* gC, int w);
	//	выводим плоскость
	void	DrawPlane(void);
	void	SetDataHight(GLubyte* d);
	void	SetTexHight(int d);
	float   GetHeight(int x, int z);
};