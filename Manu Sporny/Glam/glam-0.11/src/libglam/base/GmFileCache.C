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
#include <glam/GmFileCache.h>

static GmFileCache* gFileCache = NULL;

GmFileCache::GmFileCache()
{
}

GmFileCache::~GmFileCache()
{
   flushAllTextures();
   flushAllSvgs();
}

GmFileCache& GmFileCache::getInstance()
{
   if(gFileCache == NULL)
      gFileCache = new GmFileCache();

   return *gFileCache;
}

GLuint GmFileCache::fetchTexture(string filename)
{
   GLuint textureId = 0;
   
   map<string, GLuint>::iterator iterator = mTextures.find(filename);
   if(iterator == mTextures.end())
   {
      // Filename not in map. Allocate texture and save resulting id
      // to map.

      pngInfo info;
      textureId = pngBind(filename.c_str(), PNG_BUILDMIPMAPS,
                          PNG_ALPHA, &info, GL_CLAMP,
                          GL_NEAREST, GL_NEAREST);

      // if texture binding succeeds
      if(textureId != 0)
      {
         // unbind the texture so other stuff can draw
         glBindTexture(GL_TEXTURE_2D, 0);

         //cout << "GmFileCache: Loaded texture " << filename <<  endl;
         mTextures[filename] = textureId;
      }
      else
      {
         // Error: Couldn't load texture from file
         cout << "GmFileCache: Couldn't load texture " << filename <<  endl;
      }
   }
   else
   {
      // Return texture id stored in map
      textureId = mTextures[filename];
   }
   
   return textureId;
}

bool GmFileCache::flushTexture(string filename)
{
   map<string, GLuint>::iterator iterator;

   // check the cache for the filename, if not found return false
   iterator = mTextures.find(filename);
   if(iterator == mTextures.end())
      return false;

   // delete the texture from memory
   GLuint id = iterator->second;
   if(id != 0)
      glDeleteTextures(1, &id);

   // remove the texture from the map and return true
   mTextures.erase(iterator);
   return true;
}

void GmFileCache::flushAllTextures()
{
   // remove all the textures from memory
   map<string, GLuint>::iterator i;
   for(i = mTextures.begin(); i != mTextures.end(); i++)
   {
      GLuint id = i->second;
      if(id != 0)
         glDeleteTextures(1, &id);
   }

   // clear map
   mTextures.clear();
}

GlSvgRenderer* GmFileCache::fetchSvg(string filename)
{
   map<string, GlSvgRenderer*>::iterator iterator = mSvgs.find(filename);

   // check to see if the filename is already in the cache
   if(iterator == mSvgs.end())
   {
      // filename is not in the cache, add it
      SugSvgElement* sugarDocument = new SugSvgElement();

      //try to load the svg document file
      if(parseSvgToObject((char*)filename.c_str(), sugarDocument))
      {
         //the svg document was loaded, create a renderer and return it
         GlSvgRenderer* svgRenderer = new GlSvgRenderer();
         svgRenderer->createGlSvgGeometry(sugarDocument);
         
         delete sugarDocument;

         mSvgs[filename] = svgRenderer;
         return svgRenderer;
      }
      else
      {
         //loading the svg did not work, clean up and return NULL
         delete sugarDocument;
         
         cout << "GmFileCache: Could not load SVG " << filename << endl;
         return NULL;
      }
   }
   else
   {
      //we found the filename in the cache
      return mSvgs[filename];
   }
}

bool GmFileCache::flushSvg(string filename)
{
   // check the cache for the filename
   map<string, GlSvgRenderer*>::iterator iterator;
   iterator = mSvgs.find(filename);

   // if not found return false
   if(iterator == mSvgs.end())
      return false;

   // delete the svg from memory
   delete iterator->second;

   // remove the svg from the map
   mSvgs.erase(filename);
   
   return true;
}

void GmFileCache::flushAllSvgs()
{
   // delete all the svgs
   map<string, GlSvgRenderer*>::iterator i;
   for(i = mSvgs.begin(); i != mSvgs.end(); i++)
      delete i->second;

   // clear the map
   mSvgs.clear();
}

GmIcon* GmFileCache::fetchIcon(string filename)
{
   ifstream checkFile;
   checkFile.open(filename.c_str());
   checkFile.close();
   if(checkFile.fail())
   {
      cout << "GmFileCache: Couldn't load icon, could not read file "
           << filename << endl;
      return NULL;
   }
   
   GmIcon* icon = NULL;

   // get the filename's extension
   string extension = filename.substr(filename.length() - 3, 3);

   if(extension == "png")
   {
      // create a new pixmap icon
      icon = new GmPixmapIcon(filename);
   }
   
   else if(extension == "svg")
   {
      // create a new svg icon
      icon = new GmSvgIcon(filename);
   }

   return icon;
}
