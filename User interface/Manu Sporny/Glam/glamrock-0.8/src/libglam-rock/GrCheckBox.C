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

#include <glam/GmTypes.h>
#include <glamrock/GrCheckBox.h>
#include <glamrock/GrShapeFactory.h>

#define CBR_PERCENT 0.4

GrCheckBox::GrCheckBox()
      : GmCheckBox()
{
}

GrCheckBox::~GrCheckBox()
{
}

void GrCheckBox::render2d(GmTypes::RenderOptionType renderOptions)
{
   if(getMouseOver() && mLabel != NULL)
      mLabel->render2dToolTip(renderOptions);
   
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;

   if(!getDisplayListValid())
   {
      if(getDisplayListId() != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);
      
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);

      GrShapeFactory* sf = GrShapeFactory::getInstance();
      
      float r, g, b, a;
      mBaseColor.getRGBA(r, g, b, a);

      glColor4f(r, g, b, a);
      sf->renderRoundedRect(GrTypes::BS_ALL,
                            GrTypes::FS_SOLID,
                            mGLHeight, mGLHeight, CBR_PERCENT);
      glColor4f(r*0.5, g*0.5, b*0.5, a);
      sf->renderRoundedRect(GrTypes::BS_ALL,
                            GrTypes::FS_OUTLINE,
                            mGLHeight, mGLHeight, CBR_PERCENT);
   
      if(mSelected)
      {
         float centerX, centerY, cOffset, oOffset;
  
         centerX = mGLHeight / 2.0;
         centerY = mGLHeight / 2.0;
         cOffset = (mGLHeight / 2.0) * 0.3;
         oOffset = (mGLHeight / 2.0) * 0.8;

         sf->sfFilletSize(1.0);
      
         mCheckColor.getRGBA(r, g, b, a);
         glColor4f(r, g, b, a);
         sf->sfBegin(GrTypes::FS_SOLID);
         sf->sfVertex2f(centerX - oOffset, centerY - oOffset);
         sf->sfVertex2f(centerX - cOffset, centerY);
         sf->sfVertex2f(centerX - oOffset, centerY + oOffset);
         sf->sfVertex2f(centerX, centerY + cOffset);
         sf->sfVertex2f(centerX + oOffset, centerY + oOffset);
         sf->sfVertex2f(centerX + cOffset, centerY);
         sf->sfVertex2f(centerX + oOffset, centerY - oOffset);
         sf->sfVertex2f(centerX, centerY - cOffset);
         sf->sfEnd();
      
         glColor4f(r*0.5, g*0.5, b*0.5, a);
         sf->sfBegin(GrTypes::FS_OUTLINE);
         sf->sfVertex2f(centerX - oOffset, centerY - oOffset);
         sf->sfVertex2f(centerX - cOffset, centerY);
         sf->sfVertex2f(centerX - oOffset, centerY + oOffset);
         sf->sfVertex2f(centerX, centerY + cOffset);
         sf->sfVertex2f(centerX + oOffset, centerY + oOffset);
         sf->sfVertex2f(centerX + cOffset, centerY);
         sf->sfVertex2f(centerX + oOffset, centerY - oOffset);
         sf->sfVertex2f(centerX, centerY - cOffset);
         sf->sfEnd();
      }
   
      glEndList();
      setDisplayListValid(true);
      
      // set up text widget
      if(mLabel != NULL)
      {
         mGLPosition.getXYZ(x, y, z);
         mLabel->setGLPosition(x + mGLHeight + mLabel->getTextSize() / 4, y, z);
      }
   }
  
   mGLPosition.getXYZ(x, y, z);
    
   glPushMatrix();
   glTranslatef(x, y, z);
   glCallList(getDisplayListId());
   glPopMatrix();

   // restore color settings
   glPopAttrib();

   // Draw the text label (if any)
   if(mLabel != NULL)
      mLabel->render(renderOptions);
}

void GrCheckBox::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   GrShapeFactory* sf = GrShapeFactory::getInstance();

   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);

   glColor4f(1.0, 1.0, 0.0, 1.0);
   sf->renderRoundedRect(GrTypes::BS_ALL,
                         GrTypes::FS_SOLID,
                         mGLHeight, mGLHeight, CBR_PERCENT);

   glPopName();
   
   glPopMatrix();

   // restore color settings
   glPopAttrib();

   // render label
   if(mLabel != NULL)
      mLabel->render(renderOptions);
}
