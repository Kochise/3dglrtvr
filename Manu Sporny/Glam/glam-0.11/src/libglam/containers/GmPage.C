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
#include <glam/GmPage.h>
#include <glam/GmListLayout.h>

GmPage::GmPage()
      : GmContentPane()
{
}

GmPage::~GmPage()
{
}

void GmPage::fireHyperlinkPressed(GmHyperlinkAction* action)
{
   vector<GmHyperlinkListener*>::iterator i;
   for(i = mHyperlinkListeners.begin(); i != mHyperlinkListeners.end(); i++)
      (*i)->hyperlinkPressed(action);
}

void GmPage::addHyperlinkListener(GmHyperlinkListener* hl)
{
   mHyperlinkListeners.push_back(hl);
}

bool GmPage::removeHyperlinkListener(GmHyperlinkListener* hl)
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

void GmPage::hyperlinkPressed(GmHyperlinkAction* action)
{
   // when inheriting, create new page and pass off
   GmHyperlinkAction *ha = new GmHyperlinkAction(getObjectId(),
                                                 getWidgetId(),
                                                 action->getPage());
   fireHyperlinkPressed(action);
   delete ha;
}
