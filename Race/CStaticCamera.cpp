#include "CStaticCamera.h"

CStaticCamera::CStaticCamera()
{
	Eye = vec3(0, 0, 0);
	Center = vec3(0, 0, 0);
	Up = vec3(0, 1, 0);
}

float* CStaticCamera::GetViewMatrix(void)
{
	ViewMatrix = lookAt(Eye, Center, Up);
	return value_ptr(ViewMatrix);
}

void CStaticCamera::SetPosition(vec3 p)
{
	Eye = p;
}

void CStaticCamera::SetCenter(vec3 p)
{
	Center = p;
}
