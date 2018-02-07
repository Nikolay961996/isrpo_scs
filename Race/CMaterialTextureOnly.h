#pragma once
#include "CMaterial.h"
#include "CTexture.h"

class CMaterialTextureOnly : public CMaterial
{
public:
	CMaterialTextureOnly();

	// установка текстуры
	void	SetTexture(CTexture* t);

	// применение
	virtual void Apply(CShader* shader);

private:
	CTexture* Tex;
};