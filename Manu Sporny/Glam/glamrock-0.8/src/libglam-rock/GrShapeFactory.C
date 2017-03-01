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
#include <math.h>
#include <glmath/GlmBezierCurve2.h>
#include <glamrock/GrShapeFactory.h>

#define PI 3.141526

GrShapeFactory* gGrShapeFactoryInstance = NULL;

GrShapeFactory::GrShapeFactory()
{
   mCurrentFilletSize = 0.0;
   mCurrentFillStyle = GrTypes::FS_SOLID;
   mFirstPoint = NULL;
   mLastPoint = NULL;

   mTesselator = gluNewTess();
   gluTessCallback(mTesselator,
                   GLU_TESS_BEGIN, (GLvoid(*)())&glBegin);
   gluTessCallback(mTesselator,
                   GLU_TESS_VERTEX, (GLvoid(*)())&tessVertexCallback);
   gluTessCallback(mTesselator,
                   GLU_TESS_END, (GLvoid(*)())&glEnd);
   gluTessCallback(mTesselator,
                   GLU_TESS_ERROR, (GLvoid(*)())&tessErrorCallback);
   gluTessCallback(mTesselator,
                   GLU_TESS_COMBINE, (GLvoid(*)())&tessCombineCallback);
   gluTessProperty(mTesselator,
                   GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
}

GrShapeFactory::~GrShapeFactory()
{
   gluDeleteTess(mTesselator);
   clearPoints();
}

GrShapeFactory* GrShapeFactory::getInstance()
{
   if(gGrShapeFactoryInstance == NULL)
      gGrShapeFactoryInstance = new GrShapeFactory();

   return gGrShapeFactoryInstance;
}

void GrShapeFactory::renderRoundedRect(GrTypes::BorderStyle bStyle,
                                       GrTypes::FillStyle fStyle,
                                       float width, float height,
                                       float rounding)
{
   float minV, maxV, minH, maxH, minr;
   GlmBezierCurve2 bc; // bezier curve to use for edges

   if(width > height)
      minr = height * rounding;
   else
      minr = width * rounding;   
   
   minV = minr;
   maxV = height - minV;
   minH = minr;
   maxH = width - minH;
   
   // Draw the basic panel
   if(fStyle == GrTypes::FS_OUTLINE)
      glBegin(GL_LINE_LOOP);
   else if(fStyle == GrTypes::FS_SOLID)
      glBegin(GL_POLYGON);
   
   // Draw the lower-left corner
   glVertex2f(minH, 0.0);
   if(bStyle & GrTypes::BS_BOTTOM_LEFT)
   {
      bc.setCoefficients(minH, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, minV);
      for(float p = 0.01; p < 0.99; p += 0.05)
         glVertex2fv(bc.getPositionByPercentage(p));
   }
   else
      glVertex2f(0.0, 0.0);
   
   glVertex2f(0.0, minV);
   
   // Draw the left side of the panel
   glVertex2f(0.0, maxV);
   
   // Draw the upper-left corner
   if(bStyle & GrTypes::BS_TOP_LEFT)
   {
      bc.setCoefficients(0.0, maxV, 0.0, height, 0.0,
                         height, minH, height);
      for(float p = 0.01; p < 0.99; p += 0.05)
         glVertex2fv(bc.getPositionByPercentage(p));
   }
   else
      glVertex2f(0.0, height);
   
   glVertex2f(minH, height);

   // Draw the top part of the panel
   glVertex2f(maxH, height);
   
   // Draw the top right corner
   if(bStyle & GrTypes::BS_TOP_RIGHT)
   {
      bc.setCoefficients(maxH, height, width, height,
                         width, height, width, maxV);
      for(float p = 0.01; p < 0.99; p += 0.05)
         glVertex2fv(bc.getPositionByPercentage(p));
   }
   else
      glVertex2f(width, height);
   
   glVertex2f(width, maxV);
   
   // Draw the right side of the panel
   glVertex2f(width, minV);
   
   // Draw the bottom-right part of the panel
   if(bStyle & GrTypes::BS_BOTTOM_RIGHT)
   {
      bc.setCoefficients(width, minV, width, 0.0,
                         width, 0.0, maxH, 0.0);
      for(float p = 0.01; p < 0.99; p += 0.05)
         glVertex2fv(bc.getPositionByPercentage(p));
   }
   else
      glVertex2f(width, 0.0);
         
   glVertex2f(maxH, 0.0);
   glEnd();
}

void GrShapeFactory::renderCircle(GrTypes::FillStyle fStyle,
                                  float radius, int slices)
{
   float angleInc;
   float pX1, pX2, pY1, pY2;
      
   /* draw a circle from a bunch of short lines */
   pX1 = 0.0;
   pY1 = radius;
   angleInc = 3.14159/float(slices);
   
   if(fStyle == GrTypes::FS_OUTLINE)
      glBegin(GL_LINE_LOOP);
   else if(fStyle == GrTypes::FS_SOLID)
      glBegin(GL_POLYGON);
   for(float angle = 0.0; angle <= (2.0 * 3.14159); angle += angleInc)
   {
      pX2 = radius * (float)sin((double)angle);
      pY2 = radius * (float)cos((double)angle);
      glVertex2f(pX1, pY1);
      pY1=pY2;
      pX1=pX2;			
   }
   glEnd();
}

void GrShapeFactory::sfBegin(GrTypes::FillStyle fillStyle)
{
   mCurrentFillStyle = fillStyle;
   clearPoints();
}

void GrShapeFactory::sfVertex2f(float x, float y)
{
   GrShapeFactoryPoint* p;

   p = new GrShapeFactoryPoint;
   p->mCoordinate[0] = x;
   p->mCoordinate[1] = y;
   p->mCoordinate[2] = 0.0;
   p->mFilletSize = mCurrentFilletSize;

   p->mPreviousPoint = mLastPoint;
   if (mLastPoint)
      mLastPoint->mNextPoint = p;
   
   if (!mFirstPoint)
      mFirstPoint = p;
   mLastPoint = p;
}


void GrShapeFactory::tessErrorCallback(GLenum errorCode)
{
   printf ("GrShapeFactory: Tessellation Error: %s\n",
           gluErrorString(errorCode));
}

/*
 * Taken from "OpenGl Programming guide ver 1.1" page 416. Memory
 * leak?!
 */
void GrShapeFactory::tessCombineCallback(GLdouble coords[3],
                                         GLdouble *vertex_data[4],
                                         GLfloat weight[4],
                                         GLdouble **dataOut)
{
   GLdouble *vertex;
   
   vertex = (GLdouble*) malloc(3 * sizeof(GLdouble));
   vertex[0] = coords[0];
   vertex[1] = coords[1];
   vertex[2] = coords[2];

   *dataOut = vertex;
}

void GrShapeFactory::tessVertexCallback(GLdouble* coords)
{
   glVertex3dv(coords);
}

void GrShapeFactory::sfEnd()
{
   bool firstPoint;
   float x1, y1;
   float x2, y2;
   float x3, y3;
   float f1, f2, f3;
   float c1x, c1y;
   float c2x, c2y;

   float vx, vy;
   float l;
   
   float filletSize;
   GLdouble* tempGlPoint;
   GlmPoint2 tempGlmPoint;
   GrShapeFactoryPoint* p;
   GlmBezierCurve2 bc;
   vector<GLdouble*> tempPoints;
   vector<GLdouble*>::iterator i;
   
   if (mFirstPoint && mLastPoint)
   {
      // Make sure points form a loop
      mFirstPoint->mPreviousPoint = mLastPoint;
      mLastPoint->mNextPoint = mFirstPoint;

      // Begin appropriate GL drawing style
      if (mCurrentFillStyle == GrTypes::FS_OUTLINE)
      {
         glBegin(GL_LINE_LOOP);
      }
      else
      {
         gluTessBeginPolygon(mTesselator, NULL);
         gluTessBeginContour(mTesselator);         
      }

      // Go through every point, drawing the geometry as we go
      firstPoint = true;
      p = mFirstPoint;
      while (p != mFirstPoint || firstPoint)
      {
         firstPoint = false;

         x1 = p->mPreviousPoint->mCoordinate[0];
         y1 = p->mPreviousPoint->mCoordinate[1];
         x2 = p->mCoordinate[0];
         y2 = p->mCoordinate[1];
         x3 = p->mNextPoint->mCoordinate[0];
         y3 = p->mNextPoint->mCoordinate[1];

         f1 = p->mPreviousPoint->mFilletSize;
         f2 = p->mFilletSize;
         f3 = p->mNextPoint->mFilletSize;
         
         if (f2 > 0.0)
         {
            // There is a fillet for this point. Draw a curve.
            
            // calculate the 1st control point
            vx = x2 - x1;
            vy = y2 - y1;
            l = sqrt(vx * vx + vy * vy);

            if (l < f1 + f2)
               filletSize = l / 2.0;
            else
               filletSize = f2;
            
            if (l != 0)
            {
               vx /= l;
               vy /= l;
            }

            c1x = x2 - vx * filletSize;
            c1y = y2 - vy * filletSize;

            // calculate the 2nd control point
            vx = x3 - x2;
            vy = y3 - y2;
            l = sqrt(vx * vx + vy * vy);

            if (l < f2 + f3)
               filletSize = l / 2.0;
            else
               filletSize = f2;
            
            if (l != 0)
            {
               vx /= l;
               vy /= l;
            }

            c2x = x2 + vx * filletSize;
            c2y = y2 + vy * filletSize;

            // Setup the bezier curve and draw it
            bc.setCoefficients(c1x, c1y,
                               (c1x + x2) / 2.0, (c1y + y2) / 2.0,
                               (c2x + x2) / 2.0, (c2y + y2) / 2.0,
                               c2x, c2y);
            
            if (mCurrentFillStyle == GrTypes::FS_OUTLINE)
            {
               for(float p = 0.0; p < 1.01; p += 0.05)
                  glVertex2fv(bc.getPositionByPercentage(p));
            }
            else
            {
               for(float p = 0.0; p < 1.01; p += 0.05)
               {
                  tempGlPoint = new GLdouble[3];
                  
                  tempGlmPoint = bc.getPositionByPercentage(p);
                  tempGlPoint[0] = tempGlmPoint[0];
                  tempGlPoint[1] = tempGlmPoint[1];
                  tempGlPoint[2] = 0.0;

                  gluTessVertex(mTesselator, tempGlPoint, tempGlPoint);

                  tempPoints.push_back(tempGlPoint);
               }
            }
         }
         else
         {
            // No fillet for this point.  Just do a single vertex.
            if (mCurrentFillStyle == GrTypes::FS_OUTLINE)
            {
               glVertex2f(x2, y2);
            }
            else
            {
               tempGlPoint = new GLdouble[3];

               tempGlPoint[0] = x2;
               tempGlPoint[1] = y2;
               tempGlPoint[2] = 0.0;

               gluTessVertex(mTesselator, tempGlPoint, tempGlPoint);

               tempPoints.push_back(tempGlPoint);
            }
         }
         
         p = p->mNextPoint;
      }

      // End the appropriate GL drawing style
      if (mCurrentFillStyle == GrTypes::FS_OUTLINE)
      {
         glEnd();
      }
      else
      {
         gluTessEndContour(mTesselator);
         gluTessEndPolygon(mTesselator);      
      }
   }

   // Cleanup any points used by tesselator
   for (i = tempPoints.begin(); i != tempPoints.end(); i++)
      delete (*i);
}

void GrShapeFactory::sfFilletSize(float filletSize)
{
   mCurrentFilletSize = filletSize;
}

void GrShapeFactory::clearPoints()
{
   GrShapeFactoryPoint* p1;
   GrShapeFactoryPoint* p2;

   if (mFirstPoint && mLastPoint)
   {
      mLastPoint->mNextPoint = NULL;
      p1 = mFirstPoint;
      while (p1)
      {
         p2 = p1;
         p1 = p1->mNextPoint;

         delete p2;
      }   
   
      mFirstPoint = NULL;
      mLastPoint = NULL;
   }
}
