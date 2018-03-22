#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

using namespace glm;

class CCameraBase
{
protected:
	// матрица проекции
	mat4	ProjectionMatrix;
	// матрица наблюдения 
	mat4	ViewMatrix;
public:
	virtual float* GetViewMatrix(void) = 0;
	
	float* GetProjectionMatrix(void)
	{
		return value_ptr(ProjectionMatrix);
	};

	void SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
	{
		ProjectionMatrix = perspective(fovy, aspect, zNear, zFar);
	}
};