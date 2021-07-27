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
#include <glam/GmPushButton.h>

GmPushButton::GmPushButton()
      : GmGraphicButton()
{
   GmOptions* gmOptions = GmOptions::getInstance();
   GlmColor4* bColor = NULL;
   GlmColor4* tColor = NULL;
      
   bColor = (GlmColor4*)gmOptions->getColorOption(GMO_PUSHBUTTON_BASECOLOR);
   tColor = (GlmColor4*)gmOptions->getColorOption(GMO_PUSHBUTTON_TEXTCOLOR);
   mBorderWidth = gmOptions->getScalarOption(GMO_PUSHBUTTON_BORDERWIDTH);

   if(bColor != NULL)
      mBaseColor = *bColor;
   
   if(mLabel != NULL)
   {
      mLabel->setTextJustification(GmTypes::JTH_CENTER,
                                   GmTypes::JTV_MIDDLE);
      if(tColor != NULL)
      {
         float r, g, b, a;
         
         tColor->getRGBA(r, g, b, a);
         mLabel->setTextColor(r, g, b, a);
      }
   }
}

GmPushButton::~GmPushButton()
{
}

void GmPushButton::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   if(!getDisplayListValid())
   {
      if(getDisplayListId() != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);
      
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);

      float xo = 0.0; // x offset
      float yo = 0.0; // y offset
//       if(mState == GmTypes::BS_DOWN)
//       {
//          xo = (mGLWidth * 0.008);
//          yo = (mGLHeight * 0.008);
//       }
      
      float r, g, b, a; // red, green, blue and alpha values
      mBaseColor.getRGBA(r, g, b, a);
      
      // Left border of button
      if(mState == GmTypes::BS_UP)
         glColor4f(r*.8, g*.8, b*.8, a);
      else
         glColor4f(r*.65, g*.65, b*.65, a);
      
      // border width
      float bWidth = 0.0;
      if(mGLWidth < mGLHeight)
         bWidth = mGLWidth * (mBorderWidth / 2.0);
      else
         bWidth = mGLHeight * (mBorderWidth / 2.0);

      glBegin(GL_POLYGON);
      glVertex2f(0.0, 0.0);
      glVertex2f(bWidth + xo, bWidth - yo);
      glVertex2f(bWidth + xo, mGLHeight - bWidth - yo);
      glVertex2f(0.0, mGLHeight);
      glEnd();
      
      // Top border of button
      if(mState == GmTypes::BS_UP)
         glColor4f(r*.85, g*.85, b*.85, a);
      else
         glColor4f(r*.7, g*.7, b*.7, a);
      
      glBegin(GL_POLYGON);
      glVertex2f(0.0, mGLHeight);
      glVertex2f(mGLWidth, mGLHeight);
      glVertex2f(mGLWidth - bWidth + xo, mGLHeight - bWidth - yo);
      glVertex2f(bWidth + xo, mGLHeight - bWidth - yo);
      glEnd();
      
      // Bottom border of button
      if(mState == GmTypes::BS_UP)
         glColor4f(r*.6, g*.6, b*.6, a);
      else
         glColor4f(r*.9, g*.9, b*.9, a);
      
      glBegin(GL_POLYGON);
      glVertex2f(0.0, 0.0);
      glVertex2f(mGLWidth, 0.0);
      glVertex2f(mGLWidth - bWidth + xo, bWidth - yo);
      glVertex2f(bWidth + xo, bWidth - yo);
      glEnd();
      
      // Right border of button
      if(mState == GmTypes::BS_UP)
         glColor4f(r*.7, g*.7, b*.7, a);
      else
         glColor4f(r*.9, g*.9, b*.9, a);
      
      glBegin(GL_POLYGON);
      glVertex2f(mGLWidth, 0.0);
      glVertex2f(mGLWidth, mGLHeight);
      glVertex2f(mGLWidth - bWidth + xo, mGLHeight - bWidth - yo);
      glVertex2f(mGLWidth - bWidth + xo, bWidth - yo);
      glEnd();
      
      // Center section of button
      if(mState == GmTypes::BS_UP)
         glColor4f(r, g, b, a);
      else
         glColor4f(r*.85, g*.85, b*.85, a);
      
      glBegin(GL_POLYGON);
      glVertex2f(bWidth + xo, bWidth - yo);
      glVertex2f(mGLWidth - bWidth + xo, bWidth - yo);
      glVertex2f(mGLWidth - bWidth + xo, mGLHeight - bWidth - yo);
      glVertex2f(bWidth + xo, mGLHeight - bWidth - yo);
      glEnd();
      
      glEndList();

      setDisplayListValid(true);
   }
   
   float x, y, z; // x,y,z coordinates
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);
   glCallList(getDisplayListId());
   glPopMatrix();

   // restore color settings
   glPopAttrib();

   GmGraphicButton::render2d(renderOptions);
}
