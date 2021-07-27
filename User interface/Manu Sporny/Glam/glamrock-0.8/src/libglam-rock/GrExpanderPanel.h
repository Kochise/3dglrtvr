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
#ifndef GREXPANDERPANEL_H
#define GREXPANDERPANEL_H

#include <glam/GmContentPane.h>
#include <glamrock/GrTypes.h>
#include <glam/GmLabel.h>
#include <glam/GmContainer.h>
#include <glamrock/GrShapeFactory.h>

#define BORDER_SIZE 2.0

/**
 * This is a "button" that expands to show a dialog.
 */
class GrExpanderPanel : public GmContainer
{
 protected:
   /**
    * The current state of this widget.
    */
   GrTypes::WidgetState mWidgetState;

   /**
    * The current expanding direction of this widget
    */
   GrTypes::ExpandDirection mExpandDirection;

   /**
    * Border width of widget.
    */
   float mBorderWidth;

   /**
    * How far away the panel is from the button.
    */
   float mExpandDistance;

   /**
    * The fillet size of all the outer corners of the widget.
    */
   float mFilletSize;

   /**
    * the percentage positioning of the dialog from the
    * bottom of the button
    */
   float mPositionPercent;

   /**
    * The label of the "button"
    */
   GmLabel* mButtonText;
   
   /**
    * The content pane which holds the expander panel components
    */
   GmContentPane* mPane;

   /**
    * The expanded panel title text
    */
   GmLabel* mPanelTitle;

   /**
    * The color of the titlebar
    */
   GlmColor4 mTitlebarColor;

   /**
    * The horizontal size of the titlebar
    */
   float mPanelWidth;

   /**
    * The vertical size of the titlebar
    */
   float mPanelHeight;

 public:
   /**
    * Standard constructor
    */
   GrExpanderPanel();

   /**
    * Virtual destructor
    */
   virtual ~GrExpanderPanel();
   
   /**
    * Sets the text of the label in the "button"
    */
   void setButtonText(string text);
   
   /**
    * Sets the button's label's size.
    */
   void setButtonTextSize(float size);

   /**
    * Adds the passed in component to the ContentPane
    *
    * @param component a pointer to a drawable object to be added.
    */
   virtual void addComponent(GmDrawable* component);

   /**
    * Removes the passed in component from the ContentPane
    *
    * @param component a pointer to a drawable object to be removed.
    */
   virtual bool removeComponent(GmDrawable* component);

    /**
     * Sets the color of the expanded panel's titlebar.
     *
     * @param r the red component of the color
     * @param g the green component of the color
     * @param b the blue component of the color
     * @param a the alpha component of the color
     */
    virtual void setTitlebarColor(float r, float g, float b, float a);

    /**
     * This sets the state of the expander panel
     *
     * @param state this contains the state you wish the widget to be in
     */
    virtual void setState(GrTypes::WidgetState state);

    /**
     * This returns the current state that the widget is in
     *
     * @return the state the widget is in WS_EXPANDED or WS_NORMAL
     */
    virtual GrTypes::WidgetState getState();

    /**
     * Gets the color of the dailog's titlebar.
     *
     * @return the title text color as a GlmColor4 object, THIS IS A
     *         BORROWED POINTER, DO NOT DELETE IT!
     */
    virtual GlmColor4* getTitlebarColor();

    /**
     * Sets the title text of this expanded panel to the given text.
     *
     * @param text the title text to set as this dialog's title bar
     */
    virtual void setTitleText(string titleText);

    /**
     * Gets the title text that is currently associated with this
     * expanded panel.
     *
     * @returns the text that this widget is set to display.
     */
    virtual string getTitleText();

    /**
     * Sets the display color of the title text.
     *
     * @param color the color to set as the title text color
     */
    void setTitleTextColor(GlmColor4* color);

    /**
     * Sets the display color of the title text to the given red, green, blue
     * and alpha values.
     *
     * @param r the red component of the color
     * @param g the green component of the color
     * @param b the blue component of the color
     * @param a the alpha component of the color
     */
    void setTitleTextColor(float r, float g, float b, float a);

    /**
     * Gets the color that is currently assigned to the title text.
     *
     * @return the title text color as a GlmColor4 object, THIS IS A
     *         BORROWED REFERENCE, DO NOT DELETE IT!
     */
    GlmColor4* getTitleTextColor();

    /**
     * Sets the GL size of the title text, this will affect the size at
     * which the font is rendered for the text string.
     *
     * @param size the size of the title text in GL units.
     */
    void setTitleTextSize(float size);

    /**
     * Gets the size of the title text in GL units.
     *
     * @return the GL size of the title text that is to be rendered by
     *         this widget.
     */
    float getTitleTextSize();

    /**
     * Sets the horizontal and vertical title text justification for the
     * expanded panel.
     *
     * @param horizontal the horizontal justification to use for the
     *                   title text
     * @param vertical the vertical justification to use for the title
     *                 text
     */
    void setTitleTextJustification(GmTypes::JustificationType horizontal,
                                   GmTypes::JustificationType vertical);

    /**
     * Gets the type of justification for the title text of the 
     * expanded panel.
     *
     * @param horizontal the horizontal text justification for the
     *                   title text
     * @param vertical the vertical text justification for the title text
     */
    void getTitleTextJustification(GmTypes::JustificationType& horizontal,
                                   GmTypes::JustificationType& vertical);
    
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
    * Event processing function to process a mouse release event.
    *
    * @param event the mouse release event for this object to process
    */
   virtual void mouseReleaseEvent(GmEvent* event);

   virtual void setExpandedGeometry(float expandDistance,
                                    float percentJustify,
                                    GrTypes::ExpandDirection dir);
   
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
    * Sets the GL position of the drawable.
    *
    * @param x the X position of the drawable object
    * @param y the Y position of the drawable object
    * @param z the Z position of the drawable object
    */
   virtual void setGLPosition(float x, float y, float z);

 protected:
   /**
    * Renders the panel specific portion of the expander panel to a 2D
    * image buffer using the given shape factory.
    *
    * @param sf the shape factory to use for shape creation.
    */
   virtual void render2dPanel(GrShapeFactory* sf);

   /**
    * Renders the 2d title bar to the screen.
    *
    * @param sf the shape factory to use for shape creation.
    */
   void render2dTitlebar(GrShapeFactory* sf);

   /**
    * Renders this expander panel to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the expander panel
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this expander panel to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the expander panel.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
   
};

#endif
