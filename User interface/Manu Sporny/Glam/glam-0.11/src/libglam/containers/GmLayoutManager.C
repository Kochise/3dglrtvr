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
#include <glam/GmLayoutManager.h>

GmLayoutManager::GmLayoutManager()
{
   mCPosition = mCGLPosition = NULL;
   mCWidth = mCHeight = mCDepth = NULL;
   mCGLWidth = mCGLHeight = mCGLDepth = NULL;
   mLeftInset = mRightInset = 0;
   mTopInset = mBottomInset = 0;
}

GmLayoutManager::~GmLayoutManager()
{
}

void GmLayoutManager::setContainerDimensions(GlmPoint3& position,
                                             float& width,
                                             float& height,
                                             float& depth)
{
   mCPosition = &position;
   mCWidth = &width;
   mCHeight = &height;
   mCDepth = &depth;
}

void GmLayoutManager::setGLContainerDimensions(GlmPoint3& glPosition,
                                               float& glWidth,
                                               float& glHeight,
                                               float& glDepth)
{
   mCGLPosition = &glPosition;
   mCGLWidth = &glWidth;
   mCGLHeight = &glHeight;
   mCGLDepth = &glDepth;
}

void GmLayoutManager::setInsets(float left, float right,
                                float top, float bottom)
{
   mLeftInset = left;
   mRightInset = right;
   mTopInset = top;
   mBottomInset = bottom;
}

void GmLayoutManager::getInsets(float& left, float& right,
                                float& top, float& bottom)
{
   left = mLeftInset;
   right = mRightInset;
   top = mTopInset;
   bottom = mBottomInset;
}

float GmLayoutManager::getLeftInset()
{
   return mLeftInset;
}

float GmLayoutManager::getRightInset()
{
   return mRightInset;
}

float GmLayoutManager::getTopInset()
{
   return mTopInset;
}

float GmLayoutManager::getBottomInset()
{
   return mBottomInset;
}

void GmLayoutManager::getGLExtents(vector<GmDrawable*>* components,
                                   float &width, float &height)
{

}
