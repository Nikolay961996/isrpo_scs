#pragma once
#include "GL\freeglut.h"
#include <fstream>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "CCameraBase.h"

using namespace glm;

//	КЛАСС ДЛЯ РАБОТЫ С КАМЕРОЙ
class CCamera : public CCameraBase
{
	//	необходимые свойства
	double YawAngle;	//угол фи
	double PitchAngle;	// угол тэта

	// вектора камеры
	vec3	forward;
	vec3	up;
	vec3	right;

	vec3	Position;
public:
	// конструктор по умолчанию - загрузка начальных параметров камеры из файла
	CCamera(void);
	// деструктор - сохранение новых параметров камеры 
	~CCamera(void);

	// повороты камеры
	void	Left(int value);
	void	Right(int value);
	void	Up(int value);
	void	Down(int value);

	//	получить матрицу камеры (наблюдения)
	float* GetViewMatrix(void);

	//	установка текущей позиции наблюдателя
	void	SetPosition(float x, float y, float z);
	void	GetPosition(float & x, float & y, float & z);

	//	получение вектора вперед
	vec3	GetForwardVector(void);
	//	получение вектора вправо
	vec3	GetRightVector(void);
};
