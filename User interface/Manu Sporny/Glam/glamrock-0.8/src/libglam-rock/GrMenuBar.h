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
#ifndef GRMENUBAR_H
#define GRMENUBAR_H

#include <glam/GmMenuBar.h>

/**
 * The bar meter class embodies a horizontal or vertical progress bar.
 */
class GrMenuBar : public GmMenuBar
{
 public:
   /**
    * Standard constructor
    */
   GrMenuBar();

   /**
    * Virtual destructor
    */
   virtual ~GrMenuBar();
   
 protected:
   /**
    * Renders this menu bar to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the panel.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);
   
   /**
    * Renders this menu bar to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the dialog.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
