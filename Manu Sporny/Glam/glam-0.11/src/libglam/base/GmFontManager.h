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

#ifndef GMFONTMANAGER_H
#define GMFONTMANAGER_H

#define OGLFT_NO_QT

#include <vector>
#include <map>
#include <fstream>
#include <OGLFT/OGLFT.h>
#include <glmath/GlmColor4.h>
#include <glam/GmTypes.h>
#include <glam/GmConfig.h>

/**
 * A struct to house key font data for indexing
 * the mFaces font cache.
 */
struct fontData
{
   /**
    * The full path to the font.
    */
   string filename;

   /**
    * The pointsize of the font.
    */
   float pointsize;

   /**
    * The resolution of the font.
    */
   float resolution;

   /**
    * The OGLFT rendering type of the font.
    */
   GmTypes::FontType type;

   /**
    * The color of the font.
    */
   GlmColor4 color;
};

/**
 * Checks for equality between fontData structs.
 *
 * @param font1 the first data set for comparison
 * @param font2 the second data set for comparison
 * @return 1 if the data sets are equal, of 0 if not
 */
int operator ==(const fontData& font1, const fontData& font2);

/**
 * Checks if font1 is less than font2.
 *
 * @param font1 the first data set for comparison
 * @param font2 the second data set for comparison
 * @return 1 if font1 is less than font2, of 0 if not
 */
int operator<(const fontData& font1, const fontData& font2);

/**
 * Stream operator to send this data to a stream. The fontData is
 * converted to a string that looks something like this:
 * "[filename, pointsize, red, green, blue, alpha]".
 *
 * @param outStream the output stream specified by whatever is to
 *                  the left of this operator.
 * @param font the fontData to send to the output stream
 * @return a reference to the modified output stream
 */
ostream& operator<< (ostream& outStream, const fontData& font);

/**
 * The GmFontManager singleton class handles all font management for
 * the application. Font management includes caching, allocation and
 * deallocation of font faces.
 */
class GmFontManager
{
 private:
   /**
    * The mapping of font file names to cached OGLFT font faces.
    */
   multimap<fontData, OGLFT::Face*> mFaces;

 protected:
   /**
    * Protected constructor for singleton class
    */
   GmFontManager();

   /**
    * Virtual protected destructor for singleton class
    */
   virtual ~GmFontManager();

 public:
   /**
    * Gets the instance of the font manager
    */
   static GmFontManager& GmFontManager::getInstance();

   /**
    * Gets a font face given a specific font filename
    *
    * @param filename the filename of the scalable font to use, for
    *                 example "/usr/share/fonts/truetype/Arial.ttf"
    *
    * @param pointsize the pointsize to use for the font
    *
    * @param fgcolor the foreground color to use for the font
    *
    * @returns the OGLFT font face object associated witht the font
    *          descibed in the argument passed in to the method.
    */
   OGLFT::Face* getFontFace(string filename, float pointsize,
                            GlmColor4& fgcolor, GmTypes::FontType type);

   /**
    * Determines the maximum height of this font.
    *
    * @param fontFace the font face to examine.
    * @param resolution the resolution of the font face.
    * @param pointsize the size to use.
    * @param type the type of font.
    *
    * @return the maximum height of a character with this font.
    */
   float getFontHeight(OGLFT::Face* fontFace,
                       float resolution, float pointsize,
                       GmTypes::FontType type);
   
   /**
    * Determines the width of text of the given font.
    *
    * @param text the text to measure.
    * @param fontFace the font face to use.
    * @param resolution the resolution of the font face.
    * @param pointsize the size to use.
    * @param type the type of font.
    *
    * @return the width of the text.
    */
   float getTextWidth(string text, OGLFT::Face* fontFace,
                      float resolution, float pointsize,
                      GmTypes::FontType type);

   /**
    * Determines the maximum descender of the font this text
    * widget uses font. The descender is the distance from
    * the baseline to the bottom of a character.
    *
    * @param fontFace the font face to use.
    * @param resolution the resolution of the font face.
    * @param pointsize the size to use.
    * @param type the type of font.    
    *
    * @return the maximum descender of the characters in this font.
    */
   float getDescender(OGLFT::Face* fontFace,
                      float resolution, float pointsize,
                      GmTypes::FontType type);
};

#endif
