#include "CCamera.h"

CCamera::CCamera(void)
{
	std::ifstream inf("Camera_save.txt");
	inf >> YawAngle;
	inf >> PitchAngle;
}

CCamera::~CCamera(void)
{
	std::ofstream outf("Camera_save.txt");
	outf << YawAngle << std::endl << PitchAngle;
	outf.close();
}

void	CCamera::Left(int value)
{
	YawAngle -= value * 0.005f;

	if (YawAngle < -6.2832f)
		YawAngle = 0;
}

void	CCamera::Right(int value)
{
	YawAngle += value * 0.005f;

	if (YawAngle > 6.2832f)
		YawAngle = 0;
}

void	CCamera::Up(int value)
{
	PitchAngle += value * 0.005f;

	if (PitchAngle > 1.57f)
		PitchAngle = 1.57f;
}

void	CCamera::Down(int value)
{
	PitchAngle -= value * 0.005f;

	if (PitchAngle < -1.57f)
		PitchAngle = -1.57f;
}

vec3	CCamera::GetRightVector(void)
{
	return right;
}

vec3	CCamera::GetForwardVector(void)
{
	return forward;
}

void	CCamera::SetPosition(float x, float y, float z)
{
	Position.x = x;
	Position.y = y;
	Position.z = z;
}

void	CCamera::GetPosition(float &x, float &y, float &z)
{
	x = Position.x;
	y = Position.y;
	z = Position.z;
}

float* CCamera::GetViewMatrix(void)
{
	forward.x = cos(PitchAngle) * cos(YawAngle);
	forward.y = sin(PitchAngle);
	forward.z = cos(PitchAngle) * sin(YawAngle);
	forward = normalize(forward);

	vec3 tmp_up;
	tmp_up = vec3(0, 1, 0);
	right = cross(forward, tmp_up);
	right = normalize(right);

	up = cross(right, forward);
	up = normalize(up);

	mat4 m0 = mat4(right.x, up.x, -forward.x, 0.0,
		right.y, up.y, -forward.y, 0.0,
		right.z, up.z, -forward.z, 0.0,
		0, 0, 0, 1.0);

	mat4 m1 = mat4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-Position.x, -Position.y, -Position.z, 1);

	ViewMatrix = m0 * m1;

	return value_ptr(ViewMatrix);
}