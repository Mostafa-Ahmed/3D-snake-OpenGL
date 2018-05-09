#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include<vector>
#include "gl/glew.h"
#undef _UNICODE
#include "IL\ilut.h"
#include "FreeImage.h"

#define			BITMAP_ID			('B' + ('M'<<8))
#define			RLE_COMMAND			0
#define			RLE_ENDOFLINE		0
#define			RLE_ENDOFBITMAP		1
#define			RLE_DELTA			2

#define			BI_OS2				-1

GLuint loadDDS(const char * imagepath);

class Texture
{

public:
	bool loaded;
	Texture(const std::string& fileName, int texUnit,int l);
	/*unsigned*/ int width, height;
	int numComponents;
	unsigned char * data;
	int texUnit;
	void Bind();
	Texture::Texture(const std::string fileName[6], int texUnit_);
	Texture(const std::string& fileName, int texUnit);
	Texture(const char* fileName, int texUnit_,int l);
	Texture(char* fileName, int texUnit,bool billboard,int l);
	void LoadImageFile(const wchar_t* filename);
	void BindTexture();
	virtual ~Texture();
	GLuint m_texture;

protected:
private:
	int LoadFileBMP(char *filename, unsigned char **pixels, int *width, int *height, bool flipvert);
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}
};

#endif
//GLuint loadBMP_custom(const char * imagepath);