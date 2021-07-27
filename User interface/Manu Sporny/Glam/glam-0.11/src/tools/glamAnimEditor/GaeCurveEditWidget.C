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
#include <GaeCurveEditWidget.h>

GaeCurveEditWidget::GaeCurveEditWidget() 
      : GmPanel()
{
   mTValue = 0.3;
}

GaeCurveEditWidget::~GaeCurveEditWidget()
{
}

GlmPoint3 GaeCurveEditWidget::getSplineRotation(float t)
{
   GlmPoint3 p;

   mTValue = t;
   p = mBezierCurve.getPositionByPercentage(t);
   p *= 360.0;
   
   return p;
}

GlmPoint3 GaeCurveEditWidget::getSplinePosition(float t)
{
   mTValue = t;
   return mBezierCurve.getPositionByPercentage(t);
}

GlmPoint3 GaeCurveEditWidget::getSplineScale(float t)
{
   mTValue = t;
   return mBezierCurve.getPositionByPercentage(t);
}

void GaeCurveEditWidget::render2d(GmTypes::RenderOptionType renderOptions)
{
   float x, y, z, w, h; // x, y, z, width and height values
   float r, g, b, a; // red, green, blue and alpha values

   mGLPosition.getXYZ(x, y, z);
   w = mGLWidth;
   h = mGLHeight;
   
   mBaseColor.getRGBA(r, g, b, a);

   // Draw backdrop
   glColor4f(0.0, 0.0, 0.0, 0.4);
   glBegin(GL_QUADS);
   glVertex2f(x, y);
   glVertex2f(x+w, y);
   glVertex2f(x+w, y+h);
   glVertex2f(x, y+h);
   glEnd();

   // Draw the Bezier spline
   glPushMatrix();
   glTranslatef(0.0, (y+h/2.0), 0.0);
   glColor4f(0.0, 0.0, 1.0, 0.9);
   glBegin(GL_LINE_STRIP);
   for(float t = 0.0; t <= 1.05; t += 0.05)
   {
      float x, y, z;
      GlmPoint3 p = mBezierCurve.getPositionByPercentage(t);
      p.getXYZ(x, y, z);
      x *= w;
      y *= h;
      glVertex2f(x, y);
   }
   glEnd();
   glPopMatrix();
   
   // Draw current t-value vertical line and the 
   glColor4f(0.0, 1.0, 0.0, 0.8);
   glBegin(GL_LINES);
   glVertex2f(x+(w*mTValue), y);
   glVertex2f(x+(w*mTValue), y+h);
   glVertex2f(x, (y+h/2.0));
   glVertex2f(x+w, (y+h/2.0));
   glEnd();
   
   // Draw component border
   glColor4f(r, g, b, a);
   glBegin(GL_LINE_LOOP);
   glVertex2f(x, y);
   glVertex2f(x+w, y);
   glVertex2f(x+w, y+h);
   glVertex2f(x, y+h);
   glEnd();
}

void GaeCurveEditWidget::render2dSelect(GmTypes::RenderOptionType
                                        renderOptions)
{
}
