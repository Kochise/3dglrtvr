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
#ifndef GMICON_H
#define GMICON_H

#include <glam/GmDecorator.h>

/**
 * An icon. If you don't know what an icon is, you're silly.
 */
class GmIcon: public GmDecorator
{
 public:
   /**
    * Default constructor.
    */
   GmIcon();
   
   /**
    * Virtual destructor.
    */
   virtual ~GmIcon();
   
   /**
    * Description.
    */
   virtual void loadFromFile(string fileName);

   /**
    * Clones this icon and returns a pointer to the copy.
    *
    * @return the allocated cloned icon.
    */
   virtual GmIcon* clone();
};

#endif
