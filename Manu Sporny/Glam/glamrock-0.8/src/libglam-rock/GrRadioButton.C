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
#include <glamrock/GrRadioButton.h>

#define RB_DENSITY  40.0    // The number of verticies in the circle
#define RB_DIAMETER 5.0     // The diameter of the radio button

GrRadioButton::GrRadioButton()
{
}

GrRadioButton::~GrRadioButton()
{
}

void GrRadioButton::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   
   if(!getDisplayListValid())
   {
      if(getDisplayListId() != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);

      //generate the radio button display list
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);
      
      GrShapeFactory* sf = GrShapeFactory::getInstance();

      float r, g, b, a;
      mBaseColor.getRGBA(r, g, b, a);

      glPushMatrix();
      glTranslatef(mGLHeight / 2.0, mGLHeight / 2.0, 0.0);      

      glColor4f(r*0.5, g*0.5, b*0.5, a);
      sf->renderCircle(GrTypes::FS_OUTLINE, mGLHeight * 0.5, 20);
   
      if(mSelected)
      {
         glColor4f(r, g, b, a);
         sf->renderCircle(GrTypes::FS_SOLID, mGLHeight * 0.25, 20);
      }

      glPopMatrix();

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

void GrRadioButton::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);

   glColor4f(0.8, 0.8, 0.0, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLHeight, 0.0);
   glVertex2f(mGLHeight, mGLHeight);
   glVertex2f(0.0, mGLHeight);

   glEnd();

   glPopName();
 
   glPopMatrix();

   // restore color settings
   glPopAttrib();

   // render the label
   if(mLabel != NULL)
      mLabel->render(renderOptions);
}
