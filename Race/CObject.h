#pragma once
#include "CShader.h"
#include "CModel.h"
#include "CMaterial.h"
#include "CSourceLight.h"
#include "CCameraBase.h"

// КЛАСС ДЛЯ ПРЕДСТАВЛЕНИЯ ОБЪЕКТА
class CObject
{
private:
	// указатель на камеру, используемую для вывода всех объектов
	static CCameraBase* Camera;
	static CCameraBase* CameraLight;
	// источник света
	CSourceLight* Light;
	// используемый шейдер
	static CShader* Shader;
	// материал
	CMaterial* Material;
	// Позиция и угол поворота для объекта	
	GLfloat	Position[3];
	GLfloat	Angle;
	// Матрица модели (расположения объекта) - чтоб не вычислять каждый раз
	GLfloat* ModelMatrix;
	// Используемая модель
	CModel* Model;
public:
	//	Конструктор
	CObject(void);

	//	Задать используемый шейдер
	static void	SetShader(CShader *shader);
	// задать материал
	void	SetMaterial(CMaterial* Mat);
	//	Задать используемую модель
	void	SetModel(CModel *model);
	//	Установить используемую камеру
	static void	SetCamera(CCameraBase *camera);
	static void	SetCameraLight(CCameraBase *camera);

	void SetLight(CSourceLight* l);
	void	SetPosition(float x, float y, float z);
	//	Задать угол поворота в градусах относительно оси OY
	void	SetAngle(float a);
	void	SetTransformMatrix(const float* m);
	//	Вывести объект
	void	Draw(void);

};