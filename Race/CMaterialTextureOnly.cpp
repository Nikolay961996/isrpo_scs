#include "CMaterialTextureOnly.h"

CMaterialTextureOnly::CMaterialTextureOnly()
{
	Tex = NULL;
}

void CMaterialTextureOnly::SetTexture(CTexture* t)
{
	Tex = t;
}

void CMaterialTextureOnly::Apply(CShader* shader)
{
	Tex->ApplyCubeMap();
	int block = Tex->GetNumTex();
	shader->SetUnoform1i(Tex->GetNameTex(), &block);
}