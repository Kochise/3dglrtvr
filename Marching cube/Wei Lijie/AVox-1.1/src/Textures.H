#ifndef __Textures
#define __Textures

#include <Windows.H>
#include <StdIo.H>
#include <GL\GLu.H>	


class Tga
{
public:
	GLubyte * cMap;
	// 
	GLuint	BPP, Width, Height;
	// 
	GLenum Format;
	// 
	GLint Components;

	Tga();
	~Tga();
	
	bool Load(const char *FileName);
};


#endif