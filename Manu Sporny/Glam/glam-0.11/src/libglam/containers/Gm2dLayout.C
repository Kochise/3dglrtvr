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
#include <glam/Gm2dLayout.h>

Gm2dLayout::Gm2dLayout()
{
   mCDD = GLAM_LAYER;
}

Gm2dLayout::~Gm2dLayout()
{
}

void Gm2dLayout::apply(vector<GmDrawable*>* components)
{
   // for storing this containers position coordinates
   float x, y, z;
   mCGLPosition->getXYZ(x, y, z);

   float minX = x + mLeftInset * (*mCGLWidth);
   float minY = y + mBottomInset * (*mCGLHeight);
   
   float width = *mCGLWidth - (mLeftInset + mRightInset) * (*mCGLWidth);
   float height = *mCGLHeight - (mBottomInset + mTopInset) * (*mCGLHeight);
   
   float layer = z + mCDD;

   vector<GmDrawable*>::iterator i;
   for (i = components->begin(); i != components->end(); i++)
      (*i)->layoutInGLArea(minX, minY, width, height, layer);
}

void Gm2dLayout::setComponentDepth(float cdd)
{
   mCDD = cdd;
}

float Gm2dLayout::getComponentDepth()
{
   return mCDD;
}

void Gm2dLayout::getGLExtents(vector<GmDrawable*>* components,
                              float &width, float &height)
{
   width = 0.0;
   height = 0.0;
   float contentWidth = 0.0, contentHeight = 0.0;
   
   //add up the extents of all the container contents
   //note: this is a REALLY POOR size approximation
   vector<GmDrawable*>::iterator i;
   for(i = components->begin(); i != components->end(); i++)
   {
      (*i)->getGLExtents(contentWidth, contentHeight);
      width += contentWidth;
      height += contentHeight;
   }
}
