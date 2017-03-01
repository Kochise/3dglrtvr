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
#ifndef GMGRAPHICBUTTON_H
#define GMGRAPHICBUTTON_H

#include <glam/GmIcon.h>
#include <glam/GmButton.h>

/**
 * This class represents a button that can have two icons,
 * one that displays when the button is unpressed, and another
 * that displays when the button is pressed.
 */
class GmGraphicButton : public GmButton
{
 protected:
   /**
    * The icon to use while the button is pressed
    */
   GmIcon* mPressedIcon;

   /**
    * The icon to use while the button is not pressed
    */
   GmIcon* mUnpressedIcon;

   /**
    * Determines if this button should be rendered as translucent
    * when no mouse over, and opaque when mouse over. True turns
    * this feature on.
    */
   bool mFade;
   
 public:
   /**
    * Standard constructor
    */
   GmGraphicButton();
   
   /**
    * Virtual destructor
    */
   virtual ~GmGraphicButton();

   /**
    * Sets the pressed icon to the passed icon.
    *
    * @param icon the icon that will show up when this button is pressed
    */
    virtual void setPressedIcon(GmIcon* icon);

   /**
    * Sets the unpressed icon to the passed icon.
    *
    * @param icon the icon that will show up when this button is unpressed
    */
    virtual void setUnpressedIcon(GmIcon *icon);

   /**
    * Sets the text of this text widget to the given text.
    *
    * @param text the text to set this widget to display
    */
   virtual void setText(string text);

   /**
    * Sets the horizontal and vertical text justification for the
    * widget text.
    *
    * @param horizontal the horizontal justification to use for the
    *                   text widget.
    * @param vertical the vertical justification to use for the text widget.
    */
   virtual void setTextJustification(GmTypes::JustificationType horizontal,
                                     GmTypes::JustificationType vertical);

   /**
    * Returns the mouseover fading feature setting.
    *
    * @return true if mouseover fading is on, false if not
    */
   virtual bool getFade();

   /**
    * Set the mouseover fading feature setting.
    *
    * @param fade set true for mouseover fading, false otherwise
    */
   virtual void setFade(bool fade);

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
    * Repositions and resizes this component based on parameters
    * passed into the method. These parameters are usually passed in
    * by the parent class.
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
    * Event processing function to process a mouse entered event.
    *
    * @param event the mouse entered event for this object to process
    */
   virtual void mouseEnteredEvent(GmEvent* event);

   /**
    * Event processing function to process a mouse exited event.
    *
    * @param event the mouse exited event for this object to process
    */
   virtual void mouseExitedEvent(GmEvent* event);

   /**
    * Gets the ideal extents of the drawable in OpenGL units. In the
    * case of GmButton, this is the minimum size of the button that
    * the text will fit into with the currently set border.
    *
    * @param width the width of the drawable in OpenGL units
    * @param height the height of the drawable in OpenGL units
    */
   virtual void getGLExtents(float& width, float& height);

 protected:
   /**
    * Renders this button to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);
   
   /**
    * Renders this button to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);

   /**
    * Sets the geometry of the pressed and unpressed icons.
    */
   virtual void setIconGeometry();
};

#endif
