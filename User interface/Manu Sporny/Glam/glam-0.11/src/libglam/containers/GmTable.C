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
#include <glam/GmTable.h>

//Table
GmTable::GmTable() 
      : GmContainer()
{
   mNumRows = 0;
   mNumCols = 0;
   
   mTableColor.setRGBA(0.6, 0.6, 0.6, 1.0);
   mBorderColor.setRGBA(0.5, 0.5, 0.5, 1.0);
   
   mBorderSize = 1.0;
}

GmTable::~GmTable()
{
   //remove all table elements/ table metadata
   vector<vector<GmTableCell*> >::iterator col;
   vector<GmTableCell*>::iterator row;
   for(col = mTableData.begin(); col != mTableData.end(); col++)
   {
      for(row = col->begin(); row != col->end(); row++)
      {
         (*row)->reset();
      }
   }
}

// table functions
void GmTable::addColumn(int colNum)
{
   if(colNum < 1)
      return;

   float r, g, b, a;
   mTableColor.getRGBA(r, g, b, a);
   GmTableMetaData metaAnon;
   metaAnon.mBgColor.setRGBA(r, g, b, a);
   metaAnon.mSize = 0.0;
   metaAnon.mResizeable = true;
   
   vector<GmTableCell*> vecAnon;
   GmTableCell* cellAnon;
   
   if(mNumCols == 0)
   {
      //we have no rows or columns, insert a row and a column
      ++mNumCols;
      ++mNumRows;

      //add column metadata
      mColMeta.push_back(metaAnon);
      //add row metadata
      mRowMeta.push_back(metaAnon);

      //add column 0 with no rows to the table
      mTableData.push_back(vecAnon);

      //add a cell to the first column so we have 1 row
      cellAnon = new GmTableCell();
      cellAnon->setColor(mTableColor);
      cellAnon->addGeometryListener(this);
      cellAnon->setFocusParent(this);
      
      mTableData[0].push_back(cellAnon);
   }
   else if(colNum > mNumCols)
   {
      //we should have at least 1 column, and thus at least 1 row
      ++mNumCols;
      
      //add a col to the end of metadata
      mColMeta.push_back(metaAnon);
      
      //insert at the end of the tabledata
      mTableData.push_back(vecAnon);
      
      //add as many row elements to the column vector as there are rows
      int rowNum;
      for(rowNum = 0; rowNum < mNumRows; ++rowNum)
      {
         cellAnon = new GmTableCell();
         cellAnon->setColor(mRowMeta[rowNum].mBgColor);
         cellAnon->addGeometryListener(this);
         cellAnon->setFocusParent(this);
         
         mTableData[mNumCols - 1].push_back(cellAnon);
      }
   }
   else
   {
      //colNum is within mNumCols, so add a column to the total
      ++mNumCols;
      
      vector <GmTableMetaData>::iterator metaCol = mColMeta.begin();
      int i;
      for(i = 1; i < colNum; ++i)
         ++metaCol;
      mColMeta.insert(metaCol, metaAnon);
      
      //otherwise add a column to the table data in its spot
      vector <vector <GmTableCell*> >::iterator col = mTableData.begin();
      for(i = 1; i < colNum; ++i)
         ++col;
      mTableData.insert(col, vecAnon);
      
      //add as many row elements to the vector as there are rows
      int rowNum;
      for(rowNum = 0; rowNum < mNumRows; ++rowNum)
      {
         cellAnon = new GmTableCell();
         cellAnon->setColor(mRowMeta[rowNum].mBgColor);
         cellAnon->addGeometryListener(this);
         cellAnon->setFocusParent(this);
         
         mTableData[colNum - 1].push_back(cellAnon);
      }
   }

   //recalculate metadata?
   //applyLayout();
   assignWidgetIds();
   
   //send geometry resize
   GmGeometryAction* ga = new GmGeometryAction(this,
                                               mColMeta[colNum].mSize, 0.0,
                                               mGLWidth, mGLHeight);
   fireGeometryResize(ga);
   delete ga;
}

void GmTable::addRow(int rowNum)
{
   if(rowNum < 1)
      return;

   float r, g, b, a;
   mTableColor.getRGBA(r, g, b, a);
   GmTableMetaData metaAnon;
   metaAnon.mBgColor.setRGBA(r, g, b, a);
   metaAnon.mSize = 0.0;
   metaAnon.mResizeable = true;
   
   vector <GmTableCell*> vecAnon;
   GmTableCell* cellAnon;

   if(mNumRows == 0)
   {
      //we have no rows or columns, insert a row and a column
      ++mNumCols;
      ++mNumRows;

      //add column metadata
      mColMeta.push_back(metaAnon);
      //add row metadata
      mRowMeta.push_back(metaAnon);

      //add column 0 with no rows to the table
      mTableData.push_back(vecAnon);

      //add a cell to the first column so we have 1 row
      cellAnon = new GmTableCell();
      cellAnon->setColor(mTableColor);
      cellAnon->addGeometryListener(this);
      cellAnon->setFocusParent(this);
      
      mTableData[0].push_back(cellAnon);
   }
   else if(rowNum > mNumRows)
   {
      ++mNumRows;
      //add a row to the end of the metadata
      mRowMeta.push_back(metaAnon);
      
      //add a row to the table data
      int colNum;
      for(colNum = 0; colNum < mNumCols; ++colNum)
      {
         cellAnon = new GmTableCell();
         cellAnon->setColor(mColMeta[colNum].mBgColor);
         cellAnon->addGeometryListener(this);
         cellAnon->setFocusParent(this);
         
         mTableData[colNum].push_back(cellAnon);
      }
   }
   else
   {
      ++mNumRows;
      
      //add a row to the metadata
      vector <GmTableMetaData>::iterator metaRow = mRowMeta.begin();
      int i;
      for(i = 0; i < rowNum; ++i)
         ++metaRow;
      mRowMeta.insert(metaRow, metaAnon);
      
      //add a row to the table data
      //vector <vector <GmTableCell> >::iterator col = mTableData.begin();
      for(int colNum = 0; colNum < mNumCols; ++colNum)
      {
         vector <GmTableCell*>::iterator row = mTableData[colNum].begin();
         for(i = 0; i < rowNum; ++i)
            ++row;

         cellAnon = new GmTableCell();
         cellAnon->setColor(mColMeta[colNum].mBgColor);
         cellAnon->addGeometryListener(this);
         cellAnon->setFocusParent(this);
         
         mTableData[colNum].insert(row, cellAnon);
      }
   }

   //recalculate metadata?
   //applyLayout();
   assignWidgetIds();
   //send geometry resize
   GmGeometryAction* ga = new GmGeometryAction(this,
                                               0.0, mRowMeta[rowNum].mSize,
                                               mGLWidth, mGLHeight);
   fireGeometryResize(ga);
   delete ga;
}

void GmTable::addCell(GmDrawable* item, int row, int col)
{
   if(row < 1 || col < 1)
      return;

   //GmTableMetaData metaAnon;
   //metaAnon.mBgColor.setRGBA(1.0, 1.0, 1.0, 1.0);
   //metaAnon.mSize = 0.0;
   
   //vector <GmTableCell*> vecAnon;
   //GmTableCell* cellAnon;
   
//    if(mNumRows == 0 || mNumCols == 0)
//    {
//       //we have no rows or columns
//       //add a single row and column and then add the item
//       ++mNumCols;
//       ++mNumRows;

//       //add column metadata
//       mColMeta.push_back(metaAnon);
//       //add row metadata
//       mRowMeta.push_back(metaAnon);

//       //add column 0 with no rows to the table
//       mTableData.push_back(vecAnon);

//       //add a cell to the first column so we have 1 row
//       mTableData[0].push_back(cellAnon);

//       //add the data to the cell
//       mTableData[0][0].addComponent(item);
//    }
   if(row <= mNumRows && col <= mNumCols)
   {
      //insert the data into the cell item
      mTableData[col - 1][row - 1]->addComponent(item);
   }
   else
   {
      int rowNum, colNum;
      //insert rows/columns to get to that point
      for(colNum = mNumCols+1; colNum <= col; ++colNum)
      {
         //addColumn(colNum);

         if(colNum < 1)
            return;

         float r, g, b, a;
         mTableColor.getRGBA(r, g, b, a);
         GmTableMetaData metaAnon;
         metaAnon.mBgColor.setRGBA(r, g, b, a);
         metaAnon.mSize = 0.0;
         metaAnon.mResizeable = true;
   
         vector <GmTableCell*> vecAnon;
         GmTableCell* cellAnon;
   
         if(mNumCols == 0)
         {
            //we have no rows or columns, insert a row and a column
            ++mNumCols;
            ++mNumRows;

            //add column metadata
            mColMeta.push_back(metaAnon);
            //add row metadata
            mRowMeta.push_back(metaAnon);

            //add column 0 with no rows to the table
            mTableData.push_back(vecAnon);

            //add a cell to the first column so we have 1 row
            cellAnon = new GmTableCell();
            cellAnon->setColor(mTableColor);
            cellAnon->addGeometryListener(this);
            cellAnon->setFocusParent(this);
      
            mTableData[0].push_back(cellAnon);
         }
         else if(colNum > mNumCols)
         {
            //we should have at least 1 column, and thus at least 1 row
            ++mNumCols;
      
            //add a col to the end of metadata
            mColMeta.push_back(metaAnon);
      
            //insert at the end of the tabledata
            mTableData.push_back(vecAnon);
      
            //add as many row elements to the column vector as there are rows
            int rowNum;
            for(rowNum = 0; rowNum < mNumRows; ++rowNum)
            {
               cellAnon = new GmTableCell();
               cellAnon->setColor(mRowMeta[rowNum].mBgColor);
               cellAnon->addGeometryListener(this);
               cellAnon->setFocusParent(this);
         
               mTableData[mNumCols - 1].push_back(cellAnon);
            }
         }
         else
         {
            //colNum is within mNumCols, so add a column to the total
            ++mNumCols;
      
            vector <GmTableMetaData>::iterator metaCol = mColMeta.begin();
            int i;
            for(i = 1; i < colNum; ++i)
               ++metaCol;
            mColMeta.insert(metaCol, metaAnon);
      
            //otherwise add a column to the table data in its spot
            vector <vector <GmTableCell*> >::iterator col = mTableData.begin();
            for(i = 1; i < colNum; ++i)
               ++col;
            mTableData.insert(col, vecAnon);
      
            //add as many row elements to the vector as there are rows
            int rowNum;
            for(rowNum = 0; rowNum < mNumRows; ++rowNum)
            {
               cellAnon = new GmTableCell();
               cellAnon->setColor(mRowMeta[rowNum].mBgColor);
               cellAnon->addGeometryListener(this);
               cellAnon->setFocusParent(this);
         
               mTableData[colNum - 1].push_back(cellAnon);
            }
         }
   
         
      }
      for(rowNum = mNumRows+1; rowNum <= row; ++rowNum)
      {
         //addRow(rowNum);

         if(rowNum < 1)
            return;

         float r, g, b, a;
         mTableColor.getRGBA(r, g, b, a);
         GmTableMetaData metaAnon;
         metaAnon.mBgColor.setRGBA(r, g, b, a);
         metaAnon.mSize = 0.0;
         metaAnon.mResizeable = true;
   
         vector <GmTableCell*> vecAnon;
         GmTableCell* cellAnon;

         if(mNumRows == 0)
         {
            //we have no rows or columns, insert a row and a column
            ++mNumCols;
            ++mNumRows;

            //add column metadata
            mColMeta.push_back(metaAnon);
            //add row metadata
            mRowMeta.push_back(metaAnon);

            //add column 0 with no rows to the table
            mTableData.push_back(vecAnon);

            //add a cell to the first column so we have 1 row
            cellAnon = new GmTableCell();
            cellAnon->setColor(mTableColor);
            cellAnon->addGeometryListener(this);
            cellAnon->setFocusParent(this);
      
            mTableData[0].push_back(cellAnon);
         }
         else if(rowNum > mNumRows)
         {
            ++mNumRows;
            //add a row to the end of the metadata
            mRowMeta.push_back(metaAnon);
      
            //add a row to the table data
            int colNum;
            for(colNum = 0; colNum < mNumCols; ++colNum)
            {
               cellAnon = new GmTableCell();
               cellAnon->setColor(mColMeta[colNum].mBgColor);
               cellAnon->addGeometryListener(this);
               cellAnon->setFocusParent(this);
         
               mTableData[colNum].push_back(cellAnon);
            }
         }
         else
         {
            ++mNumRows;
      
            //add a row to the metadata
            vector <GmTableMetaData>::iterator metaRow = mRowMeta.begin();
            int i;
            for(i = 0; i < rowNum; ++i)
               ++metaRow;
            mRowMeta.insert(metaRow, metaAnon);
      
            //add a row to the table data
            //vector <vector <GmTableCell> >::iterator col = mTableData.begin();
            for(int colNum = 0; colNum < mNumCols; ++colNum)
            {
               vector <GmTableCell*>::iterator row = mTableData[colNum].begin();
               for(i = 0; i < rowNum; ++i)
                  ++row;

               cellAnon = new GmTableCell();
               cellAnon->setColor(mColMeta[colNum].mBgColor);
               cellAnon->addGeometryListener(this);
               cellAnon->setFocusParent(this);
         
               mTableData[colNum].insert(row, cellAnon);
            }
         }
      }
      //insert the data into the cell item
      addCellContent(item, row, col);
   }
   
   //recalculate metadata?
   //applyLayout();
   assignWidgetIds();
   //send geometry resize
   GmGeometryAction* ga = new GmGeometryAction(this,
                                               0.0, 0.0,
                                               mGLWidth, mGLHeight);
   fireGeometryResize(ga);
   delete ga;
}

void GmTable::removeColumn(int colNum)
{
   if(colNum < 1)
      return;

   float colSize = 0.0;
   
   //remove the column, deleting all data (if it exists)
   if(colNum <= mNumCols)
   {
      vector <GmTableMetaData>::iterator metaCol = mColMeta.begin();
      int i;
      for(i = 1; i < colNum; ++i)
         ++metaCol;
      colSize = mColMeta[i].mSize;
      mColMeta.erase(metaCol);
      
      vector <vector <GmTableCell*> >::iterator col = mTableData.begin();
      for(i = 1; i < colNum; ++i)
         ++col;
      mTableData.erase(col);
      
      mNumCols--;

      if(mNumCols == 0)
      {
         //we need to remove all the leftover rows
         mNumRows = 0;
      }
   }

   //recalculate metadata?
   //applyLayout();
   assignWidgetIds();
   //send geometry resize
   GmGeometryAction* ga = new GmGeometryAction(this,
                                               -colSize, 0.0,
                                               mGLWidth, mGLHeight);
   fireGeometryResize(ga);
   delete ga;
}

void GmTable::removeRow(int rowNum)
{
   if(rowNum < 1)
      return;

   float rowSize = 0.0;
   
   //remove the row, deleting all data (if it exists)
   if(rowNum <= mNumRows)
   {
      vector <GmTableMetaData>::iterator metaRow = mRowMeta.begin();
      int i;
      for(i = 1; i < rowNum; ++i)
         ++metaRow;
      rowSize = mRowMeta[i].mSize;
      mRowMeta.erase(metaRow);
      
      for(int colNum = 1; colNum <= mNumCols; ++colNum)
      {
         vector <GmTableCell*>::iterator row = mTableData[colNum - 1].begin();
         for(i = 1; i < rowNum; ++i)
            ++row;
         
         mTableData[colNum - 1].erase(row);
      }
      
      mNumRows--;

      if(mNumRows == 0)
         mNumCols = 0;
   }
   //applyLayout();
   assignWidgetIds();
   //send geometry resize
   GmGeometryAction* ga = new GmGeometryAction(this,
                                               0.0, -rowSize,
                                               mGLWidth, mGLHeight);
   fireGeometryResize(ga);
   delete ga;
}

void GmTable::resetCell(int row, int col)
{
   if(row < 1 || col < 1)
      return;
   
   //if the cell item exists, delete the cell data, and return the
   //cell to default cell properties
   if(row <= mNumRows && col <= mNumCols)
   {
      mTableData[col - 1][row - 1]->reset();
   }
}


//table cell functions
void GmTable::setCellColor(GlmColor4 newColor, int row, int col)
{
   if(row < 1 || col < 1)
      return;
   
   //find the cell, if it exists, set the new color
   if(row <= mNumRows && col <= mNumCols)
   {
      mTableData[col - 1][row - 1]->setColor(newColor);
   }
}

void GmTable::setCellSize(float newWidth, float newHeight, int row, int col)
{
   if(row < 1 || col < 1)
      return;
   
   //set the cell size if the cell exists
   if(row <= mNumRows && col <= mNumCols)
   {
      mColMeta[col - 1].mSize = newWidth;
      mRowMeta[col - 1].mSize = newHeight;
   }
}

void GmTable::addCellContent(GmDrawable* cellData, int row, int col)
{
   if(row < 1 || col < 1)
      return;
   
   //set new cell content if the cell exists
   if(row <= mNumRows && col <= mNumCols)
   {
      mTableData[col - 1][row - 1]->addComponent(cellData);
   }
}

void GmTable::setCellVisibility(bool visible, int row, int col)
{
   if(row < 1 || col < 1)
      return;
   
   //sets cell visibility information if it exists
   if(row <= mNumRows && col <= mNumCols)
   {
      mTableData[col - 1][row - 1]->setVisible(visible);
   }
}

GlmColor4 GmTable::getCellColor(int row, int col)
{
   if(row < 1 || col < 1)
      return GlmColor4();
   
   //gets the color of the cell if it exists
   if(row <= mNumRows && col <= mNumCols)
   {
      return mTableData[col - 1][row - 1]->getColor();
   }
   return GlmColor4();
}

void GmTable::getCellSize(float &width, float &height, int row, int col)
{
   if(row < 1 || col < 1)
   {
      width = 0;
      height = 0;
      return;
   }
   
   //gets the dimensions of the cell if it exists
   if(row <= mNumRows && col <= mNumCols)
   {
      width = mColMeta[col - 1].mSize;
      height = mRowMeta[row - 1].mSize;
   }
}

//row & col functions
void GmTable::setRowColor(GlmColor4 newColor, int rowNum)
{
   if(rowNum < 1)
      return;
   
   //set the row color if the row exists
   if(rowNum <= mNumRows)
   {
      float r, g, b, a;
      newColor.getRGBA(r, g, b, a);
      mRowMeta[rowNum - 1].mBgColor.setRGBA(r, g, b, a);

      int colNum;
      for(colNum = 1; colNum <= mNumCols; ++colNum)
      {
         mTableData[colNum - 1][rowNum - 1]->setColor(newColor);
      }
   }
}

void GmTable::setRowHeight(float newSize, int rowNum)
{
   if(rowNum < 1)
      return;
   
   //sets the row size if the row exists
   if(rowNum <= mNumRows)
   {
      mRowMeta[rowNum - 1].mSize = newSize;
      //set the height of all items in the row
   }
}

void GmTable::setRowVisibility(bool visible, int rowNum)
{
   if(rowNum < 1)
      return;

   //set the row visibility if the row exists
   if(rowNum <= mNumRows)
   {
      int colNum;
      for(colNum = 1; colNum <= mNumCols; ++colNum)
         mTableData[colNum - 1][rowNum - 1]->setVisible(visible);
   }
}

void GmTable::setRowResizability(bool resizeable, int rowNum)
{
   if(rowNum < 1)
      return;

   //change the row resizability if it exists
   if(rowNum <= mNumRows)
   {
      mRowMeta[rowNum - 1].mResizeable = resizeable;
   }
}

GlmColor4 GmTable::getRowColor(int rowNum)
{
   if(rowNum < 1)
      return GlmColor4();
   
   //get the row color if the row exists
   if(rowNum <= mNumRows)
   {
      return mRowMeta[rowNum - 1].mBgColor;
   }
   return GlmColor4();
}

float GmTable::getRowHeight(int rowNum)
{
   if(rowNum < 1)
      return 0;
   
   //return the size of the row, if it exists
   if(rowNum <= mNumRows)
   {
      return mRowMeta[rowNum - 1].mSize;
   }
   return 0;
}


void GmTable::setColColor(GlmColor4 newColor, int colNum)
{
   if(colNum < 1)
      return;
   
   //find the col, if it exists
   //set the col color
   if(colNum <= mNumCols)
   {
      float r, g, b, a;
      newColor.getRGBA(r, g, b, a);
      mColMeta[colNum - 1].mBgColor.setRGBA(r, g, b, a);

      int rowNum;
      for(rowNum = 1; rowNum <= mNumRows; ++rowNum)
      {
         mTableData[colNum - 1][rowNum - 1]->setColor(newColor);
      }
   }
}

void GmTable::setColWidth(float newSize, int colNum)
{
   if(colNum < 1)
      return;
   
   //find the col, if it exists
   //set the col size
   if(colNum <= mNumCols)
   {
      mColMeta[colNum - 1].mSize = newSize;
      //set for all col components?
   }
}

void GmTable::setColVisibility(bool visible, int colNum)
{
   if(colNum < 1)
      return;

   //set the column visibility if it exists
   if(colNum <= mNumCols)
   {
      int rowNum;
      for(rowNum = 1; rowNum <= mNumRows; ++rowNum)
         mTableData[colNum - 1][rowNum - 1]->setVisible(visible);
   }
}

void GmTable::setColResizability(bool resizeable, int colNum)
{
   if(colNum < 1)
      return;

   //set col resizability if exists
   if(colNum <= mNumCols)
   {
      mColMeta[colNum - 1].mResizeable = resizeable;
   }
}

GlmColor4 GmTable::getColColor(int colNum)
{
   if(colNum < 1)
      return GlmColor4();
   
   //find the col, if it exists
   //get the color
   if(colNum <= mNumCols)
   {
      return mColMeta[colNum - 1].mBgColor;
   }
   return GlmColor4();
}

float GmTable::getColWidth(int colNum)
{
   if(colNum < 1)
      return 0;
   
   //find the col, if it exists
   //get the size
   if(colNum <= mNumCols)
   {
      return mColMeta[colNum - 1].mSize;
   }
   return 0;
}

void GmTable::setTableColor(GlmColor4 newColor)
{
   //set the table's background color
   float r, g, b, a;
   newColor.getRGBA(r, g, b, a);
   mTableColor.setRGBA(r, g, b, a);
   
   setDisplayListValid(false);
}

void GmTable::setTableBorderColor(GlmColor4 newColor)
{
   //set the table's border color
   float r, g, b, a;
   newColor.getRGBA(r, g, b, a);
   mBorderColor.setRGBA(r, g, b, a);
   
   setDisplayListValid(false);
}

void GmTable::setTableBorderSize(float borderSize)
{
   //set the table's border size   
   mBorderSize = borderSize;

   //recalculate relative geometry?
}

GlmColor4 GmTable::getTableColor()
{
   //gets the table background color
   return mTableColor;
}

GlmColor4 GmTable::getTableBorderColor()
{
   //gets the table border color
   return mBorderColor;
}

float GmTable::getTableBorderSize()
{
   //gets the table border size
   return mBorderSize;
}

void GmTable::setRelativeGeometry(float minX, float minY, float minZ,
                                  float maxX, float maxY, float maxZ)
{
//    int colNum = 1, rowNum = 1;
//    float pX, pY, sX, sY;
//    float width, height;
//    //component's relative geometries
//    vector<vector<GmTableCell> >::iterator col;
//    vector<GmTableCell>::iterator row;
//    for(col = mTableData.begin(); col != mTableData.end(); ++col)
//    {
//       for(row = col->begin(); row != col->end(); ++row)
//       {
//          width = mColMeta[colNum - 1].mSize;
//          height = mRowMeta[rowNum - 1].mSize;
         
//          if(mNumCols != 0)
//          {
//             pX = (colNum - 1) * (float)(1.0 / mNumCols);
//             if(width == 0)
//                sX = (1.0 / mNumCols);
//             else
//                sX = width;
//          }
         
//          if(mNumRows != 0)
//          {
//             pY = 1.0 - (rowNum * (float)(1.0 / mNumRows));
//             if(height == 0)
//                sY = (1.0 / mNumRows);
//             else
//                sY = height;
//          }
         
//          row->setRelativeGeometry(pX + mBorderSize,
//                                   pY + mBorderSize,
//                                   pX + sX - mBorderSize,
//                                   pY + sY - mBorderSize);
//          ++rowNum;
//       }
//       rowNum = 1;
//       ++colNum;
//    }
}

void GmTable::setRelativeGeometry(float minX, float minY,
                                  float maxX, float maxY)
{
   setRelativeGeometry(minX, minY, 0.0, maxX, maxY, 0.0);
}

void GmTable::layoutInGLVolume(float minX, float minY, float minZ,
                               float width, float height, float depth)
{
   // layout this component in the given volume
   //GmDrawable::layoutInGLVolume(minX, minY, minZ,
   //                             width, height, depth);
   setGLPosition(minX, minY, minZ);
   setGLWidth(width);
   setGLHeight(height);
   setGLDepth(depth);

   // apply layout to encapsulated components
   applyLayout();
}

void GmTable::geometryResized(GmGeometryAction* action)
{
   int colNum, rowNum;
   float widthChange, heightChange;

   colNum = ((action->getSrcWidgetId() - 1) / mNumRows) + 1;
   rowNum = mNumRows - ((colNum * mNumRows) - action->getSrcWidgetId());

   // resize the row and column based on sizes in the action
   action->getChange(widthChange, heightChange);

   float oldColSize = mColMeta[colNum - 1].mSize;
   float oldRowSize = mRowMeta[rowNum - 1].mSize;

   mColMeta[colNum - 1].mSize += widthChange;
   mRowMeta[rowNum - 1].mSize -= heightChange;

   applyLayout();

   widthChange = mColMeta[colNum - 1].mSize - oldColSize;
   heightChange = mRowMeta[rowNum - 1].mSize - oldRowSize;

   GmGeometryAction* ga = new GmGeometryAction(this,
                                               widthChange, heightChange,
                                               mGLWidth, mGLHeight);
   fireGeometryResize(ga);
   delete ga;
}

//protected
void GmTable::assignWidgetIds()
{
   int colNum, rowNum;
   for(colNum = 1; colNum <= mNumCols; ++colNum)
   {
      for(rowNum = 1; rowNum <= mNumRows; ++rowNum)
      {
         mTableData[colNum - 1][rowNum - 1]->setWidgetId(
            ((colNum - 1) * mNumRows) + rowNum);
      }
   }
}

void GmTable::applyLayout()
{
   float pX, pY, pZ, sX, sY, sZ, tX;
   float width, height;

   getGLExtents(width, height);

//    width = width + ((mBorderSize * mNumCols) + mBorderSize);
//    height = height + ((mBorderSize * mNumRows) + mBorderSize);

   //    setGLPosition(minX, minY, minZ);
   setGLWidth(width);
   setGLHeight(height);

   //lay out components
   int colNum = 1, rowNum = 1;
   
   mGLPosition.getXYZ(pX, pY, pZ);

   sZ = pZ;
   
   vector<vector<GmTableCell*> >::iterator col;
   vector<GmTableCell*>::reverse_iterator row;
   for(col = mTableData.begin(); col != mTableData.end(); ++col)
   {
      for(row = col->rbegin(); row != col->rend(); ++row)
      {
         sX = mColMeta[colNum - 1].mSize;
         sY = mRowMeta[mNumRows - rowNum].mSize;

         (*row)->layoutInGLArea(pX + mBorderSize,
                                pY + mBorderSize,
                                sX, sY, sZ + GLAM_LAYER);

         pY += sY + mBorderSize;
         
         ++rowNum;
      }
      rowNum = 1;
      pX += mColMeta[colNum - 1].mSize + mBorderSize;
      mGLPosition.getXYZ(tX, pY, pZ);
      
      ++colNum;
   }
}

void GmTable::getGLExtents(float &width, float &height)
{
   //go through all table cells, and add up sizes
   width = height = 0.0;
   float currWidth, currHeight;

   //calculate the maximum heights and widths for each row/col
   int colNum = 1, rowNum = 1;
   for(colNum = 1; colNum <= mNumCols; ++colNum)
   {
      for(rowNum = 1; rowNum <= mNumRows; ++rowNum)
      {
         mTableData[colNum - 1][rowNum - 1]->getGLExtents(currWidth,
                                                          currHeight);

         //check if this cell is the widest in the col so far
         if(mColMeta[colNum - 1].mSize < currWidth)
            mColMeta[colNum - 1].mSize = currWidth;

         //if this cell is the highest so far, set the metadata to its
         //value
         if(mRowMeta[rowNum - 1].mSize < currHeight)
            mRowMeta[rowNum - 1].mSize = currHeight;
      }
      
      //keep the running width total here
      //calculate the height total later in the other for loop
      width += mColMeta[colNum - 1].mSize;
   }

//    int colNum = 1, rowNum = 1;
//    for(colNum = 1; colNum <= mNumCols; ++colNum)
//    {
//       maxWidth = mColMeta[colNum - 1].mSize;
      
//       for(rowNum = 1; rowNum <= mNumRows; ++rowNum)
//       {
//          mTableData[colNum - 1][rowNum - 1]->getGLExtents(currWidth,
//                                                           currHeight);
         
//          if(currWidth > maxWidth)
//             maxWidth = currWidth;
//       }
      
//       if(mColMeta[colNum - 1].mSize < maxWidth)
//          mColMeta[colNum - 1].mSize = maxWidth;
      
//       width += maxWidth;
//       maxWidth = 0.0;
//    }

//    for(rowNum = 1; rowNum <= mNumRows; ++rowNum)
//    {
//       maxHeight = mRowMeta[rowNum - 1].mSize;
      
//       for(colNum = 1; colNum <= mNumCols; ++colNum)
//       {
//          mTableData[colNum - 1][rowNum - 1]->getGLExtents(currWidth,
//                                                           currHeight);
         
//          if(currHeight > maxHeight)
//             maxHeight = currHeight;
//       }
      
//       if(mRowMeta[rowNum - 1].mSize < maxHeight)
//          mRowMeta[rowNum - 1].mSize = maxHeight;
      
//       height += maxHeight;
//       maxHeight = 0.0;
//    }
   
   for(rowNum = 1; rowNum <= mNumRows; ++rowNum)
   {
      for(colNum = 1; colNum <= mNumCols; ++colNum)
      {
         //set all the table cell dimensions
         mTableData[colNum - 1][rowNum - 1]->setDimensions(
            mColMeta[colNum - 1].mSize,
            mRowMeta[rowNum - 1].mSize);
      }
      //we also calculate the table height now,
      //based on the maximum heights in the row metadata
      height += mRowMeta[rowNum - 1].mSize;
   }

   //return the height and width, including the borders
   width = width + ((mBorderSize * mNumCols) + mBorderSize);
   height = height + ((mBorderSize * mNumRows) + mBorderSize);
}

void GmTable::setGLPosition(float x, float y, float z)
{
   float dX, dY, dZ;
   mGLPosition.getXYZ(dX, dY, dZ);

   dX = x - dX;
   dY = y - dY;
   dZ = z - dZ;

   // set container's position
   GmDrawable::setGLPosition(x, y, z);

   // set all of the components' positions
   translateComponentsGLPositions(dX, dY, dZ);
}

void GmTable::setGLPosition(GlmPoint3* point)
{
   float x, y, z;
   point->getXYZ(x, y, z);
   setGLPosition(x, y, z);
}

void GmTable::translateComponentsGLPositions(float x, float y, float z)
{
   float cX, cY, cZ;
   // translate all of the components' positions
   vector<vector<GmTableCell*> >::iterator col;
   vector<GmTableCell*>::iterator row;
   for(col = mTableData.begin(); col != mTableData.end(); ++col)
   {
      for(row = col->begin(); row != col->end(); ++row)
      {
         (*row)->getGLPosition(cX, cY, cZ);
         (*row)->setGLPosition(cX + x, cY + y, cZ + z);
      }
   }
}

void GmTable::render2d(GmTypes::RenderOptionType renderOptions)
{
   GmInteractiveWidget::render2d(renderOptions);
   
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   
   float r, g, b, a;
   mBorderColor.getRGBA(r, g, b, a);

   glPushMatrix();
   glTranslatef(x, y, z);

   //draw the background (aka the border)
   glColor4f(r, g, b, a);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   glPopMatrix();

   // restore color settings
   glPopAttrib();
   
   //draw the cell data
   vector<vector<GmTableCell*> >::iterator col;
   vector<GmTableCell*>::iterator row;
   for(col = mTableData.begin(); col != mTableData.end(); ++col)
   {
      for(row = col->begin(); row != col->end(); ++row)
      {
         (*row)->render(renderOptions);
      }
   }
}

void GmTable::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float r, g, b, a;
   mBorderColor.getRGBA(r, g, b, a);
   
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   //draw the background
   glPushMatrix();
   glTranslatef(x, y, z);
   
   glPushName(mObjectId);
   
   glColor4f(r, g, b, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();
   
   glPopName();
   
   glPopMatrix();
   //end background

   float pX, pY, pZ, tX;
   float sX, sY;
   mGLPosition.getXYZ(pX, pY, pZ);
   pX += mBorderSize;

   int colNum = 1, rowNum = 1;
   //draw the table's data selection stuff
   vector<vector<GmTableCell*> >::iterator col;
   vector<GmTableCell*>::reverse_iterator row;
   for(col = mTableData.begin(); col != mTableData.end(); ++col)
   {
      for(row = col->rbegin(); row != col->rend(); ++row)
      {
            
         (*row)->render(renderOptions);

         sX = mColMeta[colNum - 1].mSize;
         sY = mRowMeta[mNumRows - rowNum].mSize;
         
         //render the background selection
         if(mColMeta[colNum - 1].mResizeable &&
            mRowMeta[mNumRows - rowNum].mResizeable)
         {
            glPushMatrix();
            glTranslatef(pX, pY, pZ + GLAM_LAYER);
            
            glPushName((*row)->getObjectId());

            glColor4f(0.0, 0.8, 0.8, 1.0);
            glBegin(GL_TRIANGLE_STRIP);
            glVertex2f(sX - mBorderSize, 0.0);
            glVertex2f(sX - mBorderSize, mBorderSize);
            glVertex2f(sX + mBorderSize, 0.0);
            glVertex2f(sX, mBorderSize);
            glVertex2f(sX + mBorderSize, mBorderSize + mBorderSize);
            glVertex2f(sX, mBorderSize + mBorderSize);
            glEnd();
            
            glPopName();

            glPopMatrix();
         }
         
         pY += sY + mBorderSize;
         ++rowNum;
      }
      rowNum = 1;
      pX += sX + mBorderSize;
      mGLPosition.getXYZ(tX, pY, pZ);
      ++colNum;
   }

   // restore color settings
   glPopAttrib();
}
