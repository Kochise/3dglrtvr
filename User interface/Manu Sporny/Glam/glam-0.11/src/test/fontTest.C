/*
 * Copyright (c) 2002-2004, xRhino, Inc.
 *
 * This library is free software; you can redistribute and/or modify
 * it under the terms of the dual license distributed with this source
 * package (a combined Trolltech QPL/GPL-compatible license)
 *
 * Under this dual license scheme, a separate license is available for use of 
 * this code with a proprietary (closed) system. For more information on the 
 * license, please see the LICENSE file distributed with this source package.
 */
#include <glam/GmFontManager.h>
#include <iostream>
#include <GL/glut.h>

GmFontManager& gFontManager = GmFontManager::getInstance();
OGLFT::Face* face;

void init(const char* filename)
{
   GlmColor4 fgcolor;
   fgcolor.setRGBA(0.75, 1.0, 0.75, 1.0);

   face = gFontManager.getFontFace(filename, 36, fgcolor,
                                   GmTypes::FT_P_FILLED);
   
   if(face == NULL || !face->isValid())
   {
      cerr << "Could not construct face from " << filename << endl;
      exit(0);
   }
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   glEnable(GL_TEXTURE_2D);
   glClearColor(0.0, 0.0, 0.0, 1.0);
}

static void display (void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   face->draw(0., 250., "Hello, World!");
}

static void reshape(int width, int height)
{
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, width, 0, height, -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char* argv[])
{
   if(argc != 2)
   {
      cerr << "usage: " << argv[0] << " fontfile" << endl;
      return 1;
   }
   glutInit(&argc, argv);
   glutInitWindowSize(500, 500);
   glutInitDisplayMode(GLUT_RGB);
   glutCreateWindow(argv[0]);
   init(argv[1]);
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}
