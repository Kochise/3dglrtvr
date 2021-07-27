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
#ifndef GMSVGICON_H
#define GMSVGICON_H

#include <glam/GmIcon.h>
#include <sugar/gl/GlSvgRenderer.h>
#include <glam/GmFileCache.h>

/**
 * An SVG icon renderer.
 */
class GmSvgIcon : public GmIcon
{
 protected:
   /**
    * The GL SVG renderer used to render the icon.
    */
   GlSvgRenderer* mGLSvgRenderer;

 public:
   /**
    * Standard constructor.
    */
   GmSvgIcon();

   /**
    * Constructor that takes the name of the svg file to be loaded.
    *
    * @param filename The name of the file to load the svg data from
    */
   GmSvgIcon(string filename);

   /**
    * Virtual destructor.
    */
   virtual ~GmSvgIcon();

   /**
    * Loads svg from specified file. 
    *
    * @param filename The file from which to load the svg
    */
   virtual void loadFromFile(string filename);

   /**
    * Clones this icon and returns a pointer to the copy.
    *
    * @return the allocated cloned icon.
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
