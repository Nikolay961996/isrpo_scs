#include "CPlane.h"

CPlane::CPlane()
{
	Material = NULL;
	VAO_Index = 0;
	VBO_ArrayIndex = 0;
	VBO_EllementArrayIndex = 0;
	VertexCount = 0;
}
CCameraBase*		CPlane::Camera = NULL;
CSourceLight*	CPlane::Light = NULL;
CShader*		CPlane::Shader = NULL;
CCameraBase* CPlane::CameraLight = NULL;

//	Задать используемый шейдер
void	CPlane::SetShader(CShader *shader)
{
	Shader = shader;
}

//	Установить используемую камеру
void	CPlane::SetCamera(CCameraBase *camera)
{
	Camera = camera;
}

void	CPlane::SetCameraLight(CCameraBase *camera)
{
	CameraLight = camera;
}

void CPlane::SetLight(CSourceLight* l)
{
	Light = l;
}

void	CPlane::CreatePlane(PxScene* gS, PxCooking* gC, int w)
{
	// структура ддля представления вершины
	struct	Point
	{
		GLfloat		x, y, z;
		GLfloat		s, t;
		GLfloat		nx, ny, nz;
	};

	// загрузка текстуры карты высот
	// создание изображения
	GLuint ImgIndex;
	ilGenImages(1, &ImgIndex);
	// выбор текущего изображения
	ilBindImage(ImgIndex);

	// загрузка изображеи
	char C_String[50];
	C_String[0] = '\0';
	strcat_s(C_String, "Textures/heightfield.png");
	wchar_t Unicode_String[50];
	wsprintf(Unicode_String, L"%S", C_String);

	if (ilLoadImage(Unicode_String))
		std::cout << "Image success loaded\n";
	else
		std::cout << "Error of load image\n";

	//	получение параметров загруженной текстуры
	Hight = ilGetInteger(IL_IMAGE_HEIGHT);
	DataHight = ilGetData();
	ilDeleteImages(1, &ImgIndex);

	int PlaneSegCount = Hight;
	float dx = (float)w / (PlaneSegCount - 1);

	//массив точек
	Point* Verteces = new Point[PlaneSegCount * PlaneSegCount];

	// создание массива вершин для "поля высот" с необходимыми параметрами
	PxHeightFieldSample *HeightFieldArray = new PxHeightFieldSample[PlaneSegCount * PlaneSegCount];
	
	float center = (float)w / 2;
	int Num = 0;
	float x = 0, z = 0;
	for (int i = 0; i < PlaneSegCount; i++)
	{
		for (int j = 0; j < PlaneSegCount; j++)
		{
			float PointHight = (float)DataHight[3 * i * PlaneSegCount + j * 3] * ((float)40 / 255);

			Verteces[Num] = { x - center, PointHight, z - center,		(float)j / (PlaneSegCount - 1), (float)i / (PlaneSegCount - 1) };

			HeightFieldArray[j * PlaneSegCount + i].height = PointHight * 100;
			HeightFieldArray[j * PlaneSegCount + i].materialIndex0 = 0;
			HeightFieldArray[j * PlaneSegCount + i].materialIndex1 = 0;
			HeightFieldArray[j * PlaneSegCount + i].clearTessFlag();

			x += dx;
			Num++;
		}
		x = 0;
		z += dx;
	}

	// массив индексов, для каждого треугольника нужно 3 вершины
	GLuint* Indeces = new GLuint[(PlaneSegCount - 1) * (PlaneSegCount - 1) * 2 * 3];

	for (int i = 0, ind, count = 0; i < PlaneSegCount - 1; i++)
	{
		for (int j = 0; j < PlaneSegCount - 1; j++)
		{
			ind = PlaneSegCount * i + j;
			Indeces[count] = ind;
			Indeces[count + 1] = ind + PlaneSegCount;
			Indeces[count + 2] = ind + 1;

			Indeces[count + 3] = ind + 1;
			Indeces[count + 4] = ind + PlaneSegCount;
			Indeces[count + 5] = ind + PlaneSegCount + 1;
			count += 6;
		}
	}

	vec3 a, b, c;
	vec3 normal;

	vec3* normal_mat;
	normal_mat = new vec3[PlaneSegCount * PlaneSegCount];
	for (int i = 0; i < (PlaneSegCount - 1) * (PlaneSegCount - 1) * 2 * 3; i += 3)
	{
		a = vec3(Verteces[Indeces[i + 0]].x, Verteces[Indeces[i + 0]].y, Verteces[Indeces[i + 0]].z);
		b = vec3(Verteces[Indeces[i + 1]].x, Verteces[Indeces[i + 1]].y, Verteces[Indeces[i + 1]].z);
		c = vec3(Verteces[Indeces[i + 2]].x, Verteces[Indeces[i + 2]].y, Verteces[Indeces[i + 2]].z);

		normal = normalize(cross(b - a, c - a));

		normal_mat[Indeces[i + 0]] += normal;
		normal_mat[Indeces[i + 1]] += normal;
		normal_mat[Indeces[i + 2]] += normal;
	}

	for (int i = 0; i < (PlaneSegCount - 1) * (PlaneSegCount - 1) * 2 * 3; i++)
	{
		normal_mat[Indeces[i]] = normalize(normal_mat[Indeces[i]]);

		Verteces[Indeces[i]].nx = normal_mat[Indeces[i]].x;
		Verteces[Indeces[i]].ny = normal_mat[Indeces[i]].y;
		Verteces[Indeces[i]].nz = normal_mat[Indeces[i]].z;
	}

	//	создание дескриптора поля высот
	PxHeightFieldDesc HeightFieldDescriptor;
	HeightFieldDescriptor.format = PxHeightFieldFormat::eS16_TM;
	HeightFieldDescriptor.nbRows = PlaneSegCount;
	HeightFieldDescriptor.nbColumns = PlaneSegCount;
	HeightFieldDescriptor.samples.data = HeightFieldArray;
	HeightFieldDescriptor.samples.stride = sizeof(PxHeightFieldSample);

	// подготовка (cooking) данных
	PxHeightField* HeightField = gC->createHeightField(HeightFieldDescriptor,
		gS->getPhysics().getPhysicsInsertionCallback());

	// создание геометрии, соответствующей полю высот 
	PxHeightFieldGeometry HeightFieldGeometry(HeightField, PxMeshGeometryFlags(),
		0.01, ((float)w / (PlaneSegCount - 1)), ((float)w / (PlaneSegCount - 1)));

	// создание материала для поля высот
	PxMaterial *gMaterial = gS->getPhysics().createMaterial(0.6f, 0.5f, 0.5f);

	// создание геометрической формы, представляющей поле высот
	PxShape *HeightFieldShape = gS->getPhysics().createShape(HeightFieldGeometry, &gMaterial, 1);

	// установка локальной системы координат
	PxTransform localTm(-w / 2.0, 0, -w / 2.0);
	HeightFieldShape->setLocalPose(localTm);

	// создание статичного актора с формой, соответствующей полю высот
	PxTransform GlobalTm(0, 0, 0);
	PxRigidStatic *GroundPlaneActor = gS->getPhysics().createRigidStatic(GlobalTm);
	GroundPlaneActor->attachShape(*HeightFieldShape);

	// добавление актора к сцене
	gS->addActor(*GroundPlaneActor);

	// заполнение VBO - буфера вершин
	glGenBuffers(1, &VBO_ArrayIndex);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ArrayIndex);
	glBufferData(GL_ARRAY_BUFFER, PlaneSegCount * PlaneSegCount * sizeof(Point), Verteces, GL_STATIC_DRAW);

	// заполнение VBO - буффера индексов
	glGenBuffers(1, &VBO_EllementArrayIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_EllementArrayIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (PlaneSegCount - 1) * (PlaneSegCount - 1) * 2 * 3 * sizeof(GLuint), Indeces, GL_STATIC_DRAW);

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
	VertexCount = (PlaneSegCount - 1) * (PlaneSegCount - 1) * 2 * 3;
};

void	CPlane::DrawPlane(void)
{
	if ((Shader == NULL) || (Camera == NULL)) return;

	// активируем шейдер
	Shader->Activate();

	// установка матрицы проекции
	Shader->SetUniformMat4("uProjectionMatrix", Camera->GetProjectionMatrix());
	Shader->SetUniformMat4("uLightProjectionMatrix", CameraLight->GetProjectionMatrix());

	// установка матрицы наблюдения модели
	mat4	ViewMatrix = make_mat4(Camera->GetViewMatrix());

	Shader->SetUniformMat4("uModelViewMatrix", value_ptr(ViewMatrix));

	// установка матрицы наблюдения модели light
	ViewMatrix = make_mat4(CameraLight->GetViewMatrix());

	Shader->SetUniformMat4("uLightModelViewMatrix", value_ptr(ViewMatrix));

	// установка параметров источника света
	Light->Apply(Shader, Camera);

	// установка материалов объекта
	Material->Apply(Shader);

	glBindVertexArray(VAO_Index);
	glDrawElements(GL_TRIANGLES, VertexCount, GL_UNSIGNED_INT, 0);
};

void	CPlane::SetMaterial(CMaterial* Mat)
{
	Material = Mat;
};

void	CPlane::SetDataHight(GLubyte* d)
{
	DataHight = d;
}

void	CPlane::SetTexHight(int d)
{
	Hight = d;
}

float CPlane::GetHeight(int x, int z)
{
	return (float)DataHight[3 * x * Hight + z * 3] * ((float)40 / 255);
}
