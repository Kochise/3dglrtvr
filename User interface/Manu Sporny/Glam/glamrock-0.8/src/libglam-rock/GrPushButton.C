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
#include <GL/gl.h>
#include <glamrock/GrShapeFactory.h>
#include <glamrock/GrPushButton.h>

#define PR_PERCENT 0.5     // Panel rounding percentage

GrPushButton::GrPushButton()
{
}

GrPushButton::~GrPushButton()
{
}

void GrPushButton::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   if(!getDisplayListValid())
   {
      if(getDisplayListId() != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);
      
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);

      GrShapeFactory* sf = GrShapeFactory::getInstance();
      
      float r, g, b, a;
      mBaseColor.getRGBA(r, g, b, a);
         
      // Draw the base part of the button
      if(mState == GmTypes::BS_UP)
         glColor4f(r, g, b, a);
      else
         glColor4f(r*1.09, g*1.09, b*1.09, a);

      sf->renderRoundedRect(GrTypes::BS_ALL,
                            GrTypes::FS_SOLID,
                            mGLWidth, mGLHeight, PR_PERCENT);
         
      // Draw the border for the button
      glColor4f(r*0.4, g*0.4, b*0.4, a);
      sf->renderRoundedRect(GrTypes::BS_ALL,
                            GrTypes::FS_OUTLINE,
                            mGLWidth, mGLHeight, PR_PERCENT);
      
      glEndList();
      
      setDisplayListValid(true);
   }

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);
   glCallList(getDisplayListId());
   glPopMatrix();

   // restore color settings
   glPopAttrib();

   GmGraphicButton::render2d(renderOptions);
}

void GrPushButton::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   glPushName(mObjectId);

   GrShapeFactory* sf = GrShapeFactory::getInstance();

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   glColor4f(0.8, 0.0, 0.8, 1.0);
   sf->renderRoundedRect(GrTypes::BS_ALL,
                         GrTypes::FS_SOLID,
                         mGLWidth, mGLHeight, PR_PERCENT);
   glPopMatrix();

   glPopName();

   // restore color settings
   glPopAttrib();
}
