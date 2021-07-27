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
#ifndef GREXPANDABLELISTITEM_H
#define GREXPANDABLELISTITEM_H

#include <glam/GmExpandableListItem.h>

/**
 * A visual display that has two states, expanded and
 * not expanded. When expanded, more information is provided in
 * the larger display.
 */
class GrExpandableListItem : public GmExpandableListItem
{
 public:
   /**
    * Standard constructor for GrExpandableListItem which takes two
    * arguments for its title and body component.
    *
    * @param titleComponent the component that will be shown when the
    *                       expandable list item is collapsed.
    * @param bodyComponent the component that will be shown when the
    *                      expandable list item is expanded. The title
    *                      component will be shown when the list item
    *                      is expanded as well.
    */
   GrExpandableListItem(GmDrawable* titleComponent, GmDrawable* bodyComponent);
   
 protected:
   /**
    * Standard constructor.
    */
   GrExpandableListItem();
   
   /**
    * Virtual destructor.
    */
   virtual ~GrExpandableListItem();
   
   /**
    * Renders this button to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this menu item to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
