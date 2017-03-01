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
#ifndef GMBARMETER_H
#define GMBARMETER_H

#include <glam/GmMeter.h>

/**
 * A class to represent the bar form of a meter. This could be used,
 * for example, as a progress bar.
 */
class GmBarMeter : public GmMeter
{
 protected:
   /**
    * Color of the bar. 
    */
   GlmColor4 mBarColor;
   
 public:
   /**
    * Standard constructor
    */
   GmBarMeter();
   
   /**
    * Virtual destructor
    */
   virtual ~GmBarMeter();

   /**
    * Sets the bar color of the object. 
    *
    * @param r the red value 
    * @param g the green value 
    * @param b the blue value
    * @param a the alpha value 
    */
   virtual void setBarColor(float r, float g, float b, float a);
   
   /**
    * Sets the bar color of the object.
    *
    * @param color the base color
    */
   virtual void setBarColor(GlmColor4* color);

   /**
    * Gets the bar color of the object.
    *
    * @return the base color of the object, THIS IS A BORROWED
    *         POINTER, DO NOT DELETE
    */
   virtual GlmColor4* getBarColor();
   
 protected:
   /**
    * Renders this meter to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the meter.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this meter to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the meter.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
