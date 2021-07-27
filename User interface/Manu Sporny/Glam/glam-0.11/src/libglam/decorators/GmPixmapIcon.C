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

#include <glam/GmPixmapIcon.h>
#include <glam/GmFileCache.h>
 
GmPixmapIcon::GmPixmapIcon()
      : GmIcon()
{
   mPixmapId = 0;
}

GmPixmapIcon::GmPixmapIcon(string filename)
{
   mPixmapId = 0;
   loadFromFile(filename);
}

GmPixmapIcon::~GmPixmapIcon()
{ 
}

void GmPixmapIcon::loadFromFile(string filename)
{
   mPixmapId = GmFileCache::getInstance().fetchTexture(filename);
}

GmIcon* GmPixmapIcon::clone()
{
   GmPixmapIcon* newIcon = new GmPixmapIcon();
   newIcon->mPixmapId = mPixmapId;
   return newIcon;
}

void GmPixmapIcon::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   
   glPushMatrix();

   glTranslatef(x, y, z);
   applyAnimationTransforms();

   // bind the texture
   glBindTexture(GL_TEXTURE_2D, mPixmapId);

   // color must be set to opaque white
   glColor4f(1.0, 1.0, 1.0, 1.0);
   
   glBegin(GL_QUADS);
   glTexCoord2f(0.0, 1.0); glVertex2f(0.0, 0.0);
   glTexCoord2f(1.0, 1.0); glVertex2f(mGLWidth, 0.0);
   glTexCoord2f(1.0, 0.0); glVertex2f(mGLWidth, mGLHeight);
   glTexCoord2f(0.0, 0.0); glVertex2f(0.0, mGLHeight);
   glEnd();

   // unbind the texture
   glBindTexture(GL_TEXTURE_2D, 0);

   glPopMatrix();

   // restore color settings
   glPopAttrib();
}

void GmPixmapIcon::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);

   glColor4f(1.0, 1.0, 0, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   glPopName();

   glPopMatrix();

   // restore color settings
   glPopAttrib();
}
