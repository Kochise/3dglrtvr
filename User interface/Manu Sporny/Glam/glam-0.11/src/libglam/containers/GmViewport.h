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
#ifndef GMVIEWPORT_H
#define GMVIEWPORT_H

#include <glam/GmContainer.h>
   
/**
 * This is a clipped viewport that allows for content larger than the
 * viewable area.
 */
class GmViewport : public GmContainer
{
 protected:

   /**
    * The clipping plane equations for openGL.
    */
   GLdouble clipPlane0[4];
   GLdouble clipPlane1[4];
   GLdouble clipPlane2[4];
   GLdouble clipPlane3[4];
   
 public:
   /**
    * Standard constructor
    */
   GmViewport();

   /**
    * Virtual destructor
    */
   virtual ~GmViewport();

 protected:
   /**
    * Enables the clipping planes for this viewport
    */
   virtual void beginClip();

   /**
    * Enables the clipping planes for this viewport - overloaded for
    * dynamic clipping area changes
    *
    * @param minX the bottom horizontal edge of clipping
    * @param minY the bottom vertical edge of clipping
    * @param maxX the top horizontal edge of clipping
    * @param maxY the top vertical edge of clipping
    */
   virtual void beginClip(float minX, float minY,
                          float maxX, float maxY);

   /**
    * Disables the clipping planes for this viewport
    */
   virtual void endClip();

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
