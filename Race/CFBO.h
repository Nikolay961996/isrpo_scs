#pragma once
#include "GL\glew.h"
#include "GL\freeglut.h"
#include <iostream>

using namespace std;

//	КЛАСС ДЛЯ РАБОТЫ С БУФЕРОМ FBO
class CFBO
{
	// индекс буфера FBO
	GLuint	FBO_Index;
	// текстурный объект для цвета
	GLuint	Сolor_tex;
	// текстурный объект для глубины
	GLuint	Depth_tex;
	// ширина и высота буфера FBO
	int	Width;
	int	Height;
public:
	// инициализация буфера FBO (создание самого буфера, текстуры цвета и текстуры глубины)
	void	Init(int w, int h);
	// выбор буфера FBO в качестве текущего для рендеринга
	void	Start(void);
	// возврат к буферу кадра по умолчанию (для вывода на экран)
	void	Stop(void);
	// вывод буфера FBO в заданную позицию экрана
	void	Blit(int border_x = 1590, int border_y = 890);
	// привязка текстуры глубины к текстурному блоку 
	// для использования в алгоритме Shadow Mapping
	void	BindDepthTexture(GLenum TexUnit = GL_TEXTURE3);
};