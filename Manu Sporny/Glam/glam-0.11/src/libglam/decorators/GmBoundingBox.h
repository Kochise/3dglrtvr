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
#ifndef GMBOUNDINGBOX_H
#define GMBOUNDINGBOX_H

#include <glam/GmTypes.h>
#include <glam/GmDecorator.h>

/**
 * This class defines a visual bounding box.
 */
class GmBoundingBox : public GmDecorator
{
 protected:
   /**
    * The fill type for the box.
    */
   GmTypes::FillType mFillType;

   /**
    * The line type for the outline of the box.
    */
   GmTypes::LineType mOutlineType;

   /**
    * The outline thickness.
    */
   float mOutlineThickness;

 public:
   /**
    * Standard constructor.
    */
   GmBoundingBox();

   /**
    * Standard destructor.
    */
   virtual ~GmBoundingBox();   
};

#endif
