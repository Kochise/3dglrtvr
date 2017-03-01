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
#include <glam/GmOptions.h>
#include <GceControlPoint.h>

GceControlPoint::GceControlPoint() 
      : GmPushButton()
{
   mBorderWidth = 0.0;
   if(mLabel)
      mLabel->setTextJustification(GmTypes::JTH_LEFT,
                                        GmTypes::JTV_MIDDLE);
}

GceControlPoint::~GceControlPoint()
{
}

void GceControlPoint::render2d(GmTypes::RenderOptionType renderOptions)
{
   float x, y, z, w, h; // x, y, z, width and height values
   float r, g, b, a; // red, green, blue and alpha values

   mGLPosition.getXYZ(x, y, z);
   w = mGLWidth;
   h = mGLHeight;
   
   mBaseColor.getRGBA(r, g, b, a);

   // Control point
   glPushMatrix();
   glTranslatef(x, y, 0.0);
   if(mState == GmTypes::BS_UP)
      glColor4f(r, g, b, a);
   else
      glColor4f(r*.85, g*.85, b*.85, a);
   glPointSize(6.0);
   glBegin(GL_POINTS);
   glVertex2f(0.0, 0.0);
   glEnd();
   glPointSize(1.0);

   // The text on the button if it has any
   if(mLabel)
      mLabel->render(renderOptions);
   glPopMatrix();
}

void GceControlPoint::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   float x, y, z, w, h;

   mGLPosition.getXYZ(x, y, z);
   w = mGLWidth;
   h = mGLHeight;
   
   glPushName(mObjectId);
   glColor4f(1.0, 1.0, 0, 1.0);
   glPointSize(6.0);
   glBegin(GL_POINTS);
   glVertex2f(x, y);
   glEnd();
   glPointSize(1.0);
   glPopName();   
}

void GceControlPoint::mouseDragEvent(GmEvent* event)
{
   GmPushButton::mouseDragEvent(event);
   GmEvent::MouseEventType* mouseEvent = event->getMouseData();
   
   mGLPosition.setXYZ(mouseEvent->x, mouseEvent->y, 0.0);
}
