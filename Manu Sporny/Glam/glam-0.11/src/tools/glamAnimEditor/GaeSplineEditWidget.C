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

#include <glam/glam.h>
#include <GaeSplineEditWidget.h>
#include <GaeEvent.h>

GaeSplineEditWidget::GaeSplineEditWidget(GaeTypes::ViewOrientationType
                                         viewOrientation) 
      : GmInteractiveWidget()
{
   mZoomFactor = 1.0;
   mViewOrientation = viewOrientation;
   initControlPoints();

   mOrientationLabel = new GmLabel();
   mOrientationLabel->setRelativeGeometry(0.01, 0.01, 0.0,
                                          0.99, 0.99, 0.0);
   mOrientationLabel->setTextColor(1.0, 1.0, 1.0, 1.0);
   mOrientationLabel->setTextJustification(GmTypes::JTH_LEFT,
                                          GmTypes::JTV_TOP);      
   mOrientationLabel->setTextSize(6);

   switch(viewOrientation)
   {
      case GaeTypes::VO_XY:
         mOrientationLabel->setText("Side (xy)");
         break;

      case GaeTypes::VO_ZX:
         mOrientationLabel->setText("Top (zx)");
         break;

      case GaeTypes::VO_ZY:
        mOrientationLabel->setText("Front (zy)");
        break;

      case GaeTypes::VO_XYZ:
        mOrientationLabel->setText("3d (xyz)");
        break;        
   }
}

GaeSplineEditWidget::~GaeSplineEditWidget()
{
  delete mOrientationLabel;
}

void GaeSplineEditWidget::setSpline(GlmBezierSpline3* bezierSpline)
{
   mBezierSpline = bezierSpline;
}

void GaeSplineEditWidget::broadcastSplineChange()
{
   GaeEvent* event;
   GmEventDistributor* ed = GmEventDistributor::getInstance();
   
   for(vector<GaeSplineEditWidget*>::iterator l =
          mSplineChangeListeners.begin();
          l != mSplineChangeListeners.end(); l++)
   {
      event = new GaeEvent();
      event->setRestructureEvent((*l)->getObjectId(),
                                 mViewOrientation,
                                 &mCp);
      ed->distributeEvent(event);
      delete event;
   }
}

void GaeSplineEditWidget::addSplineChangeListener(GaeSplineEditWidget*
                                                  listener)
{
   mSplineChangeListeners.push_back(listener);
}

vector<GaeControlPoint*>* GaeSplineEditWidget::getControlPoints()
{
   return &mCp;
}

void GaeSplineEditWidget::userEvent(GmEvent* event)
{
   GaeEvent* gaeEvent;
   GaeEvent::RestructureEventType* restructureData;
   vector<GaeControlPoint*>::iterator i;
   vector<GaeControlPoint*>::iterator j;

   GlmPoint3 p1, p2;
   float x1, y1, z1;
   float x2, y2, z2;

   gaeEvent = (GaeEvent*)event;
   switch(gaeEvent->getGaeType())
   {
      case GaeTypes::E_RESTRUCTURE:
         restructureData = gaeEvent->getRestructureData();

         for (i=restructureData->controlPoints->begin(), j=mCp.begin();
              i!=restructureData->controlPoints->end();
              i++, j++)
         {
            (*i)->getRelativeGLPosition(p1);
            (*j)->getRelativeGLPosition(p2);

            p1.getXYZ(x1, y1, z1);
            p2.getXYZ(x2, y2, z2);

            switch(restructureData->viewOrientation)
            {
               case GaeTypes::VO_XY:
                  switch(mViewOrientation)
                  {
                     case GaeTypes::VO_ZX: y2 = x1; break;
                     case GaeTypes::VO_ZY: y2 = y1; break;
                     //case GaeTypes::V0_XYZ: x2 = x1; y2 = y1; break;
                     default: break;
                  }
                  break;
                  
               case GaeTypes::VO_ZX:
                  switch(mViewOrientation)
                  {
                     case GaeTypes::VO_XY: x2 = y1; break;
                     case GaeTypes::VO_ZY: x2 = x1; break;
                     //case GaeTypes::V0_XYZ: x2 = x1; y2 = y1; break;
                     default: break;
                  }
                  break;

               case GaeTypes::VO_ZY:
                  switch(mViewOrientation)
                  {
                     case GaeTypes::VO_XY: y2 = y1; break;
                     case GaeTypes::VO_ZX: x2 = x1; break;
                     //case GaeTypes::V0_XYZ: x2 = x1; y2 = y1; break;
                     default: break;
                  }
                  break;
                  
               default:
                  break;
            }

            p2.setXYZ(x2, y2, z2);
            (*j)->setRelativeGLPosition(p2);
         }
         break;
      default:
         break;
   }
}

GlmPoint3* GaeSplineEditWidget::getGlPosition()
{
   return &mGLPosition;
}

void GaeSplineEditWidget::render2d(GmTypes::RenderOptionType renderOptions)
{
   static float ang = 0.0;
   float x, y, z, w, h; // x, y, z, width and height values
   float r, g, b, a; // red, green, blue and alpha values

   mGLPosition.getXYZ(x, y, z);
   w = mGLWidth;
   h = mGLHeight;

   // Draw component border
   mBaseColor.getRGBA(r, g, b, a);
   glColor4f(r, g, b, a);

   glColor4f(0.2, 0.2, 0.2, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(x, y);
   glVertex2f(x+w, y);
   glVertex2f(x+w, y+h);
   glVertex2f(x, y+h);
   glEnd();

   glColor4f(0.75, 0.75, 0.75, 1.0);
   glBegin(GL_LINE_LOOP);
   glVertex2f(x, y);
   glVertex2f(x+w, y);
   glVertex2f(x+w, y+h);
   glVertex2f(x, y+h);
   glEnd();
   
   zoom(getZoomFactor() * mZoomFactor);
   renderControlPoints(renderOptions);
   
   mBaseColor.getRGBA(r, g, b, a);
   glColor4f(0.5, 0.7, 1.0, 1.0);   

   // Render curve
   switch(mViewOrientation)
   {
      case GaeTypes::VO_XY:
      case GaeTypes::VO_ZX:
      case GaeTypes::VO_ZY:
         glPushMatrix();
         glTranslatef(x, y, z);
         break;

      case GaeTypes::VO_XYZ:
         glPushMatrix();
         glTranslatef(x, y, z);
         break;     
   }
      
   glBegin(GL_LINE_STRIP);
   for(float t = 0.0; t <= 1.0; t += 0.02)
   {
      float x, y, z;
      GlmPoint3 p = mBezierSpline->getPositionByPercentage(t);
      p.getXYZ(x, y, z);

      switch(mViewOrientation)
      {
         case GaeTypes::VO_XY:
            x *= w;
            y *= h;
            glVertex2f(x, y);
            break;

         case GaeTypes::VO_ZX:
            x *= w;
            z *= h;
            glVertex2f(z, x);
            break;

         case GaeTypes::VO_ZY:
            y *= w;
            z *= h;
            glVertex2f(z, y);
            break;

         case GaeTypes::VO_XYZ:
            x *= w;
            y *= h;
            z *= w;

            //y -= (h / 2.0);
            //x -= (w / 2.0);
            //x = (x * cos(ang)) - (x * sin(ang));
            //y = (y * sin(ang)) + (y * cos(ang));
            x = x / z;
            y = y / z;
            
            ang += 0.001;
            //glVertex2f(x, y);
            //cout << "[" << x << ", " << y << ", " << z << "]" << endl;
            break;
      }
   }
   glEnd();

   switch(mViewOrientation)
   {
      case GaeTypes::VO_XY:
      case GaeTypes::VO_ZX:
      case GaeTypes::VO_ZY:
         glPopMatrix();
         break;

      case GaeTypes::VO_XYZ:
         glPopMatrix();
         break;
   }
   
   // Draw the label
   mOrientationLabel->render(renderOptions);
}

void GaeSplineEditWidget::render2dSelect(GmTypes::RenderOptionType
                                         renderOptions)
{
   renderControlPoints(renderOptions);
}

void  GaeSplineEditWidget::renderControlPoints(GmTypes::RenderOptionType
                                               renderOptions)
{
   float x1, y1, z;
   float x2, y2;
   float x3, y3;
   float x4, y4;
   GlmPoint3 p;
   
   glEnable(GL_LINE_STIPPLE);
         vector<GaeControlPoint*>::iterator c;

   if (renderOptions != GmTypes::RO_2D_SELECT)
   {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0xf0f0);
      glColor3f(0.8, 0.8, 0.0);

      c = mCp.begin();
      while (c != mCp.end())
      {
         (*c)->getGLPosition()->getXYZ(x1, y1, z);
         c++;

         (*c)->getGLPosition()->getXYZ(x2, y2, z);
         c++;
         
         (*c)->getGLPosition()->getXYZ(x3, y3, z);
         c++;
      
         (*c)->getGLPosition()->getXYZ(x4, y4, z);
         c++;

         glBegin(GL_LINE_LOOP);
         glVertex2f(x1, y1);
         glVertex2f(x2, y2);
         glVertex2f(x4, y4);
         glVertex2f(x3, y3);
         glEnd();

         if(c == mCp.end())
            break;
         else
            c--;
      }
      glDisable(GL_LINE_STIPPLE);
   }
   
   for(c = mCp.begin(); c != mCp.end(); c++)
   {
      (*c)->render(renderOptions);
   }
}

void GaeSplineEditWidget::layoutInGLArea(float minX, float minY,
                                         float width, float height,
                                         float layer)
{
   //// TEMP CODE!!
   layoutInGLVolume(minX, minY, layer, width, height, layer);
}

void GaeSplineEditWidget::layoutInGLVolume(float minX, float minY, float minZ,
                                           float width, float height,
                                           float depth)
{
   float x, y, z;
   GlmPoint3 minConstraint;
   GlmPoint3 maxConstraint;
   
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);
   
   for(vector<GaeControlPoint*>::iterator c = mCp.begin();
       c != mCp.end(); c++)
   {
      mGLPosition.getXYZ(x, y, z);
      (*c)->layoutInGLVolume(x, y, z, mGLWidth, mGLHeight, mGLDepth);

      minConstraint = mGLPosition;

      minConstraint.getXYZ(x, y, z);
      x += mGLWidth;
      y += mGLHeight;
      maxConstraint.setXYZ(x, y, z);
      (*c)->setConstraints(minConstraint, maxConstraint);
   }

   mGLPosition.getXYZ(x, y, z);
   mOrientationLabel->layoutInGLVolume(x, y, z, mGLWidth, mGLHeight, mGLDepth);

   zoom(getZoomFactor());
   
}

void GaeSplineEditWidget::onControlPointDrag(float mouseX,
                                             float mouseY,
                                             GaeControlPoint* cp)
{
   float x, y, z;
   float cpx, cpy, cpz;
   float dist;
   float factor;
   static float lastDragX, lastDragY;
     
   mGLPosition.getXYZ(x, y, z); 
   cp->getGLPosition()->getXYZ(cpx, cpy, cpz);
   //cp->getGLPosition()->setXYZ(mouseX, mouseY, cpz);

   if (mouseX > x + mGLWidth ||
       mouseX < x ||
       mouseY > y + mGLHeight ||
       mouseY < y)
   {
      GlmLine2 line(mouseX, mouseY, cpx, cpy);

      dist = line.length() * 0.25;
      //if(lastDragX > mouseX || lastDragY > mouseY)
         //   dist = -dist;
      
      factor = ((1 / ((mGLWidth + mGLHeight) / 2)) * dist + 1);

      factor = 1 / factor;
      factor *= mZoomFactor;
      zoom(factor);
   }
   else
   {
      //zoom(getZoomFactor());
      //zoom(0.5);
      
   }

   lastDragX = mouseX;
   lastDragY = mouseY;
   
   //zoom(getZoomFactor());
   //cp->getGLPosition()->setXYZ(cpx, cpy, cpz);
   
}

void GaeSplineEditWidget::addSegment()
{
   float x, y, z;
   float p2x, p2y, p3x, p3y, p4x, p4y;
   GaeControlPoint* c;
   GlmPoint3 p;
   GlmPoint3 minConstraint;
   GlmPoint3 maxConstraint;

   minConstraint = mGLPosition;
   minConstraint.getXYZ(x, y, z);
   x += mGLWidth;
   y += mGLHeight;
   maxConstraint.setXYZ(x, y, z);
   
   (mCp[mCp.size() - 1])->getGLPosition()->getXYZ(x, y, z);

   if(mViewOrientation == GaeTypes::VO_XY ||
      mViewOrientation == GaeTypes::VO_ZY)
   {   
     p2x = x + 5.0;
     p2y = y + 5.0;
     p3x = x + 5.0;
     p3y = y - 5.0;
     p4x = x + 10.0;
     p4y = y;
   }
   else
   {
     p2x = x - 5.0;
     p2y = y + 5.0;
     p3x = x + 5.0;
     p3y = y + 5.0;
     p4x = x;
     p4y = y + 10.0;      
   }
   
   c = new GaeControlPoint(this);
   c->getGLPosition()->setXYZ(p2x, p2y, 0.0);
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   c->setConstraints(minConstraint, maxConstraint);
   mCp.push_back(c);

   c = new GaeControlPoint(this);
   c->getGLPosition()->setXYZ(p3x, p3y, 0.0);
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   c->setConstraints(minConstraint, maxConstraint);
   mCp.push_back(c);
   
   c = new GaeControlPoint(this);
   c->getGLPosition()->setXYZ(p4x, p4y, 0.0);
   c->setBaseColor(0.8, 0.8, 0.8, 1.0);
   c->setConstraints(minConstraint, maxConstraint);
   mCp.push_back(c);
}

void GaeSplineEditWidget::initControlPoints()
{
   GaeControlPoint* c;

   if(mViewOrientation == GaeTypes::VO_XY ||
      mViewOrientation == GaeTypes::VO_ZY)
   {
      c = new GaeControlPoint(this);
      c->setPosition(0.025, 0.5, 0.0);
      c->setBaseColor(0.8, 0.8, 0.8, 1.0);
      mCp.push_back(c);

      c = new GaeControlPoint(this);
      c->setPosition(0.5, 0.95, 0.0);
      c->setBaseColor(0.8, 0.0, 0.0, 1.0);
      mCp.push_back(c);

      c = new GaeControlPoint(this);
      c->setPosition(0.5, 0.05, 0.0);
      c->setBaseColor(0.8, 0.0, 0.0, 1.0);
      mCp.push_back(c);

      c = new GaeControlPoint(this);
      c->setPosition(0.975, 0.5, 0.0);
      c->setBaseColor(0.8, 0.8, 0.8, 1.0);
      mCp.push_back(c);
   }
   else if(mViewOrientation == GaeTypes::VO_ZX)
   {
      c = new GaeControlPoint(this);
      c->setPosition(0.5, 0.025, 0.0);
      c->setBaseColor(0.8, 0.8, 0.8, 1.0);
      mCp.push_back(c);

      c = new GaeControlPoint(this);
      c->setPosition(0.025, 0.5, 0.0);
      c->setBaseColor(0.8, 0.0, 0.0, 1.0);
      mCp.push_back(c);

      c = new GaeControlPoint(this);
      c->setPosition(0.975, 0.5, 0.0);
      c->setBaseColor(0.8, 0.0, 0.0, 1.0);
      mCp.push_back(c);

      c = new GaeControlPoint(this);
      c->setPosition(0.5, 0.975, 0.0);
      c->setBaseColor(0.8, 0.8, 0.8, 1.0);
      mCp.push_back(c);
   }
}

void GaeSplineEditWidget::zoom(float factor)
{
   GlmPoint3 p;
   float x, y, z;
   float temp;

   temp = factor;
   factor /= mZoomFactor;
   mZoomFactor = temp;
   
   for(vector<GaeControlPoint*>::iterator c = mCp.begin();
       c != mCp.end(); c++)
   {
      (*c)->getRelativeGLPosition(p);

      p.getXYZ(x, y, z);
      x -= (mGLWidth / 2.0);
      y -= (mGLHeight / 2.0);

      x *= factor;
      y *= factor;

      x += (mGLWidth / 2.0);
      y += (mGLHeight / 2.0);

      p.setXYZ(x, y, z);
      (*c)->setRelativeGLPosition(p);
   }
}

float GaeSplineEditWidget::getZoomFactor()
{
   GlmPoint3 p;
   float max = 0.0;
   float x, y, z;
   
   for(vector<GaeControlPoint*>::iterator c = mCp.begin();
       c != mCp.end(); c++)
   {
      (*c)->getRelativeGLPosition(p);
      p.getXYZ(x, y, z);

      x -= (mGLWidth / 2.0);
      y -= (mGLHeight / 2.0);

      if(abs(x) > max)
         max = abs(x);

      if (abs(y) > max)
         max = abs(y);
   }
   
   if(mGLWidth > mGLHeight)
      return (mGLWidth / 2.0) / max;
   else
      return (mGLHeight / 2.0) / max;
}

