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
#include <GaeControlPoint.h>

GaeControlPoint::GaeControlPoint() 
      : GmPushButton()
{
   mBorderWidth = 0.0;
   if(mLabel)
      mLabel->setTextJustification(GmTypes::JTH_LEFT,
                                   GmTypes::JTV_MIDDLE);
   mParentWidget = NULL;
   mConstrained = false;
}

GaeControlPoint::GaeControlPoint(GaeSplineEditWidget* parentWidget) 
      : GmPushButton()
{
   mBorderWidth = 0.0;
   if(mLabel)
      mLabel->setTextJustification(GmTypes::JTH_LEFT,
                                   GmTypes::JTV_MIDDLE);
   mParentWidget = parentWidget;
   mConstrained = false;
}

GaeControlPoint::~GaeControlPoint()
{
}

void GaeControlPoint::render2d(GmTypes::RenderOptionType renderOptions)
{
   float x, y, z, w, h; // x, y, z, width and height values
   float r, g, b, a; // red, green, blue and alpha values

   mGLPosition.getXYZ(x, y, z);
   w = mGLWidth;
   h = mGLHeight;
   
   mBaseColor.getRGBA(r, g, b, a);

   // Control point
   if(mState == GmTypes::BS_UP)
      glColor4f(r, g, b, a);
   else
      glColor4f(r*.85, g*.85, b*.85, a);
   glPointSize(6.0);
   glBegin(GL_POINTS);
   glVertex2f(x, y);
   glEnd();
   glPointSize(1.0);

   // The text on the button if it has any
   if(mLabel)
      mLabel->render(renderOptions);
}

void GaeControlPoint::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   float x, y, z, w, h;

   mGLPosition.getXYZ(x, y, z);
   w = mGLWidth;
   h = mGLHeight;
   
   glPushName(mObjectId);
   glColor4f(1.0, 1.0, 0.0, 1.0);
   glPointSize(6.0);
   glBegin(GL_POINTS);
   glVertex2f(x, y);
   glEnd();
   glPointSize(1.0);
   glPopName();   
}

void GaeControlPoint::mouseDragEvent(GmEvent* event)
{
   float x, y, z;
   float minX, minY, minZ;
   float maxX, maxY, maxZ;
   
   GmPushButton::mouseDragEvent(event);
   GmEvent::MouseEventType* mouseEvent = event->getMouseData();

   mGLPosition.getXYZ(x, y, z);

   x = mouseEvent->x;
   y = mouseEvent->y;
   z = 0.0;
   
   if(mConstrained)
   {
      mMaxConstraint.getXYZ(maxX, maxY, maxZ);
      mMinConstraint.getXYZ(minX, minY, minZ);
      
      if (x > maxX)
         x = maxX;

      if (y > maxY)
         y = maxY;

      if (x < minX)
         x = minX;

      if (y < minY)
         y = minY;
   }
   
   mGLPosition.setXYZ(x, y, z);

   if(mParentWidget)
   {
      mParentWidget->onControlPointDrag(mouseEvent->x, mouseEvent->y, this);
      mParentWidget->broadcastSplineChange();
   }
}

GlmPoint3* GaeControlPoint::getGLPosition()
{
   return &mGLPosition;
}

void GaeControlPoint::getRelativeGLPosition(GlmPoint3& point)
{
   float x, y, z;
   float px, py, pz;

   mGLPosition.getXYZ(x, y, z);
   if(mParentWidget)
   {
      mParentWidget->getGLPosition()->getXYZ(px, py, pz);
      x -= px;
      y -= py;
      z -= pz;
   }
   
   point.setXYZ(x, y, z);
}

void GaeControlPoint::setRelativeGLPosition(GlmPoint3 point)
{
   float x, y, z;
   float px, py, pz;

   point.getXYZ(x, y, z);
   if(mParentWidget)
   {
      mParentWidget->getGLPosition()->getXYZ(px, py, pz);
      x += px;
      y += py;
      z += pz;
   }
   
   mGLPosition.setXYZ(x, y, z);
}

void GaeControlPoint::setConstraints(GlmPoint3& minPoint, GlmPoint3& maxPoint)
{
   mConstrained = true;
   mMinConstraint = minPoint;
   mMaxConstraint = maxPoint;
}
