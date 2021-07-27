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
#ifndef GCECONTROLPOINT_H
#define GCECONTROLPOINT_H

#include <glam/GmPushButton.h>

/**
 * The GceControlPoint widget is used to modify control points. A
 * common use of control points is in modifying the parameters for a curve
 */
class GceControlPoint : public GmPushButton
{
 public:
   enum CurveType
   {
      CM_HERMITE_CURVE,
      CM_BEZIER_CURVE,
      CM_HERMITE_SPLINE,
      CM_BEZIER_SPLINE
   };
   
   /**
    * Standard constructor
    */
   GceControlPoint();
   
   /**
    * Virtual destructor
    */
   virtual ~GceControlPoint();

   /**
    * Event processing function to process a mouse drag event.
    *
    * @param event the mouse drag event for this object to process
    */
   virtual void mouseDragEvent(GmEvent* event);

 protected:
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
