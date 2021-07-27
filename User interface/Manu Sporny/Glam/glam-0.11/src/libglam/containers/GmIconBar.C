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
#include <glam/GmIconBar.h>
#include <glam/GmGridLayout.h>

GmIconBar::GmIconBar()
{
   GmGridLayout* layout = new GmGridLayout();
   layout->setCellInsets(0.1, 0.1, 0.1, 0.1);
   setLayout(layout);
   mNumRows = 2;
}

GmIconBar::~GmIconBar()
{
   vector<GmButton*>::iterator i;
   for(i = mButtons.begin(); i != mButtons.end(); i++)
      delete (*i);

   mButtons.clear();
}

void GmIconBar::addButton(string buttonText)
{
   GmPushButton* newButton = new GmPushButton();
   newButton->setText(buttonText);
   
   mButtons.push_back(newButton);
   addComponent(newButton);

   GmGridLayout* layout = (GmGridLayout*)getLayout();
   unsigned int columns = (mButtons.size() + 1) / mNumRows;
   layout->setGrid(mNumRows, columns);
}
