#pragma once
#include "CShader.h"

//	 класс для работы с материалом
class CMaterial
{
public:
	// применение
	virtual void Apply(CShader* shader);
};