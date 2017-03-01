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
#ifndef GRLABEL_H
#define GRLABEL_H

#include <glam/GmLabel.h>
#include <glamrock/GrPanel.h>

/**
 * The GrLabel widget is a GmLabel that renders on top of a
 * GrPanel.
 */
class GrLabel : public GmLabel
{
 protected:
   /**
    * The panel the label renders on top of.
    */
   GrPanel* mPanel;

 public:
   /**
    * Standard constructor
    */
   GrLabel();

   /**
    * Virtual destructor
    */
   virtual ~GrLabel();

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
    * Renders this drawable to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the drawable.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this drawable to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the drawable.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
