#include "CModel.h"

CModel::CModel(void)
{
	VAO_Index = 0;
	VBO_ArrayIndex = 0;
	VBO_EllementArrayIndex = 0;
	VertexCount = 0;
}


void CModel::CreateBox(float w, float d, float h)
{
	struct	Point
	{
		GLfloat		x, y, z;
		GLfloat		nx, ny, nz;
		GLfloat		s, t;
	};

	float pw = w / 2;
	float pd = d / 2;
	float ph = h / 2;

	// создание вершин
	Point	Verteces[] =
	{
		// перед
		{ -pw, -pd, +ph,		0, 0, 1,		0, 1 }, 
		{ -pw, +pd,	+ph,		0, 0, 1,		0, 0 },
		{ +pw, +pd,	+ph,		0, 0, 1,		1, 0 },
		{ +pw, -pd,	+ph,		0, 0, 1,		1, 1 },
		// право
		{ +pw, -pd, +ph,		1, 0, 0,		0, 1 },
		{ +pw, +pd,	+ph,		1, 0, 0,		0, 0 },
		{ +pw, +pd,	-ph,		1, 0, 0,		1, 0 },
		{ +pw, -pd,	-ph,		1, 0, 0,		1, 1 },
		// зад 
		{ +pw, +pd, -ph,		0, 0, -1,		0, 1 },
		{ +pw, -pd,	-ph,		0, 0, -1,		0, 0 },
		{ -pw, -pd,	-ph,		0, 0, -1,		1, 0 },
		{ -pw, +pd,	-ph,		0, 0, -1,		1, 1 },
		// лево
		{ -pw, -pd, +ph,		-1, 0, 0,		0, 1 },
		{ -pw, -pd,	-ph,		-1, 0, 0,		0, 0 },
		{ -pw, +pd,	-ph,		-1, 0, 0,		1, 0 },
		{ -pw, +pd,	+ph,		-1, 0, 0,		1, 1 },
		// низ
		{ -pw, -pd, +ph,		0, -1, 0,		0, 1 },
		{ -pw, -pd,	-ph,		0, -1, 0,		0, 0 },
		{ +pw, -pd,	-ph,		0, -1, 0,		1, 0 },
		{ +pw, -pd,	+ph,		0, -1, 0,		1, 1 },
		// верх
		{ -pw, +pd, +ph,		0, 1, 0,		0, 1 },
		{ -pw, +pd,	-ph,		0, 1, 0,		0, 0 },
		{ +pw, +pd,	-ph,		0, 1, 0,		1, 0 },
		{ +pw, +pd,	+ph,		0, 1, 0,		1, 1 }
	};
	GLuint	Indeces[] = 
	{
		0, 2, 1, 0, 3, 2,		
		4, 6, 5, 4, 7, 6,		
		9, 11, 8, 9, 10, 11,	
		12, 15, 14, 12, 14, 13,	
		16, 17, 18, 16, 18, 19,
		20, 23, 22, 20, 22, 21	
	};

	// заполнение VBO - буфера вершин
	glGenBuffers(1, &VBO_ArrayIndex);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ArrayIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Verteces), Verteces, GL_STATIC_DRAW);

	// заполнение VBO - буффера индексов
	glGenBuffers(1, &VBO_EllementArrayIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_EllementArrayIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indeces), Indeces, GL_STATIC_DRAW);

	// создание VAO
	glGenVertexArrays(1, &VAO_Index);
	glBindVertexArray(VAO_Index);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ArrayIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_EllementArrayIndex);
	// заполнение VAO:
	// геометрические координаты
	int vPosition_index = 0;
	glVertexAttribPointer(vPosition_index, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (const void*)offsetof(Point, x));
	glEnableVertexAttribArray(vPosition_index);
	// нормали
	int vNormal_index = 1;
	glVertexAttribPointer(vNormal_index, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (const void*)offsetof(Point, nx));
	glEnableVertexAttribArray(vNormal_index);
	// текстурные координаты
	int vTexCoord_index = 2;
	glVertexAttribPointer(vTexCoord_index, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (const void*)offsetof(Point, s));
	glEnableVertexAttribArray(vTexCoord_index);

	// "отвязка" буфера VAO (чтоб случайно не испортить в дальнейшем)
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// указание количество вершин
	VertexCount = 36;
}

void CModel::Draw(void)
{
	glBindVertexArray(VAO_Index);
	glDrawElements(GL_TRIANGLES, VertexCount, GL_UNSIGNED_INT, 0);
}
