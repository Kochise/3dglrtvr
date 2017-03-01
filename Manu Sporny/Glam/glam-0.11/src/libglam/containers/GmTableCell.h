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
#ifndef GMTABLECELL_H
#define GMTABLECELL_H

#include <glam/GmContainer.h>

/**
 * The table container which uses table layouts and arranges 
 */
class GmTableCell: public GmContainer
{
 protected:

   /**
    * the cell border size (in GL units to make the selection border)
    */
   float mCellBorderSize;
   
   /**
    * the cell background color
    */
   GlmColor4 mCellColor;

   /**
    * the boolean to represent the cell in the process of recieving
    * the resize through mousePressed and mouseReleased events
    */
   bool mResize;

   /**
    * holds the position of the mouse's X coord on mousePress
    */
   float mResizeX;

   /**
    * holds the position of the mouse's Y coord on mousePress
    */
   float mResizeY;
   
 public:
   /**
    * Standard constructor
    */
   GmTableCell();

   /**
    * Standard destructor
    */
   virtual ~GmTableCell();
   
   /**
    * set a new color for the table cell background
    *
    * @param newColor the new color to set as the cell background color
    */
   virtual void setColor(GlmColor4 newColor);
   
   /**
    * sets the dimensions of the cell to an arbitrary amount, if these
    * dimensions are too small to handle the table content, the cell
    * will be stretched to accomodate
    *
    * @param newWidth the new width of the table cell
    * @param newHeight the new height of the table cell
    */
   virtual void setDimensions(float newWidth, float newHeight);

   /**
    * gets the cell color
    *
    * @return GlmColor4 the color of the cell's background
    */
   virtual GlmColor4 getColor();

   /**
    * resets the cell to default properties and removes all content
    * in the cell
    */
   virtual void reset();

   /**
    * mouse drag event
    *
    * @param event the drag event data
    */
   virtual void mouseDragEvent(GmEvent* event);

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
   virtual void getGLExtents(float& width, float& height);

   /**
    * Event processing function to process a mouse press event.
    *
    * @param event the mouse press event for this object to process
    */
   virtual void mousePressEvent(GmEvent* event);
   
   /**
    * Event processing function to process a mouse release event.
    *
    * @param event the mouse release event for this object to process
    */
   virtual void mouseReleaseEvent(GmEvent* event);
   
 protected:

   /**
    * Lay 'em out!
    */
   virtual void applyLayout();
      
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
