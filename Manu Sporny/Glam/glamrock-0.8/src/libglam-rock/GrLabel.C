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
#include <glamrock/GrLabel.h>

GrLabel::GrLabel()
   : GmLabel()
{
   mPanel = new GrPanel();
}

GrLabel::~GrLabel()
{
}

void GrLabel::setGLPosition(float x, float y, float z)
{
   GmLabel::setGLPosition(x, y, z);
   mPanel->setGLPosition(x, y, z);
}

void GrLabel::layoutInGLVolume(float minX, float minY, float minZ,
                               float width, float height, float depth)
{
   GmLabel::layoutInGLVolume(minX, minY, minZ + GLAM_LAYER,
                             width, height, depth);
   
   mPanel->layoutInGLVolume(minX, minY, minZ, width, height, depth);

   float x, y, z1, z2;
   mGLPosition.getXYZ(x, y, z1);
   mPanel->getGLPosition(x, y, z2);
}

void GrLabel::render2d(GmTypes::RenderOptionType renderOptions)
{
   // render the panel first for transparent fonts
   mPanel->render(renderOptions);
   GmLabel::render2d(renderOptions);
}

void GrLabel::render2dSelect(GmTypes::RenderOptionType renderOptions)
{
   GmLabel::render2dSelect(renderOptions);
   mPanel->render(renderOptions);
}
