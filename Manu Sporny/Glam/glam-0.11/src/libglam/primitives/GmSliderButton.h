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
#ifndef GMSLIDERBUTTON_H
#define GMSLIDERBUTTON_H

#include <GL/glut.h>
#include <glmath/GlmColor4.h>
#include <glam/GmScalarWidget.h>
#include <glam/GmScalarAction.h>
#include <glam/GmIcon.h>
#include <glam/GmGraphicButton.h>

/**
 * A slider button. Slider buttons are like regular buttons except
 * that they do not have the concept of a pressed/unpressed state
 * and therefore do not have associated visual representations of
 * those states.
 *
 * A slider button has a directional orientation, usually given to it
 * by its parent slider. It also has a range assigned by the parent
 * slider, and its current position in that range.
 * NOTE: A slider button's position is determined by the lower left
 * corner, so their range relative to the slider will be from slider
 * position to slider height - sliderbutton height.
 */
class GmSliderButton : public GmScalarWidget
{
 protected:
   /**
    * Check to see if we are using the icon or just some polys to render
    */
   bool mIconUsed;
   
   /**
    * The icon for the button, if it is added.
    * Otherwise, it just uses a default button shape.
    */
   GmGraphicButton* mButtonIcon;
   
   /**
    * The border size
    */
   float mBorderSize;

   /**
    * The last measured mouse position (either x or y based on orientation)
    */
   float mLastMousePos;

   /**
    * True when the mouse is in the range of the button, false when not.
    */
   float mMouseInRange;
   
   /**
    * The orientation of the sliding button
    */
   GmTypes::OrientationType mOrientation;
   
 public:
   /**
    * Standard constructor
    */
   GmSliderButton();
   
   /**
    * Virtual destructor
    */
   virtual ~GmSliderButton();

   /**
    * Sets the orientation of the slider button, because it needs it
    * to calculate its change when reported.
    *
    * @param orientation the orientation to set
    */
   virtual void setOrientation(GmTypes::OrientationType orientation);

   /**
    * Set the currently set value for the range. The button will
    * be rendered at this value, it is a GL position.
    *
    * @param currentValue the new current value
    */
   virtual void setCurrentValue(float currentValue);
   
   /**
    * Sets the current mouse click position
    *
    * @param event the mouse click event for this object to process
    */
   virtual void mousePressEvent(GmEvent* event);
   
   /**
    * Event processing function to process a mouse drag event.
    *
    * @param event the mouse drag event for this object to process
    */
   virtual void mouseDragEvent(GmEvent* event);

   /**
    * Set the colors for the non-icon button
    *
    * @param r the red component of color
    * @param g the green component of color
    * @param b the blue component of color
    * @param a the alpha component
    */
   virtual void setColor(float r, float g, float b, float a);
   
   /**
    * Set the icons for up and down button states
    * @param pressedIcon the icon for the button when it is pressed
    * @param unpressedIcon the icon for the button when it is
    * unpressed
    *
    * If both parameters are passed in NULL, it will delete the
    * current icons and set the slider to NOT use the graphic button.
    */
   virtual void setIcons(GmIcon* pressedIcon, GmIcon* unpressedIcon);

   /**
    * Sets the GL position of the drawable.
    *
    * @param x the X position of the drawable object
    * @param y the Y position of the drawable object
    * @param z the Z position of the drawable object
    */
   virtual void setGLPosition(float x, float y, float z);   

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
};

#endif
