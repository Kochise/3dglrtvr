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
#include <values.h>
#include <glam/GmOptions.h>
#include <glam/GmMeter.h>

GmMeter::GmMeter()
      :GmScalarWidget()
{
   GmOptions* gmOptions = GmOptions::getInstance();
   float borderWidth;

   borderWidth = gmOptions->getScalarOption(GMO_METER_BORDERWIDTH);
   
   if(borderWidth != MAXFLOAT)
      mBorderWidth = borderWidth;
}

GmMeter::~GmMeter()
{
}

void GmMeter::setOrientation(GmTypes::OrientationType orientation)
{
   mOrientation = orientation;
}

GmTypes::OrientationType GmMeter::getOrientation()
{
   return mOrientation;
}

void GmMeter::setBorderWidth(float borderWidth)
{
   mBorderWidth = borderWidth;
}

float GmMeter::getBorderWidth()
{
   return mBorderWidth;
}
