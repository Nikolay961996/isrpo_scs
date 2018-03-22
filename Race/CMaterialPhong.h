#pragma once
#include "CMaterial.h"
#include "glm\glm.hpp"

class CMaterialPhong: public CMaterial
{
public:
	CMaterialPhong();

	// задание параметров материала
	void	SetDiffuse(vec4 value);
	void	SetAmbient(vec4 value);
	void	SetSpecular(vec4 value);
	void	SetShiness(float value);

	// применение
	virtual void Apply(CShader* shader);

private:
	vec4 Ambient;
	vec4 Diffuse;
	vec4 Specular;
	float Shiness;
};