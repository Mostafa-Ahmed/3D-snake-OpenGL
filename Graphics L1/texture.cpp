#include "texture.h"
#include "stb_image.h"
#include <iostream>

#include <stdlib.h>
#include <windows.h>
#include <stdio.h>

using namespace std;


Texture::Texture(const std::string fileName[6], int texUnit_)
{
	texUnit = texUnit_;
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
	for (int i = 0; i < 6; ++i) {
		unsigned char* data = stbi_load((fileName[i]).c_str(), &width, &height, &numComponents, 4);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data);
		stbi_image_free(data);
	}
}


Texture::Texture(const std::string& fileName, int texUnit_)
{
	unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);
	texUnit = texUnit_;
	if (data == NULL)
		cout << "Unable to load texture: " << fileName << endl;

	glActiveTexture(texUnit);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

Texture::Texture(const char* fileName, int texUnit_,int l)
{
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(fileName, 0);
	FIBITMAP* imagen = FreeImage_Load(formato, fileName);

	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);

	GLubyte* textura = new GLubyte[4 * w*h];
	char* pixeles = (char*)FreeImage_GetBits(imagen);

	for (int j = 0; j<w*h; j++){
		textura[j * 4 + 0] = pixeles[j * 4 + 2];
		textura[j * 4 + 1] = pixeles[j * 4 + 1];
		textura[j * 4 + 2] = pixeles[j * 4 + 0];
		textura[j * 4 + 3] = pixeles[j * 4 + 3];
	}


	texUnit = texUnit_;
	glActiveTexture(texUnit);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}


Texture::Texture(char* fileName, int texUnit_, bool billboard, int l)
{
	texUnit = texUnit_;
	unsigned char	*texels = 0;
	int				width, height;
	int				success = 0;
	success = LoadFileBMP(fileName, &texels, &width, &height,false);
	glActiveTexture(texUnit);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);
}


int Texture::LoadFileBMP(char *filename, unsigned char **pixels, int *width, int *height, bool flipvert)
{
	FILE*		file;
	BITMAPFILEHEADER	*bmfh;
	BITMAPINFOHEADER	*bmih;
	BITMAPCOREHEADER	*bmch;
	RGBTRIPLE			*os2_palette;
	RGBQUAD				*win_palette;
	char				*buffer;
	unsigned char		*ptr;
	int					bitCount;
	int					compression;
	int					row, col, i;
	int					w, h;
	file = fopen(filename, "rb");
	if (file == NULL)
		return 0;
	fseek(file, 0, SEEK_END);
	long flen = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = new char[flen + 1];
	fread(buffer, flen, 1, file);
	char *pBuff = buffer;
	fclose(file);
	bmfh = (BITMAPFILEHEADER *)pBuff;
	pBuff += sizeof(BITMAPFILEHEADER);
	printf("%c", bmfh->bfType);
	if (bmfh->bfType != BITMAP_ID)
	{
		delete[] buffer;
		return 0;
	}
	bmch = (BITMAPCOREHEADER *)pBuff;
	bmih = (BITMAPINFOHEADER *)pBuff;
	if ((bmih->biCompression < 0) || (bmih->biCompression > 3))
	{
		pBuff += sizeof(BITMAPCOREHEADER);
		bitCount = bmch->bcBitCount;
		compression = BI_OS2;
		w = bmch->bcWidth;
		h = bmch->bcHeight;
	}
	else
	{
		pBuff += sizeof(BITMAPINFOHEADER);
		bitCount = bmih->biBitCount;
		compression = bmih->biCompression;
		w = bmih->biWidth;
		h = bmih->biHeight;
	}
	if (width)
		*width = w;
	if (height)
		*height = h;
	if (!pixels)
	{
		delete[] buffer;
		return (-1);
	}
	if (bitCount <= 8)
	{
		os2_palette = (RGBTRIPLE *)pBuff;
		win_palette = (RGBQUAD *)pBuff;
		pBuff += (1 << bitCount) * (bitCount >> 3) * sizeof(unsigned char);
	}
	*pixels = new unsigned char[w * h * 4];
	ptr = &(*pixels)[0];
	pBuff = buffer + (bmfh->bfOffBits * sizeof(char));
	switch (compression)
	{
	case BI_OS2:
	case BI_RGB:
	{
				   for (row = h - 1; row >= 0; row--)
				   {
					   if (flipvert)
						   ptr = &(*pixels)[row * w * 4];
					   switch (bitCount)
					   {
					   case 1:
					   {
								 for (col = 0; col < (int)(w / 8); col++)
								 {
									 unsigned char color = *((unsigned char *)(pBuff++));
									 for (i = 7; i >= 0; i--, ptr += 4)
									 {
										 int clrIdx = ((color & (1 << i)) > 0);
										 if (compression == BI_OS2)
										 {
											 ptr[0] = os2_palette[clrIdx].rgbtRed;
											 ptr[1] = os2_palette[clrIdx].rgbtGreen;
											 ptr[2] = os2_palette[clrIdx].rgbtBlue;
											 ptr[3] = 255;
										 }
										 else
										 {
											 ptr[0] = win_palette[clrIdx].rgbRed;
											 ptr[1] = win_palette[clrIdx].rgbGreen;
											 ptr[2] = win_palette[clrIdx].rgbBlue;
											 ptr[3] = 255;
										 }
									 }
								 }

								 break;
					   }

					   case 4:
					   {
								 for (col = 0; col < (int)(w / 2); col++, ptr += 8)
								 {
									 unsigned char color = *((unsigned char *)(pBuff++));
									 int clrIdx;
									 if (compression == BI_OS2)
									 {
										 clrIdx = (color >> 4);
										 ptr[0] = os2_palette[clrIdx].rgbtRed;
										 ptr[1] = os2_palette[clrIdx].rgbtGreen;
										 ptr[2] = os2_palette[clrIdx].rgbtBlue;
										 ptr[3] = 255;

										 clrIdx = (color & 0x0F);
										 ptr[4] = os2_palette[clrIdx].rgbtRed;
										 ptr[5] = os2_palette[clrIdx].rgbtGreen;
										 ptr[6] = os2_palette[clrIdx].rgbtBlue;
										 ptr[7] = 255;
									 }
									 else
									 {
										 clrIdx = (color >> 4);
										 ptr[0] = win_palette[clrIdx].rgbRed;
										 ptr[1] = win_palette[clrIdx].rgbGreen;
										 ptr[2] = win_palette[clrIdx].rgbBlue;
										 ptr[3] = 255;

										 clrIdx = (color & 0x0F);
										 ptr[4] = win_palette[clrIdx].rgbRed;
										 ptr[5] = win_palette[clrIdx].rgbGreen;
										 ptr[6] = win_palette[clrIdx].rgbBlue;
										 ptr[7] = 255;
									 }
								 }

								 break;
					   }

					   case 8:
					   {
								 for (col = 0; col < w; col++, ptr += 4)
								 {
									 unsigned char color = *((unsigned char *)(pBuff++));
									 if (compression == BI_OS2)
									 {
										 ptr[0] = os2_palette[color].rgbtRed;
										 ptr[1] = os2_palette[color].rgbtGreen;
										 ptr[2] = os2_palette[color].rgbtBlue;
										 ptr[3] = 255;
									 }
									 else
									 {
										 ptr[0] = win_palette[color].rgbRed;
										 ptr[1] = win_palette[color].rgbGreen;
										 ptr[2] = win_palette[color].rgbBlue;
										 ptr[3] = 255;
									 }
								 }

								 break;
					   }

					   case 24:
					   {
								  for (col = 0; col < w; col++, ptr += 4)
								  {
									  RGBTRIPLE *pix = (RGBTRIPLE *)pBuff;
									  pBuff += sizeof(RGBTRIPLE);

									  ptr[0] = pix->rgbtRed;
									  ptr[1] = pix->rgbtGreen;
									  ptr[2] = pix->rgbtBlue;
									  ptr[3] = 255;
								  }

								  break;
					   }

					   case 32:
					   {
								  for (col = 0; col < w; col++, ptr += 4)
								  {
									  RGBQUAD *pix = (RGBQUAD *)pBuff;
									  pBuff += sizeof(RGBQUAD);
									  ptr[0] = pix->rgbRed;
									  ptr[1] = pix->rgbGreen;
									  ptr[2] = pix->rgbBlue;
									  ptr[3] = 255;
								  }

								  break;
					   }
					   }
				   }

				   break;
	}

	case BI_RLE8:
	{
					for (row = h - 1; row >= 0; row--)
					{
						if (flipvert)
							ptr = &(*pixels)[row * w * 4];
						for (col = 0; col < w; /* nothing */)
						{
							unsigned char byte1 = *((unsigned char *)(pBuff++));
							unsigned char byte2 = *((unsigned char *)(pBuff++));
							if (byte1 == RLE_COMMAND)
							{
								for (i = 0; i < byte2; i++, ptr += 4, col++)
								{
									unsigned char color = *((unsigned char *)(pBuff++));
									ptr[0] = win_palette[color].rgbRed;
									ptr[1] = win_palette[color].rgbGreen;
									ptr[2] = win_palette[color].rgbBlue;
									ptr[3] = 255;
								}

								if ((byte2 % 2) == 1)
									pBuff++;
							}
							else
							{
								for (i = 0; i < byte1; i++, ptr += 4, col++)
								{
									ptr[0] = win_palette[byte2].rgbRed;
									ptr[1] = win_palette[byte2].rgbGreen;
									ptr[2] = win_palette[byte2].rgbBlue;
									ptr[3] = 255;
								}
							}
						}
					}

					break;
	}

	case BI_RLE4:
	{
					unsigned char color;
					int	bytesRead = 0;
					for (row = h - 1; row >= 0; row--)
					{
						if (flipvert)
							ptr = &(*pixels)[row * w * 4];

						for (col = 0; col < w; /* nothing */)
						{
							unsigned char byte1 = *((unsigned char *)(pBuff++));
							unsigned char byte2 = *((unsigned char *)(pBuff++));

							bytesRead += 2;
							if (byte1 == RLE_COMMAND)
							{
								unsigned char databyte;
								for (i = 0; i < byte2; i++, ptr += 4, col++)
								{
									if ((i % 2) == 0)
									{
										databyte = *((unsigned char *)(pBuff++));
										bytesRead++;

										color = (databyte >> 4);
									}
									else
									{
										color = (databyte & 0x0F);
									}
									ptr[0] = win_palette[color].rgbRed;
									ptr[1] = win_palette[color].rgbGreen;
									ptr[2] = win_palette[color].rgbBlue;
									ptr[3] = 255;
								}

								while ((bytesRead % 2) != 0)
								{
									pBuff++;
									bytesRead++;
								}
							}
							else
							{
								for (i = 0; i < byte1; i++, ptr += 4, col++)
								{
									if ((i % 2) == 0)
										color = (byte2 >> 4);
									else
										color = (byte2 & 0x0F);
									ptr[0] = win_palette[color].rgbRed;
									ptr[1] = win_palette[color].rgbGreen;
									ptr[2] = win_palette[color].rgbBlue;
									ptr[3] = 255;
								}
							}
						}
					}

					break;
	}
	}

	delete[] buffer;
	return 1;
}


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
GLuint loadDDS(const char * imagepath){

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}

	free(buffer);

	return textureID;


}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::Bind()
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
