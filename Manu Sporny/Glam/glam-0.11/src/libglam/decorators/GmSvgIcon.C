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
#include <glam/GmSvgIcon.h>

GmSvgIcon::GmSvgIcon()
      : GmIcon()
{
   mGLSvgRenderer = NULL;
}

GmSvgIcon::GmSvgIcon(string filename)
{
   mGLSvgRenderer = NULL;
   loadFromFile(filename);
}

GmSvgIcon::~GmSvgIcon()
{
}

void GmSvgIcon::loadFromFile(string filename)
{
   mGLSvgRenderer = GmFileCache::getInstance().fetchSvg(filename);
}

GmIcon* GmSvgIcon::clone()
{
   GmSvgIcon* newIcon = new GmSvgIcon();
   newIcon->mGLSvgRenderer = mGLSvgRenderer;
   return newIcon;
}

void GmSvgIcon::render2d(GmTypes::RenderOptionType renderOptions)
{
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   if(mGLSvgRenderer != NULL)
   {
      glScalef(mGLWidth / mGLSvgRenderer->getSvgWidth(),
               mGLHeight / mGLSvgRenderer->getSvgHeight(),
               1.0);
   
      mGLSvgRenderer->render();
   }
   else // render a rectangle
   {
      // save color settings
      glPushAttrib(GL_CURRENT_BIT);

      glColor4f(1.0, 1.0, 0, 1.0);
      glBegin(GL_POLYGON);
      glVertex2f(0.0, 0.0);
      glVertex2f(mGLWidth, 0.0);
      glVertex2f(mGLWidth, mGLHeight);
      glVertex2f(0.0, mGLHeight);
      glEnd();
      
      // restore color settings
      glPopAttrib();
   }

   // pop off translation
   glPopMatrix();
}

void GmSvgIcon::render2dSelect(GmTypes::RenderOptionType renderOptions)
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
