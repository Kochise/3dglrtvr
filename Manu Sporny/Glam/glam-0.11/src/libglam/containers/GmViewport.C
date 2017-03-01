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
#include <glam/GmViewport.h>

GmViewport::GmViewport()
{
   clipPlane0[0] = 0.0;
   clipPlane0[1] = 1.0;
   clipPlane0[2] = 0.0;
   clipPlane0[3] = 0.0;

   clipPlane1[0] = 0.0;
   clipPlane1[1] = -1.0;
   clipPlane1[2] = 0.0;
   clipPlane1[3] = 0.0;

   clipPlane2[0] = 1.0;
   clipPlane2[1] = 0.0;
   clipPlane2[2] = 0.0;
   clipPlane2[3] = 0.0;

   clipPlane3[0] = -1.0;
   clipPlane3[1] = 0.0;
   clipPlane3[2] = 0.0;
   clipPlane3[3] = 0.0;
}

GmViewport::~GmViewport()
{
}

void GmViewport::beginClip()
{
   float x, y, z;

   clipPlane0[3] = 0.0;
   clipPlane1[3] = mGLHeight;
   clipPlane2[3] = 0.0;
   clipPlane3[3] = mGLWidth;

   glPushMatrix();

   mGLPosition.getXYZ(x, y, z);
   glTranslatef(x, y, z);
   
   glClipPlane(GL_CLIP_PLANE0, clipPlane0);
   glClipPlane(GL_CLIP_PLANE1, clipPlane1);
   glClipPlane(GL_CLIP_PLANE2, clipPlane2);
   glClipPlane(GL_CLIP_PLANE3, clipPlane3);
   
   glEnable(GL_CLIP_PLANE0);
   glEnable(GL_CLIP_PLANE1);
   glEnable(GL_CLIP_PLANE2);
   glEnable(GL_CLIP_PLANE3);

   glPopMatrix();
}

void GmViewport::beginClip(float minX, float minY,
                           float maxX, float maxY)
{
   float x, y, z;

   clipPlane0[3] = minY;
   clipPlane1[3] = maxY;
   clipPlane2[3] = minX;
   clipPlane3[3] = maxX;

   glPushMatrix();

   mGLPosition.getXYZ(x, y, z);
   glTranslatef(x, y, z);
   
   glClipPlane(GL_CLIP_PLANE0, clipPlane0);
   glClipPlane(GL_CLIP_PLANE1, clipPlane1);
   glClipPlane(GL_CLIP_PLANE2, clipPlane2);
   glClipPlane(GL_CLIP_PLANE3, clipPlane3);
   
   glEnable(GL_CLIP_PLANE0);
   glEnable(GL_CLIP_PLANE1);
   glEnable(GL_CLIP_PLANE2);
   glEnable(GL_CLIP_PLANE3);

   glPopMatrix();
}

void GmViewport::endClip()
{
   glDisable(GL_CLIP_PLANE0);
   glDisable(GL_CLIP_PLANE1);
   glDisable(GL_CLIP_PLANE2);
   glDisable(GL_CLIP_PLANE3);
}

void GmViewport::render2d(GmTypes::RenderOptionType renderOptions)
{
   beginClip();
   GmContainer::render2d(renderOptions);
   endClip();
}

void GmViewport::render2dSelect(GmTypes::RenderOptionType
                                         renderOptions)
{
   beginClip();
   GmContainer::render2dSelect(renderOptions);
   endClip();
}
