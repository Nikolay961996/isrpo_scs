#include "CMaterialDiffuse.h"

CMaterialDiffuse::CMaterialDiffuse()
{
	Ambient = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	Diffuse = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	Specular = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	Shiness = 0.0f;
}

void CMaterialDiffuse::SetAmbient(vec4 value)
{
	Ambient = value;
}

void CMaterialDiffuse::SetDiffuse(vec4 value)
{
	Diffuse = value;
}

void CMaterialDiffuse::SetSpecular(vec4 value)
{
	Specular = value;
}

void CMaterialDiffuse::SetShiness(float value)
{
	Shiness = value;
}

void CMaterialDiffuse::SetTexture(CTexture* t)
{
	Tex = t;
}

void CMaterialDiffuse::Apply(CShader* shader)
{
	shader->SetUniformVec4("mAmbient", value_ptr(Ambient));
	shader->SetUniformVec4("mDiffuse", value_ptr(Diffuse));
	shader->SetUniformVec4("mSpecular", value_ptr(Specular));
	shader->SetUniform1f("mShiness", &Shiness);

	Tex->Apply();
	int block = Tex->GetNumTex();
	shader->SetUnoform1i(Tex->GetNameTex(), &block);
}