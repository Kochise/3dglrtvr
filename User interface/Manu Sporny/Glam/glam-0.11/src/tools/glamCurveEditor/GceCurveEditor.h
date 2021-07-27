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
#include <GceControlPoint.h>

#ifndef GCECURVEEDITOR_H
#define GCECURVEEDITOR_H
using namespace std;

#define RO_CONTROL_POINTS (1<<20)
#define SET_RENDER_OPTION(ro, x) ro = (GmTypes::RenderOptionType)(ro | x)
#define TOGGLE_RENDER_OPTION(ro, x) ro = (GmTypes::RenderOptionType)(ro ^ x)
#define CLEAR_RENDER_OPTION(ro, x) ro = (GmTypes::RenderOptionType)(ro & (~x))
#define RENDER_OPTION_ISSET(ro, x) (ro & x)

/* The widgets that we are going to use */
extern vector<GceControlPoint*> gCp;
extern vector<GmInteractiveWidget*> gWidgets;

#endif /* GCECURVEEDITOR_H */
