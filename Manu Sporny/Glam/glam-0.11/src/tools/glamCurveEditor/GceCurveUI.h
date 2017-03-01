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
#ifndef GCECURVEUI_H
#define GCECURVEUI_H

#include <glam/GmTypes.h>
#include <glam/GmButtonListener.h>

#define ADD_HERMITE_SEGMENT_BUTTON 0
#define ADD_BEZIER_SEGMENT_BUTTON  1

class GceCurveUI : public GmButtonListener
{
 protected:
   GlmHermiteCurve2 hermiteCurve;
   GlmBezierCurve2 bezierCurve;
   GlmHermiteSpline2 hermiteSpline;
   GlmBezierSpline2 bezierSpline;

 public:
   GceCurveUI();
   virtual ~GceCurveUI();
   void renderHermiteCurve(GmTypes::RenderOptionType renderOptions);
   void renderBezierCurve(GmTypes::RenderOptionType renderOptions);
   void renderHermiteSpline(GmTypes::RenderOptionType renderOptions);
   void renderBezierSpline(GmTypes::RenderOptionType renderOptions);
   void initHermiteCurveWidgets();
   void removeHermiteCurveWidgets();
   void initBezierCurveWidgets();
   void removeBezierCurveWidgets();
   void initHermiteSplineWidgets();
   void removeHermiteSplineWidgets();
   void initBezierSplineWidgets();
   void removeBezierSplineWidgets();
   void addHermiteSegment();
   void addBezierSegment();

   virtual void buttonPressed(GmButtonAction* action);
};

#endif
