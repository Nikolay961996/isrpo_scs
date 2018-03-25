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

//	������ ������������ ������
void	CPlane::SetShader(CShader *shader)
{
	Shader = shader;
}

//	���������� ������������ ������
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
	// ��������� ���� ������������� �������
	struct	Point
	{
		GLfloat		x, y, z;
		GLfloat		s, t;
		GLfloat		nx, ny, nz;
	};

	// �������� �������� ����� �����
	// �������� �����������
	GLuint ImgIndex;
	ilGenImages(1, &ImgIndex);
	// ����� �������� �����������
	ilBindImage(ImgIndex);

	// �������� ���������
	char C_String[50];
	C_String[0] = '\0';
	strcat_s(C_String, "Textures/Bonaparte.bmp");
	wchar_t Unicode_String[50];
	wsprintf(Unicode_String, L"%S", C_String);

	if (ilLoadImage(Unicode_String))
		std::cout << "Image success loaded\n";
	else
		std::cout << "Error of load image\n";

	//	��������� ���������� ����������� ��������
	Hight = ilGetInteger(IL_IMAGE_HEIGHT);
	DataHight = ilGetData();
	ilDeleteImages(1, &ImgIndex);

	int PlaneSegCount = Hight;
	float dx = (float)w / (PlaneSegCount - 1);

	//������ �����
	Point* Verteces = new Point[PlaneSegCount * PlaneSegCount];

	// �������� ������� ������ ��� "���� �����" � ������������ �����������
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

	// ������ ��������, ��� ������� ������������ ����� 3 �������
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

	//	�������� ����������� ���� �����
	PxHeightFieldDesc HeightFieldDescriptor;
	HeightFieldDescriptor.format = PxHeightFieldFormat::eS16_TM;
	HeightFieldDescriptor.nbRows = PlaneSegCount;
	HeightFieldDescriptor.nbColumns = PlaneSegCount;
	HeightFieldDescriptor.samples.data = HeightFieldArray;
	HeightFieldDescriptor.samples.stride = sizeof(PxHeightFieldSample);

	// ���������� (cooking) ������
	PxHeightField* HeightField = gC->createHeightField(HeightFieldDescriptor,
		gS->getPhysics().getPhysicsInsertionCallback());

	// �������� ���������, ��������������� ���� ����� 
	PxHeightFieldGeometry HeightFieldGeometry(HeightField, PxMeshGeometryFlags(),
		0.01, ((float)w / (PlaneSegCount - 1)), ((float)w / (PlaneSegCount - 1)));

	// �������� ��������� ��� ���� �����
	PxMaterial *gMaterial = gS->getPhysics().createMaterial(0.6f, 0.5f, 0.5f);

	// �������� �������������� �����, �������������� ���� �����
	PxShape *HeightFieldShape = gS->getPhysics().createShape(HeightFieldGeometry, &gMaterial, 1);

	// ��������� ��������� ������� ���������
	PxTransform localTm(-w / 2.0, 0, -w / 2.0);
	HeightFieldShape->setLocalPose(localTm);

	// �������� ���������� ������ � ������, ��������������� ���� �����
	PxTransform GlobalTm(0, 0, 0);
	PxRigidStatic *GroundPlaneActor = gS->getPhysics().createRigidStatic(GlobalTm);
	GroundPlaneActor->attachShape(*HeightFieldShape);

	// ���������� ������ � �����
	gS->addActor(*GroundPlaneActor);

	// ���������� VBO - ������ ������
	glGenBuffers(1, &VBO_ArrayIndex);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ArrayIndex);
	glBufferData(GL_ARRAY_BUFFER, PlaneSegCount * PlaneSegCount * sizeof(Point), Verteces, GL_STATIC_DRAW);

	// ���������� VBO - ������� ��������
	glGenBuffers(1, &VBO_EllementArrayIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_EllementArrayIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (PlaneSegCount - 1) * (PlaneSegCount - 1) * 2 * 3 * sizeof(GLuint), Indeces, GL_STATIC_DRAW);

	// �������� VAO
	glGenVertexArrays(1, &VAO_Index);
	glBindVertexArray(VAO_Index);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ArrayIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_EllementArrayIndex);
	// ���������� VAO:
	// �������������� ����������
	int vPosition_index = 0;
	glVertexAttribPointer(vPosition_index, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (const void*)offsetof(Point, x));
	glEnableVertexAttribArray(vPosition_index);
	// �������
	int vNormal_index = 1;
	glVertexAttribPointer(vNormal_index, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (const void*)offsetof(Point, nx));
	glEnableVertexAttribArray(vNormal_index);
	// ���������� ����������
	int vTexCoord_index = 2;
	glVertexAttribPointer(vTexCoord_index, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (const void*)offsetof(Point, s));
	glEnableVertexAttribArray(vTexCoord_index);

	// "�������" ������ VAO (���� �������� �� ��������� � ����������)
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// �������� ���������� ������
	VertexCount = (PlaneSegCount - 1) * (PlaneSegCount - 1) * 2 * 3;
};

void	CPlane::DrawPlane(void)
{
	if ((Shader == NULL) || (Camera == NULL)) return;

	// ���������� ������
	Shader->Activate();

	// ��������� ������� ��������
	Shader->SetUniformMat4("uProjectionMatrix", Camera->GetProjectionMatrix());
	Shader->SetUniformMat4("uLightProjectionMatrix", CameraLight->GetProjectionMatrix());

	// ��������� ������� ���������� ������
	mat4	ViewMatrix = make_mat4(Camera->GetViewMatrix());

	Shader->SetUniformMat4("uModelViewMatrix", value_ptr(ViewMatrix));

	// ��������� ������� ���������� ������ light
	ViewMatrix = make_mat4(CameraLight->GetViewMatrix());

	Shader->SetUniformMat4("uLightModelViewMatrix", value_ptr(ViewMatrix));

	// ��������� ���������� ��������� �����
	Light->Apply(Shader, Camera);

	// ��������� ���������� �������
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
	return 50;
	return (float)DataHight[3 * x * Hight + z * 3] * ((float)40 / 255);
}
