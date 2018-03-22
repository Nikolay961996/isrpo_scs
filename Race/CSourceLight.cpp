#include "CSourceLight.h"

CSourceLight::CSourceLight()
{
	HaveAmbient = false;
	HaveDiffuse = false;
	HaveSpecular = false;
	HavePosition = false;
}

void CSourceLight::SetAmbient(vec4 value)
{
	Ambient = value;
	HaveAmbient = true;
}

void CSourceLight::SetDiffuse(vec4 value)
{
	Diffuse = value;
	HaveDiffuse = true;
}

void CSourceLight::SetSpecular(vec4 value)
{
	Specular = value;
	HaveSpecular = true;
}

void CSourceLight::SetPosition(vec4 value)
{
	Position = value;
	HavePosition = true;
}

void	CSourceLight::Apply(CShader* s, CCameraBase* c)
{
	if(HaveAmbient)
		s->SetUniformVec4("lAmbient", value_ptr(Ambient));
	if (HaveDiffuse)
		s->SetUniformVec4("lDiffuse", value_ptr(Diffuse));
	if (HaveSpecular)
		s->SetUniformVec4("lSpecular", value_ptr(Specular));
	if (HavePosition)
		s->SetUniformVec4("lPosition", value_ptr(make_mat4x4(c->GetViewMatrix()) * Position));
}