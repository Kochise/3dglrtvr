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
#ifndef GMDIALOG_H
#define GMDIALOG_H

#include <glam/GmContentPane.h>
#include <glam/GmLabel.h>

/**
 * The GmDialog is a container that represents a dialog with the
 * user. Things such as file selection, pop-up, progress meter, and
 * the like would use a GmDialog as their base class.
 */
class GmDialog : public GmContentPane
{
 protected:
   /**
    * Some text for labeling the title bar of the dialog
    */
   GmLabel* mTitleTextWidget;

   /**
    * The color of the titlebar
    */
   GlmColor4 mTitlebarColor;
   
  public:
    /**
     * Standard constructor
     */
    GmDialog();

    /**
     * Virtual destructor
     */
    virtual ~GmDialog();

    /**
     * Sets the color of the dialog's titlebar.
     *
     * @param r the red component of the color
     * @param g the green component of the color
     * @param b the blue component of the color
     * @param a the alpha component of the color
     */
    virtual void setTitlebarColor(float r, float g, float b, float a);

    /**
     * Gets the color of the dailog's titlebar.
     *
     * @return the title text color as a GlmColor4 object, THIS IS A
     *         BORROWED POINTER, DO NOT DELETE IT!
     */
    virtual GlmColor4* getTitlebarColor();
    
    /**
     * Sets the title text of this dialog to the given text.
     *
     * @param text the title text to set as this dialog's title bar
     */
    virtual void setTitleText(string titleText);
    
    /**
     * Gets the title text that is currently associated with this dialog.
     *
     * @returns a pointer to the text that this widget is set to
     *          display. THIS IS A BORROWED POINTER, DO NOT DELETE IT!
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
     * dialog.
     *
     * @param horizontal the horizontal justification to use for the
     *                   title text
     * @param vertical the vertical justification to use for the title
     *                 text
     */
    void setTitleTextJustification(GmTypes::JustificationType horizontal,
                                   GmTypes::JustificationType vertical);

    /**
     * Gets the type of justification for the title text of the dialog
     *
     * @param horizontal the horizontal text justification for the
     *                   title text
     * @param vertical the vertical text justification for the title text
     */
    void getTitleTextJustification(GmTypes::JustificationType& horizontal,
                                   GmTypes::JustificationType& vertical);

   /**
    * Sets the GL position of the drawable.
    *
    * @param x the X position of the drawable object
    * @param y the Y position of the drawable object
    * @param z the Z position of the drawable object
    */
   virtual void setGLPosition(float x, float y, float z);
};

#endif
