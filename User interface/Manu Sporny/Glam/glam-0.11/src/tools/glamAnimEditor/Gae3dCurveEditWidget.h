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
#ifndef GAE3DCURVEEDITPANEL_H
#define GAE3DCURVEEDITPANEL_H

#include <glmath/GlmBezierSpline3.h>
#include <glam/GmPanel.h>
#include <glam/GmLabel.h>
#include <GaeSplineEditWidget.h>

/**
 * The GaeCurveEditPanel widget is used to modify several different
 * types of animation curve primitives (Hermite, Bezier, etc).
 */
class Gae3dCurveEditWidget : public GmPanel
{
 protected:
   GaeSplineEditWidget* mXYView;
   GaeSplineEditWidget* mZXView;
   GaeSplineEditWidget* mZYView;
   GaeSplineEditWidget* mXYZView;
   
   /**
    * Bezier spline that will store the Bezier basis co-efficients.
    */
   GlmBezierSpline3 mBezierSpline;

   /**
    * Button used to add a segment to the spline being edited.
    */
   GmPushButton* mAddSegmentButton;
   
 public:
   /**
    * Standard constructor
    */
   Gae3dCurveEditWidget();
   
   /**
    * Virtual destructor
    */
   virtual ~Gae3dCurveEditWidget();

   /**
    * Get the position given a t-value in the range of 0.0
    * to 1.0.
    *
    * @param t the t-value to use to evaluate the curve, you could
    *          think of the t-value as the position along the curve
    *          from 0% to 100%.
    * @return a GlmPoint3 object that provides the 3D coordinates at
    *         the given t-value. The returned value will have the X, Y
    *         and Z position at the given t-value.
    */
   GlmPoint3 getSplinePosition(float t);
   
   /**
    * Get the rotation in degrees given a t-value in the range of 0.0
    * to 1.0.
    *
    * @param t the t-value to use to evaluate the curve, you could
    *          think of the t-value as the position along the curve
    *          from 0% to 100%.
    * @return a GlmPoint3 object that provides the 3D coordinates at
    *         the given t-value. The returned value will have the X, Y
    *         and Z rotation at the given t-value.
    */
   GlmPoint3 getSplineRotation(float t);

   /**
    * Get the scaling factor given a t-value in the range of 0.0
    * to 1.0.
    *
    * @param t the t-value to use to evaluate the curve, you could
    *          think of the t-value as the position along the curve
    *          from 0% to 100%.
    * @return a GlmPoint3 object that provides the 3D scale factors at
    *         the given t-value. The returned value will have the X, Y
    *         and Z scale factors at the given t-value.
    */
   GlmPoint3 getSplineScale(float t);

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
    * Event handler called when a key has been pressed.
    *
    * @param event The pointer to the keypress event.
    */
   virtual void keypressEvent(GmEvent* event);

   static void test();
   
   
 protected:

   /**
    * Renders this control point to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the widget.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Recompute the spline based on the location of the control points.
    */
   void recomputeSpline();

   /**
    * Adds a segment to the spline that is being edited.
    */
   void addSegment();
};

#endif
