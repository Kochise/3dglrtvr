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
#include <glam/GmRadioButton.h>

#define CIRCLE_DENSITY 40//100

GmRadioButton::GmRadioButton()
      : GmCheckBox()
{   
}

GmRadioButton::~GmRadioButton()
{
}

void GmRadioButton::mouseReleaseEvent(GmEvent* event)
{
   // radio buttons cannot be unselected by pressing the same button
   if(mSelected)
   {
      // release the button press
      mPressed = false;
      return;
   }

   GmCheckBox::mouseReleaseEvent(event);
}

void GmRadioButton::render2d(GmTypes::RenderOptionType renderOptions)
{
   render2dToolTip(renderOptions);
   
   // save color and line settings
   glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

   float x, y, z;
      
   if(!getDisplayListValid())
   {
      if(mDisplayListId != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);

      float r, g, b, a;
      mBaseColor.getRGBA(r, g, b, a);
      
      float bWidth;
      bWidth = mGLHeight * (mBorderWidth / 2.0);
      
      // Display list for the two-tone radio button border
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);

      glLineWidth(bWidth);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glBegin(GL_POLYGON);
      glColor4f(0.0, 0.0, 0.0, a);
      
      float shift = CIRCLE_DENSITY / 8.0;
      float i, cosine, sine;
      for(i = shift; i < (CIRCLE_DENSITY/2) + shift; i++)
      {
         cosine = (mGLHeight/2) * cos(i*2*M_PI / CIRCLE_DENSITY);
         sine = (mGLHeight/2) * sin(i*2*M_PI / CIRCLE_DENSITY);
         glVertex2f(cosine + (mGLHeight/2.0), sine + (mGLHeight/2.0));
      }

      glColor4f(1.0, 1.0, 1.0, a);
      for(i = (CIRCLE_DENSITY/2) + shift; i < CIRCLE_DENSITY + shift; i++)
      {
         cosine = (mGLHeight/2) * cos(i*2*M_PI / CIRCLE_DENSITY);
         sine = (mGLHeight/2) * sin(i*2*M_PI / CIRCLE_DENSITY);
         glVertex2f(cosine + (mGLHeight/2.0), sine + (mGLHeight/2.0));
      }
      glEnd();

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glLineWidth(1.0);
      glBegin(GL_POLYGON);
      
      // the "selection dot"
      if(getSelected())
      {
         glColor4f(r, g, b, a);
         for(i = 0; i < CIRCLE_DENSITY; i++)
         {
            cosine = ((mGLHeight/2) * 0.5) * cos(i*2*M_PI / CIRCLE_DENSITY);
            sine = ((mGLHeight/2) * 0.5) * sin(i*2*M_PI / CIRCLE_DENSITY);
            glVertex2f(cosine + (mGLHeight/2.0), sine + (mGLHeight / 2.0));
         }
      }

      glEnd();

      glEndList();
      
      setDisplayListValid(true);

      // set up text widget
      if(mLabel != NULL)
      {
         mGLPosition.getXYZ(x, y, z);
         mLabel->setGLPosition(x + mGLHeight + mLabel->getTextSize() / 4, y, z);
      }
   }
   
   // Draw the radio button

   glPushMatrix();
   mGLPosition.getXYZ(x, y, z);
   glTranslatef(x, y, z);
   glCallList(getDisplayListId());
   glPopMatrix();

   // restore color and line settings
   glPopAttrib();
   
   // Draw the text label (if any)
   if(mLabel != NULL)
      mLabel->render(renderOptions);
}

void GmRadioButton::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);
   
   glColor4f(1.0, 1.0, 0.0, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLHeight, 0.0);
   glVertex2f(mGLHeight, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   glPopName();
   
   glPopMatrix();

   glPopAttrib();

   // render the label
   if(mLabel != NULL)
      mLabel->render(renderOptions);
}
