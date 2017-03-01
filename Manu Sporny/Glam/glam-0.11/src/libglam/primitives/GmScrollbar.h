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
#ifndef GMSCROLLBAR_H
#define GMSCROLLBAR_H

#include <glam/GmScalarWidget.h>
#include <glam/GmPushButton.h>
#include <glam/GmSlider.h>

/**
 * A grouping of tokens for scrollbar widget identification.
 */
enum ScrollWidgets
{
   /**
    * Token to identify the increment button scrollbar widget.
    */
   SCROLL_INCREMENT,
   
   /**
    * Token to identify the decrement button scrollbar widget.
    */
   SCROLL_DECREMENT,
   
   /**
    * Token to identify the slider scrollbar widget.
    */
   SCROLL_SLIDER
};

/**
 * A scrollbar. Scrollbars provide a way for the user to visually
 * slide between a defined range. They are commonly used in
 * conjunction with text areas, list boxes, etc.
 */
class GmScrollbar : public GmScalarWidget,
                    public GmButtonListener,
                    public GmScalarListener
{
 protected:
   /**
    * The orientation of the scrollbar. This allows the object to
    * know how it should draw itself.
    */
   GmTypes::OrientationType mOrientation;

   /**
    * The width of the border around the scrollbar.
    */
   float mBorderWidth;

   /**
    * A button to increment the value of the scrollbar.
    */
   GmGraphicButton* mIncrementButton;

   /**
    * A button to decrement the value of the scrollbar.
    */
   GmGraphicButton* mDecrementButton;

   /**
    * The slider for the scrollbar.
    */
   GmSlider* mSlider;

 public:
   /**
    * Standard constructor
    */
   GmScrollbar();

   /**
    * Virtual destructor
    */
   virtual ~GmScrollbar();

   /**
    * Set the current orientation of the scrollbar. This
    * informs the object how do draw itsef.
    *
    * @param orientation
    */
   virtual void setOrientation(GmTypes::OrientationType orientation);
   
   /**
    * Get the current orientation of the scrollbar.
    *
    * @return the current orientation of the scrollbar
    */
   virtual GmTypes::OrientationType getOrientation();

   /**
    * Set the currently set value for the range.
    *
    * @param currentValue the new current value
    */
   virtual void setCurrentValue(float currentValue);

   /**
    * Set the floor value for the range.
    *
    * @param minValue the new minimum
    */
   virtual void setMinimumValue(float minValue);
   
   /**
    * Set the ceiling value for the range.
    *
    * @param maxValue the new maximum
    */
   virtual void setMaximumValue(float maxValue);

   /**
    * Set the currently set value for the range via a percentage
    * value.
    *
    * @param currentPercentage the new current percentage value (0.0 -
    * 1.0)
    */
   virtual void setCurrentPercentage(float currentPercentage);

   /**
    * Simultaneously set the floor and ceiling values for the range.
    *
    * @param minValue the new minimum
    * @param maxValue the new maximum
    */
   virtual void setRange(float minValue, float maxValue);

   /**
    * Set the step size of a line.
    *
    * @param lineStep the new line step size
    */
   virtual void setLineStep(float lineStep);
   
   /**
    * Set the step size of a page.
    *
    * @param pageStep the new page step size
    */
   virtual void setPageStep(float pageStep);

   /**
    * Sets the width of the border around the scrollbar. The border width
    * is the total percentage that the border should visually
    * encompass on the scrollbar.
    *
    * @param border the width of the border around the scrollbar. The
    *               value is the total percentage that the border
    *               should visually encompass the scrollbar
    */
   virtual void setBorderWidth(float borderWidth);
   
   /**
    * Gets the width of the border around the scrollbar.
    *
    * @return the width of the border around the scrollbar
    */
   virtual float getBorderWidth();

   /**
    * Sets the GL position of the drawable.
    *
    * @param x the X position of the drawable object
    * @param y the Y position of the drawable object
    * @param z the Z position of the drawable object
    */
   virtual void setGLPosition(float x, float y, float z);   

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
    * Sets the base color of the object.
    *
    * @param color the base color
    */
   virtual void setBaseColor(GlmColor4* color);

   /**
    * Sets the base color of the object. 
    *
    * @param r the red value 
    * @param g the green value 
    * @param b the blue value
    * @param a the alpha value 
    */
   virtual void setBaseColor(float r, float g, float b, float a);

   /**
    * Called whenever an associated button is pressed.
    *
    * @param action the button action involved in the press.
    */
   virtual void buttonPressed(GmButtonAction* action);
   
   /**
    * Called whenever the associated scalar widget is changed.
    *
    * @param action the scalar action involved in the change.
    */
   virtual void changeOccurred(GmScalarAction* action);   

 protected:
   /**
    * Renders this scrollbar to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the scrollbar.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);
   
   /**
    * Renders this scrollbar to a 2D selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the scrollbar.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
