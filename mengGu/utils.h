#pragma once
#include "ggl.h"
unsigned char * LoadFileContent(const char * path, int & filesize);
//����BMP
unsigned char * DecodeBMP(unsigned char * bmpFileData, int & width, int & height);
//�����������ݴ���һ���������
GLuint CreateTexture2D(unsigned char * pixelData, int width, int height, GLenum type);
//��һ��BMP�ļ��д���һ������������������������ۺϣ�
GLuint CreateTexture2DFromBMP(const char * bmpPath);
//
GLuint CreateTextureCubeFromBMP(const char * front, const char * back, const char * left,
	const char * right, const char * top, const char * bottom);