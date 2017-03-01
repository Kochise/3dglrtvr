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
#ifndef GAECONTROLPOINT_H
#define GAECONTROLPOINT_H

#include <glam/GmPushButton.h>
#include <GaeSplineEditWidget.h>
#include <GaeTypes.h>

/**
 * The GaeControlPoint widget is used to modify control points. A
 * common use of control points is in modifying the parameters for a curve
 */
class GaeSplineEditWidget;
class GaeControlPoint : public GmPushButton
{
 protected:
   /**
    * Parent spline edit widget.
    */
   GaeSplineEditWidget* mParentWidget;
   
   /**
    * Indicates if this control point is constrained or not.
    */
   bool mConstrained;

   /**
    * Min constraint.
    */
   GlmPoint3 mMinConstraint;

   /**
    * Max constraint.
    */
   GlmPoint3 mMaxConstraint;
   
 public:
   /**
    * Standard constructor
    */
   GaeControlPoint();

   /**
    * Overloaded constructor
    */
   GaeControlPoint(GaeSplineEditWidget* parentWidget);
   
   /**
    * Virtual destructor
    */
   virtual ~GaeControlPoint();

   /**
    * Event processing function to process a mouse drag event.
    *
    * @param event the mouse drag event for this object to process
    */
   virtual void mouseDragEvent(GmEvent* event);

   /**
    * Gets the GL position of the drawable object.
    *
    * @return The GL position of the drawable object
    */
   virtual GlmPoint3* getGLPosition();
   
   /**
    * Gets the relative GL position of the drawable object.
    *
    * @param The point that will store the current
    *        relative GL position of the drawable object.
    */
   void getRelativeGLPosition(GlmPoint3& point);
   
   /**
    * Sets the relative GL position of the drawable object.
    *
    * @param The point to set the relative GL position
    *        of the drawable object to.
    */
   void setRelativeGLPosition(GlmPoint3 point);

  /**
    * Sets the max constraint of this control point. The control point
    * can't be moved "past" these two point.
    *
    * @param minPoint The control point can't be moved "below" this
    *                 point.
    * @param maxPoint The control point can't be moved "above" this
    *                 point.
    */
   void setConstraints(GlmPoint3& minPoint, GlmPoint3& maxPoint);
   
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
