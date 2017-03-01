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
#include <glam/GmOptions.h>
#include <glam/GmTextField.h>

GmTextField::GmTextField() 
      : GmInteractiveWidget()
{
   GmOptions* gmOptions = GmOptions::getInstance();

   GlmColor4* tColor = NULL;
   GlmColor4* bColor = NULL;

   bColor = (GlmColor4*)gmOptions->getColorOption(GMO_PANEL_BASECOLOR);
   tColor = (GlmColor4*)gmOptions->getColorOption(GMO_TEXT_COLOR);

   mText = new GmTextWidget();
   
   if(tColor != NULL)
      mText->setTextColor(tColor);
   if(bColor != NULL)
   {
      setBaseColor(bColor);
      mText->setBaseColor(bColor);
   }

   mText->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);

   acceptDragMimeType("text/plain");
}

GmTextField::~GmTextField()
{
   delete mText;
}

void GmTextField::setText(string text)
{
   mText->setText(text);
}

string GmTextField::getText()
{
   return mText->getText();
}

void GmTextField::setEditable(bool state)
{
   mEditable = state;
}

void GmTextField::keyPressEvent(GmEvent* event)
{
   unsigned char c;
   string cText;
   GmEvent::KeyEventType* kEvent = event->getKeyData();
   
   c = kEvent->key;
   cText = mText->getText();

   if(c == '\b')
   {
      if(cText.length() > 0)
      {
         string substr = cText.substr(0, cText.length() - 1);
         mText->setText(substr);
      }
   }
   else
   {
      cText += c;
      mText->setText(cText);
   }
   
   // pass the key press event to the focus parent by default
   if(mFocusParent != NULL)
      mFocusParent->keyPressEvent(event);
}

void GmTextField::receiveDrop(GmDropAction* dropAction)
{
   mText->appendText((char*)dropAction->getDropData());
   
   cout << "GmTextField received drop!" << endl;
}

void GmTextField::layoutInGLVolume(float minX, float minY, float minZ,
                                   float width, float height, float depth)
{
   float x, y, z;
   
   GmDrawable::layoutInGLVolume(minX, minY, minZ,
                                width, height, depth);

   mGLPosition.getXYZ(x, y, z);
   mText->layoutInGLVolume(x, y, z,
                           mGLWidth, mGLHeight, mGLDepth);
}

void GmTextField::render2d(GmTypes::RenderOptionType renderOptions)
{
   float x, y, z;
   float r, g, b, a;
   float tw, th;
   
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   mGLPosition.getXYZ(x, y, z);
   mBaseColor.getRGBA(r, g, b, a);
   mText->getGLExtents(tw, th);
   
   glPushMatrix();
   glTranslatef(x, y, z);

   //draw the background color
   glColor4f(r*0.9, g*0.9, b*0.9, a*0.8);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();
   
   glColor4f(r*1.3, g*1.3, b*1.3, a*0.8);
   glBegin(GL_LINE_LOOP);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   if(mFocusState == GmTypes::FT_ACTIVE)
   {
      glColor4f(r*1.5, g*1.5, b*0.5, a*0.8);
      glBegin(GL_LINES);
      glVertex2f(tw, 0.0);
      glVertex2f(tw, mGLHeight);
      glEnd();
   }
   
   glPopMatrix();

   // restore color settings
   glPopAttrib();
   
   mText->render(renderOptions);
}

void GmTextField::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   float x, y, z;

   // save color settings
   glPushAttrib(GL_CURRENT_BIT);
   
   mGLPosition.getXYZ(x, y, z);
   
   glPushMatrix();
   glTranslatef(x, y, z);
   
   //draw the background color
   glPushName(mObjectId);
   glColor4f(0.8, 0.0, 0.0, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   // Restore the previous drawing environment
   glPopName();
   glPopMatrix();
   glPopAttrib();
}
