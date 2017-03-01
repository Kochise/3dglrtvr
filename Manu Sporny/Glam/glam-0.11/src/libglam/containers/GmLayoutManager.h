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
#ifndef GMLAYOUTMANAGER_H
#define GMLAYOUTMANAGER_H

#include <glmath/GlmPoint3.h>
#include <glam/GmDrawable.h>

/**
 * The Layout Manager class
 */
class GmLayoutManager
{
 protected:
   /**
    * position of associated container
    */
   GlmPoint3* mCPosition;

   /**
    * OpenGL unit position of container
    */
   GlmPoint3* mCGLPosition;

   /**
    * Dimensions of the container. Both in application units
    * and OpenGL units.
    */
   float *mCWidth, *mCHeight, *mCDepth;
   float *mCGLWidth, *mCGLHeight, *mCGLDepth;

   /**
    * insets from container edge
    */
   float mLeftInset, mRightInset;
   float mTopInset, mBottomInset;
   
 public:
   /**
    * Standard constructor
    */
   GmLayoutManager();

   /**
    * Virtual destructor
    */
   virtual ~GmLayoutManager();

   /**
    * Applies the layout to the container. All components will be
    * positioned according to the layout design. This function
    * must be implemented by inheriting layout classes.
    *
    * @param components the vector of components in the container
    */
   virtual void apply(vector<GmDrawable*>* components) = 0;

   /**
    * Associates the container's position and dimensions with this
    * layout manager. Association is used so that layout managers
    * do not have to be updated if container positions or dimensions
    * change.
    *
    * @param position the starting position of the constraint volume
    * @param width the width of the constraint volume
    * @param height the height of the constraint volume
    * @param depth the depth of the constraint volume
    */
   void setContainerDimensions(GlmPoint3& position,
                               float& width, float& height, float& depth);

   /**
    * Associates the container's OpenGL position and dimensions with this
    * layout manager. Association is used so that layout managers
    * do not have to be updated if container positions or dimensions
    * change.
    *
    * @param glPosition the starting position of the constraint volume
    * @param glWidth the OpenGL unit width of the constraint volume
    * @param glHeight the OpenGL unit height of the constraint volume
    * @param glDepth the OpenGL unit depth of the constraint volume
    */
   void setGLContainerDimensions(GlmPoint3& glPosition,
                                 float &glWidth,
                                 float& glHeight,
                                 float& glDepth);

   /**
    * Sets the inset amounts.
    *
    * @param left sets the left inset
    * @param right sets the right inset
    * @param top sets the top inset
    * @param bottom sets the bottom inset
    */
   void setInsets(float left, float right, float top, float bottom);

   /**
    * Gets the inset amounts.
    *
    * @param left gets the left inset
    * @param right gets the right inset
    * @param top gets the top inset
    * @param bottom gets the bottom inset
    */
   void getInsets(float& left, float& right, float& top, float& bottom);

   /**
    * Returns the left inset amount.
    *
    * @return the left inset amount
    */
   float getLeftInset();

   /**
    * Returns the right inset amount.
    *
    * @return the right inset amount
    */
   float getRightInset();

   /**
    * Returns the top inset amount.
    *
    * @return the top inset amount
    */
   float getTopInset();
   
   /**
    * Returns the bottom inset amount.
    *
    * @return the bottom inset amount
    */
   float getBottomInset();

   /**
    * Gets the ideal extents of the drawable in OpenGL units. This is
    * sometimes used by parent containers to determine the optimal
    * layout strategy by taking each components extents into account.
    *
    * @param width the width of the drawable in OpenGL units
    * @param height the height of the drawable in OpenGL units
    */
   virtual void getGLExtents(vector<GmDrawable*>* components,
                             float &width, float &height);
};

#endif
