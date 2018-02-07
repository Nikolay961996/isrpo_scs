#pragma once
#include "GL/freeglut.h"
#include "CShader.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include <string>

//	КЛАСС ДЛЯ РАБОТЫ С ТЕКСТУРОЙ
class	CTexture
{
	//	индекс текстурного объекта
	GLuint TexIndex;
	int ActiveBlock;

public:
	// конструктор
	CTexture(void);

	//	загрузка текстуры из внешнего файла
	void	SetActiveBlock(const int TexB);
	char*	GetNameTex();
	int		GetNumTex();
	void	Load(const char* filename);
	void	LoadKubMap(const char* PX, const char* PY, const char* PZ, \
						const char* NX, const char* NY, const char* NZ);

	//	применение текстуры
	void	Apply(void);
	void	ApplyCubeMap(void);
};