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
#ifndef GM2DLAYOUT_H
#define GM2DLAYOUT_H

#include <glam/GmLayoutManager.h>

/**
 * This layout causes all components that reside within a
 * container to be positioned at the same depth, which is
 * a certain distance (the depth differential) from their
 * container. All components are top level, such that the
 * container is in the background and they are in the
 * foreground.
 */
class Gm2dLayout : public GmLayoutManager
{
 protected:
   /**
    * The component depth differential for this layout. This
    * number refers to difference in layer distance between all components
    * and the container they reside within.
    */
   float mCDD;

 public:
   /**
    * Standard constructor
    */
   Gm2dLayout();

   /**
    * Virtual destructor
    */
   virtual ~Gm2dLayout();

   /**
    * Applies the layout to the container. All components will be
    * positioned according to the layout design. This function
    * must be implemented by inheriting layout classes.
    *
    * @param components the vector of components in the container
    */
   virtual void apply(vector<GmDrawable*>* components);

   /**
    * Associates the container's position and dimensions with this
    * layout manager. Association is used so that layout managers
    * do not have to be updated if container positions or dimensions
    * change.
    *
    * @param position the starting position of the constraint volume
    * @param width the width of the constraint volume
    * @param height the height of the constraint volume
    */
   void setContainerDimensions(GlmPoint3* position,
                               float& width, float& height, float& depth);

   /**
    * Sets the component depth differential.
    *
    * @param the component depth differential to use
    */
   void setComponentDepth(float cdd);

   /**
    * Gets the component depth differential.
    *
    * @return the component depth differential
    */
   float getComponentDepth();

   /**
    * Gets the ideal extents of the drawable in OpenGL units. This is
    * sometimes used by parent containers to determine the optimal
    * layout strategy by taking each components extents into account.
    *
    * @param components the components of the layout which need to be sized
    * @param width the width of the drawable in OpenGL units
    * @param height the height of the drawable in OpenGL units
    */
   virtual void getGLExtents(vector<GmDrawable*>* components,
                             float &width, float &height);
};

#endif
