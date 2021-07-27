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
#include <glam/GmOptions.h>
#include <glam/GmTextArea.h>
#include <glam/GmFontManager.h>

GmTextArea::GmTextArea()
      : GmTextWidget()
{
   mEditable = false;
}

GmTextArea::~GmTextArea()
{
}

void GmTextArea::setEditable(bool state)
{
   mEditable = state;
}

void GmTextArea::splitText()
{
   mTextWidthValid = false;
   
   // delete support lists
   for(vector<GLuint>::iterator i = mSupportLists.begin();
       i != mSupportLists.end(); i++)
   {
      glDeleteLists((*i), 1);
   }

   // clear out support lists
   mSupportLists.clear();

   // delete the old display lists
   for(vector<textLine>::iterator i = mLines.begin(); i != mLines.end(); i++)
   {
      if(i->displayList != GL_ZERO)
         glDeleteLists(i->displayList, 1);
   }

   // clear the old vector of lines
   mLines.clear();
   
   if(mText == "" || mFontFace == NULL)
      return;

   GmFontManager* fm = &GmFontManager::getInstance();

   string line = "";
   string substr = "";
   int start = 0;
   int stop = 0;
   
   while(stop < (int)mText.length())
   {
      // find the next space
      stop = mText.find(" ", start);

      // find the next end of line
      int endLine = mText.find('\n', start);
      if(endLine != -1 && endLine < stop)
         stop = endLine;

      if(stop == -1)
         stop = (int)mText.length();
      else
      {
         // add spaces if they are consequtive
         while(stop != endLine && (int)mText.find(" ", stop + 1) == (stop + 1))
            stop++;
      }
      
      substr = mText.substr(start, stop - start);

      float tWidth = 0.0;
      if(getMeasureMode() == GmTypes::MMT_EXACT)
      {
         tWidth = fm->getTextWidth(line + substr, mFontFace,
                                   mTextResolution, mTextSize,
                                   mFontType);
      }
      else if(getMeasureMode() == GmTypes::MMT_APPROXIMATE)
      {
         // approximate width of 1 character to be ratio of its height
         tWidth = (line + substr).length() * 0.58 * mTextSize;
      }    

      // if line stops, text >= width, or end of line character
      if(stop == (int)mText.length() || tWidth >= mGLWidth || stop == endLine)
      {
         // if the line is blank, then the substr is too large for
         // a single line--put it all on one line anyway.
         if(line == "")
         {
            line = substr;

            // increment for next word
            start = stop + 1;
         }

         // if line + substr is less than width or stop is endLine
         else if(tWidth < mGLWidth || stop == endLine)
         {
            // add a space and the substr
            line += " " + substr;

            // increment for the next word
            start = stop + 1;
         }

         // run loop again to try to get word onto the next line
         else
            stop = start;

         // initialzie and add the new text line to the list
         textLine newLine;
         newLine.text = line;
         newLine.displayList = GL_ZERO;
         newLine.HJOffset = 0.0;
         mLines.push_back(newLine);

         // reset the line
         line = "";
      }
      else
      {
         // add the next token onto the line
         if(line == "")
            line = substr;
         else
            line += " " + substr;
         
         start = stop + 1;
      }
   }
}

void GmTextArea::justifyFull(textLine &line)
{
   line.HJOffset = 0.0;

   // keep track of all the words in the line
   vector<string> words;

   string word = "";
   string substr = "";
   int start = 0;
   int stop = 0;
   while(stop < (int)line.text.length())
   {
      // get the next word in the line
      stop = line.text.find(" ", start);

      if(stop == -1)
         stop = (int)line.text.length();
      else
      {
         // add spaces if they are consequtive
         while((int)mText.find(" ", stop + 1) == (stop + 1))
            stop++;
      }

      substr = line.text.substr(start, stop - start);

      // no more words in the line
      if(substr == "")
         break;

      // add the word to the vector
      words.push_back(substr);

      // advance
      start = stop + 1;
   }

   GmFontManager* fm = &GmFontManager::getInstance();

   // ratio for font sizing
   float ratio = mTextSize / mFontHeight;
   
   // for storing word widths
   vector<float> wordWidths;
   
   float lineWidth = 0.0;
   for(vector<string>::iterator i = words.begin(); i != words.end(); i++)
   {
      // necessary to generate character display lists
      mSupportLists.push_back(mFontFace->compile(i->c_str()));

      float wordWidth = 0.0;
      if(getMeasureMode() == GmTypes::MMT_EXACT)
      {
         wordWidth = fm->getTextWidth(*i, mFontFace,
                                      mTextResolution, mTextSize,
                                      mFontType);
      }
      else if(getMeasureMode() == GmTypes::MMT_APPROXIMATE)
      {
         // approximate width of 1 character to be ratio of its height
         wordWidth = i->length() * 0.58 * mTextSize;
      }      

      wordWidths.push_back(wordWidth / ratio);
      lineWidth += wordWidth;
   }

   // get the space width
   float spaceWidth = 0.0;
   if(lineWidth != 0.0 && words.size() != 1)
      spaceWidth = (mGLWidth - lineWidth) / (float)(words.size() - 1);

   spaceWidth /= ratio;
   
   // create the display list for this line
   line.displayList = glGenLists(1);
   glNewList(line.displayList, GL_COMPILE);

   glPushMatrix();

   vector<string>::iterator wi;
   vector<float>::iterator wwi;
   for(wi = words.begin(), wwi = wordWidths.begin();
       wi != words.end() && wwi != wordWidths.end(); wi++, wwi++)
   {
      // draw the words
      mFontFace->draw(0.0, 0.0, wi->c_str());
      glTranslatef(spaceWidth + (*wwi), 0.0, 0.0);
   }

   glPopMatrix();
   
   glEndList();
}

void GmTextArea::justifyLine(textLine &line)
{
   float lineWidth = 0.0;
   if(getMeasureMode() == GmTypes::MMT_EXACT)
   {
      GmFontManager* fm = &GmFontManager::getInstance();
      lineWidth = fm->getTextWidth(line.text, mFontFace,
                                   mTextResolution, mTextSize, mFontType);
   }
   else if(getMeasureMode() == GmTypes::MMT_APPROXIMATE)
   {
      // approximate width of 1 character to be ratio of its height
      lineWidth = line.text.length() * 0.58 * mTextSize;
   }

   // determine justification
   switch(mTextHJustification)
   {
      case GmTypes::JTH_LEFT:
         line.HJOffset = 0.0;
         break;
      case GmTypes::JTH_ORIGIN:
         line.HJOffset = 0.0;
         break;
      case GmTypes::JTH_CENTER:
         line.HJOffset = (mGLWidth / 2.0) - (lineWidth / 2.0);
         break;
      case GmTypes::JTH_RIGHT:
         line.HJOffset = mGLWidth - lineWidth;
         break;
      case GmTypes::JTH_FULL:
         justifyFull(line);
         break;
      default:
         line.HJOffset = 0.0;
         break;
   }
}

void GmTextArea::justifyTextArea()
{
   // set up the vertical justification
   int lineCount = mLines.size() + 1;

   float xo, yo;
   getOffset(xo, yo);

   switch(mTextVJustification)
   {
      case GmTypes::JTV_BOTTOM:
         yo = 0.0;
         break;
      case GmTypes::JTV_BASELINE:
         yo = 0.0;
         break;
      case GmTypes::JTV_MIDDLE:
         yo = (mGLHeight / 2.0) - (mTextSize / 2.0) * lineCount;
         break;
      case GmTypes::JTV_TOP:
         yo = mGLHeight - mTextSize * lineCount;
         break;
      default:
         yo = 0.0;
         break;
   }

   setOffset(xo, yo);

   // justify horizontally
   for(vector<textLine>::iterator i = mLines.begin(); i != mLines.end(); i++)
      justifyLine(*i);
}

void GmTextArea::generateLineDisplayLists()
{
   if(mFontFace == NULL)
      return;
   
   // create new display lists
   for(vector<textLine>::iterator i = mLines.begin(); i != mLines.end(); i++)
      i->displayList = mFontFace->compile(i->text.c_str());
}

void GmTextArea::layoutInGLVolume(float minX, float minY, float minZ,
                                  float width, float height, float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   // split the text up into its individual lines
   splitText();
}

void GmTextArea::getGLExtents(float& width, float& height)
{
   width = 0.0;
   height = mTextSize * mLines.size();

   if(mText == "")
      return;
   
   // load the font face if necessary
   if(mFontFace == NULL)
   {
      // if it couldn't be loaded, return
      if(!loadFontFace())
         return;
   }

   // if the stored text width is valid, use it to save time
   if(mTextWidthValid)
   {
      width = mTextWidth;
      return;
   }

   // split the text up into its individual lines
   if(mLines.size() == 0)
      splitText();

   // find the widest line
   float maxWidth = 0.0;
   for(vector<textLine>::iterator i = mLines.begin(); i != mLines.end(); i++)
   {
      if(getMeasureMode() == GmTypes::MMT_EXACT)
      {
         // allows for spaces to be measured
         string text = i->text;
         int space = text.find_first_of(' ');
         while(space != -1)
         {
            text.replace(space, 1, "0");
            space = text.find_first_of(' ');
         }
         
         OGLFT::BBox bbox = mFontFace->measure(text.c_str());
         width = (bbox.x_max_ - bbox.x_min_) * mTextSize / mFontHeight;
      }
      else if(getMeasureMode() == GmTypes::MMT_APPROXIMATE)
      {
         // approximate width of 1 character to be ratio of its height
         width = i->text.length() * 0.58 * mTextSize;
      }
      
      if(width > maxWidth)
         maxWidth = width;
   }

   mTextWidth = width = maxWidth;
   mTextWidthValid = true;
   
   height = mTextSize * mLines.size();
}

void GmTextArea::render2d(GmTypes::RenderOptionType renderOptions)
{
   if(mText == "")
      return;

   // save color and texture settings
   glPushAttrib(GL_CURRENT_BIT | GL_TEXTURE_BIT);
   
   if(!getDisplayListValid())
   {
      // load the font face
      if(mFontFace == NULL)
      {
         if(!loadFontFace())
            return;
      }

      if(getDisplayListId() != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);

      // split the text up into its individual lines
      splitText();

      // justify the text
      justifyTextArea();

      // generate the lines' display lists
      // display lists are created at justification time if
      // horizontal justification is set to full
      if(mTextHJustification != GmTypes::JTH_FULL)
         generateLineDisplayLists();

      // create the display list that includes the scaling
      setDisplayListId(glGenLists(1));      
      glNewList(getDisplayListId(), GL_COMPILE);

      // scale the text to the proper size, 1:1 font height to GL unit height
      glPushMatrix();
      float ratio = mTextSize / mFontHeight;
      glScalef(ratio, ratio, 1.0);

      // translate by the baseline descender
      glTranslatef(0.0, mDescender, 0.0);

      // call all of the line display lists
      vector<textLine>::reverse_iterator i;      
      for(i = mLines.rbegin(); i != mLines.rend(); i++)
      {
         glTranslatef(0.0, mFontHeight, 0.0);

         glPushMatrix();
         glTranslatef(i->HJOffset / ratio, 0.0, 0.0);
         glCallList(i->displayList);
         glPopMatrix();
      }

      // pop off translation matrices
      glPopMatrix();

      // end the list
      glEndList();

      setDisplayListValid(true);
   }

   // position text
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   float xo, yo;
   getOffset(xo, yo);

   glPushMatrix();
   glTranslatef(x + xo, y + yo, z);
   glCallList(getDisplayListId());
   glPopMatrix();

   // restore color and texture settings
   glPopAttrib();
}
