#pragma once
#include "GL\glew.h"
#include "GL\freeglut.h"


//	КЛАСС ДЛЯ РАБОТЫ С МОДЕЛЬЮ
class CModel
{
	// индекс VAO-буфера
	GLuint	VAO_Index;
	// индекс VBO-буфера вершин
	GLuint	VBO_ArrayIndex;
	// индекс VBO-буфера индексов
	GLuint	VBO_EllementArrayIndex;
	// количество вершин для построения полигонов (количество индексов)
	int		VertexCount;
public:
	//	конструктор
	CModel(void);
	//	создаем кубик нужного размера
	void CreateBox(float w, float d, float h);
	//	вывод кубиков
	void Draw(void);
};
