#pragma once
#include "CShader.h"
#include "CCamera.h"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"

using namespace glm;

//	 класс для работы с источником света
class CSourceLight
{
	vec4 Ambient;
	vec4 Diffuse;
	vec4 Specular;
	vec4 Position;

	bool HaveAmbient;
	bool HaveDiffuse;
	bool HaveSpecular;
	bool HavePosition;

public:
	//	конструктор по умолчанию
	CSourceLight(void);

	// задание параметров источником света
	void	SetDiffuse(vec4 value);
	void	SetAmbient(vec4 value);
	void	SetSpecular(vec4 value);
	void	SetPosition(vec4 value);

	// установка параметров источником света
	void	Apply(CShader* s, CCameraBase* c);
};