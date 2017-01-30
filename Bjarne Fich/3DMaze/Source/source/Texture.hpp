#ifndef __Texture_HPP_
#define __Texture_HPP_

#include <gl\gl.h>

#include "Image.hpp"
#define BPP2GLTYPE(a) (((a)==24)?(GL_RGB):(GL_RGBA))
#define BPP2GLFORMAT(a) (((a)==24)?(GL_RGB8):(GL_RGBA8))

class TEXTURELIST // Nodes
{
  private:
    GLuint tex;
    char *fname;
    TEXTURELIST *next;

  public:
    TEXTURELIST();              // Initialize
    ~TEXTURELIST();             // Free
    GLuint Get(char *filename); // Get/Add a texture to the list from a file
};

GLuint CopyImage2glTexture(Image * image);

#endif /* __Texture_HPP_ */

