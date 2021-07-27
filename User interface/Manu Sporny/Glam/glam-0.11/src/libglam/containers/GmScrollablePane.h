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
#ifndef GMSCROLLABLEPANE_H
#define GMSCROLLABLEPANE_H

#include <glam/GmViewport.h>
#include <glam/GmScrollbar.h>

/**
 * This is a scrollable viewport area that allows for content larger
 * than the viewable area.
 */
class GmScrollablePane : public GmViewport, public GmScalarListener,
                         public GmGeometryListener
{
 protected:
   /**
    * The horizontal scrollbar
    */
   GmScrollbar* mHorzScrollbar;

   /**
    * The vertical scrollbar
    */
   GmScrollbar* mVertScrollbar;

   /**
    * Holds the widget ids for the two scrollbars
    */
   enum PaneWidgetIds
      {
         /**
          * Token to identify the horizontal scrollbar
          */
         SCROLLBAR_H,
         
         /**
          * Token to identify the vertical scrollbar
          */
         SCROLLBAR_V
      };

 public:
   /**
    * standard constructor
    */
   GmScrollablePane();
   
   /**
    * virtual destructor
    */
   virtual ~GmScrollablePane();

   /**
    * Adds a drawable component to the container.
    *
    * @param component the component to add to the container. DO NOT
    *                  DELETE THIS ARGUMENT AFTER THE METHOD CALL. The
    *                  container doesn't make a copy, rather it uses
    *                  the object that is passed in directly. This
    *                  class will handle the components finalization/deletion.
    */
   virtual void addComponent(GmDrawable* component);

   /**
    * Adds an interactive component.
    */
   virtual void addComponent(GmInteractiveWidget* component);

   /**
    * Called whenever the associated scalar widget is changed.
    *
    * @param action the scalar action involved in the change.
    */
   virtual void changeOccurred(GmScalarAction* action);

   /**
    * Called whenever an associated widget has its geometry changed.
    *
    * @param action the geometry action involved in geometry change.
    */
   virtual void geometryResized(GmGeometryAction* action);

   /**
    * Sets the GL position of the drawable.
    *
    * @param x the X position of the drawable object
    * @param y the Y position of the drawable object
    * @param z the Z position of the drawable object
    */
   virtual void setGLPosition(float x, float y, float z);

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
   
 protected:

   /**
    * Determines the size of contained objects and sets the scrollbar
    * values appropriately.
    */
   virtual void setScrollbarValues();

   /**
    * Renders this listbox to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the scrollbar.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);
   
   /**
    * Renders this listbox to a 2D selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the scrollbar.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
   
};

#endif
