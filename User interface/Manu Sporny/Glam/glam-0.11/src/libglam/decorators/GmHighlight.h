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
#ifndef GMHIGHLIGHT_H
#define GMHIGHLIGHT_H

#include <glam/GmTypes.h>
#include <glam/GmDecorator.h>

/**
 * This class has not yet been implemented.
 */
class GmHighlight : public GmDecorator
{
 protected:
   /**
    * The type of highlighting to use.
    */
   GmTypes::HighlightType mHighlightType;
   
 public:
   /**
    * Standard constructor.
    */
   GmHighlight();

   /**
    * Virtual destructor.
    */
   virtual ~GmHighlight();
   
   /**
    * Sets the highlight type. Good luck with no params! :)
    */
   void setHighlightType();
   
   /**
    * Gets the highlight type.
    *
    * @return the highlight type.
    */
   GmTypes::HighlightType getHighlightType();
};

#endif
