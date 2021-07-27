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
#include <glamrock/GrScrollbar.h>
#include <glamrock/GrPushButton.h>
#include <glam/GmSlider.h>

#define PR_PERCENT 0.5     // Panel rounding percentage

GrScrollbar::GrScrollbar()
{
   GmOptions* gmOptions = GmOptions::getInstance();
   
   mIncrementButton = new GrPushButton();
   mDecrementButton = new GrPushButton();
   mSlider = new GmSlider();

   setOrientation(GmTypes::OT_VERTICAL);
   setRange(0.0, 1.0);
   setSteps(0.01, 0.1);

   mIncrementButton->setWidgetId(SCROLL_INCREMENT);
   mDecrementButton->setWidgetId(SCROLL_DECREMENT);
   mSlider->setWidgetId(SCROLL_SLIDER);

   mIncrementButton->addButtonListener(this);
   mDecrementButton->addButtonListener(this);
   mSlider->addScalarListener(this);

   mIncrementButton->setFocusParent(this);
   mDecrementButton->setFocusParent(this);
   mSlider->setFocusParent(this);

   setBaseColor((GlmColor4*)gmOptions->getColorOption(GMO_SCROLLBAR_BASECOLOR));
   setBorderWidth(gmOptions->getScalarOption(GMO_SCROLLBAR_BORDERWIDTH));
}

GrScrollbar::~GrScrollbar()
{
   delete mIncrementButton;
   delete mDecrementButton;
   delete mSlider;
}

void GrScrollbar::render2d(GmTypes::RenderOptionType renderOptions)
{
   mIncrementButton->render(renderOptions);
   mDecrementButton->render(renderOptions);
   mSlider->render(renderOptions);
}

void GrScrollbar::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   mIncrementButton->render(renderOptions);
   mDecrementButton->render(renderOptions);
   mSlider->render(renderOptions);
}
