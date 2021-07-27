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
#ifndef GMCONTENTPANE_H
#define GMCONTENTPANE_H

#include <glam/GmContainer.h>

/**
 * A content pane provides an invisible rectangular pane
 * for grouping components together.
 */
class GmContentPane : public GmContainer
{
 public:
   /**
    * Standard constructor
    */
   GmContentPane();

   /**
    * Virtual destructor
    */
   virtual ~GmContentPane();
   
 protected:
   /**
    * Renders this panel to a 2D image buffer. A content pane is
    * invisible.
    *
    * @param renderOptions the render options to use when rendering
    *                      the panel.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this panel to a selection buffer. A content pane is
    * invisible.
    *
    * @param renderOptions the render options to use when rendering
    *                      the panel.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
