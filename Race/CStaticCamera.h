#pragma once
#include "CCameraBase.h"

using namespace glm;

class CStaticCamera : public CCameraBase
{
	vec3 Eye;
	vec3 Center;
	vec3 Up;
public:
	CStaticCamera();
	float* GetViewMatrix(void);
	void SetPosition(vec3 p);
	void SetCenter(vec3 p);
};