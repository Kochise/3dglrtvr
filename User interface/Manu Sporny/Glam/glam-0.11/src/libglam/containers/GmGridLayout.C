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
#include <glam/GmGridLayout.h>

GmGridLayout::GmGridLayout()
      : GmFlowLayout()
{
   mRows = mCols = 1;
}

GmGridLayout::GmGridLayout(int rows, int cols)
      : GmFlowLayout()
{
   mRows = (rows < 1) ? 1 : rows;
   mCols = (cols < 1) ? 1 : cols;
}

GmGridLayout::~GmGridLayout()
{
}

void GmGridLayout::apply(vector<GmDrawable*>* components)
{
   float width = 1.0 / mCols;
   float height = 1.0 / mRows;

   float li = mCellLeftInset * width;
   float ri = mCellRightInset * width;
   float ti = mCellTopInset * height;
   float bi = mCellBottomInset * height;

   int component = 0;
   int numComponents = components->size();
   
   float minX, minY, maxX, maxY;
   int r, c;
   for(r = 0; r < mRows && component < numComponents; r++)
   {
      for(c = 0; c < mCols && component < numComponents; c++, component++)
      {
         minX = width * c;
         minY = height * (mRows - 1 - r);
         maxX = width * (c + 1);
         maxY = height * (mRows - r);
         
         (*components)[component]->setVisible(true);
         (*components)[component]->setRelativeGeometry(minX + li, minY + bi,
                                                       maxX - ri, maxY - ti);
      }
   }

   for(; component < numComponents; component++)
      (*components)[component]->setVisible(false);
   
   // layout all the components at the same depth
   Gm2dLayout::apply(components);
}

void GmGridLayout::setGrid(int rows, int cols)
{
   if(rows >= 1)
      mRows = rows;
   if(cols >= 1)
      mCols = cols;
}

void GmGridLayout::setNumRows(int rows)
{
   if(rows >= 1)
      mRows = rows;
}

void GmGridLayout::setNumColumns(int cols)
{
   if(cols >= 1)
      mCols = cols;
}

int GmGridLayout::getNumRows()
{
   return mRows;
}

int GmGridLayout::getNumColumns()
{
   return mCols;
}

void GmGridLayout::getGLExtents(vector<GmDrawable*>* components,
                              float &width, float &height)
{
   width = 0.0;
   height = 0.0;
   float maxWidth, contentWidth = 0.0, contentHeight = 0.0;
   int numComponents = components->size();

   float li = mCellLeftInset/mCols;
   float ri = mCellRightInset/mCols;
   float ti = mCellTopInset/mRows;
   float bi = mCellBottomInset/mRows;

   int r, c, component = 0;
   for(r = 0; r < mRows && component < numComponents; r++)
   {
      width = 0.0;
      for(c = 0; c < mCols && component < numComponents; c++, component++)
      {
         (*components)[component]->getGLExtents(contentWidth, contentHeight);
         
         width += contentWidth + li + ri;
         height += contentHeight + ti + bi;
      }

      if(width > maxWidth)
         maxWidth = width;
   }
   
   width = maxWidth;
}
