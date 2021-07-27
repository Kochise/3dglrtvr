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
#include <GceCurveEditor.h>
#include <GceControlPoint.h>
#include <GceCurveUI.h>

GceCurveUI::GceCurveUI()
{
}

GceCurveUI::~GceCurveUI()
{
}

void GceCurveUI::renderHermiteCurve(GmTypes::RenderOptionType renderOptions)
{
   float p1x, p1y, p4x, p4y, r1x, r1y, r4x, r4y, dummy;
   
   (gCp[0]->getPosition())->getXYZ(p1x, p1y, dummy);
   (gCp[1]->getPosition())->getXYZ(r1x, r1y, dummy);
   (gCp[2]->getPosition())->getXYZ(p4x, p4y, dummy);
   (gCp[3]->getPosition())->getXYZ(r4x, r4y, dummy);
   
   if(RENDER_OPTION_ISSET(renderOptions, RO_CONTROL_POINTS))
   {
      // Render the tangent vectors
      glBegin(GL_LINES);
      glColor3f(0.8, 0.8, 0.0);
      glVertex2f(p1x, p1y);
      glVertex2f(r1x, r1y);
      glVertex2f(p4x, p4y);
      glVertex2f(r4x, r4y);
      
      // Render the control points
      gCp[0]->render(renderOptions);
      gCp[1]->render(renderOptions);
      gCp[2]->render(renderOptions);
      gCp[3]->render(renderOptions);
   }

   // re-scale the values
   p1x /= 100.0;
   p4x /= 100.0;
   r1x /= 25.0;
   r4x /= 25.0;
   p1y /= 100.0;
   p4y /= 100.0;
   r1y /= 25.0;
   r4y /= 25.0;

   hermiteCurve.setCoefficients(p1x, p1y, r1x, r1y,
                                p4x, p4y, r4x, r4y);
   
   glColor3f(0.8, 0.8, 0.8);
   glBegin(GL_LINE_STRIP);
   for(float t = 0.0; t <= 1.0; t += 0.001)
   {
      GlmPoint2 p;
      float x, y;
      
      p = hermiteCurve.getPositionByPercentage(t);
      p.getXY(x, y);
      x *= 100.0;
      y *= 100.0;
      glVertex2f(x, y);
   }
   glEnd();
}

void GceCurveUI::renderBezierCurve(GmTypes::RenderOptionType renderOptions)
{
   float p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, dummy;
   
   (gCp[0]->getPosition())->getXYZ(p1x, p1y, dummy);
   (gCp[1]->getPosition())->getXYZ(p2x, p2y, dummy);
   (gCp[2]->getPosition())->getXYZ(p3x, p3y, dummy);
   (gCp[3]->getPosition())->getXYZ(p4x, p4y, dummy);

   if(RENDER_OPTION_ISSET(renderOptions, RO_CONTROL_POINTS))
   {
      // Render the convex hull for the bezier curve
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0xf0f0);
      glColor3f(0.8, 0.8, 0.0);
      glBegin(GL_LINE_LOOP);
      glVertex2f(p1x, p1y);
      glVertex2f(p2x, p2y);
      glVertex2f(p4x, p4y);
      glVertex2f(p3x, p3y);
      glEnd();
      glDisable(GL_LINE_STIPPLE);
      
      // Render the control points
      gCp[0]->render(renderOptions);
      gCp[1]->render(renderOptions);
      gCp[2]->render(renderOptions);
      gCp[3]->render(renderOptions);
   }
   
   p1x /= 100.0;
   p1y /= 100.0;
   p2x /= 100.0;
   p2y /= 100.0;
   p3x /= 100.0;
   p3y /= 100.0;
   p4x /= 100.0;
   p4y /= 100.0;

   bezierCurve.setCoefficients(p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y);
   
   glColor3f(0.8, 0.8, 0.8);
   glBegin(GL_LINE_STRIP);
   for(float t = 0.0; t <= 1.0; t += 0.001)
   {
      GlmPoint2 p;
      float x, y;
      
      p = bezierCurve.getPositionByPercentage(t);
      p.getXY(x, y);
      x *= 100.0;
      y *= 100.0;
      glVertex2f(x, y);
   }
   glEnd();
}
  
void GceCurveUI::initHermiteCurveWidgets()
{
   GceControlPoint* c;

   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(0.0, 0.0, 0.0);
   c->setText("P1");
   c->setBaseColor(0.8, 0.8, 0.8, 1.0);
   gCp.push_back(c);
   
   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(25.0, 25.0, 0.0);
   c->setText("R1");
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   gCp.push_back(c);

   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(100.0, 0.0, 0.0);
   c->setText("P4");
   c->setBaseColor(0.8, 0.8, 0.8, 1.0);
   gCp.push_back(c);
   
   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(75.0, 75.0, 0.0);
   c->setText("R4");
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   gCp.push_back(c);
}

void GceCurveUI::removeHermiteCurveWidgets()
{
   //   GmEventDistributor* ed = GmEventDistributor::getInstance();

   //   for(unsigned int i = 0; i < gCp.size(); i++)
      //      ed->unregisterWidget(gCp[i]);
   
   gCp.clear();
   gWidgets.clear();
}

void GceCurveUI::initBezierCurveWidgets()
{
   GceControlPoint* c;

   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(0.0, 0.0, 0.0);
   c->setText("P1");
   c->setBaseColor(0.8, 0.8, 0.8, 1.0);
   gCp.push_back(c);
   
   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(25.0, 25.0, 0.0);
   c->setText("P2");
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   gCp.push_back(c);

   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(75.0, -25.0, 0.0);
   c->setText("P3");
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   gCp.push_back(c);

   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(100.0, 0.0, 0.0);
   c->setText("P4");
   c->setBaseColor(0.8, 0.8, 0.8, 1.0);
   gCp.push_back(c);
}

void GceCurveUI::removeBezierCurveWidgets()
{
   //   GmEventDistributor* ed = GmEventDistributor::getInstance();
   //   for(unsigned int i = 0; i < gCp.size(); i++)
   //      ed->unregisterWidget(gCp[i]);

   gCp.clear();
   gWidgets.clear();
}

void GceCurveUI::renderHermiteSpline(GmTypes::RenderOptionType renderOptions)
{
   // Modify the spline based on each control point
   int i = 0;
   int ci = 0;
   float pstep = 1.0 / (float((gCp.size() - 1)) / 3.0);
   float percent = 0.0;
   for(vector<GceControlPoint*>::iterator c = gCp.begin();
       c != gCp.end(); c += 3, i++, percent += pstep)
   {
      float p1x, p1y, p4x, p4y, r1x, r1y, r4x, r4y, dummy;
   
      ((gCp[ci++])->getPosition())->getXYZ(p1x, p1y, dummy);
      ((gCp[ci++])->getPosition())->getXYZ(r1x, r1y, dummy);
      ((gCp[ci++])->getPosition())->getXYZ(r4x, r4y, dummy);
      ((gCp[ci])->getPosition())->getXYZ(p4x, p4y, dummy);
      
      if(RENDER_OPTION_ISSET(renderOptions, RO_CONTROL_POINTS))
      {
         // Render the tangent vectors
         glBegin(GL_LINES);
         glColor3f(0.8, 0.8, 0.0);
         glVertex2f(p1x, p1y);
         glVertex2f(r1x, r1y);
         glVertex2f(p4x, p4y);
         glVertex2f(r4x, r4y);
         glEnd();
      }

      // re-scale the values
      p1x /= 100.0;
      p4x /= 100.0;
      r1x /= 25.0;
      r4x /= 25.0;
      p1y /= 100.0;
      p4y /= 100.0;
      r1y /= 25.0;
      r4y /= 25.0;
      
      hermiteSpline.setSplineSegment(i, percent,
                                     p1x, p1y, r1x, r1y,
                                     p4x, p4y, r4x, r4y);
      if(i == 0)
         c++;
   }   

   // Render the entire spline
   glColor3f(0.8, 0.8, 0.8);
   glBegin(GL_LINE_STRIP);
   for(float t = 0.0; t <= 1.0; t += 0.001)
   {
      GlmPoint2 p;
      float x, y;
      
      p = hermiteSpline.getPositionByPercentage(t);
      p.getXY(x, y);
      x *= 100.0;
      y *= 100.0;
      glVertex2f(x, y);
   }
   glEnd();
   
   // Render each control point
   if(RENDER_OPTION_ISSET(renderOptions, RO_CONTROL_POINTS))
   {
      for(vector<GceControlPoint*>::iterator c = gCp.begin();
          c != gCp.end(); c++)
         (*c)->render(renderOptions);
   }
}

void GceCurveUI::renderBezierSpline(GmTypes::RenderOptionType renderOptions)
{
   // Modify the spline based on each control point
   int i = 0;
   int ci = 0;
   float pstep = 1.0 / (float((gCp.size() - 1)) / 3.0);
   float percent = 0.0;
   for(vector<GceControlPoint*>::iterator c = gCp.begin();
       c != gCp.end(); c += 3, i++, percent += pstep)
   {
      float p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, dummy;
   
      ((gCp[ci++])->getPosition())->getXYZ(p1x, p1y, dummy);
      ((gCp[ci++])->getPosition())->getXYZ(p2x, p2y, dummy);
      ((gCp[ci++])->getPosition())->getXYZ(p3x, p3y, dummy);
      ((gCp[ci])->getPosition())->getXYZ(p4x, p4y, dummy);

      if(RENDER_OPTION_ISSET(renderOptions, RO_CONTROL_POINTS))
      {
         // Render the convex hull for the bezier curve
         glEnable(GL_LINE_STIPPLE);
         glLineStipple(1, 0xf0f0);
         glColor3f(0.8, 0.8, 0.0);
         glBegin(GL_LINE_LOOP);
         glVertex2f(p1x, p1y);
         glVertex2f(p2x, p2y);
         glVertex2f(p4x, p4y);
         glVertex2f(p3x, p3y);
         glEnd();
         glDisable(GL_LINE_STIPPLE);
      }
      
      // re-scale the values
      p1x /= 100.0;
      p1y /= 100.0;
      p2x /= 100.0;
      p2y /= 100.0;
      p3x /= 100.0;
      p3y /= 100.0;
      p4x /= 100.0;
      p4y /= 100.0;
      
      bezierSpline.setSplineSegment(i, percent,
                                    p1x, p1y, p2x, p2y,
                                    p3x, p3y, p4x, p4y);
      if(i == 0)
         c++;
   }

   // Render the entire spline
   glColor3f(0.8, 0.8, 0.8);
   glBegin(GL_LINE_STRIP);
   for(float t = 0.0; t <= 1.0; t += 0.001)
   {
      GlmPoint2 p;
      
      p = bezierSpline.getPositionByPercentage(t);
      p *= 100.0;
      glVertex2fv(p);
   }
   glEnd();
   
   // Render each control point
   if(RENDER_OPTION_ISSET(renderOptions, RO_CONTROL_POINTS))
   {
      for(vector<GceControlPoint*>::iterator c = gCp.begin();
          c != gCp.end(); c++)
         (*c)->render(renderOptions);
   }
}

void GceCurveUI::addHermiteSegment()
{
   float x, y, z;
   float r1x, r1y, p4x, p4y, r4x, r4y;
   GceControlPoint* c;
   GlmPoint3* p;
   unsigned int segments;
   char labelText[16];

   segments = ((gCp.size() - 1) / 3) + 1;
   
   cout << "Adding Hermite Segment: " << segments << endl;
   p = (gCp[gCp.size() - 1])->getPosition();
   p->getXYZ(x, y, z);
   
   r1x = x + 10.0;
   r1y = y + 10.0;
   p4x = x + 30.0;
   p4y = y;
   r4x = x + 20.0;
   r4y = y + 10.0;
   hermiteSpline.addSplineSegment(0.5,
                                  r1x, r1y, p4x, p4y, r4x, r4y);

   c = new GceControlPoint();
   sprintf(labelText, "R%ib", segments);
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(r1x, r1y, 0.0);
   c->setText(labelText);
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   gCp.push_back(c);

   c = new GceControlPoint();
   sprintf(labelText, "R%ia", segments+1);
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   x += 10.0;
   y -= 20.0;
   c->setPosition(r4x, r4y, 0.0);
   c->setText(labelText);
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   gCp.push_back(c);
   
   c = new GceControlPoint();
   sprintf(labelText, "P%i", segments+1);
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   x += 10.0;
   y += 10.0;
   c->setPosition(p4x, p4y, 0.0);
   c->setText(labelText);
   c->setBaseColor(0.8, 0.8, 0.8, 1.0);
   gCp.push_back(c);
}

void GceCurveUI::addBezierSegment()
{
   float x, y, z;
   float p2x, p2y, p3x, p3y, p4x, p4y;
   GceControlPoint* c;
   GlmPoint3* p;
   unsigned int segments;
   char labelText[16];

   segments = gCp.size();
   
   p = (gCp[gCp.size() - 1])->getPosition();
   p->getXYZ(x, y, z);
   
   p2x = x + 10.0;
   p2y = y + 10.0;
   p3x = x + 20.0;
   p3y = y - 10.0;
   p4x = x + 30.0;
   p4y = y;
   bezierSpline.addSplineSegment(0.99,
                                 p2x, p2y, p3x, p3y, p4x, p4y);

   c = new GceControlPoint();
   sprintf(labelText, "P%i", segments+1);
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(p2x, p2y, 0.0);
   c->setText(labelText);
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   gCp.push_back(c);

   c = new GceControlPoint();
   sprintf(labelText, "P%i", segments+2);
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(p3x, p3y, 0.0);
   c->setText(labelText);
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   gCp.push_back(c);
   
   c = new GceControlPoint();
   sprintf(labelText, "P%i", segments+3);
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(p4x, p4y, 0.0);
   c->setText(labelText);
   c->setBaseColor(0.8, 0.8, 0.8, 1.0);
   gCp.push_back(c);
}

void GceCurveUI::initHermiteSplineWidgets()
{
   GceControlPoint* c;
   GmPushButton* b = new GmPushButton();
   
   b->setBaseColor(0.4, 0.4, 0.9, 0.9);
   b->setBorderWidth(0.1);
   b->setText("Add Segment");
   b->setTextJustification(GmTypes::JTH_CENTER,GmTypes::JTV_MIDDLE);
   b->setWidgetId(ADD_HERMITE_SEGMENT_BUTTON);
   b->addButtonListener(this);
   b->layoutInGLArea(0.0, -20.0, 40.0, 10.0, 0.0);
   gWidgets.push_back(b);

   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(0.0, 0.0, 0.0);
   c->setText("P1");
   c->setBaseColor(0.8, 0.8, 0.8, 1.0);
   gCp.push_back(c);
   
   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(25.0, 25.0, 0.0);
   c->setText("P2");
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   gCp.push_back(c);

   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(75.0, 75.0, 0.0);
   c->setText("R2a");
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   gCp.push_back(c);
   
   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(100.0, 0.0, 0.0);
   c->setText("P2");
   c->setBaseColor(0.8, 0.8, 0.8, 1.0);
   gCp.push_back(c);
}

void GceCurveUI::initBezierSplineWidgets()
{
   GceControlPoint* c;
   GmPushButton* b = new GmPushButton();
   
   b->setBaseColor(0.4, 0.4, 0.9, 0.9);
   b->setBorderWidth(0.1);
   b->setText("Add Segment");
   b->setTextJustification(GmTypes::JTH_CENTER,GmTypes::JTV_MIDDLE);
   b->setWidgetId(ADD_BEZIER_SEGMENT_BUTTON);
   b->addButtonListener(this);
   gWidgets.push_back(b);
   b->layoutInGLArea(0.0, -20.0, 40.0, 10.0, 0.0);

   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(0.0, 0.0, 0.0);
   c->setText("P1");
   c->setBaseColor(0.8, 0.8, 0.8, 1.0);
   gCp.push_back(c);
   
   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(25.0, 25.0, 0.0);
   c->setText("P2");
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   gCp.push_back(c);

   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(75.0, -25.0, 0.0);
   c->setText("P3");
   c->setBaseColor(0.8, 0.0, 0.0, 1.0);
   gCp.push_back(c);
   
   c = new GceControlPoint();
   c->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);
   c->setPosition(100.0, 0.0, 0.0);
   c->setText("P4");
   c->setBaseColor(0.8, 0.8, 0.8, 1.0);
   gCp.push_back(c);
}

void GceCurveUI::removeHermiteSplineWidgets()
{
   //   GmEventDistributor* ed = GmEventDistributor::getInstance();
   //   for(unsigned int i = 0; i < gCp.size(); i++)
   //      ed->unregisterWidget(gCp[i]);

   gCp.clear();
   gWidgets.clear();
}

void GceCurveUI::removeBezierSplineWidgets()
{
   //   GmEventDistributor* ed = GmEventDistributor::getInstance();
   //   for(unsigned int i = 0; i < gCp.size(); i++)
   //      ed->unregisterWidget(gCp[i]);

   gCp.clear();
   gWidgets.clear();
}

void GceCurveUI::buttonPressed(GmButtonAction* action)
{
   switch(action->getSrcWidgetId())
   {
      case ADD_HERMITE_SEGMENT_BUTTON:
         addHermiteSegment();
         break;
      case ADD_BEZIER_SEGMENT_BUTTON:
         addBezierSegment();
         break;
      default:
         break;
   }
}
