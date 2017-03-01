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
#include <glam/GmDrawable.h>

GmDrawable::GmDrawable()
      : GmObject()
{
   mVisible = true;
   mPosition.setXYZ(0.0, 0.0, 0.0);
   mGLPosition.setXYZ(0.0, 0.0, 0.0);
   mBaseColor.setRGBA(1.0, 1.0, 1.0, 1.0);
   mWidth = 1.0;
   mGLWidth = 1.0;
   mHeight = 1.0;
   mGLHeight = 1.0;
   mDepth = 0.0;
   mGLDepth = 0.0;
   mDisplayListId = GL_ZERO;
   mDisplayListValid = false;

   // default widgetId
   mWidgetId = GLAM_STATIC_WIDGETID_MAX;

   // set relayout invalid
   mLastLayout.depth = -1.0;
}

GmDrawable::~GmDrawable()
{
   if(mDisplayListId != GL_ZERO)
      glDeleteLists(mDisplayListId, 1);
   
   setDisplayListId(GL_ZERO);
}

void GmDrawable::render(GmTypes::RenderOptionType renderOptions)
{
   // do not draw if not visible, return
   if(!mVisible)
      return;

   if(renderOptions & GmTypes::RO_2D)
      render2d(renderOptions);
   else if(renderOptions & GmTypes::RO_2D_SELECT)
      render2dSelect(renderOptions);
}

void GmDrawable::setGLPosition(float x, float y, float z)
{
   mGLPosition.setXYZ(x, y, z);

   // update the relayout parameters
   mLastLayout.minX = x;
   mLastLayout.minY = y;
   mLastLayout.minZ = z;
}

void GmDrawable::setGLPosition(GlmPoint3* point)
{
   float x, y, z;
   point->getXYZ(x, y, z);
   setGLPosition(x, y, z);
}

void GmDrawable::setPosition(float x, float y, float z)
{
   mPosition.setXYZ(x, y, z);
   setDisplayListValid(false);
}

void GmDrawable::setPosition(GlmPoint3* point)
{
   float x, y, z;
   point->getXYZ(x, y, z);   
   setPosition(x, y, z);
}

GlmPoint3* GmDrawable::getPosition()
{
   return &mPosition;
}

GlmPoint3* GmDrawable::getGLPosition()
{
   return &mGLPosition;
}

void GmDrawable::getGLPosition(float &x, float &y, float &z)
{
   mGLPosition.getXYZ(x, y, z);
}

void GmDrawable::setBaseColor(float r, float g, float b, float a)
{
   mBaseColor.setRGBA(r, g, b, a);
}

void GmDrawable::setBaseColor(GlmColor4* color)
{
   float r, g, b, a;

   color->getRGBA(r, g, b, a);
   setBaseColor(r, g, b, a);
}

GlmColor4* GmDrawable::getBaseColor()
{
   return &mBaseColor;
}

void GmDrawable::getBaseColor(float &r, float &g, float &b, float &a)
{
   mBaseColor.getRGBA(r, g, b, a);
}

void GmDrawable::setWidth(float width)
{
   if(width > 0.0f)
      mWidth = width;
   
   setDisplayListValid(false);
}

float GmDrawable::getWidth()
{
   return mWidth;
}

void GmDrawable::setGLWidth(float width)
{
   mGLWidth = width;
   setDisplayListValid(false);
}

float GmDrawable::getGLWidth()
{
   return mGLWidth;
}

void GmDrawable::setHeight(float height)
{
   if(height > 0.0f)
      mHeight = height;
   
   setDisplayListValid(false);
}

float GmDrawable::getHeight()
{
   return mHeight;
}

void GmDrawable::setGLHeight(float height)
{
   mGLHeight = height;
}

float GmDrawable::getGLHeight()
{
   return mGLHeight;
}

void GmDrawable::setDepth(float depth)
{
   if(depth > 0.0f)
      mDepth = depth;
}

float GmDrawable::getDepth()
{
   return mDepth;
}

void GmDrawable::setGLDepth(float depth)
{
   mGLDepth = depth;
}

float GmDrawable::getGLDepth()
{
   return mGLDepth;
}

void GmDrawable::setVisible(bool visible)
{
   mVisible = visible;
}

bool GmDrawable::getVisible()
{
   return mVisible;
}

bool GmDrawable::getFocusable()
{
   return false;
}

void GmDrawable::setWidgetId(WidgetIDType name)
{
   mWidgetId = name;
}

WidgetIDType GmDrawable::getWidgetId()
{
   return mWidgetId;
}

void GmDrawable::setRelativeGeometry(float minX, float minY, float minZ,
                                     float maxX, float maxY, float maxZ)
{
   setPosition(minX, minY, minZ);
   setWidth(abs(maxX - minX));
   setHeight(abs(maxY - minY));
   setDepth(abs(maxZ - minZ));
}

void GmDrawable::setRelativeGeometry(float minX, float minY,
                                     float maxX, float maxY)
{
   setRelativeGeometry(minX, minY, 0.0, maxX, maxY, 0.0);
}

void GmDrawable::render2d(GmTypes::RenderOptionType renderOptions)
{
}

void GmDrawable::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
}

void GmDrawable::layoutInGLArea(float minX, float minY,
                                float width, float height,
                                float layer)
{
   // lay the component out
   layoutInGLVolume(minX, minY, layer, width, height, 0.0);
}

void GmDrawable::layoutInGLVolume(float minX, float minY, float minZ,
                                  float width, float height, float depth)
{
   // store the passed parameters for future calls to relayout()
   mLastLayout.minX = minX;
   mLastLayout.minY = minY;
   mLastLayout.minZ = minZ;
   mLastLayout.width = width;
   mLastLayout.height = height;
   mLastLayout.depth = depth;
   
   // (minX, minY, minZ) Parent container x, y and z base position
   float dX, dY, dZ; // Drawable's x, y and z base position
   
   mPosition.getXYZ(dX, dY, dZ);
   
   mGLPosition.setXYZ(minX + (width * dX),
                      minY + (height * dY),
                      minZ + (depth * dZ));
   mGLWidth = width * mWidth;
   mGLHeight = height * mHeight;
   mGLDepth = depth * mDepth;
   
   setDisplayListValid(false);
}

void GmDrawable::relayout()
{
   // check to see if last layout call was valid
   if(mLastLayout.depth == -1.0)
   {
      // if not, set default values for relayout
      mLastLayout.minX = 0.0;
      mLastLayout.minY = 0.0;
      mLastLayout.minZ = 0.0;
      mLastLayout.depth = 0.0;
      getGLExtents(mLastLayout.width, mLastLayout.height);
   }
   
   layoutInGLVolume(mLastLayout.minX, mLastLayout.minY, mLastLayout.minZ,
                    mLastLayout.width, mLastLayout.height, mLastLayout.depth);
}

void GmDrawable::setDisplayListId(int id)
{
   mDisplayListId = id;
}

int GmDrawable::getDisplayListId()
{
   return mDisplayListId;
}

void GmDrawable::setDisplayListValid(bool valid)
{
   mDisplayListValid = valid;
}

bool GmDrawable::getDisplayListValid()
{
   return mDisplayListValid;
}

void GmDrawable::getGLExtents(float& width, float& height)
{
   width = mGLWidth;
   height = mGLHeight;
}
