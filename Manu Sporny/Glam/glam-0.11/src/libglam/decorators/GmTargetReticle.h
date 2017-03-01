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
#ifndef GMTARGETRETICLE_H
#define GMTARGETRETICLE_H

#include <glam/GmIcon.h>
#include <glam/GmDecorator.h>

/**
 * This class has not yet been implemented.
 */
class GmTargetReticle : public GmDecorator
{
 public:
   /**
    * The reticle icon.
    */
   GmIcon* mReticleIcon;

   /**
    * Standard constructor.
    */
   GmTargetReticle();

   /**
    * Standard destructor.
    */
   virtual ~GmTargetReticle();

   /**
    * Sets the reticle icon.
    *
    * @param the icon to use.
    */
   void setReticleIcon(GmIcon* icon);
};

#endif