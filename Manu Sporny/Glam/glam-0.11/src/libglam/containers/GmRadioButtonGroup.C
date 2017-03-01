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
#include <glam/GmRadioButtonGroup.h>
#include <glam/Gm2dLayout.h>

GmRadioButtonGroup::GmRadioButtonGroup()
      :GmContainer()
{
   mSelected = -1;

   /// TEMP CODE: probably change this
   Gm2dLayout* layout = new Gm2dLayout();
   setLayout(layout);
}

GmRadioButtonGroup::~GmRadioButtonGroup()
{
}

void GmRadioButtonGroup::fireSelection(GmSelectionAction* action)
{
   vector<GmSelectionListener*>::iterator i;
   for(i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
      (*i)->selectionOccurred(action);
}

void GmRadioButtonGroup::fireDeselection(GmSelectionAction* action)
{
   vector<GmSelectionListener*>::iterator i;
   for(i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
      (*i)->deselectionOccurred(action);
}

void GmRadioButtonGroup::addSelectionListener(GmSelectionListener* sl)
{
   mSelectionListeners.push_back(sl);
}

bool GmRadioButtonGroup::removeSelectionListener(GmSelectionListener* sl)
{
   vector<GmSelectionListener*>::iterator i;
   for(i = mSelectionListeners.begin(); i != mSelectionListeners.end(); i++)
   {
      // compare the listeners
      if(*(*i) == *sl)
      {
         mSelectionListeners.erase(i);
         return true;
      }
   }

   return false;
}

void GmRadioButtonGroup::addComponent(GmRadioButton* component,
                                      WidgetIDType widgetId)
{
   GmContainer::addComponent(component);
   component->setWidgetId(widgetId);
   component->addButtonListener(this);
}

void GmRadioButtonGroup::layoutInGLArea(float minX, float minY,
                                        float width, float height,
                                        float layer)
{
   layoutInGLVolume(minX, minY, layer, width, height, 0.0);
}

void GmRadioButtonGroup::layoutInGLVolume(float minX, float minY, float minZ,
                                          float width, float height,
                                          float depth)
{  
   GmContainer::layoutInGLVolume(minX, minY, minZ, width, height, depth);
}

void GmRadioButtonGroup::render2dSelect(
   GmTypes::RenderOptionType renderOptions)
{
   // save color settings
   glPushAttrib(GL_CURRENT_BIT);

   float x, y, z;
   mGLPosition.getXYZ(x, y, z);

   glPushMatrix();
   glTranslatef(x, y, z);

   glColor4f(0.5, 0.5, 0.5, 1.0);
   glBegin(GL_POLYGON);
   glVertex2f(0.0, 0.0);
   glVertex2f(mGLWidth, 0.0);
   glVertex2f(mGLWidth, mGLHeight);
   glVertex2f(0.0, mGLHeight);
   glEnd();

   glPopMatrix();

   // restore color settings
   glPopAttrib();
   
   GmContainer::render2dSelect(renderOptions);   
}

GmRadioButton* GmRadioButtonGroup::getSelected()
{
   if(mSelected == -1)
      return NULL;
   
   vector<GmDrawable*>::iterator i;
   for(i = mComponents.begin(); i != mComponents.end(); i++)
   {
      GmRadioButton* rb = (GmRadioButton*)(*i);
      
      if(rb->getWidgetId() == (unsigned)mSelected)
         return rb;
   }

   return NULL;
}

void GmRadioButtonGroup::buttonPressed(GmButtonAction* action)
{
   GmSelectionAction* select;
   GmSelectionAction* deselect;

   vector<GmDrawable*>::iterator i;
   for(i = mComponents.begin(); i != mComponents.end(); i++)
   {
      GmRadioButton* rb = (GmRadioButton*)(*i);

      // find the previously selected radio button and deselect it
      if(rb->getSelected() && rb->getWidgetId() != action->getSrcWidgetId())
      {
         rb->setSelected(false);

         // fire the deselection to listeners and break
         deselect = new GmSelectionAction(this, rb->getWidgetId());
         fireDeselection(deselect);
         delete deselect;
         break;
      }
   }
   
   // set the new selection
   mSelected = action->getSrcWidgetId();

   // fire the selection to listeners
   select = new GmSelectionAction(this, mSelected);
   fireSelection(select);
   delete select;
}
