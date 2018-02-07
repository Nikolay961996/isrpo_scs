#include "CObject.h"

//	Конструктор
CObject::CObject(void)
{
	memset(Position, 0, 3 * sizeof(GLfloat));
	ModelMatrix = new GLfloat[16];
	memset(ModelMatrix, 0, 16 * sizeof(GLfloat));
	ModelMatrix[5] = 1;
	ModelMatrix[15] = 1;

	Angle = 0;
	Model = NULL;
	Light = NULL;
}

CCameraBase* CObject::Camera = NULL;
CShader*  CObject::Shader = NULL;
CCameraBase* CObject::CameraLight = NULL;

//	Задать используемый шейдер
void	CObject::SetShader(CShader *shader)
{
	Shader = shader;
}

//	Задать используемую модель
void	CObject::SetModel(CModel *model)
{
	Model = model;
}

//	Установить используемую камеру
void	CObject::SetCamera(CCameraBase *camera)
{
	Camera = camera;
}

void	CObject::SetCameraLight(CCameraBase *camera)
{
	CameraLight = camera;
}

//	Задать позицию объекта
void	CObject::SetPosition(float x, float y, float z)
{
	Position[0] = x;
	Position[1] = y;
	Position[2] = z;

	ModelMatrix[12] = Position[0];
	ModelMatrix[13] = Position[1];
	ModelMatrix[14] = Position[2];
}

//	Задать угол поворота в градусах относительно оси OY
void	CObject::SetAngle(float a)
{
	Angle = a;

	double radAngle = Angle * 0.01745;

	ModelMatrix[0] = (float)cos(radAngle);
	ModelMatrix[2] = (float)-sin(radAngle);
	ModelMatrix[8] = (float)sin(radAngle);
	ModelMatrix[10] = (float)cos(radAngle);
}

//	Вывести объект
void	CObject::Draw(void)
{
	if ((Shader == NULL) || (Model == NULL) || (Camera == NULL)) return;

	// активируем шейдер
	Shader->Activate();

	// установка матрицы проекции
	Shader->SetUniformMat4("uProjectionMatrix", Camera->GetProjectionMatrix());
	Shader->SetUniformMat4("uLightProjectionMatrix", CameraLight->GetProjectionMatrix());

	// установка матрицы наблюдения модели
	mat4	ViewMatrix = make_mat4(Camera->GetViewMatrix());
	mat4	ModelMatrix = make_mat4(this->ModelMatrix);
	mat4	ModelViewMatrix = ViewMatrix * ModelMatrix;
	
	Shader->SetUniformMat4("uModelViewMatrix", value_ptr(ModelViewMatrix));
	
	// установка матрицы наблюдения модели light
	ViewMatrix = make_mat4(CameraLight->GetViewMatrix());
	ModelMatrix = make_mat4(this->ModelMatrix);
	ModelViewMatrix = ViewMatrix * ModelMatrix;

	Shader->SetUniformMat4("uLightModelViewMatrix", value_ptr(ModelViewMatrix));

	// установка параметров источника света
	if (Light != NULL)
		Light->Apply(Shader, Camera);

	// установка материалов объекта	
	if(Material != NULL)
		Material->Apply(Shader);

	// вывод объекта
	Model->Draw();
}

void	CObject::SetMaterial(CMaterial* Mat)
{
	Material = Mat;
};

void	CObject::SetTransformMatrix(const float* m)
{
	std::copy(m, &m[15], ModelMatrix);
}

void CObject::SetLight(CSourceLight* l)
{
	Light = l;
}
