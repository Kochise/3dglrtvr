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
#ifndef GMPIXMAPICON_H
#define GMPIXMAPICON_H

#include <glam/GmIcon.h>

/**
 * An icon that uses a pixmap for its display.
 */
class GmPixmapIcon : public GmIcon
{
 protected:
   /**
    * GL texture id of the texture.
    */
   GLuint mPixmapId;
   
 public:
   /**
    * Constructor that takes the filename of the icon to be loaded as
    * an argument.
    *
    * @param filename Name of file to load the image from
    */
   GmPixmapIcon(string filename);

   /**
    * Standard constructor.
    */
   GmPixmapIcon();
   
   /**
    * Standard destructor.
    */
   virtual ~GmPixmapIcon();

   /**
    * Loads texture from specified file. Only PNGs are supported at
    * this time.
    *
    * @param filename The file from which to load the texture
    */
   virtual void loadFromFile(string filename);

   /**
    * Clones this icon and returns a pointer to the copy.
    *
    * @return the allocated cloned icon
    */
   virtual GmIcon* clone();
   
protected:

   /**
    * Renders this icon to a 2D image buffer.
    *
    * @param renderOptions The render options to use when rendering
    *              the icon
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this icon to a selection buffer.
    *
    * @param renderOptions The render options to use when rendering
    *              the icon
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
