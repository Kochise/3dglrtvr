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
#ifndef GMSCALARWIDGET_H
#define GMSCALARWIDGET_H

#include <glam/GmInteractiveWidget.h>
#include <glam/GmScalarListener.h>

/**
 * A class to handle floating point bounded ranges.
 */
class GmScalarWidget : public GmInteractiveWidget
{
 protected:
   /**
    * The vector of listeners that listen to this component.
    */
   vector<GmScalarListener*> mScalarListeners;
   
   /**
    * The floor of the range.
    */
   float mMinimumValue;

   /**
    * The ceiling of the range.
    */
   float mMaximumValue;

   /**
    * The currently set value of the range.
    */
   float mCurrentValue;

   /**
    * The buttons current position in the range as a percentage.
    */
   float mCurrentPercent;

   /**
    * The step size of a line.
    */
   float mLineStep;

   /**
    * The step size of a page.
    */
   float mPageStep;

   /**
    * Fires the appropriate scalar action to all listeners.
    *
    * @param action the action to pass to the listeners
    */
   virtual void fireChangeOccurred(GmScalarAction* action);

 public:
   /**
    * Standard constructor
    */
   GmScalarWidget();

   /**
    * Virtual destructor
    */
   virtual ~GmScalarWidget();

   /**
    * Adds a scalar listener to this scalar.
    *
    * @param sl a scalar listener that will listen to this scalar.
    */
   virtual void addScalarListener(GmScalarListener* sl);

   /**
    * Removes a scalar listener from this scalar.
    *
    * @param sl the scalar listener to remove.
    * @return true if the listener was removed, false if not.
    */
   virtual bool removeScalarListener(GmScalarListener* sl);
   
   /**
    * Set the floor value for the range.
    *
    * @param minValue the new minimum
    */
   virtual void setMinimumValue(float minValue);
   
   /**
    * Get the floor value for the range.
    *
    * @return the minimum
    */
   virtual float getMinimumValue();

   /**
    * Set the ceiling value for the range.
    *
    * @param maxValue the new maximum
    */
   virtual void setMaximumValue(float maxValue);

   /**
    * Get the ceiling value for the range.
    *
    * @return the maximum
    */
   virtual float getMaximumValue();

   /**
    * Set the currently set value for the range.
    *
    * @param currentValue the new current value
    */
   virtual void setCurrentValue(float currentValue);

   /**
    * Get the currently set value for the range.
    *
    * @return the current value
    */
   virtual float getCurrentValue();

   /**
    * Get the current percentage of the range that is occupied
    * by the space between minimum and current values. 
    *
    * @return the current percentage, value is between 0.0 and 1.0
    */
   virtual float getCurrentPercentage();

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
    * Get the step size of a line.
    *
    * @return the line step size
    */
   virtual float getLineStep();

   /**
    * Set the step size of a page.
    *
    * @param pageStep the new page step size
    */
   virtual void setPageStep(float pageStep);
   
   /**
    * Get the step size of a page.
    *
    * @return the page step size
    */
   virtual float getPageStep();

   /**
    * Simultaneously set the line and page step sizes for the range.
    *
    * @param lineStep the new line step size
    * @param pageStep the new page step size
    */
   virtual void setSteps(float lineStep, float pageStep);

   /**
    * Increment the current range value by a page.
    * (If the result is in the minimum/maximum bounds.)
    */
   virtual void addPage();

   /**
    * Decrement the current range value by a page.
    * (If the result is in the minimum/maximum bounds.)
    */
   virtual void subtractPage();

   /**
    * Increment the current range value by a line.
    * (If the result is in the minimum/maximum bounds.)
    */
   virtual void addLine();

   /**
    * Decrement the current range value by a line.
    * (If the result is in the minimum/maximum bounds.)
    */
   virtual void subtractLine();
};

#endif
