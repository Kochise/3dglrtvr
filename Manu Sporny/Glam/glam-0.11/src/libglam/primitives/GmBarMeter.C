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
#include <glam/GmOptions.h>
#include <glam/GmBarMeter.h>

GmBarMeter::GmBarMeter()
      : GmMeter()
{
   GmOptions* gmOptions = GmOptions::getInstance();
   GlmColor4* baseColor = NULL;
   GlmColor4* barColor = NULL;

   baseColor = (GlmColor4*)gmOptions->getColorOption(GMO_BARMETER_BASECOLOR);
   barColor = (GlmColor4*)gmOptions->getColorOption(GMO_BARMETER_BARCOLOR);   

   if(baseColor != NULL)
      mBaseColor = *baseColor;
   if(barColor != NULL)
      mBarColor = *barColor;
   
   setRange(0.0, 1.0);
   setSteps(0.01, 0.1);
   setCurrentValue(0.0);
   setOrientation(GmTypes::OT_HORIZONTAL);
}

GmBarMeter::~GmBarMeter()
{
}

void GmBarMeter::setBarColor(float r, float g, float b, float a)
{
   mBarColor.setRGBA(r, g, b, a);
}

void GmBarMeter::setBarColor(GlmColor4* color)
{
   float r, g, b, a;

   color->getRGBA(r, g, b, a);
   mBarColor.setRGBA(r, g, b, a);
}

GlmColor4* GmBarMeter::getBarColor()
{
   return &mBarColor;
}

void GmBarMeter::render2d(GmTypes::RenderOptionType renderOptions)
{
   GmInteractiveWidget::render2d(renderOptions);
   
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   // border width
   float bWidth = mGLWidth * (mBorderWidth/2.0);
   
   float r, g, b, a; // red, green, blue and alpha values
   mBaseColor.getRGBA(r, g, b, a);
      
   if(!getDisplayListValid())
   {
      if(mDisplayListId != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);
      
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);

      // Left border
      glColor4f(0, 0, 0, a);
      glBegin(GL_POLYGON);
      glVertex2f(0.0, 0.0);
      glVertex2f(bWidth, bWidth);
      glVertex2f(bWidth, mGLHeight - bWidth);
      glVertex2f(0.0, mGLHeight);
      glEnd();
      
      // Top border
      glColor4f(0, 0, 0, a);
      glBegin(GL_POLYGON);
      glVertex2f(0.0, mGLHeight);
      glVertex2f(mGLWidth, mGLHeight);
      glVertex2f(mGLWidth - bWidth, mGLHeight - bWidth);
      glVertex2f(bWidth, mGLHeight - bWidth);
      glEnd();
      
      // Bottom border
      glColor4f(1.0, 1.0, 1.0, a);
      glBegin(GL_POLYGON);
      glVertex2f(0.0, 0.0);
      glVertex2f(mGLWidth, 0.0);
      glVertex2f(mGLWidth - bWidth, bWidth);
      glVertex2f(bWidth, bWidth);
      glEnd();
   
      // Right border
      glColor4f(1.0, 1.0, 1.0, a);
      glBegin(GL_POLYGON);
      glVertex2f(mGLWidth, 0.0);
      glVertex2f(mGLWidth, mGLHeight);
      glVertex2f(mGLWidth - bWidth, mGLHeight - bWidth);
      glVertex2f(mGLWidth - bWidth, bWidth);
      glEnd();
      
      // Background
      glColor4f(r, g, b, a);
      glBegin(GL_POLYGON);
      glVertex2f(bWidth, bWidth);
      glVertex2f(mGLWidth - bWidth, bWidth);
      glVertex2f(mGLWidth - bWidth, mGLHeight - bWidth);
      glVertex2f(bWidth, mGLHeight - bWidth);
      glEnd();

      glEndList();

      setDisplayListValid(true);
   }

   float x, y, z; // x,y,z coordinates
   mGLPosition.getXYZ(x, y, z);
      
   // push matrix
   glPushMatrix();
   glTranslatef(x, y, z);
   glCallList(getDisplayListId());

   // Percentage complete
   if(getCurrentPercentage() > 0)
   {
      mBarColor.getRGBA(r, g, b, a);   
      glColor4f(r, g, b, a);
      
      glBegin(GL_POLYGON);
      glVertex2f(bWidth, bWidth);
      switch(mOrientation)
      {
         case GmTypes::OT_HORIZONTAL:
            glVertex2f(bWidth + ((mGLWidth-(2*bWidth))*getCurrentPercentage()),
                       bWidth);
            glVertex2f(bWidth + ((mGLWidth-(2*bWidth))*getCurrentPercentage()),
                       mGLHeight - bWidth);
            glVertex2f(bWidth, mGLHeight - bWidth);
            break;
         case GmTypes::OT_VERTICAL:
            glVertex2f(mGLWidth - bWidth, bWidth);
            glVertex2f(mGLWidth - bWidth, bWidth +
                       ((mGLHeight-(2*bWidth))*getCurrentPercentage()));
            glVertex2f(bWidth, bWidth +
                       ((mGLHeight-(2*bWidth))*getCurrentPercentage()));
            break;
      }
      
      glEnd();
   }

   // pop off matrix
   glPopMatrix();

   // restore color settings
   glPopAttrib();
}

void GmBarMeter::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
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

   glPopAttrib();
}
