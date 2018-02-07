#include "CMaterialDiffSpec.h"

CMaterialDiffSpec::CMaterialDiffSpec()
{
	Tex_Diff = NULL;
	Tex_Spec = NULL;
	Diffuse = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	Shiness = 0.0f;
}

void CMaterialDiffSpec::SetDiffuse(vec4 value)
{
	Diffuse = value;
}

void CMaterialDiffSpec::SetShiness(float value)
{
	Shiness = value;
}

void CMaterialDiffSpec::SetTextures(CTexture* d, CTexture* s)
{
	Tex_Diff = d;
	Tex_Spec = s;
}

void CMaterialDiffSpec::Apply(CShader* shader)
{
	shader->SetUniformVec4("mDiffuse", value_ptr(Diffuse));
	shader->SetUniform1f("mShiness", &Shiness);

	int block;

	Tex_Diff->Apply();
	block = Tex_Diff->GetNumTex();
	shader->SetUnoform1i(Tex_Diff->GetNameTex(), &block);

	Tex_Spec->Apply();
	block = Tex_Spec->GetNumTex();
	shader->SetUnoform1i(Tex_Spec->GetNameTex(), &block);
}