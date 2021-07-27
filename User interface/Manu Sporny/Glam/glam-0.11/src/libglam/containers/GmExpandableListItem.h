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
#ifndef GMEXPANDABLELISTITEM_H
#define GMEXPANDABLELISTITEM_H

#include <glam/GmInteractiveWidget.h>

/**
 * Encapsulates the functionality of a list item in a
 * viewable/selectable list. A list item could be in a scrollable list
 * box, pull-down menu, pop-up menu, or any other UI component that
 * contains a list of widgets.
 */
class GmExpandableListItem : public GmInteractiveWidget
{
 protected:
   /**
    * The title component of this list item.
    */
   GmDrawable* mTitleComponent;

   /**
    * The body component of this list item.
    */
   GmDrawable* mBodyComponent;
   
   /**
    * Variable indicating whether the widget is expanded or
    * unexpanded.
    */
   GmTypes::ExpandStateType mExpandState;

   /**
    * Standard protected constructor, we want to force developers to
    * provide title and body components for the expandable list item.
    */
   GmExpandableListItem();

 public:
   /**
    * Standard constructor for GmExpandableListItem which takes two
    * arguments for its title and body component.
    *
    * @param titleComponent the component that will be shown when the
    *                       expandable list item is collapsed.
    * @param bodyComponent the component that will be shown when the
    *                      expandable list item is expanded. The title
    *                      component will be shown when the list item
    *                      is expanded as well.
    */
   GmExpandableListItem(GmDrawable* titleComponent, GmDrawable* bodyComponent);

   /**
    * Virtual destructor
    */
   virtual ~GmExpandableListItem();
   
   /**
    * Event processing function to process a mouse press event.
    *
    * @param event the mouse press event for this object to process
    */
   virtual void mousePressEvent(GmEvent* event);
   
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
    * Sets which widget we should draw in the unexpanded and expanded
    * state.
    *
    * @return the unexpanded widget to use for this expandable list item
    */
   virtual GmDrawable* getTitleWidget();

    /**
    * Sets which widget we should draw in the expanded state.
    *
    * @return the expanded widget to use for this expandable list item
    */
   virtual GmDrawable* getBodyWidget();
   
   /**
    * Gets the ideal extents of the drawable in OpenGL units. This is
    * sometimes used by parent containers to determine the optimal
    * layout strategy by taking each components extents into account.
    *
    * @param width the width of the drawable in OpenGL units
    * @param height the height of the drawable in OpenGL units
    */
   virtual void getGLExtents(float& width, float& height);
   
 protected:
   /**
    * Renders this menu item to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this menu item to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
