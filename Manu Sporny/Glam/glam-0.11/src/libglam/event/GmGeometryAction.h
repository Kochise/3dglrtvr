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
#ifndef GMGEOMETRYACTION_H
#define GMGEOMETRYACTION_H

#include <glam/GmWidgetAction.h>

/**
 * This action encapsulates information about actions that changed
 * the geometry of a widget.
 */
class GmGeometryAction : public GmWidgetAction
{
 protected:
   /**
    * The change of the geometry width.
    */
   float mWidthChange;

   /**
    * The change of the geometry height.
    */
   float mHeightChange;

   /**
    * The new geometry width.
    */
   float mNewWidth;

   /**
    * The new geometry height.
    */
   float mNewHeight;
   
   /**
    * Standard constructor.
    */
   GmGeometryAction();

 public:
   /**
    * Constructs a geometry action with the given source.
    *
    * @param source the widget that is the source
    * @param widthChange the change in width
    * @param heightChange the change in height
    * @param newWidth the new width
    * @param newHeight the new height
    */
   GmGeometryAction(GmInteractiveWidget* source,
                    float widthChange, float heightChange,
                    float newWidth, float newHeight);

   /**
    * Constructs a geometry action with the given
    * object id and widget id of the source.
    *
    * @param objectId the object id of the source
    * @param widgetId the widget id of the source
    * @param widthChange the change in width
    * @param heightChange the change in height
    * @param newWidth the new width
    * @param newHeight the new height
    */
   GmGeometryAction(unsigned short srcObjectId, WidgetIDType srcWidgetId,
                    float widthChange, float heightChange,
                    float newWidth, float newHeight);

   /**
    * Virtual destructor
    */
   virtual ~GmGeometryAction();
   
   /**
    * Gets the change in geometry.
    *
    * @param width the change in width
    * @param height the change in height
    */
   virtual void getChange(float& width, float& height);

   /**
    * Gets the new geometry.
    *
    * @param width the new geometry width
    * @param height the new geometry height
    */
   virtual void getNewGeometry(float& width, float& height);
   
   /**
    * Gets the width change.
    *
    * @param width the change in width
    */
   virtual void getWidthChange(float& width);

   /**
    * Gets the height change.
    *
    * @param height the change in the height
    */
   virtual void getHeightChange(float& height);

   /**
    * Gets the new width.
    *
    * @param width the new geometry width
    */
   virtual void getNewWidth(float& width);

   /**
    * Gets the new height.
    *
    * @param height the new geometry height
    */
   virtual void getNewHeight(float& height);
};

#endif
