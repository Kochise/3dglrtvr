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
#include <glam/GmContainer.h>

GmContainer::GmContainer()
      : GmInteractiveWidget()
{
   mLayoutManager = NULL;
}

GmContainer::~GmContainer()
{
   if(mLayoutManager != NULL)
      delete mLayoutManager;

   removeAllComponents();
}

void GmContainer::addComponent(GmDrawable* component)
{
   mComponents.push_back(component);
}

void GmContainer::addComponent(GmInteractiveWidget* component)
{
   mComponents.push_back(component);
   component->setFocusParent(this);
}

bool GmContainer::insertComponent(GmDrawable* component, int index)
{
   unsigned int size = mComponents.size();

   if(size == 0 && index == 0)
   {
      mComponents.push_back(component);
      return true;
   }
   
   int count = 0;
   vector<GmDrawable*>::iterator i;
   for(i = mComponents.begin(); i != mComponents.end(); i++)
   {
      if(count == index)
      {
         mComponents.insert(i, component);
         return true;
      }

      count++;
   }

   if(i == mComponents.end() && count == index)
   {
      mComponents.push_back(component);
      return true;
   }

   // if the index was not matched, return false
   return false;
}

bool GmContainer::insertComponent(GmInteractiveWidget* component, int index)
{
   if(insertComponent((GmDrawable*)component, index))
   {
      component->setFocusParent(this);
      return true;
   }
   return false;
}

bool GmContainer::removeComponent(GmDrawable* component, bool destroy)
{
   vector<GmDrawable*>::iterator i;
   for(i = mComponents.begin(); i != mComponents.end(); i++)
   {
      if((*i)->getObjectId() == component->getObjectId())
      {
         mComponents.erase(i);

         if(destroy)
            delete component;

         return true;
      }
   }

   return false;
}

bool GmContainer::removeComponent(GmDrawable* component)
{
   return removeComponent(component, true);
}

void GmContainer::removeAllComponents(bool destroy)
{
   if(destroy)
   {
      vector<GmDrawable*>::iterator i;
      for (i = mComponents.begin(); i != mComponents.end(); i++)
         delete (*i);
   }

   mComponents.clear();
}

void GmContainer::removeAllComponents()
{
   removeAllComponents(true);
}

void GmContainer::keyPressEvent(GmEvent* event)
{
   bool handled = false;
   char key = event->getKeyData()->key;
   switch(key)
   {
      case '\t':
      {
         GmEventDistributor* ed = GmEventDistributor::getInstance();
         GmInteractiveWidget* focus = ed->getFocus();

         // if focus is container, set it to the first component
         if(focus != NULL)
         {
            if(focus->getObjectId() == getObjectId())
            {
               if(!mComponents.empty() && mComponents.front()->getFocusable())
               {
                  ((GmInteractiveWidget*)mComponents.front())->setFocused(true);
                  handled = true;
                  break;
               }
            }

            // switch focus to the next focusable component
            vector<GmDrawable*>::iterator i = mComponents.begin();
            for(; i != mComponents.end(); i++)
            {
               // if component is focusable then it is an interactive widget
               if((*i)->getFocusable())
               {
                  GmInteractiveWidget* iw = (GmInteractiveWidget*)(*i);

                  // set the focus to the next component
                  if(iw->getFocusOrder() == focus->getFocusOrder() + 1)
                  {
                     iw->setFocused(true);
                     handled = true;
                     break;
                  }
               }
            }
         }
      }
         break;
      default:
         break;
   }

   if(!handled)
      GmInteractiveWidget::keyPressEvent(event);   
}

void GmContainer::setGLPosition(float x, float y, float z)
{
   float dX, dY, dZ;
   mGLPosition.getXYZ(dX, dY, dZ);

   dX = x - dX;
   dY = y - dY;
   dZ = z - dZ;

   // set container's position
   GmDrawable::setGLPosition(x, y, z);

   // set all of the components' positions
   translateComponentsGLPositions(dX, dY, dZ);
}

void GmContainer::setGLPosition(GlmPoint3* point)
{
   float x, y, z;
   point->getXYZ(x, y, z);
   setGLPosition(x, y, z);
}

void GmContainer::translateComponentsGLPositions(float x, float y, float z)
{
   // translate all of the components' positions
   vector<GmDrawable*>::iterator i;
   for(i = mComponents.begin(); i != mComponents.end(); i++)
   {
      float cx, cy, cz;
      (*i)->getGLPosition(cx, cy, cz);
      (*i)->setGLPosition(cx + x, cy + y, cz + z);
   }
}

void GmContainer::translateComponentsGLPositions(GlmPoint3* point)
{
   float x, y, z;
   point->getXYZ(x, y, z);
   translateComponentsGLPositions(x, y, z);
}

void GmContainer::applyLayout()
{
   // apply layout without any extra insets
   applyLayout(0.0, 0.0, 0.0, 0.0);
}

void GmContainer::applyLayout(float li, float ri, float ti, float bi)
{
   // if no layout manager is allocated, use default layout method
   if(mLayoutManager == NULL)
   {
      /**
       * layoutInVolume() is called regardless of 2d or 3d nature
       * of the layout. If the layout is meant for an area and not
       * a volume, the values of pZ and mGLDepth will be identical
       * rendering the volume equivalent to an area. */

      // for storing this container's position coordinates
      float pX, pY, pZ;
      mGLPosition.getXYZ(pX, pY, pZ);

      // calculate absolute values of passed insets
      li *= mGLWidth;
      ri *= mGLWidth;
      ti *= mGLHeight;
      bi *= mGLHeight;
      
      vector<GmDrawable*>::iterator i;
      for(i = mComponents.begin(); i != mComponents.end(); i++)
      {
         (*i)->layoutInGLVolume(pX + li, pY + bi, pZ,
                                mGLWidth - ri, mGLHeight - ti, mGLDepth);
      }
   }
   else
   {
      // get the current insets for this layout
      float l, r, t, b;
      mLayoutManager->getInsets(l, r, t, b);

      // set the new insets and apply the layout
      mLayoutManager->setInsets(l + li, r + ri, t + ti, b + bi);
      mLayoutManager->apply(&mComponents);

      // reset to the old insets
      mLayoutManager->setInsets(l, r, t, b);
   }
}

void GmContainer::layoutInGLVolume(float minX, float minY, float minZ,
                                   float width, float height, float depth)
{
   // layout this component in the given volume
   GmDrawable::layoutInGLVolume(minX, minY, minZ, width, height, depth);

   // apply layout to encapsulated components
   applyLayout();
}

void GmContainer::setLayout(GmLayoutManager* layoutManager)
{
   if(mLayoutManager != NULL)
      delete mLayoutManager;
   
   mLayoutManager = layoutManager;
   if (mLayoutManager != NULL)
   {
      mLayoutManager->setContainerDimensions(mPosition,
                                             mWidth, mHeight, mDepth);

      mLayoutManager->setGLContainerDimensions(mGLPosition,
                                               mGLWidth, mGLHeight, mGLDepth);
   }
   
   setDisplayListValid(false);
}

GmLayoutManager* GmContainer::getLayout()
{
   return mLayoutManager;
}

int GmContainer::getNumComponents()
{
   return mComponents.size();
}

int GmContainer::findComponent(float x, float y)
{   
   int returnValue = -1;
   int position = 0;
   GlmPoint3* point;
   float glX, glY, glZ;

   vector<GmDrawable*>::iterator i;
   for(i = mComponents.begin(); i != mComponents.end(); i++)
   {
      point = (*i)->getGLPosition();
      point->getXYZ(glX, glY, glZ);
      
      if((x >= glX) && (x <= glX+(*i)->getGLWidth()) &&
         (y >= glY) && (y <= glY+(*i)->getGLHeight()))
      {
         returnValue = position;
         break;
      }
      
      position++;
   }
   
   return returnValue;
}

void GmContainer::getGLExtents(float& width, float& height)
{
   width = 0.0;
   height = 0.0;
   float contentWidth = 0.0, contentHeight = 0.0;

   if(mLayoutManager == NULL)
   {
      //add up the extents of all the container contents
      //note: this is a REALLY POOR size approximation
      vector<GmDrawable*>::iterator i;
      for(i = mComponents.begin(); i != mComponents.end(); i++)
      {
         (*i)->getGLExtents(contentWidth, contentHeight);
         width += contentWidth;
         height += contentHeight;
      }
   }
   else
      mLayoutManager->getGLExtents(&mComponents, width, height);
}

void GmContainer::render2d(GmTypes::RenderOptionType renderOptions)
{
   GmInteractiveWidget::render2d(renderOptions);
   
   vector<GmDrawable*>::iterator i;
   for(i = mComponents.begin(); i != mComponents.end(); i++)
      (*i)->render(renderOptions);
}

void GmContainer::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   GmInteractiveWidget::render2dSelect(renderOptions);
   
   vector<GmDrawable*>::iterator i;
   for(i = mComponents.begin(); i != mComponents.end(); i++)
      (*i)->render(renderOptions);
}
