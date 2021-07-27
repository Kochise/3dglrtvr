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
#ifndef GMLISTLAYOUT_H
#define GMLISTLAYOUT_H

#include <glam/GmFlowLayout.h>

/**
 * This layout positions all of the components that
 * reside within a container according to their minimum height
 * and the width of the container. They flow like a vertical
 * list.  The bottom inset is irrelevant in a list layout. The
 * only cell insets that are pertinent are top and bottom.
 */
class GmListLayout : public GmFlowLayout
{
 public:
   /**
    * Standard constructor
    */
   GmListLayout();

   /**
    * Virtual destructor
    */
   virtual ~GmListLayout();

   /**
    * Applies the layout to the container. All components will be
    * positioned according to the layout design. This function
    * must be implemented by inheriting layout classes.
    *
    * @param components the vector of components in the container
    */
   virtual void apply(vector<GmDrawable*>* components);

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
