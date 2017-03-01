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
#include <glmath/glmath.h>
#include <glam/glam.h>
#include <GaeControlPoint.h>

#ifndef GAECURVEEDITOR_H
#define GAECURVEEDITOR_H
using namespace std;

#define RO_CONTROL_POINTS (1<<20)
#define SET_RENDER_OPTION(ro, x) ro = (GmTypes::RenderOptionType)(ro | x)
#define TOGGLE_RENDER_OPTION(ro, x) ro = (GmTypes::RenderOptionType)(ro ^ x)
#define CLEAR_RENDER_OPTION(ro, x) ro = (GmTypes::RenderOptionType)(ro & (~x))
#define RENDER_OPTION_ISSET(ro, x) (ro & x)

enum GaeAnimationMode
{
   AM_PREVIEW,
   AM_EDIT_TRANSLATION,
   AM_EDIT_ROTATION,
   AM_EDIT_SCALE
};

/* The widgets that we are going to use */
extern vector<GaeControlPoint*> gCp;
extern vector<GmDrawable*> gWidgets;

void gaeRenderPreview(GmTypes::RenderOptionType renderOptions);
void gaeInitPreviewWidgets();
void gaeRemovePreviewWidgets();

#endif /* GAECURVEEDITOR_H */
