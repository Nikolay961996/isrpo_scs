#pragma once
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <fstream>
#include <iostream>

using namespace glm;

// КЛАСС ДЛЯ РАБОТЫ С ШЕЙДЕРОМ
class CShader
{
public:
	//	вершинный шейдер
	GLuint	Vsh;
	//	фрагментный шейдер
	GLuint	Fsh;
	//	шейдерная программа (шейдер)
	GLuint	Program;
public:
	//	загрузить шейдер
	int Load(const char *VertexFileName, const char *FragmentFileName);

	//	активизировать шейдер (сделать текущим)
	void Activate();
	//	отключить шейдер (использовать нулевую шейдерную программу)
	static	void Deactivate();

	//	получение индекса атрибут-переменной
	int	GetAttribLocation(const char *name);
	//	запись вектора из 4-х комопнент в uniform-переменную
	void SetUniformVec4(const char *name, const GLfloat *value);
	void SetUniform1f(const char * name, const GLfloat* value);
	void SetUnoform1i(const char* name, const GLint* value);
	void SetUniformMat4(const char *name, const GLfloat* value);
};
