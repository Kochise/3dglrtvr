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
#ifndef GMLABEL_H
#define GMLABEL_H

#include <glam/GmTextWidget.h>
#include <glam/GmInteractiveWidget.h>

/**
 * The label widget is used to display a text string as part of the
 * interface. It is used for information purposes for the most part.
 *
 * If the text in the label is too long to fit within its boundaries,
 * then its display will be abbreviated with "..."
 */
class GmLabel : public GmInteractiveWidget
{
 protected:
   /**
    * The text to display. This is different from the text value
    * of this widget; this allows for a different display of
    * the text than its normal one (i.e. really long text
    * (i.e. "This is a really long label") can be displayed
    * abbreviated: "This is a really long...").
    */
   GmTextWidget* mTextWidget;

   /**
    * The actual text this label represents.
    */
   string mText;
   
 public:
   /**
    * Standard constructor
    */
   GmLabel();

   /**
    * Virtual destructor
    */
   virtual ~GmLabel();

   /**
    * Sets the text of this text widget to the given text.
    *
    * @param text the text to set this widget to display
    */
   virtual void setText(string text);
   
   /**
    * Gets the text that is currently associated with this widget
    *
    * @returns a the text that this widget is set to display.
    */
   virtual string getText();

   /**
    * Validates the text. If the text is too long to fit inside
    * the GL boundaries of the label, it is abbreviated.
    */
   virtual void validateText();

   /**
    * Unabbreviates the text of this label if it was abbreviated.
    */
   virtual void unAbbreviateText();

   /**
    * Returns true if the text has been abbreviated, false if not.
    *
    * @return true if text is abbreviated, false if not.
    */
   virtual bool isAbbreviated();

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
    * Gets the size of the text (height in GL units).
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
   virtual void setOffset(float xOffset, float yOffset);

   /**
    * Gets the X and Y offset for the final drawn text.
    *
    * @param xOffset the X offset
    * @param yOffset the Y offset
    */
   virtual void getOffset(float& xOffset, float& yOffset);

   /**
    * Gets the ideal extents of the text widget in OpenGL units. This is
    * sometimes used by parent containers to determine the optimal
    * layout strategy by taking each components extents into account.
    *
    * For GmLabels, if this function is called before any calls
    * to validateText() or layoutIn*(), the extents returned
    * will be the minimum width for the text assigned to this
    * label. If this function is called thereafter, the width
    * returned will represent the current width of the displayed
    * text (which could be abbreviated).
    *
    * @param width the width of the text widget in OpenGL units
    * @param height the height of the text widget in OpenGL units
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

 protected:
   /**
    * Renders this drawable to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the drawable.
    */
   virtual void render2d(GmTypes::RenderOptionType renderOptions);

   /**
    * Renders this drawable to a selection buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the drawable.
    */
   virtual void render2dSelect(GmTypes::RenderOptionType renderOptions);
};

#endif
