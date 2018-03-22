#include "CMaterialMix.h"

CMaterialMix::CMaterialMix()
{
	Tex_Mask = NULL;
	Tex_1 = NULL;
	Tex_2 = NULL;
	Ambient = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	Diffuse = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

void CMaterialMix::SetAmbient(vec4 value)
{
	Ambient = value;
}

void CMaterialMix::SetDiffuse(vec4 value)
{
	Diffuse = value;
}

void CMaterialMix::SetTextures(CTexture* mask, CTexture* t1, CTexture* t2)
{
	Tex_Mask = mask;
	Tex_1 = t1;
	Tex_2 = t2;
}

void CMaterialMix::Apply(CShader* shader)
{
	shader->SetUniformVec4("mAmbient", value_ptr(Ambient));
	shader->SetUniformVec4("mDiffuse", value_ptr(Diffuse));

	int block;

	Tex_Mask->Apply();
	block = Tex_Mask->GetNumTex();
	shader->SetUnoform1i(Tex_Mask->GetNameTex(), &block);

	Tex_1->Apply();
	block = Tex_1->GetNumTex();
	shader->SetUnoform1i(Tex_1->GetNameTex(), &block);

	Tex_2->Apply();
	block = Tex_2->GetNumTex();
	shader->SetUnoform1i(Tex_2->GetNameTex(), &block);
}