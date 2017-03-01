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
#include <glam/GmHyperlink.h>

GmHyperlink::GmHyperlink()
      : GmLabel()
{
   GmOptions* gmOptions = GmOptions::getInstance();
   GlmColor4* color = NULL;

   color = (GlmColor4*)gmOptions->getColorOption(GMO_HYPERLINK_COLOR);
   setTextColor(color);
}

GmHyperlink::~GmHyperlink()
{
}

void GmHyperlink::fireHyperlinkPressed(GmHyperlinkAction* action)
{
   vector<GmHyperlinkListener*>::iterator i;
   for(i = mHyperlinkListeners.begin(); i != mHyperlinkListeners.end(); i++)
      (*i)->hyperlinkPressed(action);
}

void GmHyperlink::addHyperlinkListener(GmHyperlinkListener* hl)
{
   mHyperlinkListeners.push_back(hl);
}

bool GmHyperlink::removeHyperlinkListener(GmHyperlinkListener* hl)
{
   vector<GmHyperlinkListener*>::iterator i;
   for(i = mHyperlinkListeners.begin(); i != mHyperlinkListeners.end(); i++)
   {
      // compare the listeners
      if(*(*i) == *hl)
      {
         mHyperlinkListeners.erase(i);
         return true;
      }
   }

   return false;
}

void GmHyperlink::mousePressEvent(GmEvent* event)
{
   GmInteractiveWidget::mousePressEvent(event);
   
   // put code to copy the hyperlink
}

void GmHyperlink::mouseReleaseEvent(GmEvent* event)
{
   GmInteractiveWidget::mouseReleaseEvent(event);
   
   // if the mouse is over the hyperlink when released
   // then fire a hyperlink pressed event
   if(getMouseOver())
   {
      GmHyperlinkAction* ha = new GmHyperlinkAction(this);
      fireHyperlinkPressed(ha);
      delete ha;
   }
}

void GmHyperlink::render2d(GmTypes::RenderOptionType renderOptions)
{
   // if the mouse is over the hyperlink
   if(getMouseOver())
   {
      render2dToolTip(renderOptions);
      
      // save color and line settings
      glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

      float x, y, z;
      mGLPosition.getXYZ(x, y, z);

      if(!getDisplayListValid())
      {
         float textWidth, textHeight;
         mTextWidget->getGLExtents(textWidth, textHeight);

         float r, g, b, a;
         mTextWidget->getTextColor()->getRGBA(r, g, b, a);

         if(getDisplayListId() != GL_ZERO)
            glDeleteLists(getDisplayListId(), 1);
         
         setDisplayListId(glGenLists(1));
         glNewList(getDisplayListId(), GL_COMPILE);
         
         glColor4f(r, g, b, a);
         glLineWidth(2.0);
         
         // draw the underline
         glBegin(GL_LINES);
         glVertex2f(0.0, -textHeight * 0.1);
         glVertex2f(textWidth, -textHeight * 0.1);
         glEnd();

         glEndList();

         setDisplayListValid(true);
      }

      glPushMatrix();
      glTranslatef(x, y, z);
      glCallList(getDisplayListId());
      glPopMatrix();

      // restore color settings
      glPopAttrib();
   }

   // Draw the text
   mTextWidget->render(renderOptions);
}

void GmHyperlink::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);
   
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   float textWidth = mTextWidget->getGLWidth();
   float textHeight = mTextWidget->getGLHeight();

   glPushMatrix();
   glTranslatef(x, y, z);

   glPushName(mObjectId);
   
   glColor4f(1.0, 1.0, 0, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(textWidth, 0.0);
   glVertex2f(textWidth, textHeight);
   glVertex2f(0.0, textHeight);
   glEnd();
   
   glPopName();
   
   glPopMatrix();

   // restore color settings
   glPopAttrib();
}
