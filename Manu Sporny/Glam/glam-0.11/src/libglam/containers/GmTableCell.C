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
#include <glam/GmTableCell.h>
#include <glam/Gm2dLayout.h>
#include <glam/GmGeometryAction.h>

GmTableCell::GmTableCell() 
      : GmContainer()
{
   //sets the alpha to 0 by default, so the color will not show up
   mCellColor.setRGBA(1.0, 0.0, 0.0, 1.0);
   mVisible = true;
   mCellBorderSize = 0.0;
}

GmTableCell::~GmTableCell()
{
   //remove all container components
   mComponents.clear();
}

void GmTableCell::setColor(GlmColor4 newColor)
{
   float r, g, b, a;
   newColor.getRGBA(r, g, b, a);
   mCellColor.setRGBA(r, g, b, a);
}

void GmTableCell::setDimensions(float newWidth, float newHeight)
{
   //first we see if the new arbitrary dimensions are large enough to
   //hold all the container's content
   //then we set them
   if(newWidth > mGLWidth)
   {
      //setGLWidth(newWidth);
      mGLWidth = newWidth;
   }
   if(newHeight > mGLHeight)
   {
      //setGLHeight(newHeight);
      mGLHeight = newHeight;
   }
}

GlmColor4 GmTableCell::getColor()
{
   return mCellColor;
}

void GmTableCell::reset()
{
   mCellColor.setRGBA(1.0, 1.0, 1.0, 0.0);
   mVisible = false;
   //remove container items
   mComponents.clear();
}

void GmTableCell::mouseDragEvent(GmEvent* event)
{
   GmInteractiveWidget::mouseDragEvent(event);
}

void GmTableCell::layoutInGLVolume(float minX, float minY, float minZ,
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
void GmTableCell::getGLExtents(float& width, float& height)
{
   GmContainer::getGLExtents(width, height);
}

void GmTableCell::mousePressEvent(GmEvent* event)
{
   GmInteractiveWidget::mousePressEvent(event);
   
   mResizeX = event->getMouseData()->GLx;
   mResizeY = event->getMouseData()->GLy;
   
   mResize = true;
}

void GmTableCell::mouseReleaseEvent(GmEvent* event)
{
   GmInteractiveWidget::mouseReleaseEvent(event);
   
   mResizeX = event->getMouseData()->GLx - mResizeX;
   mResizeY = event->getMouseData()->GLy - mResizeY;
   
   float newWidth = mGLWidth + mResizeX;
   float newHeight = mGLHeight - mResizeY;
   
   mResize = false;
   GmGeometryAction* ga = new GmGeometryAction(this,
                                               mResizeX, mResizeY,
                                               newWidth, newHeight);

   //fire off geometry resize action to listeners
   fireGeometryResize(ga);
   delete ga;
}

//protected
void GmTableCell::applyLayout()
{
//     float width, height;
//     getGLExtents(width, height);
//     setGLWidth(width);
//     setGLHeight(height);
    
   GmContainer::applyLayout();
}

void GmTableCell::render2d(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   
   float r, g, b, a;
   mCellColor.getRGBA(r, g, b, a);

   glPushMatrix();
   glTranslatef(x, y, z);

   //draw the background color
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
   
   // draw the items
   GmContainer::render2d(renderOptions);
}

void GmTableCell::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // render the container contents
   GmContainer::render2dSelect(renderOptions);
}
