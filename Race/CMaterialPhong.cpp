#include "CMaterialPhong.h"

CMaterialPhong::CMaterialPhong()
{
	Ambient = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	Diffuse = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	Specular = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	Shiness = 0.0f;
}

void CMaterialPhong::SetAmbient(vec4 value)
{
	Ambient = value;
}

void CMaterialPhong::SetDiffuse(vec4 value)
{
	Diffuse = value;
}

void CMaterialPhong::SetSpecular(vec4 value)
{
	Specular = value;
}

void CMaterialPhong::SetShiness(float value)
{
	Shiness = value;
}

void CMaterialPhong::Apply(CShader* shader)
{
	shader->SetUniformVec4("mAmbient", value_ptr(Ambient));
	shader->SetUniformVec4("mDiffuse", value_ptr(Diffuse));
	shader->SetUniformVec4("mSpecular", value_ptr(Specular));
	shader->SetUniform1f("mShiness", &Shiness);
}