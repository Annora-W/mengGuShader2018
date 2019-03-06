#pragma once
#include "ggl.h"
unsigned char * LoadFileContent(const char * path, int & filesize);
//解码BMP
unsigned char * DecodeBMP(unsigned char * bmpFileData, int & width, int & height);
//利用像素数据创建一个纹理对象
GLuint CreateTexture2D(unsigned char * pixelData, int width, int height, GLenum type);
//从一个BMP文件中创建一个纹理对象（上述两个函数的综合）
GLuint CreateTexture2DFromBMP(const char * bmpPath);
//
GLuint CreateTextureCubeFromBMP(const char * front, const char * back, const char * left,
	const char * right, const char * top, const char * bottom);