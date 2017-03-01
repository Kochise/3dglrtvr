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
#ifndef GMFILECACHE_H
#define GMFILECACHE_H

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <GL/glpng.h>
#include <GL/glut.h>
#include <glam/GmSvgIcon.h>
#include <glam/GmPixmapIcon.h>
#include <sugar/ParseSvgToObject.h>
#include <sugar/gl/GlSvgRenderer.h>

/**
 * This class caches important information from files so
 * that it needn't be reloaded from disk whenever necessary.
 * It currently caches textures and svg renderers.
 */
class GmFileCache
{
protected:

   /**
    * Map linking filenames to textures ids
    */
   map<string, GLuint> mTextures;

   /**
    * The mapping of filenames to svg renderers.
    */
   map<string, GlSvgRenderer*> mSvgs;
   
   /**
    * Protected constructor
    */
   GmFileCache();

   /**
    * Virtual destructor
    */
   virtual ~GmFileCache();

public:
   /**
    * Returns pointer to this singleton object
    */
   static GmFileCache& getInstance();

   /**
    * Returns id of specified texture
    *
    * @param filename name of the file where texture information is
    * stored
    */
   GLuint fetchTexture(string filename);

   /**
    * Removes specified texture from cache
    *
    * @param filename name of the texture to be flushed
    */
   bool flushTexture(string filename);

   /**
    * Removes all textures from cache
    */
   void flushAllTextures();

   /**
    * Gets or loads an SvgRenderer opened on an svg icon. If
    * successful, it will return a valid pointer to a GlSvgRenderer
    * that can be used to render the icon. If unsuccessful, will
    * return NULL.
    *
    * @return GlSvgRenderer* a pointer to the stored SVG renderer,
    * used in the same way as a GL texture id.
    * @param filename name of the file where the icon is stored
    */
   GlSvgRenderer* fetchSvg(string filename);

   /**
    * Removes specified icon from cache
    *
    * @return bool returns true if the file was flushed, false if failed
    * @param filename of the file with the icon to be flushed
    */
   bool flushSvg(string filename);
   
   /**
    * Removes all icons from the cache
    */
   void flushAllSvgs();

   /**
    * Creates a new icon and passes a pointer to it.
    *
    * @param filename name of the file that defines the icon.
    */
   GmIcon* fetchIcon(string filename);
};

#endif
