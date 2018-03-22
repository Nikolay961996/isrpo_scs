#include "CSprite.h"

CSprite::CSprite()
{
	height = 0;
	width = 0;
	Data = NULL;
}

int CSprite::ScreenWidth = 0;
int CSprite::ScreenHeight = 0;

void CSprite::SetScreenParam(int w, int h)
{
	ScreenWidth = w;
	ScreenHeight = h;
}

void	CSprite::Load(const char* filename)
{

	ilGenImages(1, &ImageId);
	// выбор текущего изображения
	ilBindImage(ImageId);

	char C_String[50];
	C_String[0] = '\0';
	strcat_s(C_String, "Images/");
	strcat_s(C_String, filename);
	wchar_t* Unicode_String = new wchar_t[50];
	wsprintf(Unicode_String, L"%S", C_String);

	if (ilLoadImage(Unicode_String))
		cout << "Image success loaded" << endl;
	else
		cout << "Error of load image" << endl;

	//	получение параметров загруженного изображения
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	// копирование пикселей из памяти DevIL в Data
	Data = new GLubyte[width*height * 4];
	ilCopyPixels(0,0,0,width, height, 1, IL_RGBA, IL_UNSIGNED_BYTE, Data);

	//Загрузка спрайта в буфер GL_PIXEL_UNPACK_BUFFER
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, bufferID);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, width*height*4*sizeof(GLubyte), Data, GL_STATIC_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	
	delete Data;
	ilDeleteImages(1, &ImageId);

};

void	CSprite::DrawSprite()
{
	//Вывод спрайта в буфер кадра
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float coeffX = 1.0f, coeffY = -1.0f;
	//установка координат левого нижнего угла
	glWindowPos2f(ScreenWidth/2.0f - width/2.0f, height);
	glPixelZoom(coeffX, coeffY);

	//выравнивание
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	//вывод пикселей изображения
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, bufferID);
	glDrawPixels(width, height, IL_RGBA, IL_UNSIGNED_BYTE, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glDisable(GL_BLEND);
};


