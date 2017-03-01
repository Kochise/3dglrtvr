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
#ifndef GMMETER_H
#define GMMETER_H

#include <glam/GmTypes.h>
#include <glam/GmScalarWidget.h>

/**
 * A base class to represent a meter, something which measures
 * a position within a defined range.
 */
class GmMeter: public GmScalarWidget
{
 protected:
   /**
    * The orientation of the meter. This allows the object to
    * know how it should draw itself.
    */
   GmTypes::OrientationType mOrientation;

   /**
    * The width of the border around the meter
    */
   float mBorderWidth;
   
 public:
   /**
    * Default constructor
    */
   GmMeter();

   /**
    * Virtual destructor
    */
   virtual ~GmMeter();

   /**
    * Set the current orientation of the meter. This
    * informs the object how do draw itsef.
    *
    * @param orientation
    */
   virtual void setOrientation(GmTypes::OrientationType orientation);
   
   /**
    * Get the current orientation of the meter.
    *
    * @return the current orientation of the meter
    */
   virtual GmTypes::OrientationType getOrientation();

   /**
    * Sets the width of the border around the meter. The border width
    * is the total percentage that the border should visually
    * encompass the meter.
    *
    * @param border the width of the border around the meter. The
    *               value is the total percentage that the border
    *               should visually encompass the meter
    */
   virtual void setBorderWidth(float borderWidth);
   
   /**
    * Gets the width of the border around the meter.
    *
    * @return the width of the border around the meter
    */
   virtual float getBorderWidth();
};

#endif
