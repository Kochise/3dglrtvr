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
#include <glam/GmTypes.h>
#include <glamrock/GrShapeFactory.h>
#include <glamrock/GrToolTip.h>

#define TTR_PERCENT 0.5

GrToolTip::GrToolTip()
      : GmToolTip()
{
}

GrToolTip::~GrToolTip()
{
}

void GrToolTip::render2d(GmTypes::RenderOptionType renderOptions)
{
   GmToolTip::render2d(renderOptions);
//    // save color settings
//    glPushAttrib(GL_CURRENT_BIT);
   
//    GrShapeFactory* sf = GrShapeFactory::getInstance();

//    float x, y, z;
//    mGLPosition.getXYZ(x, y, z);

//    float r, g, b, a;
//    mBaseColor.getRGBA(r, g, b, a);

//    float textHeight, textWidth;
//    getGLExtents(textWidth, textHeight);

//    float twTranslate = textWidth;
//    float thTranslate = textHeight;
//    textWidth *= 1.05;
//    textHeight *= 1.2;

//    twTranslate = (textWidth - twTranslate) / 2.0;
//    thTranslate = (textHeight - thTranslate) / 1.0;
   
//    glPushMatrix();
//    glColor4f(r, g, b, a);
//    sf->renderRoundedRect(GrTypes::BS_ALL,
//                          GrTypes::FS_SOLID,
//                          textWidth, textHeight, TTR_PERCENT);

//    glPushMatrix();
//    glTranslatef(twTranslate, thTranslate, 0.0);
//    mTextColor.getRGBA(r, g, b, a);
//    glColor4f(r, g, b, a);

//    // render the text
//    GmTextWidget::render2d(renderOptions);

//    glPopMatrix();
   
//    // Draw the widget text outline
//    glColor4f(r*0.5, g*0.5, b*0.5, a);
//    sf->renderRoundedRect(GrTypes::BS_ALL,
//                          GrTypes::FS_OUTLINE,
//                          textWidth, textHeight, TTR_PERCENT);

//    glPopMatrix();

//    // restore color settings
//    glPopAttrib();
}
