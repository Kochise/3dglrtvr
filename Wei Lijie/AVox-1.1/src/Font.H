#ifndef __FONT_H
#define __FONT_H

#include "Log.H"
#include "Includes.H"
#include "Textures.H"

class Font 
{
private:
	GLuint	FontTexture;
	GLuint	FontBase;
	GLint xWidth, xHeight;

public:
	Font();
	~Font();

	void Init(int xScreen, int yScreen);
	bool BuildFont();
	void glPrint(GLint Xp, GLint Yp, char Set, const char *String, ...);
};

#endif
