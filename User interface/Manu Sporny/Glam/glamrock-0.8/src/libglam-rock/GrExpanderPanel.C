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
#include <glamrock/GrOptions.h>
#include <glamrock/GrExpanderPanel.h>
#include <glam/GmListLayout.h>

GrExpanderPanel::GrExpanderPanel() 
      : GmContainer()
{
   GmOptions* gmOptions = GmOptions::getInstance();
   GlmColor4* baseColor = NULL;
   mButtonText = new GmLabel();
   mButtonText->setText("Default");
   //mButtonText->setTextJustification(GmTypes::JTH_CENTER,
   //                                  GmTypes::JTV_MIDDLE);
   
   baseColor =
      (GlmColor4*)gmOptions->getColorOption(GRO_EXPANDERTOOLBAR_BASECOLOR);

   mWidgetState = GrTypes::WS_NORMAL;

   if(baseColor != NULL)
      mBaseColor = *baseColor;
   
   mPane = new GmContentPane();
   mPane->setRelativeGeometry(0.0, 0.0, 1.0, 1.0);
   mPane->setGLWidth(10.0);
   mPane->setGLHeight(10.0);
   mPane->setLayout(new GmListLayout());
   
   mPanelTitle = new GmLabel();
   mPanelTitle->setText("Default");
   //mPanelTitle->setTextJustification(GmTypes::JTH_CENTER,
   //                                  GmTypes::JTV_MIDDLE);
   
   mExpandDirection = GrTypes::EX_RIGHT;
   mExpandDistance = 1.0;
   mPositionPercent = 0.5;
}

GrExpanderPanel::~GrExpanderPanel()
{
   if(mButtonText != NULL)
      delete mButtonText;
   
   delete mPane;
}

void GrExpanderPanel::setButtonText(string text)
{
   if(mButtonText != NULL)
      mButtonText->setText(text);
   else
   {
      mButtonText = new GmLabel();
      mButtonText->setText(text);
      mButtonText->setRelativeGeometry(0.1, 0.1, 0.9, 0.9);
   }
}

void GrExpanderPanel::setButtonTextSize(float size)
{
   if(mButtonText != NULL)
     mButtonText->setTextSize(size);
}

void GrExpanderPanel::setExpandedGeometry(float expandDistance,
                                          float percentJustify,
                                          GrTypes::ExpandDirection dir)
{
   mExpandDistance = expandDistance;
   mPositionPercent = percentJustify;
   mExpandDirection = dir;
   setDisplayListValid(false);
}

void GrExpanderPanel::layoutInGLVolume(float minX, float minY, float minZ,
                                       float width, float height, float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   //mBorderWidth = width * BORDER_INSET;

   float pWidth, pHeight;
   pWidth = pHeight = 0.0;
   mPane->getGLExtents(pWidth, pHeight);
   
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);
   
   // panel text x, y, z
   float ptX, ptY, ptZ;
   // text width and height
   float textWidth, textHeight;
   
   mPanelTitle->getGLExtents(textWidth, textHeight);

   if(textWidth > pWidth)
      pWidth = textWidth;
   
   mPanelHeight = pHeight + 2*BORDER_SIZE;
   mPanelWidth = pWidth + 2*BORDER_SIZE;
  
   switch(mExpandDirection)
   {
      case GrTypes::EX_RIGHT:
         
         if(mPanelTitle != NULL)
         {
            ptX = x + mGLWidth + mExpandDistance
               + (mPanelWidth/2.0 - textWidth/2.0);
            ptY = y + mGLHeight + mPanelHeight*mPositionPercent;
            ptZ = z;
            
            mPanelTitle->layoutInGLVolume(ptX, ptY, ptZ,
                                         textWidth, textHeight, 0.0);
         }

         mPane->layoutInGLVolume(
            x + mExpandDistance + mGLWidth + BORDER_SIZE,
            y + mGLHeight - (1.0 - mPositionPercent)*mPanelHeight
            + BORDER_SIZE,
            z,
            pWidth, pHeight, 0.0);
         
         break;
      
      case GrTypes::EX_LEFT:
         break;
         
      case GrTypes::EX_UP:
         
         if(mPanelTitle != NULL)
         {
            ptX = x - (mPanelWidth * mPositionPercent)
               + (mPanelWidth/2.0 - textWidth/2.0);
            ptY = y + mGLHeight + mExpandDistance + mPanelHeight;
            ptZ = z;
            
            mPanelTitle->layoutInGLVolume(ptX, ptY, ptZ,
                                         textWidth, textHeight, 0.0);
         }

         mPane->layoutInGLVolume(
            x - mPanelWidth*mPositionPercent + BORDER_SIZE,
            y + mGLHeight + mExpandDistance + BORDER_SIZE,
            z,
            pWidth, pHeight, 0.0);
         
         break;
      
      case GrTypes::EX_DOWN:
         break;
         
      default:
         break;
   }
   
   if(mButtonText != NULL)
   {
      mButtonText->layoutInGLVolume(x, y, z, mGLWidth, mGLHeight, 0.0);
   }
}

void GrExpanderPanel::addComponent(GmDrawable* component)
{
   mPane->addComponent(component);
   relayout();
}

bool GrExpanderPanel::removeComponent(GmDrawable* component)
{
   return mPane->removeComponent(component);
}

void GrExpanderPanel::mouseReleaseEvent(GmEvent* event)
{
   mWidgetState = (mWidgetState == GrTypes::WS_NORMAL)
      ? GrTypes::WS_EXPANDED : GrTypes::WS_NORMAL;

   setDisplayListValid(false);
}

void GrExpanderPanel::render2dTitlebar(GrShapeFactory* sf)
{
   // Draw Extended button & body
   float textWidth, textHeight;
   float r, g, b, a; // red, green, blue, alpha

   mPanelTitle->getGLExtents(textWidth, textHeight);
         
   //this renders the titlebar and titletext if necessary
   if(mPanelTitle != NULL)
   {
      float ptX, ptY, ptZ;
            
      mGLPosition.getXYZ(ptX, ptY, ptZ);
      glPushMatrix();
            
      //ptlx = mGLWidth + mExpandDistance;
      //ptly = (pHeight * mPositionPercent) + (mGLHeight);
      // Draw the titlebar of the dialog
      switch(mExpandDirection)
      {
         case GrTypes::EX_RIGHT:
         {
            ptX = mGLWidth + mExpandDistance;
            // + (mPanelWidth/2.0 - textWidth/2.0);
            ptY = (mPanelHeight)*mPositionPercent + mGLHeight;
            ptZ = 0.0;
            break;
         }
         case GrTypes::EX_UP:
         {
            ptX = -(mPanelWidth * mPositionPercent);
            // + (mPanelWidth/2.0 - textWidth/2.0);
            ptY = mGLHeight + mExpandDistance + mPanelHeight;
            ptZ = 0.0;
            break;
         }
         default:
            //cout << "GrExpanderPanel: Unknown expand type" << endl;
            break;
      }
      
            
      glTranslatef(ptX, ptY, ptZ);

      glPushAttrib(GL_CURRENT_BIT);
      mTitlebarColor.getRGBA(r, g, b, a);
      glColor4f(r, g, b, a);
      sf->renderRoundedRect(GrTypes::BS_BOTTOM,
                            GrTypes::FS_SOLID,
                            mPanelWidth, textHeight, 0.2);
            
      // Draw the border for the titlebar
      mBaseColor.getRGBA(r, g, b, a);
      glColor4f(r*0.5, g*0.5, b*0.5, a);
      sf->renderRoundedRect(GrTypes::BS_BOTTOM,
                            GrTypes::FS_OUTLINE,
                            mPanelWidth, textHeight, 0.2);
      
      glPopAttrib();
      glPopMatrix();
   }
}

void GrExpanderPanel::render2dPanel(GrShapeFactory* sf)
{
   //float pWidth = mPane->getGLWidth();
   //float pHeight = mPane->getGLHeight();
   float textWidth, textHeight;
   mPanelTitle->getGLExtents(textWidth, textHeight);
   
   //pWidth *= 1.2;
   //pHeight *= 1.2;
   
   // button bottom left, button bottom right, button top right,
   // button bottom right
   float bblx, btlx, btrx, bbrx;
   float bbly, btly, btry, bbry;
   // panel bottom left, panel bottom right, panel top right,
   // panel bottom right
   float pblx, ptlx, ptrx, pbrx;
   float pbly, ptly, ptry, pbry;

   bblx = 0.0;
   bbly = 0.0;
   
   btlx = 0.0;
   btly = mGLHeight;
   
   btrx = mGLWidth;
   btry = mGLHeight;
   
   bbrx = mGLWidth;
   bbry = 0.0;
         
   switch(mExpandDirection)
   {
      case GrTypes::EX_RIGHT:
      {
         ptlx = mGLWidth + mExpandDistance;
         ptly = (mPanelHeight*mPositionPercent) + mGLHeight + textHeight;

         ptrx = mGLWidth + mExpandDistance + mPanelWidth;
         ptry = (mPanelHeight*mPositionPercent) + mGLHeight + textHeight;

         pbrx = mGLWidth + mExpandDistance + mPanelWidth;
         pbry = mGLHeight -(mPanelHeight*(1.0 - mPositionPercent));

         pblx = mGLWidth + mExpandDistance;
         pbly = mGLHeight -(mPanelHeight*(1.0 - mPositionPercent));
         break;
      }
      case GrTypes::EX_UP:
      {
         pblx = -(mPanelWidth * mPositionPercent);
         pbly = mGLHeight + mExpandDistance;

         ptlx = -(mPanelWidth * mPositionPercent);
         ptly = mGLHeight + mExpandDistance + mPanelHeight + textHeight;

         ptrx = mPanelWidth*(1.0 - mPositionPercent);
         ptry = mGLHeight + mExpandDistance + mPanelHeight + textHeight;

         pbrx = mPanelWidth*(1.0 - mPositionPercent);
         pbry = mGLHeight + mExpandDistance;
         break;
      }
      default:
         //cout << "GrExpanderPanel: Unknown expand direction" << endl;
         break;
   }

   switch(mExpandDirection)
   {
      case GrTypes::EX_RIGHT:
      {
         sf->sfVertex2f(bblx, bbly);
         sf->sfVertex2f(btlx, btly);
         sf->sfVertex2f(btrx, btry);
         sf->sfVertex2f(ptlx, ptly);
         sf->sfVertex2f(ptrx, ptry);
         sf->sfVertex2f(pbrx, pbry);
         sf->sfVertex2f(pblx, pbly);
         sf->sfVertex2f(bbrx, bbry);
         break;
      }
      case GrTypes::EX_UP:
      {
         sf->sfVertex2f(bblx, bbly);
         sf->sfVertex2f(btlx, btly);
         sf->sfVertex2f(pblx, pbly);
         sf->sfVertex2f(ptlx, ptly);
         sf->sfVertex2f(ptrx, ptry);
         sf->sfVertex2f(pbrx, pbry);
         sf->sfVertex2f(btrx, btry);
         sf->sfVertex2f(bbrx, bbry);
         break;
      }
      default:
         //cout << "GrExpanderPanel: Unknown expand direction" << endl;
         break;
   }
      
}

void GrExpanderPanel::render2d(GmTypes::RenderOptionType renderOptions)
{
   float r, g, b, a;
   float x, y, z;
   GrShapeFactory* sf = GrShapeFactory::getInstance();
   
   mBaseColor.getRGBA(r, g, b, a);
   mGLPosition.getXYZ(x, y, z);
    
   // save color settings
   //glPushAttrib(GL_CURRENT_BIT);
   glPushAttrib(GL_ALL_ATTRIB_BITS);
  
   if(!getDisplayListValid())
   {
      if(getDisplayListId() != GL_ZERO)
         glDeleteLists(getDisplayListId(), 1);
      
      setDisplayListId(glGenLists(1));
      glNewList(getDisplayListId(), GL_COMPILE);
   
      sf->sfFilletSize(mFilletSize);

      if(mWidgetState == GrTypes::WS_NORMAL)
      {
         glPushMatrix();
         glColor4f(r, g, b, a);
         sf->renderRoundedRect(GrTypes::BS_ALL,
                               GrTypes::FS_SOLID,
                               mGLWidth, mGLHeight, 0.3);

         glColor4f(r*0.5, g*0.5, b*0.5, a);
         sf->renderRoundedRect(GrTypes::BS_ALL,
                               GrTypes::FS_OUTLINE,
                               mGLWidth, mGLHeight, 0.3);
         glPopMatrix();
      }
      else
      {

         
         glPushMatrix();
         
         glColor4f(r, g, b, a);
         sf->sfFilletSize(2.0);
         sf->sfBegin(GrTypes::FS_SOLID);
         render2dPanel(sf);
         sf->sfEnd();
   
         glColor4f(r*0.5, g*0.5, b*0.5, a);
         sf->sfBegin(GrTypes::FS_OUTLINE);
         render2dPanel(sf);
         sf->sfEnd();
         glPopMatrix();

         render2dTitlebar(sf);
      }
   
      glEndList();
      //setDisplayListValid(true);
   }

   glPushMatrix();
   mGLPosition.getXYZ(x, y, z);
   glTranslatef(x, y, z);
   glCallList(getDisplayListId());
   glPopMatrix();

   // restore color settings
   glPopAttrib();
   
   if(mButtonText != NULL)
   {  
      mButtonText->render(renderOptions);
   }

   if(mWidgetState == GrTypes::WS_EXPANDED)
   {
      mPane->render(renderOptions);
      
      if(mPanelTitle != NULL)
         mPanelTitle->render(renderOptions);
   }
   
   //  Draw the tooltip if there is one
   if(mWidgetToolTip)
      render2dToolTip(renderOptions);
}

void GrExpanderPanel::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);
   
   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);
   
   glPushName(mObjectId);
   //Expander Button rendering
   glColor4f(0.4, 0.4, 0.4, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();
   glPopName();

   //Expanded Panel to prevent pick-throughs
   if(mWidgetState == GrTypes::WS_EXPANDED)
   {
      switch(mExpandDirection)
      {
         case GrTypes::EX_RIGHT:
         {
            glBegin(GL_POLYGON);
            glVertex2f(mGLWidth + mExpandDistance,
                       mGLHeight - (mPanelHeight*(1.0 - mPositionPercent)));
            glVertex2f(mGLWidth + mExpandDistance,
                       (mPanelHeight*mPositionPercent) + mGLHeight);
            glVertex2f(mGLWidth + mExpandDistance + mPanelWidth,
                       (mPanelHeight*mPositionPercent) + mGLHeight);
            glVertex2f(mGLWidth + mExpandDistance + mPanelWidth,
                       mGLHeight -(mPanelHeight*(1.0 - mPositionPercent)));
            glEnd();
            break;
         }
         case GrTypes::EX_UP:
         {
            glBegin(GL_POLYGON);
            glVertex2f(-(mPanelWidth * mPositionPercent),
                       mGLHeight + mExpandDistance);
            glVertex2f(-(mPanelWidth * mPositionPercent),
                       mGLHeight + mExpandDistance + mPanelHeight);
            glVertex2f(mPanelWidth*(1.0 - mPositionPercent),
                       mGLHeight + mExpandDistance + mPanelHeight);
            glVertex2f(mPanelWidth*(1.0 - mPositionPercent),
                       mGLHeight + mExpandDistance);
            glEnd();
            break;
         }
         default:
            break;
      }
   }

   glPopMatrix();

   // restore color settings
   glPopAttrib();
   
   if(mWidgetState == GrTypes::WS_EXPANDED)
   {
      mPane->render(renderOptions);
      
      if(mPanelTitle != NULL)
         mPanelTitle->render(renderOptions);
   }
}

void GrExpanderPanel::setTitlebarColor(float r, float g, float b, float a)
{
   mTitlebarColor.setRGBA(r,g,b,a);
   setDisplayListValid(false);
}

GlmColor4* GrExpanderPanel::getTitlebarColor()
{
   return &mTitlebarColor;
}

void GrExpanderPanel::setTitleText(string text)
{
   if(mPanelTitle!=NULL)
   {
      mPanelTitle->setText(text);
   }
   else
   {
      mPanelTitle = new GmLabel();
      mPanelTitle->setText(text);
   }
}

string GrExpanderPanel::getTitleText()
{
   return mPanelTitle->getText();
}

void GrExpanderPanel::setTitleTextColor(GlmColor4* color)
{
   mPanelTitle->setTextColor(color);
}

void GrExpanderPanel::setTitleTextColor(float r, float g, float b, float a)
{
   mPanelTitle->setTextColor(r, g, b, a);
}

GlmColor4* GrExpanderPanel::getTitleTextColor()
{
   return mPanelTitle->getTextColor();
}

void GrExpanderPanel::setTitleTextSize(float size)
{
   mPanelTitle->setTextSize(size);
}

float GrExpanderPanel::getTitleTextSize()
{
   return mPanelTitle->getTextSize();  
}

void GrExpanderPanel::setTitleTextJustification(
   GmTypes::JustificationType horizontal,
   GmTypes::JustificationType vertical)
{
   mPanelTitle->setTextJustification(horizontal, vertical);
}

void GrExpanderPanel::getTitleTextJustification(
   GmTypes::JustificationType& horizontal,
   GmTypes::JustificationType& vertical)
{
   mPanelTitle->getTextJustification(horizontal, vertical);
}

void GrExpanderPanel::setState(GrTypes::WidgetState state)
{
   mWidgetState = state;
   setDisplayListValid(false);
}

GrTypes::WidgetState GrExpanderPanel::getState()
{
   return mWidgetState;
}

void GrExpanderPanel::getGLExtents(float& width, float& height)
{
   if(mButtonText != NULL)
   {
      mButtonText->getGLExtents(width, height);

      width += BORDER_SIZE;
      height += BORDER_SIZE;
   }
}

void GrExpanderPanel::setGLPosition(float x, float y, float z)
{
   float dX, dY, dZ;
   mGLPosition.getXYZ(dX, dY, dZ);

   dX = x - dX;
   dY = y - dY;
   dZ = z - dZ;

   // set container's position
   GmContainer::setGLPosition(x, y, z);

   //text
   float cx, cy, cz;

   //translate the pane's components
   mPane->getGLPosition(cx, cy, cz);
   mPane->setGLPosition(cx + dX, cy + dY, cz + dZ);
   
   mButtonText->getGLPosition(cx, cy, cz);
   mButtonText->setGLPosition(cx + dX, cy + dY, cz + dZ);
   
   mPanelTitle->getGLPosition(cx, cy, cz);
   mPanelTitle->setGLPosition(cx + dX, cy + dY, cz + dZ);
}
