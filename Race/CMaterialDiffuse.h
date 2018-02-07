#pragma once
#include "CMaterial.h"
#include "CTexture.h"
#include "glm\glm.hpp"

class CMaterialDiffuse : public CMaterial
{
public:
	CMaterialDiffuse();

	// задание параметров материала
	void	SetDiffuse(vec4 value);
	void	SetAmbient(vec4 value);
	void	SetSpecular(vec4 value);
	void	SetShiness(float value);

	// установка текстуры
	void	SetTexture(CTexture* t);

	// применение
	virtual void Apply(CShader* shader);

private:
	CTexture* Tex;
	vec4 Ambient;
	vec4 Diffuse;
	vec4 Specular;
	float Shiness;
};