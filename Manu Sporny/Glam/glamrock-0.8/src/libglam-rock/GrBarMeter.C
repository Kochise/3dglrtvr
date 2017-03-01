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
#include <glamrock/GrBarMeter.h>

#define BMR_PERCENT 0.5     // Panel rounding percentage

GrBarMeter::GrBarMeter()
{
}

GrBarMeter::~GrBarMeter()
{
}

void GrBarMeter::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   if(!getDisplayListValid())
   {
      if(mDisplayListId != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);

      // Display list creation
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);

      GrShapeFactory* sf = GrShapeFactory::getInstance();
      
      float r, g, b, a;
      mBaseColor.getRGBA(r, g, b, a);

      // Draw the background
      glColor4f(r, g, b, a);
      sf->renderRoundedRect(GrTypes::BS_ALL,
                            GrTypes::FS_SOLID,
                            mGLWidth, mGLHeight, BMR_PERCENT);

      // **** this should updated to exclude the actual
      // **** meter from the display list
      
      mBarColor.getRGBA(r, g, b, a);   
      glColor4f(r, g, b, a);
      switch(mOrientation)
      {
         case GmTypes::OT_HORIZONTAL:
            sf->renderRoundedRect(GrTypes::BS_ALL,
                                  GrTypes::FS_SOLID,
                                  mGLWidth*mCurrentPercent, mGLHeight,
                                  BMR_PERCENT);
            glColor4f(r*0.5, g*0.5, b*0.5, a);
            sf->renderRoundedRect(GrTypes::BS_ALL,
                                  GrTypes::FS_OUTLINE,
                                  mGLWidth*mCurrentPercent, mGLHeight,
                                  BMR_PERCENT);
            break;
         case GmTypes::OT_VERTICAL:
            sf->renderRoundedRect(GrTypes::BS_ALL,
                                  GrTypes::FS_SOLID,
                                  mGLWidth, mGLHeight*mCurrentPercent,
                                  BMR_PERCENT);
            glColor4f(r*0.5, g*0.5, b*0.5, a);
            sf->renderRoundedRect(GrTypes::BS_ALL,
                                  GrTypes::FS_OUTLINE,
                                  mGLWidth, mGLHeight*mCurrentPercent,
                                  BMR_PERCENT);
            break;
      }
   
      // Draw the bar meter border
      mBaseColor.getRGBA(r, g, b, a);
      glColor4f(r*0.5, g*0.5, b*0.5, a);
      sf->renderRoundedRect(GrTypes::BS_ALL,
                            GrTypes::FS_OUTLINE,
                            mGLWidth, mGLHeight, BMR_PERCENT);

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
}

void GrBarMeter::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   GrShapeFactory* sf = GrShapeFactory::getInstance();

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);

   glColor4f(0.3, 0.8, 0.3, 1.0);
   sf->renderRoundedRect(GrTypes::BS_ALL,
                         GrTypes::FS_SOLID,
                         mGLWidth, mGLHeight, BMR_PERCENT);

   glPopName();
   
   glPopMatrix();

   // restore color settings
   glPopAttrib();
}
