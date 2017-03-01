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
#ifndef GMTEXTAREA_H
#define GMTEXTAREA_H

#include <glam/GmTextWidget.h>
#include <vector>

/**
 * A text line includes a string of text, a display list for
 * for rendering that text, and a horizontal justification offset.
 */
struct textLine
{
   string text;
   GLuint displayList;
   float HJOffset;
};

/**
 * A TextArea displays more than one line of text and adheres to
 * the given text justification and size constraints.
 */
class GmTextArea: public GmTextWidget
{
 protected:
   /**
    * The flag to set this text widget to either editable or
    * uneditable (static) state.
    */
   bool mEditable;

   /**
    * A vector to keep track of each text line in the text area.
    */
   vector<textLine> mLines;

   /**
    * A vector of support display lists that are necessary for
    * generating initial character display lists in OGLFT. If OGLFT's
    * draw() method is called for the first time on a character or set
    * of characters within a display list, OGLFT will be unable to
    * render the characters to the screen because no display lists
    * can be generated within another display list generation.
    *
    * Therefore this vector is used to store compiled
    * display lists for full justification text areas because they
    * use the draw() method. Individual text lines do not have to
    * keep track of multiple display lists and the extra overhead
    * (stl and otherwise) that comes along with it.
    */
   vector<GLuint> mSupportLists;

 public:
   /**
    * Standard constructor.
    */
   GmTextArea();

   /**
    * Virtual destructor.
    */
   virtual ~GmTextArea();

   /**
    * Sets the editable state of the text widget to either true or
    * false. If true, the text is editable, if false it is static.
    */
   virtual void setEditable(bool state);

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
    * Gets the ideal extents of the text widget in OpenGL units. This is
    * sometimes used by parent containers to determine the optimal
    * layout strategy by taking each components extents into account.
    *
    * @param width the width of the text widget in OpenGL units
    * @param height the height of the text widget in OpenGL units
    */
   virtual void getGLExtents(float& width, float& height);

 protected:
   /**
    * Splits mText on whitespace.
    */
   virtual void splitText();

   /**
    * Justifies a text line according to the current
    * justification settings.
    *
    * @param line the text line to justify
    */
   virtual void justifyLine(textLine &line);

   /**
    * Justifies a line to full. Creates its display list.
    *
    * @param line the text line to justify full
    */
   virtual void justifyFull(textLine &line);

   /**
    * Justifies the text according to current justification settings.
    */
   virtual void justifyTextArea();

   /**
    * Generates a display list for each line of text.
    */
   virtual void generateLineDisplayLists();

   /**
    * Renders this text area to a 2D image buffer.
    *
    * @param renderOptions the render options to use when rendering
    *                      the text area.
    */   
   virtual void render2d(GmTypes::RenderOptionType renderOptions);
};

#endif
