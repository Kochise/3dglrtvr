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
#include <glam/GmListLayout.h>

GmListLayout::GmListLayout()
      : GmFlowLayout()
{
}

GmListLayout::~GmListLayout()
{
}

void GmListLayout::apply(vector<GmDrawable*>* components)
{
   // if no components to layout, then return
   if(components->size() == 0)
      return;

   // for storing this containers position coordinates
   float x, y, z;
   mCGLPosition->getXYZ(x, y, z);

   // calculate the X position of the components
   float minX = x + mLeftInset * (*mCGLWidth);

   // calculate the top of the layout
   float minY = y + *mCGLHeight - mTopInset * (*mCGLHeight);
   
   // calculate width of components
   float width = *mCGLWidth - (mLeftInset + mRightInset) * (*mCGLWidth);
   
   // add component depth differential to layer
   float layer = z + mCDD;

   float ti = mCellTopInset * (*mCGLHeight);
   float bi = mCellBottomInset * (*mCGLHeight);

   vector<GmDrawable*>::iterator i;
   for(i = components->begin(); i != components->end(); i++)
   {
      float cWidth, cHeight;
      (*i)->getGLExtents(cWidth, cHeight);
      
      // calculate Y position of component, move past top cell inset
      minY -= (cHeight + ti);

      // component occupies entire space
      (*i)->setRelativeGeometry(0.0, 0.0, 1.0, 1.0);

      // layout in designated area
      (*i)->layoutInGLArea(minX, minY, width, cHeight, layer + GLAM_LAYER);

      // move down past bottom inset
      minY -= bi;
   }
}

void GmListLayout::getGLExtents(vector<GmDrawable*>* components,
                                float &width, float &height)
{
   //note: probably incorrect as of first writing
   
   if(components->size() == 0)
   {
      width = 0.0;
      height= 0.0;
      return;
   }

   float tci = mCellTopInset * (*mCGLHeight);
   float bci = mCellBottomInset * (*mCGLHeight);

   width = 0.0;
   height = *mCGLHeight;
   vector<GmDrawable*>::iterator i;
   for(i = components->begin(); i != components->end(); i++)
   {
      float cWidth, cHeight;
      (*i)->getGLExtents(cWidth, cHeight);
      height += cHeight + tci + bci;
      if(cWidth > width)
         width = cWidth;
   }

   height -= *mCGLHeight;
}
