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
#ifndef GAESPLINEEDITWIDGET_H
#define GAESPLINEEDITWIDGET_H

#include <vector>
#include <glmath/GlmBezierSpline3.h>
#include <glam/GmDrawable.h>
#include <glam/GmLabel.h>
#include <GaeControlPoint.h>
#include <GaeTypes.h>

/**
 * The GaeCurveEditPanel widget is used to modify several different
 * types of animation curve primitives (Hermite, Bezier, etc).
 */
class GaeControlPoint;
class GaeSplineEditWidget : public GmInteractiveWidget
{
 protected:
   float mZoomFactor;
   GmLabel* mOrientationLabel;
   GaeTypes::ViewOrientationType mViewOrientation;
   vector<GaeControlPoint*> mCp;
   
 public:   
   /**
    * Constructor that accepts the view orientation of this spline
    * edit widget.
    *
    * @param viewOrientation The view orientation that this widget
    *                        will have.
    */
   GaeSplineEditWidget(GaeTypes::ViewOrientationType viewOrientation);
   
   /**
    * Virtual destructor
    */
   virtual ~GaeSplineEditWidget();

   /**
    * Sets which spline this widget will display/modify.
    *
    * @param bezierSpline Address of the bezier spline that will be
    * associated with this widget.
    */
   void setSpline(GlmBezierSpline3* bezierSpline);

   /**
    * Sets the view orientation of this widget.
    *
    * @param viewOrientation The new orientation value for this
    * widget.
    */
   void setViewOrientation(GaeTypes::ViewOrientationType viewOrientation);

   /**
    * Broadcasts events to edits when a spline is edited (i.e. a
    * control point is moved).
    */
   void broadcastSplineChange();
      
   /**
    * Adds the specified widget to the list that this widget
    * broadcasts messages to.
    *
    * @param listener The widget to add to the list.
    */
   void addSplineChangeListener(GaeSplineEditWidget* listener);

   /**
    * Gets control points of this edit widget.
    *
    * @return Pointer to the vector of control points.
    */
   vector<GaeControlPoint*>* getControlPoints();

   /**
    * Function that is called when an user event is received.
    */
   virtual void userEvent(GmEvent* event);
   
   /**
    * Gets the GL position of the drawable object.
    *
    * @return The GL position of the drawable object
    */
   virtual GlmPoint3* getGlPosition();

   /**
    * Repositions and resizes this component based on parameters
    * passed into the method. These parameters are usually passed in
    * by the parent class.
    *
    * @param minX the starting X position of the constraint volume
    * @param minY the starting Y position of the constraint volume
    * @param width the width of the constraint volume
    * @param height the height of the constraint volume
    * @param layer the layer the component should be placed in
    */
   virtual void layoutInGLArea(float minX, float minY,
                               float width, float height,
                               float layer);

   /**
    * Repositions and resizes this component based on parameters
    * passed into the method. These parameters are usually passed in
    * by the parent class.
    *
    * @param minX the starting X position of the constraint volume
    * @param minY the starting Y position of the constraint volume
    * @param minZ the starting Z position of the constraint volume
    * @param width the width of the constraint volume
    * @param height the height of the constraint volume
    * @param depth the depth of the constraint volume
    */
   virtual void layoutInGLVolume(float minX, float minY, float minZ,
                                 float width, float height, float depth);

   /**
    * Called when a control point is dragged with the mouse.
    *
    * @param x The x coordinate of the mouse cursor.
    * @param y The y coordinate of the mouse cursor.
    * @param cp Pointer to the control point that called the function.
    */
   void onControlPointDrag(float mouseX, float mouseY, GaeControlPoint* cp);

   /**
    * Adds a segment to the spline that is being edited.
    */
   void addSegment();
   
 protected:
   /**
    * Bezier spline that will store the Bezier basis co-efficients.
    */
   GlmBezierSpline3* mBezierSpline;

   /**
    * List of widgets that listen to this widget's events.
    */
   vector<GaeSplineEditWidget*> mSplineChangeListeners;

   /**
    * Renders this control point to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      this control point.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this widget to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      this widget.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders the control points.
    *
    * @param renderOptions the render options to use when rendering
    *                      this widget's control points.    
    */
   void renderControlPoints(GmTypes::RenderOptionType renderOptions);

   /**
    * Initializes control points.
    */
   void initControlPoints();

   /**
    * Test function for zooming
    */
   void zoom(float factor);

   /**
    * Test function for determining how much to zoom
    */
   float getZoomFactor();
};

#endif
