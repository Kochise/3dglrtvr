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
#ifndef GAECURVEEDITPANEL_H
#define GAECURVEEDITPANEL_H

#include <glmath/GlmBezierSpline3.h>
#include <glam/GmPanel.h>

/**
 * The GaeCurveEditPanel widget is used to modify several different
 * types of animation curve primitives (Hermite, Bezier, etc).
 */
class GaeCurveEditWidget : public GmPanel
{
 public:
   /**
    * Standard constructor
    */
   GaeCurveEditWidget();
   
   /**
    * Virtual destructor
    */
   virtual ~GaeCurveEditWidget();

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
   
 protected:
   /**
    * Bezier spline that will store the Bezier basis co-efficients.
    */
   GlmBezierSpline3 mBezierCurve;

   /**
    * The current t-value. The t-value range is from 0.0-1.0 and is
    * used to determine what the curve position is at a particular
    * t. The t-value is used by the curve edit widget to draw a
    * vertical line to show the current queried value by the
    * application and is shown via a vertical green bar in the curve
    * edit widget
    */
   float mTValue;
   
   /**
    * Renders this control point to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the control point.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this control point to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the control point.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
