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
#include <iostream>

using namespace physx;
using namespace std;

class CSprite
{
	//id объекта DevIL
	ILuint ImageId;
	//массив пикселе	 загруженного изображения
	//формат всегда GL_RGBA, GL_UNSIGNED_BYTE
	GLubyte*	Data;
	//ширина, высота изображения
	int			width, height, format, type;
	//буфер OpenGL для хранения массива текселей
	GLuint	bufferID;
public:
	//размеры окна (ширина и высота)
	static int ScreenWidth;
	static int ScreenHeight;
	static void SetScreenParam(int w, int h);

	CSprite();
	// загрузка изображения
	void	Load(const char* filename);
	//	вывод спрайта
	void	DrawSprite();
};
