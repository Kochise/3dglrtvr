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
#ifndef GMTABLE_H
#define GMTABLE_H

#include <glam/GmContainer.h>
#include <glam/GmTableCell.h>

/**
 * The table meta-data structure
 */
struct GmTableMetaData
{
   /**
    * the row/column background color
    */
   GlmColor4 mBgColor;

   /**
    * the width/height of the row/column
    */
   float mSize;

   /**
    * the resizeability of the row/column
    */
   bool mResizeable;
};

/**
 * The table container which uses table layouts and arranges items in
 * a resizeable grid.
 * The application of metadata has the following order:
 * Cell, Column, Row, Table
 */
class GmTable: public GmContainer, public GmGeometryListener
{
 protected:
   /**
    * Our vector of vectors used for the table cell containment
    * we are going to store them using
    * outer vector: column
    * inner vector: row
    */
   vector <vector <GmTableCell*> > mTableData;

   /**
    * keeps track of the number of rows
    */
   int mNumRows;

   /**
    * keeps track of the number of cols
    */
   int mNumCols;
   
   /**
    * the horizontal vector of table row metadata that applies to the
    * entire row
    */
   vector <GmTableMetaData> mRowMeta;
   
   /**
    * the vertical vector of table column metadata that applies to the
    * entire column
    */
   vector <GmTableMetaData> mColMeta;

   /**
    * the default table background color
    */
   GlmColor4 mTableColor;

   /**
    * the table border color
    */
   GlmColor4 mBorderColor;

   /**
    * the table border size
    */
   float mBorderSize;
   
 public:
   /**
    * Standard constructor
    */
   GmTable();

   /**
    * Standard destructor
    */
   virtual ~GmTable();

   /**
    * attempts to add a column at the indicated column number, if the
    * table is not that large, it inserts it at the end
    *
    * @param colNum the number of the column to add
    */
   virtual void addColumn(int colNum);

   /**
    * attempts to add a row at the indicated row number, if the
    * table is not that large, it inserts it at the end
    *
    * @param rowNum the number of the column to add
    */
   virtual void addRow(int rowNum);

   /**
    * attemps to add a cell item in the indicated column/row, if the
    * col/row does not exist, it fills in emtpy cells until it reaches
    * the correct col/row
    *
    * @param item the item to add to the cell
    * @param row the row number to add the cell in
    * @param col the col number to add the cell in
    */
   virtual void addCell(GmDrawable* item, int row, int col);

   /**
    * attemps to remove a column, if the number is invalid it does
    * nothing
    *
    * @param colNum the number of the column to remove
    */
   virtual void removeColumn(int colNum);

   /**
    * same as above, except with rows
    *
    * @param rowNum the number of the row to remove
    */
   virtual void removeRow(int rowNum);

   /**
    * attemps to remove a cell item at the indicated row/col
    * this does not delete the rows/cols the item was in
    *
    * @param row the row number to reset
    * @param col the column number to reset
    */
   virtual void resetCell(int row, int col);

   /**
    * sets a cell color
    *
    * @param newColor the new color of the cell
    * @param row the row number of the cell
    * @param col the column number of the cell
    */
   virtual void setCellColor(GlmColor4 newColor, int row, int col);

   /**
    * sets the cell dimensions
    *
    * @param newWidth the new width of the cell
    * @param newHeight the new height of the cell
    * @param row the row number to change
    * @param col the column number to change
    */
   virtual void setCellSize(float newWidth, float newHeight, int row, int col);

   /**
    * adds cell content
    *
    * @param cellData the new data to add to the cell
    * @param row the row number of the cell to add the data to
    * @param col the column number of the cell to add the data to
    */
   virtual void addCellContent(GmDrawable* cellData, int row, int col);

   /**
    * sets the cell visibility // necessary?
    *
    * @param visible the boolean visibility setting of the cell
    * @param row the row number of the cell
    * @param col the column number of the cell
    */
   virtual void setCellVisibility(bool visible, int row, int col);
   
   /**
    * gets the cell color, if it exists
    *
    * @return GlmColor4 the color of the cell
    * @param row the row number of the cell
    * @param col the col number of the cell
    */
   virtual GlmColor4 getCellColor(int row, int col);

   /**
    * get the cell dimensions
    *
    * @param width the width of the cell
    * @param height the height of the cell
    * @param row the row number of the cell
    * @param col the col number of the cell
    */
   virtual void getCellSize(float &width, float &height, int row, int col);

   //row & col functions
   /**
    * sets the row color
    *
    * @param newColor the new color of the row
    * @param row the row number for the new color
    */
   virtual void setRowColor(GlmColor4 newColor, int rowNum);

   /**
    * sets the row size (the row's height)
    *
    * @param newSize the new height of the entire row
    * @param rowNum the row number of the row
    */
   virtual void setRowHeight(float newSize, int rowNum);

   /**
    * sets the visibility of the entire row
    *
    * @param visible the visibility state of the row
    * @param rowNum the row number to set
    */
   virtual void setRowVisibility(bool visible, int rowNum);

   /**
    * sets the resizability of the row
    *
    * @param resizeable flag to allow resizing of the row
    * @param rowNum number of the row to set
    */
   virtual void setRowResizability(bool resizable, int rowNum);

   /**
    * gets the row color
    *
    * @return GlmColor4 the row color
    * @param rowNum the number of the row
    */
   virtual GlmColor4 getRowColor(int rowNum);

   /**
    * gets the row size (row height)
    *
    * @return float the row height
    * @param rowNum the number of the row
    */
   virtual float getRowHeight(int rowNum);

   /**
    * sets the column color
    *
    * @param newColor the new color of the column
    * @param colNum the column number
    */
   virtual void setColColor(GlmColor4 newColor, int colNum);

   /**
    * sets the column size
    *
    * @param newSize the new width of the column
    * @param colNum the number of the column
    */
   virtual void setColWidth(float newSize, int colNum);

   /**
    * sets the column visibility
    *
    * @param visible the visibility state of the column
    * @param colNum the number of the column to set
    */
   virtual void setColVisibility(bool visible, int colNum);

   /**
    * sets the resizability of the col
    *
    * @param resizeable flag to allow resizing of the col
    * @param colNum number of the col to set
    */
   virtual void setColResizability(bool resizable, int colNum);

   /**
    * gets the column color
    *
    * @return GlmColor4 the color of the column
    * @param colNum the number of the column
    */
   virtual GlmColor4 getColColor(int colNum);

   /**
    * gets the column size
    *
    * @return float the column width
    * @param colNum the column number
    */
   virtual float getColWidth(int colNum);

   //table functions
   /**
    * sets the table color
    *
    * @param newColor the new color to set as the table color
    */
   virtual void setTableColor(GlmColor4 newColor);

   /**
    * sets the table border color
    *
    * @param newColor the new color to set as the table border color
    */
   virtual void setTableBorderColor(GlmColor4 newColor);

   /**
    * sets the table border size
    *
    * @param borderSize the new size of the table border
    */
   virtual void setTableBorderSize(float borderSize);

   /**
    * gets the table color
    *
    * @return GlmColor4 the table color
    */
   virtual GlmColor4 getTableColor();

   /**
    * gets the table border color
    *
    * @return GlmColor4 the table border color
    */
   virtual GlmColor4 getTableBorderColor();

   /**
    * gets the table border size
    *
    * @return float the table border size
    */
   virtual float getTableBorderSize();
   
   /**
    * Sets the relative geometry to use for this drawable object. This
    * method is provided merely for convenience.
    *
    * @param minX the relative min X value for this drawable
    * @param minY the relative min Y value for this drawable
    * @param minZ the relative min Z value for this drawable
    * @param maxX the relative max X of this drawable
    * @param maxY the relative max Y of this drawable
    * @param maxZ the relative max Z of this drawable
    */
   virtual void setRelativeGeometry(float minX, float minY, float minZ,
                                    float maxX, float maxY, float maxZ);

   /**
    * Sets the relative geometry to use for this drawable object. This
    * method is provided merely for convenience. This method should be
    * used for 2d objects that will be in a container that uses a
    * layout manager.
    *
    * @param minX the relative min X value for this drawable
    * @param minY the relative min Y value for this drawable
    * @param maxX the relative max X of this drawable
    * @param maxY the relative max Y of this drawable
    */
   virtual void setRelativeGeometry(float minX, float minY,
                                    float maxX, float maxY);

   /**
    * Repositions and resizes this component based on parameters
    * passed into the method.
    *
    * @param minX the starting X position of the constraint volume
    * @param minY the starting Y position of the constraint volume
    * @param minZ the starting Z position of the constraint volume
    * @param width the width of the constraint volume
    * @param height the height of the constraint volume
    * @param depth the depth of the constraint volume
    */
   virtual void layoutInGLVolume(float minX, float minY, float minZ,
                                 float width, float height, float depth);
   
   /**
    * Gets the ideal extents of the drawable in OpenGL units. This is
    * sometimes used by parent containers to determine the optimal
    * layout strategy by taking each components extents into account.
    *
    * @param width the width of the drawable in OpenGL units
    * @param height the height of the drawable in OpenGL units
    */
   virtual void getGLExtents(float &width, float &height);

   /**
    * Sets the GL position of the drawable.
    *
    * @param x the X position of the drawable object
    * @param y the Y position of the drawable object
    * @param z the Z position of the drawable object
    */
   virtual void setGLPosition(float x, float y, float z);

   /**
    * Sets the GL position of the drawable.
    *
    * @param point the point to use as the rendering location
    *              of the drawable.
    */
   virtual void setGLPosition(GlmPoint3* point);

   /**
    * Translate the GL positions of all of the components by
    * x, y, z.
    *
    * @param x the amount to translate in the x direction
    * @param y the amount to translate in the y direction
    * @param z the amount to translate in the z direction
    */
   virtual void translateComponentsGLPositions(float x, float y, float z);   

   /**
    * Called whenever an associated widget has its geometry changed.
    *
    * @param action the geometry action involved in geometry change.
    */
   virtual void geometryResized(GmGeometryAction* action);

   /**
    * Applies this container's layout to the components that it
    * encapsulates.
    */
   virtual void applyLayout();
   
 protected:

   /**
    * (re)assigns widget ids to all cells of the table, when cells are
    * added/deleted
    */
   virtual void assignWidgetIds();
   
   /**
    * Renders this container and all of its sub-components to a 2D
    * image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the container.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this container and all of its sub-components to a
    * selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the container.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
