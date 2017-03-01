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
#ifndef GMPANEL_H
#define GMPANEL_H

#include <glam/GmContentPane.h>

/**
 * A Panel class for adding and displaying widgets
 * Unlike a content pane, the panel is actually drawn to the screen
 */
class GmPanel : public GmContentPane
{
 public:
   /**
    * Standard constructor
    */
   GmPanel();

   /**
    * Virtual destructor
    */
   virtual ~GmPanel();
   
 protected:
   /**
    * Renders this panel to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the panel.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this panel to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the panel.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
