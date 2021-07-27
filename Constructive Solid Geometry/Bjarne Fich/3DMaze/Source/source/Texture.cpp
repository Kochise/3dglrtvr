#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <fstream.h>
#include <ctype.h>

#include "Texture.hpp"
#include "FileProcs.hpp"
#include "Log.hpp"
#include "Image.hpp"

// **********************************************************
// Texturelist is used as an dynamic array to hold textures..
// **********************************************************

TEXTURELIST::TEXTURELIST()
{
  tex = 0;
  fname = 0;
  next = 0;
}

TEXTURELIST::~TEXTURELIST()
{
  if (tex!=0) glDeleteTextures(1, &tex);
  if (fname!=0) delete [] fname;
  if (next!=0) delete next;
}

GLuint TEXTURELIST::Get(char *filename)
{
  Image *image;
  if ((next==0) || (fname==0))
  {
    if (tex==0)
    {
      fname = new char [strlen(filename)+1];
      strcpy(fname, filename);
      image = new Image(fname);
      tex = CopyImage2glTexture(image);
      next = 0;
      LOG("Get texture '%s', create new (first) id '%i'", filename, tex);
      return tex;
    }
    else
    {
      next = new TEXTURELIST();
      next->fname = new char [strlen(filename)+1];
      strcpy(next->fname, filename);
      image = new Image(next->fname);
      next->tex = CopyImage2glTexture(image);
      next->next = 0;
      LOG("Get texture '%s', create new id '%i'", filename, next->tex);
      return next->tex;
    }
  }
  else if (strcmp(filename,fname)==0)
  {
    LOG("Get texture '%s', found id '%i'", filename, tex);
    return tex;
  }
  else { return next->Get(filename); }
}

/******************************************************/
/*                                                    */
/* Bind image to glTexture                            */
/*                                                    */
/******************************************************/

GLuint CopyImage2glTexture(Image * image)
{
  GLuint texture;

/*
  // allocate a texture name
  glGenTextures( 1, &texture );

  // select our current texture
  glBindTexture( GL_TEXTURE_2D, texture );

  // select modulate to mix texture with color for shading
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

  // when texture area is small, bilinear filter the closest mipmap
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
  // when texture area is large, bilinear filter the first mipmap
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // if wrap is true, the texture wraps over at the edges (repeat)
  //       ... false, the texture ends at the edges (clamp)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP );
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP );

  // build our texture mipmaps
  //gluBuild2DMipmaps( GL_TEXTURE_2D, BPP2GLTYPE(image->bpp), image->width, image->height, BPP2GLTYPE(image->bpp), GL_UNSIGNED_BYTE,  image->imageData);
*/

  glGenTextures(1, &texture );
  glBindTexture(GL_TEXTURE_2D, texture );
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexImage2D( 
   	GL_TEXTURE_2D,						// Target
		0,												// Detail level 
		BPP2GLFORMAT(image->bpp), // Internal Texture Formats (Use GL_RGB, GL_RGBA, GL_RGB8, GL_RGB4, GL_RGBA8 ...tec)
		image->width, 
		image->height, 
		0,												// Border size 
		BPP2GLTYPE(image->bpp) , 	// (Use GL_RGB or GL_RGBA)
		GL_UNSIGNED_BYTE,					// index_type can be GL_UNSIGNED_BYTE or GL_UNSIGNED_SHORT 
		image->imageData					// IndexMap is defined as (unsigned short)* or (unsigned char)* 
  ); 

  return texture;
}

