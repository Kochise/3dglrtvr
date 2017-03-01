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
#ifndef GRSHAPEFACTORY_H
#define GRSHAPEFACTORY_H

#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glamrock/GrTypes.h>

using namespace std;

struct GrShapeFactoryPoint
{
   GLdouble mCoordinate[3];
   float mFilletSize;

   GrShapeFactoryPoint* mNextPoint;
   GrShapeFactoryPoint* mPreviousPoint;
};

/**
 * The shape factory singleton can draw a variety of curved objects
 * and surfaces to an OpenGL canvas.
 */
class GrShapeFactory
{
 protected:
   /**
    * The current fillet size. Applied to every new point
    */
   float mCurrentFilletSize;

   /**
    * The glu tesselator used to draw solid shape factory geometries.
    */
   GLUtesselator* mTesselator;

   /**
    * The current style used to fill shapes drawn by the shape
    * factory.
    */
   GrTypes::FillStyle mCurrentFillStyle;

   /**
    * The first point in the shape factory's point list.
    */
   GrShapeFactoryPoint* mFirstPoint;

   /**
    * The last point in the shape factory's point list.
    */
   GrShapeFactoryPoint* mLastPoint;

   /**
    * Standard constructor
    */
   GrShapeFactory();

   /**
    * Virtual destructor
    */
   virtual ~GrShapeFactory();

   /**
    * Error callback for the tesselator.
    *
    * @param errorCode The error code for the tesselator error.
    */
   static void tessErrorCallback(GLenum errorCode);
   
   /**
    * Combine vertex callback for the tesselator. Called when
    * tesselator needs to combine vertices
    *
    * @param coords The coordinate of the new vertex.
    * @param vertex_data Pointers to the vertices that were combined.
    * @param weight The weight of each vertex that went into the
    *               combined vertex.
    * @param dataOut The resulting data of the new vertex.
    */
   static void tessCombineCallback(GLdouble coords[3],
                                   GLdouble *vertex_data[4],
                                   GLfloat weight[4],
                                   GLdouble **dataOut);

   /**
    * The vertex callback for the tesselator.  Called when the
    * tesselator draws a vertex.
    *
    * @param coords The coordinate of the vertex being drawn.
    */
   static void tessVertexCallback(GLdouble* coords);

   /**
    * Frees the memory of the list of points.
    */
   virtual void clearPoints();
   
 public:
   /**
    * Gets an instance of the GrShapeFactory singleton.
    *
    * @return the instance of the GrShapeFactory singleton.
    */
   static GrShapeFactory* getInstance();
   
   /**
    * Renders a rounded rectangle to the screen.
    *
    * @param bStyle the border style to use when drawing the
    *               component. The factory will only round the corners
    *               that are specified in the border style. For
    *               example, if you want to round the bottom edges of
    *               a shape, but not the top you could specify the
    *               following as the border style:
    *               (GrTypes::BS_TOP_LEFT | GrTypes::BS_TOP_RIGHT)
    * @param fStyle the fill style to use, for example, whether you
    *               want the shape to be solid, or just an outline.
    * @param width the width of the rectangle
    * @param height the height of the rectangle
    * @param rounding the percentage of width and height that should
    *                 be rounded 
    *
    */
   virtual void renderRoundedRect(GrTypes::BorderStyle bStyle,
                                  GrTypes::FillStyle fStyle,
                                  float width, float height, float rounding);

   /**
    * Renders a circle to the screen.
    *
    * @param fStyle the fill style to use, for example, whether you
    *               want the shape to be solid, or just an outline.
    * @param radius the radius of the circle in OpenGL units
    * @param slices the number of slices to use when rendering the
    *               circle, the more slices that are used, the
    *               smoother the edges of the circle will be.
    */
   virtual void renderCircle(GrTypes::FillStyle fStyle, float radius,
                             int slices);

   /**
    * Indicates we are beginning a shape factory shape.
    *
    * @param fillStyle The fill style of the shape we are drawing.
    */
   virtual void sfBegin(GrTypes::FillStyle fillStyle);

   /**
    * Indicates the end of a shape factory shape. This triggers the
    * actual drawing of the shape.
    */
   virtual void sfEnd();

   /**
    * Adds a vertex to the current shape factory shape.
    *
    * @param x The x coordinate of the vertex.
    * @param y The y coordinate of the vertex.
    */
   virtual void sfVertex2f(float x, float y);

   /**
    * Changes the fillet size of all subsequent points for the shape
    * factory shape being drawn.
    *
    * @param filletSize The new fillet size.
    */
   virtual void sfFilletSize(float filletSize);
};

#endif
