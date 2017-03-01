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
#ifndef GMTEXTWIDGET_H
#define GMTEXTWIDGET_H

#include <glam/GmTypes.h>
#include <glam/GmDrawable.h>

/**
 * A GmTextWidget object is used to display text to the screen in some
 * form or another. TextWidget objects could be used by buttons, menu items,
 * text fields, text labels, etc.
 */
class GmTextWidget : public GmDrawable
{
 protected:
   /**
    * The text string that this widget should display
    */
   string mText;

   /**
    * The font face to use when rendering the text for this button
    */
   OGLFT::Face* mFontFace;

   /**
    * The type of font to load. (i.e. translucent font, solid font)
    */
   GmTypes::FontType mFontType;

   /**
    * Holds the internal OGLFT text displaylist ID.
    */
   GLuint mTextDisplayListId;
   
   /**
    * The foreground color to use when drawing the text
    */
   GlmColor4 mTextColor;

   /**
    * The GL size to use when drawing the text.
    */
   float mTextSize;

   /**
    * The resolution of the text.
    */
   float mTextResolution;

   /**
    * The max height of the font this text widget is using.
    */
   float mFontHeight;

   /**
    * The max descender for the font this text widget is using.
    * The descender is the distance from the baseline to the
    * bottom of a character.
    */
   float mDescender;

   /**
    * The width of the text at the last valid measurement.
    */
   float mTextWidth;

   /**
    * Determines if the previously stored text width is valid.
    */
   bool mTextWidthValid;

   /**
    * The current measure mode. The measure mode determines
    * how text will be measured, (i.e. exactly or by some
    * approximation method). When the text will be
    * measured approximately, there will be a gain in
    * performance but a loss in accuracy.
    */
   GmTypes::MeasureModeType mMeasureMode;

   /**
    * The horizontal justification to use when drawing the text
    */
   GmTypes::JustificationType mTextHJustification;
   
   /**
    * The vertical justification to use when drawing the text
    */
   GmTypes::JustificationType  mTextVJustification;

   /**
    * The horizontal offset of the text (within the widget)
    * from the left edge.
    */
   float mXOffset;
   
   /**
    * The vertical offset of the text (within the widget)
    * from the bottom edge.
    */
   float mYOffset;

 public:
   /**
    * Standard constructor
    */
   GmTextWidget();

   /**
    * Virtual destructor
    */
   virtual ~GmTextWidget();

   /**
    * Sets the text of this text widget to the given text.
    *
    * @param text the text to set this widget to display
    */
   virtual void setText(string text);

   /**
    * Appends text to the end of existing text.
    *
    * @param text the text to append.
    */
   virtual void appendText(string text);

   /**
    * Prefixes text to the beginning of existing text.
    *
    * @param text the text to append.
    */
   virtual void prefixText(string text);
   
   /**
    * Gets the text that is currently associated with this widget
    *
    * @returns the text that this widget is set to display.
    */
   virtual string getText();

   /**
    * Sets the font type.
    *
    * @param fontType the fontType to set.
    */
   virtual void setFontType(GmTypes::FontType fontType);

   /**
    * Gets the font type.
    *
    * @return the font type.
    */
   virtual GmTypes::FontType getFontType();

   /**
    * Sets the display color of the text.
    *
    * @param color the color to set the text color to
    */
   void setTextColor(GlmColor4* color);

   /**
    * Sets the display color of the text to the given red, green, blue
    * and alpha values.
    *
    * @param r the red component of the color
    * @param g the green component of the color
    * @param b the blue component of the color
    * @param a the alpha component of the color
    */
   void setTextColor(float r, float g, float b, float a);

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
   void setTextSize(float size);

   /**
    * Gets the GL size of the text (height in GL units).
    *
    * @return the GL size of the text that is to be rendered by
    *         this widget.
    */
   float getTextSize();

   /**
    * Sets the horizontal and vertical text justification for the
    * widget text.
    *
    * @param horizontal the horizontal justification to use for the
    *                   text widget.
    * @param vertical the vertical justification to use for the text widget.
    */
   void setTextJustification(GmTypes::JustificationType horizontal,
                             GmTypes::JustificationType vertical);

   /**
    * Gets the type of justification for the text in this widget.
    *
    * @param horizontal the horizontal text justification for the
    *                   widget
    * @param vertical the vertical text justification for the widget
    */
   void getTextJustification(GmTypes::JustificationType& horizontal,
                             GmTypes::JustificationType& vertical);
   
   /**
    * Sets an X and Y offset for the final drawn text.
    *
    * @param xOffset the new X offset
    * @param yOffset the new Y offset
    */
   void setOffset(float xOffset, float yOffset);

   /**
    * Gets the X and Y offset for the final drawn text.
    *
    * @param xOffset the X offset
    * @param yOffset the Y offset
    */
   void getOffset(float& xOffset, float& yOffset);

   /**
    * Sets the measure mode to use when measuring text.
    *
    * @param mode the measure mode.
    */
   void setMeasureMode(GmTypes::MeasureModeType mode);

   /**
    * Gets the current measure mode.
    *
    * @return the current measure mode.
    */
   GmTypes::MeasureModeType getMeasureMode();

   /**
    * Gets the ideal extents of the text widget in OpenGL units. This is
    * sometimes used by parent containers to determine the optimal
    * layout strategy by taking each components extents into account.
    *
    * @param width the width of the text widget in OpenGL units
    * @param height the height of the text widget in OpenGL units
    */
   void getGLExtents(float& width, float& height);

   /**
    * Determines justification offsets according to preset text
    * justification variables.
    */
   void justifyText();

 protected:
   /**
    * Loads the font face with the currently set text.
    *
    * @return true if font face loaded, false if failed to load.
    */
   virtual bool loadFontFace();
   
   /**
    * Renders this button to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the button.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);
};

#endif
