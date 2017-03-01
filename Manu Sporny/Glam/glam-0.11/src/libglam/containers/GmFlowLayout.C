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
#include <glam/GmFlowLayout.h>

GmFlowLayout::GmFlowLayout()
{
   mCellLeftInset = mCellRightInset = 0;
   mCellTopInset = mCellBottomInset = 0;   
}

GmFlowLayout::~GmFlowLayout()
{
}

void GmFlowLayout::calculateRowGeometry(vector<GmDrawable*>* components,
                                        vector<GmDrawable*>::iterator cBegin,
                                        vector<GmDrawable*>::iterator& cEnd,
                                        float& maxRowHeight)
{
   float cumulativeGLWidth = 0.0;
   maxRowHeight = 0.0;
   cEnd = components->end();
   
   for(vector<GmDrawable*>::iterator i = cBegin; i != cEnd;)
   {
      float width, height;

      (*i)->getGLExtents(width, height);
      
      if(height > maxRowHeight)
         maxRowHeight = height;

      if((cumulativeGLWidth + width) < *mCGLWidth)
      {
         cumulativeGLWidth += width;
         i++;   
      }
      else
         cEnd = i;
   }
   
   if(*mCGLHeight != 0.0)
      maxRowHeight = maxRowHeight / (*mCGLHeight);
}

void GmFlowLayout::apply(vector<GmDrawable*>* components)
{
   float minX = 0.0;
   float minY = 1.0;
   int numComponents = components->size();
   
   // if no components to layout, then return
   if(numComponents == 0)
      return;

   // Process all the rows in the flow layout
   vector<GmDrawable*>::iterator cBegin;
   vector<GmDrawable*>::iterator cEnd;
   for(cBegin = components->begin();
       cBegin != components->end(); cBegin = components->end())
   {
      float cWidth, cHeight, maxHeight, li, ri, ti, bi;
      // process each row in the flow layout
      calculateRowGeometry(components, cBegin, cEnd, maxHeight);
         
      for(vector<GmDrawable*>::iterator ci = cBegin; ci != cEnd; ci++)
      {
         (*ci)->getGLExtents(cWidth, cHeight);
         
         if(*mCGLWidth != 0.0)
            cWidth = cWidth / (*mCGLWidth);
         
         if(*mCGLHeight != 0.0)
            cHeight = cHeight / (*mCGLHeight);
         
         li = mCellLeftInset * cWidth;
         ri = mCellRightInset * cWidth;
         ti = mCellTopInset * cHeight;
         bi = mCellBottomInset * cHeight;
         
         if(ci == components->begin())
            minY -= maxHeight;
         
//          if(minX + cWidth - ri >= 1.0)
//          {
//             minX = 0.0;
//             minY -= cHeight;
//          }
         
         (*ci)->setRelativeGeometry(minX + li, minY + bi,
                                    minX + cWidth - ri, minY + maxHeight - ti);
         
         minX += cWidth;
      }
   }
   
   // layout all the components at the same depth
   Gm2dLayout::apply(components);
}

void GmFlowLayout::setCellInsets(float left, float right,
                                 float top, float bottom)
{
   mCellLeftInset = left;
   mCellRightInset = right;
   mCellTopInset = top;
   mCellBottomInset = bottom;
}

void GmFlowLayout::setCellLeftInset(float left)
{
   mCellLeftInset = left;
}

void GmFlowLayout::setCellRightInset(float right)
{
   mCellRightInset = right;
}

void GmFlowLayout::setCellTopInset(float top)
{
   mCellTopInset = top;
}

void GmFlowLayout::setCellBottomInset(float bottom)
{
   mCellBottomInset = bottom;
}

float GmFlowLayout::getCellLeftInset()
{
   return mCellLeftInset;
}

float GmFlowLayout::getCellRightInset()
{
   return mCellRightInset;
}

float GmFlowLayout::getCellTopInset()
{
   return mCellTopInset;
}

float GmFlowLayout::getCellBottomInset()
{
   return mCellBottomInset;
}

void GmFlowLayout::getGLExtents(vector<GmDrawable*>* components,
                                float &width, float &height)
{
   width = 0.0;
   height = 0.0;
   //float contentWidth = 0.0, contentHeight = 0.0;
   int numComponents = components->size();

   if(numComponents == 0)
      return;

   //MAY BE BROKEN

   // Process all the rows in the flow layout
   vector<GmDrawable*>::iterator cBegin;
   vector<GmDrawable*>::iterator cEnd;

   cBegin = components->begin();

   float cWidth, cHeight, maxHeight, li, ri, ti, bi;
   calculateRowGeometry(components, cBegin, cEnd, maxHeight);
   
   // process each row in the flow layout
   for(vector<GmDrawable*>::iterator ci = cBegin; ci != cEnd; ci++)
   {
      (*ci)->getGLExtents(cWidth, cHeight);
         
      li = mCellLeftInset * cWidth;
      ri = mCellRightInset * cWidth;
      ti = mCellTopInset * cHeight;
      bi = mCellBottomInset * cHeight;

      width += cWidth + li + ri;
      height += cHeight + ti + bi;
   }
}
