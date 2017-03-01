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
#ifndef GMBUTTON_H
#define GMBUTTON_H

#include <glam/GmInteractiveWidget.h>
#include <glam/GmButtonListener.h>
#include <glam/GmLabel.h>

/**
 * This class defines a generic visual button.
 */
class GmButton : public GmInteractiveWidget
{
 protected:
   /**
    * The vector of listeners for this button.
    */
   vector<GmButtonListener*> mButtonListeners;
   
   /**
    * The state of the button. This can usually be GMBS_PRESSED or
    * GMBS_UNPRESSED.
    */
   GmTypes::ButtonStateType mState;

   /**
    * True if the button has been pressed and no mouse release
    * has yet been received. False otherwise.
    */
   bool mPressed;

   /**
    * The width of the border around the button
    */
   float mBorderWidth;

   /**
    * Some text for labeling the button
    */
   GmLabel* mLabel;

   /**
    * Fires the appropriate button action to all listeners.
    *
    * @param action the action to pass to the listeners
    */
   virtual void fireButtonPressed(GmButtonAction* action);

 public:
   /**
    * Standard constructor.
    */
   GmButton();

   /**
    * Virtual destructor.
    */
   virtual ~GmButton();

   /**
    * Adds a button listener to this button.
    *
    * @param bl a button listener that will listen to this button
    */
   virtual void addButtonListener(GmButtonListener* bl);

   /**
    * Removes a button listener from this button.
    *
    * @param bl the button listener to remove
    * @return true if the listener was removed, false if not
    */
   virtual bool removeButtonListener(GmButtonListener* bl);
   
   /**
    * Sets the state of the button. A button can have two states,
    * pressed and unpressed.
    *
    * @param state the state to set the button to
    */
   virtual void setState(GmTypes::ButtonStateType state);

   /**
    * Gets the state of the button.
    *
    * @return the pressed state of the button.
    */
   virtual GmTypes::ButtonStateType getState();

   /**
    * Sets the width of the border around the button. The border width
    * is the total percentage that the border should visually
    * encompass on the button.
    *
    * @param border the width of the border around the button. The
    *               value is the total percentage that the border
    *               should visually encompass on the button
    */
   virtual void setBorderWidth(float borderWidth);
   
   /**
    * Gets the width of the border around the button.
    *
    * @return the width of the border around the button
    */
   virtual float getBorderWidth();

   /**
    * Sets the text of this text widget to the given text.
    */
   virtual void setText(string text);

   /**
    * Gets the text that is currently associated with this widget
    *
    * @returns the text that this widget is set to display.
    */
   virtual string getText();

   /**
    * Sets the display color of the text.
    *
    * @param color the color to set the text color to
    */
   virtual void setTextColor(GlmColor4* color);

   /**
    * Sets the display color of the text to the given red, green, blue
    * and alpha values.
    *
    * @param r the red component of the color
    * @param g the green component of the color
    * @param b the blue component of the color
    * @param a the alpha component of the color
    */
   virtual void setTextColor(float r, float g, float b, float a);

   /**
    * Gets the text color that is currently assigned to the text.
    *
    * @return the text color as a GlmColor4 object, THIS IS A BORROWED
    *         REFERENCE, DO NOT DELETE IT!
    */
   GlmColor4* getTextColor();

   /**
    * Sets the GL size of the text, this will affect the size at
    * which the font is rendered for the text string.
    *
    * @param size the size of the text in GL units.
    */
   virtual void setTextSize(float size);

   /**
    * Gets the size of the text in GL units.
    *
    * @return the GL size of the text that is to be rendered by
    *         this widget.
    */
   virtual float getTextSize();

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
    * Gets the type of justification for the text in this widget.
    *
    * @param horizontal the horizontal text justification for the
    *                   widget
    * @param vertical the vertical text justification for the widget
    */
   virtual void getTextJustification(GmTypes::JustificationType& horizontal,
                                     GmTypes::JustificationType& vertical);
   
   /**
    * Sets the GL position of the drawable.
    *
    * @param x the X position of the drawable object
    * @param y the Y position of the drawable object
    * @param z the Z position of the drawable object
    */
   virtual void setGLPosition(float x, float y, float z);   

   /**
    * Sets the visibility of the drawable object. If the visibility is
    * set to false, the object is not drawn. If the visibility is set
    * to true, the object will be drawn.
    *
    * @param visible the visibility status to set this drawable to
    */
   virtual void setVisible(bool visible);

   /**
    * Sets the visibility of the text in this object.
    *
    * @param visible the visibility status to set the text to.
    */
   virtual void setTextVisible(bool visible);

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
};

#endif
