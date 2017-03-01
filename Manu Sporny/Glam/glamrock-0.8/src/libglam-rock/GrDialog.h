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
#ifndef GRDIALOG_H
#define GRDIALOG_H

#include <glmath/GlmPoint3.h>
#include <glam/GmDialog.h>

/**
 * A glamrock style dialog box.
 */
class GrDialog : public GmDialog
{
 public:
   /**
    * Standard constructor.
    */
   GrDialog();

   /**
    * Virtual destructor.
    */
   virtual ~GrDialog();

   /**
    * Repositions and resizes this component based on parameters
    * passed into the method. These parameters are usually passed in
    * by the parent class.
    *
    * @param minX the starting X position of the constraint volume
    * @param minY the starting Y position of the constraint volume
    * @param width the width of the constraint volume
    * @param height the height of the constraint volume
    * @param layer the layer the component should be placed in
    */
   virtual void layoutInGLArea(float minX, float minY,
                               float width, float height,
                               float layer);

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
    * Renders this dialog to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the dialog
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this dialog to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the dialog
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
   
};

#endif
