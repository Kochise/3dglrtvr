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
#ifndef GMFLOWLAYOUT_H
#define GMFLOWLAYOUT_H

#include <glam/Gm2dLayout.h>

/**
 * This layout positions all of the components that
 * reside within a container according to their minimum size
 * and flowing according to their order of addition to the
 * container.
 */
class GmFlowLayout : public Gm2dLayout
{
 protected:
   /**
    * insets between cells
    **/
   float mCellLeftInset;
   float mCellRightInset;
   float mCellTopInset;
   float mCellBottomInset;

 public:
   /**
    * Standard constructor
    */
   GmFlowLayout();

   /**
    * Virtual destructor
    */
   virtual ~GmFlowLayout();

   /**
    * Calculates the number of components and maximum height of a row
    * in the flow layout.
    *
    * @param components the components that are to be placed using
    *                   the flow layout.
    * @param cBegin the beginning iterator for the row
    * @param cEnd the ending iterator for the row. This value is set
    *             by the method to denote the ending item in the row.
    * @param maxRowHeight the height of the tallest component in the row
    */
   virtual void calculateRowGeometry(vector<GmDrawable*>* components,
                                     vector<GmDrawable*>::iterator cBegin,
                                     vector<GmDrawable*>::iterator& cEnd,
                                     float& maxRowHeight);
   
   /**
    * Applies the layout to the container. All components will be
    * positioned according to the layout design. This function
    * must be implemented by inheriting layout classes.
    *
    * @param components the vector of components in the container
    */
   virtual void apply(vector<GmDrawable*>* components);

   /**
    * Sets the cells' inset amounts.
    *
    * @param left sets all cells' left inset
    * @param right sets all cells' right inset
    * @param top sets all cells' top inset
    * @param bottom sets all cells' bottom inset
    */
   virtual void setCellInsets(float left, float right,
                              float top, float bottom);

   /**
    * Sets the left inset amount.
    *
    * @param left the left inset amount
    */
   virtual void setCellLeftInset(float left);

   /**
    * Sets the right inset amount.
    *
    * @param right the right inset amount
    */
   virtual void setCellRightInset(float right);

   /**
    * Sets the top inset amount.
    *
    * @param top the top inset amount
    */
   virtual void setCellTopInset(float top);

   /**
    * Sets the bottom inset amount.
    *
    * @param bottom the bottom inset amount
    */
   virtual void setCellBottomInset(float bottom);

   /**
    * Returns the left inset amount.
    *
    * @return the left inset amount
    */
   virtual float getCellLeftInset();

   /**
    * Returns the right inset amount.
    *
    * @return the right inset amount
    */
   virtual float getCellRightInset();

   /**
    * Returns the top inset amount.
    *
    * @return the top inset amount
    */
   virtual float getCellTopInset();
   
   /**
    * Returns the bottom inset amount.
    *
    * @return the bottom inset amount
    */
   virtual float getCellBottomInset();

   /**
    * Gets the ideal extents of the drawable in OpenGL units. This is
    * sometimes used by parent containers to determine the optimal
    * layout strategy by taking each components extents into account.
    *
    * @param components the components of the layout which need to be sized
    * @param width the width of the drawable in OpenGL units
    * @param height the height of the drawable in OpenGL units
    */
   virtual void getGLExtents(vector<GmDrawable*>* components,
                             float &width, float &height);
};

#endif
