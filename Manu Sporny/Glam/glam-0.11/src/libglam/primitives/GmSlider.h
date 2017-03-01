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
#ifndef GMSLIDER_H
#define GMSLIDER_H

#include <glam/GmTypes.h>
#include <glam/GmScalarWidget.h>
#include <glam/GmSliderButton.h>

enum SliderButtonId
{
   SLIDER_BUTTON_ID
};

/**
 * This class defines a visual slider. A slider is a scalar widget
 * that the user can "slide" to change its value.
 */
class GmSlider : public GmScalarWidget, public GmScalarListener
{
 protected:
   /**
    * The child sliderbutton that goes on the slider.
    */
   GmSliderButton* mSliderButton;

   /**
    * The orientation of the control.
    */
   GmTypes::OrientationType mOrientation;
   
 public:
   /**
    * Default constructor
    */
   GmSlider();

   /**
    * Virtual destructor
    */
   virtual ~GmSlider();   

   /**
    * Set the step size of a page.
    *
    * @param pageStep the new page step size
    */
   virtual void setPageStep(float pageStep);   
   
   /**
    * Sets the orientation of the control.
    *
    * @param orientation holds the new orientation
    */
   virtual void setOrientation(GmTypes::OrientationType orientation);

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
    * Sets the colors for the slider and its button.
    *
    * @param r the red component
    * @param g the green component
    * @param b the blue component
    * @param a the alpha component
    */
   virtual void setColor(float r, float g, float b, float a);

   /**
    * takes mouse press events and uses the mouse press to move the
    * sliderbutton to the new position
    *
    * @param event the GmEvent sent to the item upon mouse press
    */
   virtual void mousePressEvent(GmEvent* event);
   
   /**
    * Called whenever the associated scalar widget is changed.
    *
    * @param action the scalar action involved in the change.
    */
   virtual void changeOccurred(GmScalarAction* action);

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
   
 protected:
   /**
    * Converts the slider button value to the slider value.
    *
    * @return the slider's equivalent value
    */
   virtual float convertSliderButtonValue();

   /**
    * Sets the slider button's range.
    */
   virtual void setSliderButtonRange();

   /**
    * Resizes the slider button based on line step value.
    */
   virtual void resizeSliderButton();

   /**
    * render2d renders the object
    *
    * @param renderOptions the render options to use when rendering
    * the drawable.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * render2dselect renders this object to the selection buffer
    *
    * @param renderOptions the render options to use when rendering
    * the drawable
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
