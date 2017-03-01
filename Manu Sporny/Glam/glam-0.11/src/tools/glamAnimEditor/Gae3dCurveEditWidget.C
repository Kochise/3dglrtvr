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
#include <Gae3dCurveEditWidget.h>

Gae3dCurveEditWidget* temp;
   
Gae3dCurveEditWidget::Gae3dCurveEditWidget() 
      : GmPanel()
{
   mXYView = new GaeSplineEditWidget(GaeTypes::VO_XY);
   mZXView = new GaeSplineEditWidget(GaeTypes::VO_ZX);
   mZYView = new GaeSplineEditWidget(GaeTypes::VO_ZY);
   mXYZView = new GaeSplineEditWidget(GaeTypes::VO_XYZ);
   
   mXYView->setRelativeGeometry(0.0, 0.0, 0.0,
                               0.49, 0.49, 0.0);
   mZXView->setRelativeGeometry(0.0, 0.51, 0.0,
                               0.49, 1.0, 0.0);
   mZYView->setRelativeGeometry(0.51, 0.51, 0.0,
                                1.0, 1.0, 0.0);
   mXYZView->setRelativeGeometry(0.51, 0.0, 0.0,
                                 1.0, 0.49, 0.0);   

   mXYView->setSpline(&mBezierSpline);
   mZXView->setSpline(&mBezierSpline);   
   mZYView->setSpline(&mBezierSpline);
   mXYZView->setSpline(&mBezierSpline);
     
   addComponent(mXYView);
   addComponent(mZXView);
   addComponent(mZYView);
   addComponent(mXYZView);

   mXYView->addSplineChangeListener(mZXView);
   mXYView->addSplineChangeListener(mZYView);

   mZXView->addSplineChangeListener(mXYView);
   mZXView->addSplineChangeListener(mZYView);

   mZYView->addSplineChangeListener(mXYView);
   mZYView->addSplineChangeListener(mZXView);

   mAddSegmentButton = new GmPushButton();
   mAddSegmentButton->setRelativeGeometry(0.0, 0.0, 0.0,
                                          0.1, 0.1, 0.0);
   mAddSegmentButton->setBaseColor(0.4, 0.4, 0.9, 0.9);
   mAddSegmentButton->setBorderWidth(0.1);
   mAddSegmentButton->setText("Add");
   mAddSegmentButton->setTextSize(6.0);

   addComponent(mAddSegmentButton);
   //mAddSegmentButton->mMousePressFunc = Gae3dCurveEditWidget::test;

   temp = this;
}

Gae3dCurveEditWidget::~Gae3dCurveEditWidget()
{
   delete mXYView;
   delete mZXView;
   delete mZYView;
   delete mXYZView;
   delete mAddSegmentButton;
}

GlmPoint3 Gae3dCurveEditWidget::getSplineRotation(float t)
{
   GlmPoint3 p;

   p = mBezierSpline.getPositionByPercentage(t);
   p *= 360.0;
   
   return p;
}

GlmPoint3 Gae3dCurveEditWidget::getSplinePosition(float t)
{
   return mBezierSpline.getPositionByPercentage(t);
}

GlmPoint3 Gae3dCurveEditWidget::getSplineScale(float t)
{
   return mBezierSpline.getPositionByPercentage(t);
}            

void Gae3dCurveEditWidget::render2d(GmTypes::RenderOptionType renderOptions)
{
   recomputeSpline();
   GmPanel::render2d(renderOptions);
}

void Gae3dCurveEditWidget::recomputeSpline()
{
   // Modify the spline based on each control point
   int i = 0;
   float pstep;
   float percent = 0.0;
   float p1x, p1y, p1z;
   float p2x, p2y, p2z;
   float p3x, p3y, p3z;
   float p4x, p4y, p4z;
   float dummy;

   GlmPoint3 p;
   vector<GaeControlPoint*>::iterator cXY;
   vector<GaeControlPoint*>::iterator cZY;
   
   cXY = mXYView->getControlPoints()->begin();
   cZY = mZYView->getControlPoints()->begin();

   pstep = 1.0 / (float((mXYView->getControlPoints()->size() - 1)) / 3.0);
   while(true)
   {
      (*cXY)->getRelativeGLPosition(p);
      p.getXYZ(p1x, p1y, dummy);
      
      (*cZY)->getRelativeGLPosition(p);
      p.getXYZ(p1z, dummy, dummy);
      cXY++;
      cZY++;

      (*cXY)->getRelativeGLPosition(p);
      p.getXYZ(p2x, p2y, dummy);

      (*cZY)->getRelativeGLPosition(p);
      p.getXYZ(p2z, dummy, dummy);
      cXY++;
      cZY++;
      
      (*cXY)->getRelativeGLPosition(p);
      p.getXYZ(p3x, p3y, dummy);

      (*cZY)->getRelativeGLPosition(p);
      p.getXYZ(p3z, dummy, dummy);
      cXY++;
      cZY++;
      
      (*cXY)->getRelativeGLPosition(p);
      p.getXYZ(p4x, p4y, dummy);

      (*cZY)->getRelativeGLPosition(p);
      p.getXYZ(p4z, dummy, dummy);
      cXY++;
      cZY++;

      // re-scale the values
      p1x /= 25.0;
      p1y /= 25.0;
      p1z /= 25.0;
      p2x /= 25.0;
      p2y /= 25.0;
      p2z /= 25.0;
      p3x /= 25.0;
      p3y /= 25.0;
      p3z /= 25.0;
      p4x /= 25.0;
      p4y /= 25.0;
      p4z /= 25.0;

      /*
      printf("Gae3dCurveEditWidget: p1[%f, %f, %f]\n", p1x, p1y, p1z);
      printf("Gae3dCurveEditWidget: p2[%f, %f, %f]\n", p2x, p2y, p2z);
      printf("Gae3dCurveEditWidget: p3[%f, %f, %f]\n", p3x, p3y, p3z);
      printf("Gae3dCurveEditWidget: p4[%f, %f, %f]\n", p4x, p4y, p4z);
      */
      
      mBezierSpline.setSplineSegment(i, percent,
                                     p1x, p1y, p1z,
                                     p2x, p2y, p2z,
                                     p3x, p3y, p3z,
                                     p4x, p4y, p4z);

      if (cXY == mXYView->getControlPoints()->end())
         break;
      cXY--;
      cZY--;
      
      i++;
      percent += pstep;
   }
}

void Gae3dCurveEditWidget::addSegment()
{
   mXYView->addSegment();
   mZXView->addSegment();   
   mZYView->addSegment();

   mZXView->broadcastSplineChange();
   
   mBezierSpline.addSplineSegment(1.0,
                                  0.0, 0.0, 0.0,
                                  0.0, 0.0, 0.0,
                                  0.0, 0.0, 0.0);
}

void Gae3dCurveEditWidget::layoutInGLArea(float minX, float minY,
                                          float width, float height,
                                          float layer)
{
   // TEMP CODE!!!
   layoutInGLVolume(minX, minY, layer, width, height, layer);
}

void Gae3dCurveEditWidget::layoutInGLVolume(float minX, float minY,
                                            float minZ,
                                            float width, float height,
                                            float depth)
{
   GmPanel::layoutInGLVolume(minX, minY, minZ, width, height, depth);
   mZXView->broadcastSplineChange();
}

void Gae3dCurveEditWidget::keypressEvent(GmEvent* event)
{
   cout << event->getKeyData()->key << endl;
   
   switch (event->getKeyData()->key)
   {
      case 'c':
         addSegment();
         break;
   }
}

void Gae3dCurveEditWidget::test()
{
   temp->addSegment();
}
