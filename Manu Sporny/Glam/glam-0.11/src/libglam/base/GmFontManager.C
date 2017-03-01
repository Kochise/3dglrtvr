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
#include <glam/GmFontManager.h>
#include <glam/GmOptions.h>

#include <pthread.h>

// The singleton instance
GmFontManager* gGmFontManagerInstance = NULL;

static pthread_mutex_t gFontFaceMutex;

GmFontManager::GmFontManager()
{
}

GmFontManager::~GmFontManager()
{
   map<fontData, OGLFT::Face*>::iterator i;
   for(i = mFaces.begin(); i != mFaces.end(); i++)
      delete i->second;

   mFaces.clear();
}

GmFontManager& GmFontManager::getInstance()
{
   if(gGmFontManagerInstance == NULL)
      gGmFontManagerInstance = new GmFontManager();
   
   return *gGmFontManagerInstance;
}

OGLFT::Face* GmFontManager::getFontFace(string filename,
                                        float pointsize,
                                        GlmColor4& fgcolor,
                                        GmTypes::FontType type)
{
   if(pointsize == 0.0)
   {
      cout << "Error: Font point size cannot be zero!" << endl;
      return NULL;
   }
   
   const char *fontFilename = filename.c_str();

   // check to see if the font file exists
   ifstream checkFile;
   checkFile.open(fontFilename);
   if(!checkFile)
   {
      checkFile.close();
      cout << "Could not load font file: " << fontFilename << endl;
      cout << "Loading default font file..." << endl;
      string name = GLAM_DATA_DIR PATH_SEPARATOR "stereo.ttf";
      fontFilename = name.c_str();

      // check the default file
      checkFile.clear();
      checkFile.open(fontFilename);
      if(!checkFile)
      {
         checkFile.close();
         cout << "Default font file could not load." << endl
              << "Fatal error. Exiting program..." << endl;
         exit(EXIT_FAILURE);
      }
      else
         checkFile.close();
   }
   else
      checkFile.close();

   // lock thread for font loading/manipulation
   pthread_mutex_lock(&gFontFaceMutex);

   map<fontData, OGLFT::Face*>::iterator search;
   OGLFT::Face* result = NULL;

   GmOptions* gmOptions = GmOptions::getInstance();

   // set up the font data for searching/adding
   fontData newData;
   newData.filename = fontFilename;
   newData.pointsize = pointsize;
   newData.resolution = gmOptions->getScalarOption(GMO_TEXT_RESOLUTION);
   newData.type = type;
   newData.color = fgcolor;
   
   // search for the font data (and corresponding font face)
   search = mFaces.find(newData);
   if(search == mFaces.end())
   {
      // load this type of font...
      switch(type)
      {
         case GmTypes::FT_P_FILLED:
            result = new OGLFT::Filled(fontFilename, pointsize);
            break;

         case GmTypes::FT_P_SOLID:
            result = new OGLFT::Solid(fontFilename, pointsize);
            break;
    
         case GmTypes::FT_P_OUTLINE:
            result = new OGLFT::Outline(fontFilename, pointsize);
            break;
     
         case GmTypes::FT_R_GRAYSCALE:
            result = new OGLFT::Grayscale(fontFilename, pointsize);
            break;

         case GmTypes::FT_R_MONOCHROME:
            result = new OGLFT::Monochrome(fontFilename, pointsize);
            break;

         case GmTypes::FT_R_TRANSLUCENT:
            result = new OGLFT::Translucent(fontFilename, pointsize);
            break;

         case GmTypes::FT_T_GRAYSCALE:
            result = new OGLFT::GrayscaleTexture(fontFilename, pointsize);
            break;

         case GmTypes::FT_T_MONOCHROME:
            result = new OGLFT::MonochromeTexture(fontFilename, pointsize);
            break;

         case GmTypes::FT_T_TRANSLUCENT:
            result = new OGLFT::TranslucentTexture(fontFilename, pointsize);
            break;
      }

      if(result != NULL)
      {
         // ...and add it to the cache if the font load succeeded
         if(result->isValid())
         {
            // set text color
            float r, g, b, a;
            fgcolor.getRGBA(r, g, b, a);
            result->setForegroundColor(r, g, b, a);

            // set text resolution
            result->setResolution((unsigned int)newData.resolution);

            // set justification
            result->setVerticalJustification(OGLFT::Face::BASELINE);

            // add the font to the cache map
            mFaces.insert(make_pair(newData, result));
         }
         else
            delete result;
      }
   }
   else
      result = search->second;

   // unlock thread, font loading and manipulation complete
   pthread_mutex_unlock(&gFontFaceMutex);

   //return the face
   return result;
}

float GmFontManager::getFontHeight(OGLFT::Face* fontFace,
                                   float resolution, float pointsize,
                                   GmTypes::FontType type)
{
   if(fontFace == NULL)
      return 0.0;

   float height;
   switch(type)
   {
      case GmTypes::FT_P_SOLID:
         // height from OGLFT =
         // freetype height * pointsize * resolution / (72 * units/EM)
         // where units per EM = 2048.0
         // so GL height =
         // height * 72 * 2048 / (26DOT6 * 100dpi * 64.0)
         // where 72 * 2048 / (26DOT6 * 100dpi * 64.0) = 576/665
         height = fontFace->height() * 576.0f / 665.0f;
         break;
      case GmTypes::FT_T_TRANSLUCENT:
         // height from OGLFT = freetype height / 64.0
         // so GL height =
         // height / 26DOT6, resolution / 100dpi, * ptsize = GLunits
         // = (height * resolution * ptsize) / 2660 = GLunits
         height = (fontFace->height() * resolution * pointsize) / 2660.0f;
         break;
      default:
         height = 0.0;
         break;
   }

   return height;
}

float GmFontManager::getTextWidth(string text, OGLFT::Face* fontFace,
                                  float resolution, float pointsize,
                                  GmTypes::FontType type)
{
   if(fontFace != NULL)
   {
      // allows for spaces to be measured
      int i = text.find_first_of(' ');
      while(i != -1)
      {
         text.replace(i, 1, "|");
         i = text.find_first_of(' ');
      }

      // load thread for measuring
      pthread_mutex_lock(&gFontFaceMutex);
      
      OGLFT::BBox bbox = fontFace->measure(text.c_str());

      // unlock thread, measuring complete
      pthread_mutex_unlock(&gFontFaceMutex);
      
      float fontHeight = getFontHeight(fontFace, resolution, pointsize, type);
      return (bbox.x_max_ - bbox.x_min_) * pointsize / fontHeight;
   }

   return 0.0;
}

float GmFontManager::getDescender(OGLFT::Face* fontFace,
                                  float resolution, float pointsize,
                                  GmTypes::FontType type)
{
   // check fontFace validity
   if(fontFace == NULL)
      return 0.0;

   float height;
   switch(type)
   {
      case GmTypes::FT_P_SOLID:
         // height from OGLFT =
         // freetype height * pointsize * resolution / (72 * units/EM)
         // where units per EM = 2048.0
         // so freetype height / 64.0 =
         // height * 72 * 2048 / (pointsize * resolution * 64.0)
         // where 72 * 2048 / 64.0 = 2304
         height = fontFace->height() * 2304.0f / (pointsize * resolution);
         break;
      case GmTypes::FT_T_TRANSLUCENT:
         // height from OGLFT = freetype height / 64.0
         height = fontFace->height();
         break;
      default:
         height = 0.0;
         break;
   }
   
   return -fontFace->descender() / height;
}

int operator==(const fontData& font1, const fontData& font2)
{
   float r1, g1, b1, a1;
   float r2, g2, b2, a2;
   GlmColor4 color1 = font1.color;
   GlmColor4 color2 = font2.color;
   color1.getRGBA(r1, g1, b1, a1);
   color2.getRGBA(r2, g2, b2, a2);
   
   int result = (font1.filename == font2.filename) &&
      (font1.pointsize == font2.pointsize) &&
      (font1.resolution == font2.resolution) &&
      (font1.type == font2.type) &&
      (r1 == r2) && (g1 == g2) && (b1 == b2) && (a1 == a2);
   
   return result;
}

int operator<(const fontData& font1, const fontData& font2)
{
   float r1, g1, b1, a1;
   float r2, g2, b2, a2;
   GlmColor4 color1 = font1.color;
   GlmColor4 color2 = font2.color;
   color1.getRGBA(r1, g1, b1, a1);
   color2.getRGBA(r2, g2, b2, a2);

   int result = (font1.filename < font2.filename) ||
      (font1.pointsize < font2.pointsize) ||
      (font1.resolution < font2.resolution) ||
      (font1.type < font2.type) ||
      (r1 < r2) || (g1 < g2) || (b1 < b2) || (a1 < a2);
   
   return result;
}

ostream& operator<< (ostream& outStream, const fontData& font)
{
   outStream << "[" << font.filename
             << ", " << font.pointsize
             << ", " << font.resolution
             << "]";
   
   return outStream;
}
