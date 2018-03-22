#include <windows.h>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "CPhysX.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
#include "stdio.h"
#include <time.h>

#include "CShader.h"
#include "CCamera.h"
#include "CStaticCamera.h"
#include "CModel.h"
#include "CObject.h"
#include "CPlane.h"
#include "CMaterialPhong.h"
#include "CMaterialDiffSpec.h"
#include "CMaterialMix.h"
#include "CSourceLight.h"
#include "CPxMaterial.h"
#include "CPxShape.h"
#include "CPxActor.h"
#include "CTexture.h"
#include "CMaterialTextureOnly.h"
#include "CFBO.h"
#include "CSprite.h"

using namespace std;
using namespace physx;

CPhysX PhysX;						// физика
CShader ShaderDiffSpec;				// шейдер с диффузной и зеркальной составлющей
CShader ShaderMix;					// шейдер с 2 текстурами и маской
CShader ShaderSkyBox;				// шейдер для неба
CShader ShaderDepth;				// шейдер для вычисления только глуб
CCamera Camera;						// камера
CStaticCamera CameraShadow;			// камера тени
CModel ModelKub;					// модель куба
CObject* Kub;						// объекты кубиков
CObject* AddedKub;					// добавляемые объекты кубиков
CObject SkyBox;						// объект неба
CPlane Plane;						// объект плоскости
CMaterialDiffSpec KubMatDiffSpec;	// материал для 2 текстур кубика (диффузная и зеркальная)
CMaterialMix PlaneMatMix;			// материал с 2 текстурой и маской для плоскости
CMaterialTextureOnly SkyBoxMat;		// материал для неба
CSourceLight LightDiffSpec;			// источник света для диффузной и зеркальной составлющей
CSourceLight LightMix;				// источник света для 2 текстур и маски
CTexture Tex_Mask;					// карта высот
CTexture Tex_Grass;					// текстура травы
CTexture Tex_Dirt;					// текстура грязи
CTexture Tex_Wood;					// текстура дерева
CTexture Tex_Wood_Specular;			// зеркальная текстура дерева
CTexture Tex_Sky_box;				// кубическая текстура неба
CFBO ShadowFBO;						// буфер кадра для теней
CSprite SpriteHero;                 // спрайт персонажа
CObject* targets;					// объекты мишеней
CModel targetModel;					// модель мишени
CMaterialPhong targetsMat;			// материал мишеней (красный по задаче SCSG-6)
CSourceLight LightTargets;          // источник для мишеней (по задаче SCSG-6)
CShader ShaderPhong;                // шейдер для мишеней (по задаче SCSG-6)

CModel bulletModel;					// модель пули
CObject* bullet;					// объект пули
CMaterialPhong bulletMaterial;      // Материал пули
CPxActor*	bulletActor;			// актор для представления пули
CSourceLight bulletLight;           // источник для пули
const int BULLET_SIZE = 5;			// размер пули

CPxMaterial gMaterial;				// объект для представления материала (задает упругость, коэффициент трения и т.д.)
CPxShape	gShape;					// физическая модель
CPxActor*	KubActor;				// актор для представления статического объекта - куба
CPxActor*	AddedKubActor;			// актор для представления статического объекта - добавляемый куба
CPxMaterial gMaterialTarget;        // объект для представления материала мишени
CPxShape    gShapeTarget;           // физическая модель мишени
CPxActor*   TargetActor;            // актор для представления статического объекта - мишени

// PIRAMID MANAGER
const int COUNT_KUB_ROWS = 5;		// количество рядов
const float KUB_SIZE = 10;			// размер кубика
const float KUB_INTERVAL = 5.0f;	// промежуток между кубиками

// МИШЕНИ
const float TARGET_INTERVAL = 10.0f;  //расстояние между мишенями
const float TARGET_SIZE = 5.0f;       //размер мишени

// ПЕРСОНАЖ
PxControllerManager	*manager;		// менеджер всех персонажей
PxController		*controller;	// контроллер персонажа

// ЭКРАН
const int window_w = 1550;
const int window_h = 900;				

// FBO
const int FBO_w = 512;
const int FBO_h = 512;

ULONGLONG t1, t2;					// время между кадрами
ULONGLONG TicsPerSec;				// частота тиков
float Simulation_Time_Passed;		// время между кадрами
bool Mode_Face = false;				// режим вывода полигонов
bool Simulate_PhysX = false;		// режим симуляции физики
short AddedIndex = 0;				// индексирование добавляемых объектов
POINT now_position_mause;			// нынешняя позиция мыши
int mause_move_x, mause_move_y;		// перемещение мыши
float width_plane = 200;			// размер плоскости
int numTargets;                     // количество мишеней
vec3 LightPosition = \
		vec3((float)-width_plane / 2, 40.0f, (float)-width_plane / 2);// позиция источника света

// FPS
int count_frame = 0;
ULONGLONG tc1, tc2; 
string Title;

void	InitObject(void) 
{
	CameraShadow.SetPosition(LightPosition);
	CameraShadow.SetProjectionMatrix(45.0, (float)FBO_w / FBO_h, 0.1f, 500.0);
	CObject::SetCameraLight(&CameraShadow);
	CPlane::SetCameraLight(&CameraShadow);

	// инициализация FBO
	ShadowFBO.Init(FBO_w, FBO_h);
	ShadowFBO.Stop();

	//******************* МАТЕРИАЛЫ *******************
	// параметры источника света	
	LightDiffSpec.SetAmbient(vec4(0.8f, 0.8f, 0.8f, 0.8f));
	LightDiffSpec.SetDiffuse(vec4(0.3f, 0.3f, 0.3f, 0.8f));
	LightDiffSpec.SetSpecular(vec4(0.8f, 0.8f, 0.8f, 0.8f));
	LightDiffSpec.SetPosition(vec4(LightPosition, 1.0f));

	LightMix.SetAmbient(vec4(0.8f, 0.8f, 0.8f, 0.8f));
	LightMix.SetDiffuse(vec4(0.8f, 0.8f, 0.8f, 0.8f));
	LightMix.SetPosition(vec4(LightPosition, 1.0f));

	LightTargets.SetAmbient(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	LightTargets.SetDiffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	LightTargets.SetPosition(vec4(LightPosition, 1.0f));

	// источник для пули
	bulletLight.SetAmbient(vec4(0.0f, 0.0f, 0.0f, 0.0f));
	bulletLight.SetDiffuse(vec4(0.0f, 0.0f, 0.0f, 0.0f));
	bulletLight.SetPosition(vec4(LightPosition, 1.0f));

	// текстуры плоскости
	Tex_Mask.SetActiveBlock(GL_TEXTURE0);
	Tex_Grass.SetActiveBlock(GL_TEXTURE1);
	Tex_Dirt.SetActiveBlock(GL_TEXTURE2);
	Tex_Mask.Load("heightfield.png");
	Tex_Grass.Load("grass1.png");
	Tex_Dirt.Load("dirt.png");

	// текстуры кубика
	Tex_Wood.SetActiveBlock(GL_TEXTURE0);
	Tex_Wood_Specular.SetActiveBlock(GL_TEXTURE1);
	Tex_Wood.Load("container2.png");
	Tex_Wood_Specular.Load("container2_specular.png");

	// текстура неба
	Tex_Sky_box.SetActiveBlock(GL_TEXTURE0);
	Tex_Sky_box.LoadKubMap("SkyBox_back.png", "SkyBox_top.png", "SkyBox_left.png", \
		"SkyBox_front.png", "SkyBox_bottom.png", "SkyBox_right.png");

	// материал плоскости
	PlaneMatMix.SetAmbient(vec4(0.8f, 0.8f, 0.8f, 1.0f));
	PlaneMatMix.SetDiffuse(vec4(0.7f, 0.7f, 0.7f, 0.7f));
	PlaneMatMix.SetTextures(&Tex_Mask, &Tex_Grass, &Tex_Dirt);

	// материал куба 
	KubMatDiffSpec.SetDiffuse(vec4(0.2f, 0.2f, 0.2f, 0.7f));
	KubMatDiffSpec.SetShiness(32.0f);
	KubMatDiffSpec.SetTextures(&Tex_Wood, &Tex_Wood_Specular);

	// настройка плоскости
	CPlane::SetCamera(&Camera);
	CPlane::SetLight(&LightMix);
	Plane.SetMaterial(&PlaneMatMix);
	Plane.CreatePlane(PhysX.GetScene(), PhysX.GetCooking(), width_plane);

	// материал неба
	SkyBoxMat.SetTexture(&Tex_Sky_box);

	// настройки спрайта персонажа
	SpriteHero.Load("hero.png");
	SpriteHero.SetScreenParam(window_w, window_h);

	// материал мишени
	targetsMat.SetAmbient(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	targetsMat.SetDiffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	targetsMat.SetShiness(32.0f);

	// модель пули
	bulletModel.CreateBox(BULLET_SIZE, BULLET_SIZE, BULLET_SIZE);
	// материал пули
	bulletMaterial.SetAmbient(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	bulletMaterial.SetDiffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	bulletMaterial.SetShiness(32.0f);
	//******************* ФИЗИЧЕСКИЕ ОБЪЕКТЫ *******************
	CPxActor::SetPhysicsModel(PhysX.GetPhysicsModule());
	CPxActor::SetScene(PhysX.GetScene());

	KubActor = new CPxActor[(COUNT_KUB_ROWS + 1) * COUNT_KUB_ROWS / 2];
	CPxMaterial::SetPhysicsModel(PhysX.GetPhysicsModule());
	gMaterial.SetMaterial(0.6f, 0.5f, 0.5f);
	gMaterialTarget.SetMaterial(0.6f, 0.5f, 0.5f);

	CPxShape::SetPhysicsModel(PhysX.GetPhysicsModule());
	gShape.SetMaterial(&gMaterial);
	gShape.AddBoxShape((float)KUB_SIZE / 2, (float)KUB_SIZE / 2, (float)KUB_SIZE / 2);
	gShapeTarget.SetMaterial(&gMaterialTarget);
	gShapeTarget.AddBoxShape(TARGET_SIZE, 5.0f * TARGET_SIZE, TARGET_SIZE);

	// создание менеджера персонажей
	manager = PxCreateControllerManager(*PhysX.GetScene());

	//	создание одного персонажа
	PxCapsuleControllerDesc controllerDesc;
	controllerDesc.setToDefault();
	controllerDesc.radius = 2;
	controllerDesc.height = 10;
	controllerDesc.upDirection = PxVec3(0, 1, 0);
	controllerDesc.stepOffset = 0.2;
	controllerDesc.slopeLimit = 0.7;
	controllerDesc.material = gMaterial.GetMaterial();
	controllerDesc.position = PxExtendedVec3(30, 150, 30);
	bool	valid = controllerDesc.isValid();
	controller = manager->createController(controllerDesc);

	// расположение кубиков
	ModelKub.CreateBox(KUB_SIZE, KUB_SIZE, KUB_SIZE);
	Kub = new CObject[(COUNT_KUB_ROWS + 1) * COUNT_KUB_ROWS / 2];
	CObject::SetCamera(&Camera);

	// расположение и настройка неба
	SkyBox.SetModel(&ModelKub);
	SkyBox.SetMaterial(&SkyBoxMat);
	SkyBox.SetAngle(0);
	SkyBox.SetPosition(0, 20, 0);

	int index = 0;
	float start_x = 0;
	float start_y = KUB_SIZE / 2 + 40.2f;
	float start_z = 0;

	for (int i = COUNT_KUB_ROWS; i > 0; i--)
	{
		start_x = -1 * (((i - 1) * KUB_INTERVAL + i * KUB_SIZE) / 2 - 0.5f);
		for (int j = i; j > 0; j--)
		{
			index = (i - 1) * i / 2 - 1 + j;

			Kub[index].SetModel(&ModelKub);
			Kub[index].SetMaterial(&KubMatDiffSpec);
			Kub[index].SetAngle(0);
			Kub[index].SetLight(&LightDiffSpec);
			Kub[index].SetPosition(start_x, start_y, start_z);

			KubActor[index].SetShape(&gShape);
			KubActor[index].CreatDymaicKub(start_x, start_y, start_z);
			KubActor[index].SetMass(0.5);
			KubActor[index].SetGraf(&Kub[index]);
			PhysX.AddActor(KubActor[index].GetActor());

			start_x += KUB_SIZE + KUB_INTERVAL;
		}

		start_y += KUB_SIZE + 0.2f;
		start_z += 0.3f;
	}

	AddedKubActor = new CPxActor[100];
	AddedKub = new CObject[100];

	// мишени
	targetModel.CreateBox(TARGET_SIZE, 5 * TARGET_SIZE, TARGET_SIZE);
	int center = width_plane / 2;
	numTargets = center / TARGET_INTERVAL;
	TargetActor = new CPxActor[numTargets];

	int x_tmp = 2 * TARGET_INTERVAL, z_tmp = 2 * TARGET_INTERVAL; 
	float x = 0, z = 0;
	targets = new CObject[numTargets];

	TargetActor[0].SetShape(&gShapeTarget);
	TargetActor[0].CreatDymaicKub(x_tmp - center, Plane.GetHeight(x_tmp, z_tmp), z_tmp - center);
	targets[0].SetPosition(x_tmp - center, Plane.GetHeight(x_tmp, z_tmp), z_tmp - center);

	srand(time(0));
	for (int i = 0; i < numTargets; i++)
	{
		targets[i].SetModel(&targetModel);
		targets[i].SetMaterial(&targetsMat);
		targets[i].SetAngle(0);
		targets[i].SetLight(&LightTargets);

		TargetActor[i].SetShape(&gShapeTarget);

		for (int j = 0; j < i; j++)
		{
			targets[j].GetPosition(x, z);
			x_tmp = (int)(rand() % (int)(width_plane - TARGET_INTERVAL));
			z_tmp = (int)(rand() % (int)(width_plane - TARGET_INTERVAL));
			if (x_tmp > (x - TARGET_INTERVAL) && x_tmp < (x + TARGET_INTERVAL) && z_tmp > (z - TARGET_INTERVAL) && z_tmp < (z + TARGET_INTERVAL))
			{
				x_tmp -= TARGET_INTERVAL;
				z_tmp -= TARGET_INTERVAL;
			}
			targets[i].SetPosition(x_tmp - center, Plane.GetHeight(x_tmp, z_tmp), z_tmp - center);
		}
		if (i != 0)	TargetActor[i].CreatDymaicKub(x_tmp - center, Plane.GetHeight(x_tmp, z_tmp), z_tmp - center);
		TargetActor[i].SetMass(0.5);
		TargetActor[i].SetGraf(&targets[i]);
		PhysX.AddActor(TargetActor[i].GetActor());
	}

	// FPS
	QueryPerformanceFrequency((LARGE_INTEGER*)&TicsPerSec);
	QueryPerformanceCounter((LARGE_INTEGER*)&tc1);
}

void	DrawKubs(void)
{
	for (int i = 0; i < (COUNT_KUB_ROWS + 1) * COUNT_KUB_ROWS / 2; i++)
		Kub[i].Draw();
}

void	DrawAddedKubs()
{
	for (int i = 0; i < AddedIndex; i++)
		AddedKub[i].Draw();
}

void InitBullet() {
	bulletActor = new CPxActor();
	bullet = new CObject();

	bullet->SetModel(&bulletModel);
	bullet->SetMaterial(&bulletMaterial);
	bullet->SetAngle(0);
	bullet->SetLight(&bulletLight);
	float x = 0, y = 0, z = 0;
	Camera.GetPosition(x, y, z);
	vec3 forwardVector = Camera.GetForwardVector();

	bullet->SetPosition(forwardVector.x * window_w / 10 + x, forwardVector.y * 150, forwardVector.z * window_w / 10 + z);

	bulletActor->SetShape(&gShape);
	bulletActor->CreatDymaicKub(forwardVector.x * window_w / 10 + x, forwardVector.y * 150, forwardVector.z * window_w / 10 + z);
	bulletActor->SetMass(0.5);
	bulletActor->SetGraf(bullet);
	PhysX.AddActor(bulletActor->GetActor());
}

void PressKeys()
{
	//выход из программы
	if (GetAsyncKeyState(VK_ESCAPE))
		exit(0);
	if (GetAsyncKeyState(VK_LBUTTON))
		InitBullet();
	//режим линий полигонов
	if (GetAsyncKeyState(0x31))
	{
		if (Mode_Face)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			Mode_Face = false;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			Mode_Face = true;
		}
		Sleep(150);
	};

	// режим симуляции физики
	if (GetAsyncKeyState(VK_SPACE))
	{
		if (Simulate_PhysX)
			Simulate_PhysX = false;
		else
			Simulate_PhysX = true;

		Sleep(150);
	}

	// добавление кубиков
	if (GetAsyncKeyState(VK_RETURN))
	{
		if (AddedIndex < 100)
		{
			float x, y, z;
			y = 30;
			x = rand() % 80 - 40;
			z = rand() % 80 - 40;

			AddedKub[AddedIndex].SetModel(&ModelKub);
			AddedKub[AddedIndex].SetShader(&ShaderDiffSpec);
			AddedKub[AddedIndex].SetMaterial(&KubMatDiffSpec);
			AddedKub[AddedIndex].SetAngle(0);
			AddedKub[AddedIndex].SetPosition(x, y, z);

			AddedKubActor[AddedIndex].SetShape(&gShape);
			AddedKubActor[AddedIndex].CreatDymaicKub(x, y, z);
			AddedKubActor[AddedIndex].SetMass(0.5);
			AddedKubActor[AddedIndex].SetGraf(&AddedKub[AddedIndex]);
			PhysX.AddActor(AddedKubActor[AddedIndex].GetActor());
			AddedIndex++;
		}
	}
}

void MauseSimulation()
{
	GetCursorPos(&now_position_mause);
	mause_move_x = window_w / 2 - now_position_mause.x;
	mause_move_y = window_h / 2 - now_position_mause.y;

	if (mause_move_x != 0)
	{
		if (mause_move_x < 0)
			Camera.Right(abs(mause_move_x));
		else
			Camera.Left(mause_move_x);
	}

	if (mause_move_y != 0)
	{
		if (mause_move_y < 0)
			Camera.Down(abs(mause_move_y));
		else
			Camera.Up(mause_move_y);
	}

	// перемещение к центру
	SetCursorPos(window_w / 2, window_h / 2);
	ShowCursor(false);
}

void SimulatePersonMove(float time)
{
	bool forward = GetAsyncKeyState(0x57);
	bool backward = GetAsyncKeyState(0x53);
	bool left = GetAsyncKeyState(0x41); 
	bool right = GetAsyncKeyState(0x44);
	
	PxVec3	moveDirection = PxVec3(0, -0.98, 0);
	float speed = 0.7;

	if (forward)
	{
		moveDirection.x += Camera.GetForwardVector().x * speed;
		moveDirection.y += Camera.GetForwardVector().y * speed;
		moveDirection.z += Camera.GetForwardVector().z * speed;
	}

	if (backward)
	{
		moveDirection.x -= Camera.GetForwardVector().x * speed;
		moveDirection.y -= Camera.GetForwardVector().y * speed;
		moveDirection.z -= Camera.GetForwardVector().z * speed;
	}

	if (right)
	{
		moveDirection.x += Camera.GetRightVector().x * speed;
		moveDirection.y += Camera.GetRightVector().y * speed;
		moveDirection.z += Camera.GetRightVector().z * speed;
	}

	if (left)
	{
		moveDirection.x -= Camera.GetRightVector().x * speed;
		moveDirection.y -= Camera.GetRightVector().y * speed;
		moveDirection.z -= Camera.GetRightVector().z * speed;
	}

	// накопленное значение времени симуляции в мс
	static	float	mAccumulator = 0.0f;
	// минимальный шаг симуляции (1/60 секунды)
	static	float	mStepSize = 1.0f / 60.0f;
	// проверка необходимости симуляции и непосредственно симуляция если необходимо
	mAccumulator += time / 1000.0f;
	if (mAccumulator >= mStepSize)
	{
		mAccumulator -= mStepSize;

		// передвижение персонажа
		controller->move(moveDirection, 0.1, mStepSize, PxControllerFilters());
		
		// установка позиции камеры
		PxExtendedVec3	Positon = controller->getPosition();
		
		// контроль границ
		float border = 5.0f;
		if (Positon.x > width_plane / 2 - border)
			Positon.x = width_plane / 2 - border;
		if (Positon.x < -width_plane / 2 + border)
			Positon.x = -width_plane / 2 + border;
		if (Positon.z > width_plane / 2 - border)
			Positon.z = width_plane / 2 - border;
		if (Positon.z < -width_plane / 2 + border)
			Positon.z = -width_plane / 2 + border;

		Camera.SetPosition(Positon.x, Positon.y - 3, Positon.z);
		CameraShadow.SetCenter(vec3(Positon.x, Positon.y - 3, Positon.z));
		PxExtendedVec3 tmp = PxExtendedVec3(Positon.x, Positon.y, Positon.z);
		controller->setPosition(tmp);
	};
}

void Display(void)
{
	count_frame++;

	// *********** МИНИКАРТА ***************
	ShadowFBO.Start();
	glViewport(0, 0, FBO_w, FBO_h);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CPlane::SetCamera(&CameraShadow);
	CPlane::SetShader(&ShaderDepth);
	CObject::SetCamera(&CameraShadow);
	CObject::SetShader(&ShaderDepth);
	glCullFace(GL_FRONT);
	float x = 0, y = 0, z = 0;
	Camera.GetPosition(x, y, z);
	vec3 forwardVector = Camera.GetForwardVector();
	Camera.GetViewMatrix();
	forwardVector = normalize(forwardVector);

	// вывод объектов
	Plane.DrawPlane();
	DrawKubs();
	DrawAddedKubs();
	if (bullet != nullptr) 
		bullet->Draw();

	for (int i = 0; i < numTargets; i++)
		targets[i].Draw();

	// *********** ОСНОВНАЯ КАМЕРА ***************
	ShadowFBO.Stop();
	glViewport(0, 0, window_w, window_h);
	CPlane::SetCamera(&Camera);
	CPlane::SetShader(&ShaderMix);
	CObject::SetCamera(&Camera);
	glCullFace(GL_BACK);

	// очистка небом 
	glDepthFunc(GL_ALWAYS); glCullFace(GL_FRONT);
	CObject::SetShader(&ShaderSkyBox); SkyBox.Draw();
	glDepthFunc(GL_LESS); glCullFace(GL_BACK);

	CObject::SetShader(&ShaderDiffSpec);
	ShadowFBO.BindDepthTexture(GL_TEXTURE3);
	int tmp = 3;
	ShaderDiffSpec.SetUnoform1i("ShadowMap", &tmp);
	ShaderMix.SetUnoform1i("ShadowMap", &tmp);

	Plane.DrawPlane();
	DrawKubs();
	DrawAddedKubs();
	if (bullet != nullptr)
		bullet->Draw();
	CObject::SetShader(&ShaderPhong);
	for (int i = 0; i < numTargets; i++)
		targets[i].Draw();

	//вывод спрайта персонажа
	SpriteHero.DrawSprite();

	//ShadowFBO.Blit(window_w - 10, window_h - 10);

	glutSwapBuffers();

	//вывод FPS
	QueryPerformanceCounter((LARGE_INTEGER*)&tc2);
	if ((double)(tc2 - tc1) / TicsPerSec > 1)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&tc1);
		Title = "FPS: " + to_string(count_frame) + "; ";

		//учет режимов
		if (Mode_Face)
			Title += "[Full kub]; ";
		else
			Title += "[Line kub]; ";

		Title += "[Shadow qualeti = " + to_string(FBO_w) + "]; ";
		glutSetWindowTitle(Title.c_str());
		count_frame = 0;
	}
};

void Reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	Camera.SetProjectionMatrix(45.0, (float)w / h, 0.1f, 200.0);
	SpriteHero.SetScreenParam(w, h);
};

void Simulation(void)
{
	//	ОПРЕДЕЛЕНИЕ ВРЕМЕНИ ПРОШЕДШЕГО С МОМЕНТА ПОСЛЕДНЕЙ СИМУЛЯЦИИ
	QueryPerformanceCounter((LARGE_INTEGER*)&t2);
	Simulation_Time_Passed = (float)(t2 - t1) / TicsPerSec * 1000;
	QueryPerformanceCounter((LARGE_INTEGER*)&t1);

	// симуляция мыши
	MauseSimulation();

	// симуляция персонажа
	SimulatePersonMove(Simulation_Time_Passed);

	// СИМУЛЯЦИЯ ФИЗИКИ
	if (Simulate_PhysX)
	{
		for (int i = 0; i < (COUNT_KUB_ROWS + 1) * COUNT_KUB_ROWS / 2; i++)
			KubActor[i].Simulating(Simulation_Time_Passed * 0.1f);

		for (int i = 0; i < AddedIndex; i++)
			AddedKubActor[i].Simulating(Simulation_Time_Passed * 0.1f);

		/*for (int i = 0; i < numTargets; i++)
			TargetActor[i].Simulating(Simulation_Time_Passed * 0.1f);*/
	}
	
	// нажатия клавиш
	PressKeys();

	glutPostRedisplay();
};

void Enables()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	// инициализация таймеров
	QueryPerformanceCounter((LARGE_INTEGER*)&t1);
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	
	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowPosition(1600 - window_w, 900 - window_h);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Race");
	GLenum err = glewInit();

	// загрузка шейдеров
	if (!ShaderDiffSpec.Load("SHADER\\DiffuseAndSpecular_Shadow.vsh", "SHADER\\DiffuseAndSpecular_Shadow.fsh"))
	{
		cout << "Don't find shader file!\n";
		exit(0);
	}

	if (!ShaderMix.Load("SHADER\\Mix_Shadow.vsh", "SHADER\\Mix_Shadow.fsh"))
	{
		cout << "Don't find shader file!\n";
		exit(0);
	}

	if (!ShaderSkyBox.Load("SHADER\\SkyBox.vsh", "SHADER\\SkyBox.fsh"))
	{
		cout << "Don't find shader file!\n";
		exit(0);
	}

	if (!ShaderDepth.Load("SHADER\\DepthOnly.vsh", "SHADER\\DepthOnly.fsh"))
	{
		cout << "Don't find shader file!\n";
		exit(0);
	}

	if (!ShaderPhong.Load("SHADER\\Phong.vsh", "SHADER\\Phong.fsh"))
	{
		cout << "Don't find shader file!\n";
		exit(0);
	}

	// включение общих параметрров
	Enables();
	
	// инициализация  PhysX
	PhysX.Init();

	// инициализация библиотеки DevIL
	ilInit();
	iluInit();
	ilutInit();

	InitObject();

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Simulation);
	
	glutMainLoop();
};