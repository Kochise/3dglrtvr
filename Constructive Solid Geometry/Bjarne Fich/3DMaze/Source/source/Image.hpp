#ifndef __Image_H__
#define __Image_H__

#pragma comment(lib, "Opengl32.lib")					//Link to OpenGL32.lib so we can use OpenGL stuff

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
//#include <gl\gl.h>
//#include <gl\glu.h>

class Image
{
  public:
/*
  	GLubyte	*imageData;						// Image Data (Up To 32 Bits)
  	GLuint	bpp;									// Image Color Depth In Bits Per Pixel
  	GLuint	width;								// Image Width
  	GLuint	height;								// Image Height
  	GLuint	texID;								// Texture ID Used To Select A Texture
  	GLuint	type;									// Image Type (GL_RGB, GL_RGBA)
*/
  	unsigned char	*imageData;						// Image Data (Up To 32 Bits)
  	unsigned int	bpp;									// Image Color Depth In Bits Per Pixel
  	unsigned int	width;								// Image Width
  	unsigned int	height;								// Image Height
//  	GLuint	texID;								// Texture ID Used To Select A Texture
//  	GLuint	type;									// Image Type (GL_RGB, GL_RGBA)

    Image(char *filename);
    ~Image();
};

#endif /* __Image_H__ */
