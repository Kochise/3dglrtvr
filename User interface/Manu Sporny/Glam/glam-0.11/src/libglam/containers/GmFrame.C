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
#include <glam/GmFrame.h>
#include <glam/Gm2dLayout.h>
#include <glam/GmSvgIcon.h>

GmFrame::GmFrame()
      : GmDialog()
{
   mCloseButton = new GmGraphicButton();
   mCloseButton->addButtonListener(this);

   // set mouseover fading for closebutton to true
   mCloseButton->setFade(true);

   // Setup the close button's appearance
   GmSvgIcon* pIcon = new GmSvgIcon(GLAM_DATA_DIR "/checkbox-checked.svg");
   GmSvgIcon* upIcon = new GmSvgIcon(GLAM_DATA_DIR "/checkbox-checked.svg");

   mCloseButton->setPressedIcon(pIcon);
   mCloseButton->setUnpressedIcon(upIcon);
   
   mContentPanel = new GmContentPane();

   // add the components to the frame
   GmContainer::addComponent(mCloseButton);
   GmContainer::addComponent(mContentPanel);

   // set the layout of the frame
   Gm2dLayout* layout = new Gm2dLayout();
   GmContainer::setLayout(layout);

   // set the default title bar height (TEMP CODE)
   mTitlebarHeight = 0.1;
   mFrameBorderWidth = 0.0;
   mCloseButtonSize = 0.5;    // default is 1/2 height of titlebar
   mCloseButtonOffset = 1.1;

   positionCloseButton();
   positionContentPanel();
   
   if(mTitleTextWidget)
   {
      mTitleTextWidget->setTextJustification(GmTypes::JTH_LEFT,
                                             GmTypes::JTV_MIDDLE);

      positionTitleText();
   }
}

GmFrame::~GmFrame()
{
   if(mContentPanel)
      delete mContentPanel;
   
   if(mCloseButton)
      delete mCloseButton;
}

void GmFrame::addComponent(GmDrawable* component)
{
   mContentPanel->addComponent(component);
}

bool GmFrame::removeComponent(GmDrawable* component)
{
   return mContentPanel->removeComponent(component);
}

void GmFrame::setLayout(GmLayoutManager *layoutManager)
{
   mContentPanel->setLayout(layoutManager);
}

GmLayoutManager* GmFrame::getLayout()
{
   return mContentPanel->getLayout();
}

void GmFrame::setContentPanel(GmContentPane* panel)
{
   GmContainer::removeComponent(mContentPanel);
   delete mContentPanel;

   mContentPanel = panel;
   GmContainer::addComponent(mContentPanel);
}

GmContentPane* GmFrame::getContentPanel()
{
   return mContentPanel;
}

int GmFrame::findComponent(float x, float y)
{
   return mContentPanel->findComponent(x, y);
}

void GmFrame::setCloseButtonPressedIcon(GmIcon* icon)
{
   mCloseButton->setPressedIcon(icon);
}

void GmFrame::setCloseButtonUnpressedIcon(GmIcon* icon)
{
   mCloseButton->setUnpressedIcon(icon);
}

void GmFrame::buttonPressed(GmButtonAction* action)
{
   if(action->getSrcWidgetId() == mCloseButton->getWidgetId())
      setVisible(false);
}

void GmFrame::calculateTitlebarHeight()
{
   if(mTitleTextWidget != NULL)
   {
      float tbHeight = mTitleTextWidget->getTextSize();
      
      if(mGLHeight != 0.0)
         mTitlebarHeight = tbHeight / mGLHeight;
   }
}

void GmFrame::positionCloseButton()
{
   float width = mTitlebarHeight * mCloseButtonSize;
   
   // center the button vertically, shift horizontally by offset
   float x = 1.0 - width * mCloseButtonOffset;
   float y = 1.0 - mTitlebarHeight + (mTitlebarHeight - width) / 2.0;

   // set the geometry of the close button
   // changed to use relative geometry function and
   // minx, miny, maxx, maxy
   mCloseButton->setRelativeGeometry(x, y, x + width, y + width);
}

void GmFrame::positionContentPanel()
{
   float bWidth = mFrameBorderWidth / 2.0;
   
   float width = 1.0 - bWidth;
   float height = 1.0 - mTitlebarHeight;

   // set the geometry of the content panel
   mContentPanel->setRelativeGeometry(bWidth, bWidth, width, height);
}

void GmFrame::positionTitleText()
{ 
   if(mTitleTextWidget != NULL)
   {
      float bWidth = mFrameBorderWidth / 2.0;
      mTitleTextWidget->setRelativeGeometry(bWidth,
                                            1.0 - mTitlebarHeight + bWidth,
                                            1.0 - bWidth,
                                            1.0 - bWidth);
   }
}

void GmFrame::setTitleText(string text)
{
   GmDialog::setTitleText(text);

   float tbHeight = mTitleTextWidget->getTextSize();
   
   if(mGLHeight != 0.0)
      mTitlebarHeight = tbHeight / mGLHeight;
   
   positionTitleText();
}

void GmFrame::setRelativeGeometry(float minX, float minY, float minZ,
                                  float maxX, float maxY, float maxZ)
{
   GmDialog::setRelativeGeometry(minX, minY, minZ, maxX, maxY, maxZ);

   positionCloseButton();
   positionContentPanel();
   positionTitleText();
}

void GmFrame::setRelativeGeometry(float minX, float minY,
                                  float maxX, float maxY)
{
   setRelativeGeometry(minX, minY, 0.0, maxX, maxY, 0.0);
}

void GmFrame::layoutInGLVolume(float minX, float minY, float minZ,
                               float width, float height, float depth)
{
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);
   
   // calculate the titlebar height
   if(mTitleTextWidget)
   {
      calculateTitlebarHeight();
      
      // gl height is updated so reposition the components
      positionCloseButton();
      positionContentPanel();
      positionTitleText();

      float x, y, z;
      mGLPosition.getXYZ(x, y, z);

      mTitleTextWidget->layoutInGLVolume(x, y, z,
                                         mGLWidth, mGLHeight, mGLDepth);
   }

   // layout components
   GmContainer::applyLayout();
}

void GmFrame::render2d(GmTypes::RenderOptionType renderOptions)
{
   // render title text
   if(mTitleTextWidget)
      mTitleTextWidget->render(renderOptions);

   // render components
   GmContainer::render2d(renderOptions);
}

void GmFrame::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   // render title text
   if(mTitleTextWidget)
      mTitleTextWidget->render(renderOptions);

   // render the close button
   mCloseButton->render(renderOptions);
   mContentPanel->render(renderOptions);
}
