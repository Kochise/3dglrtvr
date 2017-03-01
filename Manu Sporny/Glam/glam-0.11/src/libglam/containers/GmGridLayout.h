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
#ifndef GMGRIDLAYOUT_H
#define GMGRIDLAYOUT_H

#include <glam/GmFlowLayout.h>

/**
 * This layout positions all of the components that
 * reside within a container according to a grid.
 */
class GmGridLayout : public GmFlowLayout
{
 protected:
   /**
    * The number of rows in the grid
    */
   int mRows;

   /**
    * The number of columns in the grid
    */
   int mCols;

 public:
   /**
    * Standard constructor
    */
   GmGridLayout();

   /**
    * Constructs a GridLayout with the given rows and columns
    */
   GmGridLayout(int rows, int cols);

   /**
    * Virtual destructor
    */
   virtual ~GmGridLayout();

   /**
    * Applies the layout to the container. All components will be
    * positioned according to the layout design. This function
    * must be implemented by inheriting layout classes.
    *
    * @param components the vector of components in the container
    */
   virtual void apply(vector<GmDrawable*>* components);

   /**
    * Sets the number of rows and columns in the grid.
    *
    * @param rows the number of rows in the grid
    * @param cols the number of columns in the grid
    */
   virtual void setGrid(int rows, int cols);

   /**
    * Sets the number of rows in the grid.
    *
    * @param rows the number of rows in the grid
    */
   virtual void setNumRows(int rows);

   /**
    * Sets the number of columns in the grid.
    *
    * @param cols the number of columns in the grid
    */
   virtual void setNumColumns(int cols);
   
   /**
    * Returns the number of rows in this layout
    *
    * @return the number of rows in this layout
    */
   virtual int getNumRows();
   
   /**
    * Returns the number of columns in this layout
    *
    * @return the number of columns in this layout
    */
   virtual int getNumColumns();

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
