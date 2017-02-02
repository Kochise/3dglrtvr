/*
    Fusion2X - OpenGL ES-CL 1.0 Implementation
    Copyright (C) 2008 Markus Trenkwalder

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Markus Trenkwalder
    trenki2@gmx.net
*/

#ifndef FUSION2X_H_
#define FUSION2X_H_

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifdef _cplusplus
extern "C" {
#endif

typedef struct F2X_Context F2X_Context;

typedef struct F2X_ContextCreateParams {
	unsigned size;
} F2X_ContextCreateParams;

#define F2X_FORMAT_UINT16_R5_G5_A1_B5 0x0000

typedef struct F2X_RenderSurface {
	unsigned format;
	unsigned width;
	unsigned height;
	unsigned pitch;
	void *data;
} F2X_RenderSurface;

F2X_Context* F2X_CreateContext(F2X_ContextCreateParams *params);
void F2X_DeleteContext(F2X_Context *context);
F2X_Context* F2X_GetCurrentContext();
void F2X_MakeCurrent(F2X_Context *context);

#define F2X_COLOR_BUFFER 0x0001
#define F2X_DEPTH_BUFFER 0x0002

int F2X_SetParam(F2X_Context *context, unsigned pname, void *data);
int F2X_GetParam(F2X_Context *context, unsigned pname, void *data);

#ifdef _cplusplus
};
#endif

#endif
