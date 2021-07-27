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

#include "context.h"

F2X_Context* F2X_CreateContext(F2X_ContextCreateParams *params) try 
{
	// application must not use a newer (larger) param struct than we can support.
	if (params && params->size > sizeof(*params)) return 0;

	return new F2X_Context(params);
} 
catch (...) 
{
	return 0;
}

void F2X_DeleteContext(F2X_Context *context)
{
	if (current_context == context) current_context = 0;
	delete context;
}

F2X_Context* F2X_GetCurrentContext()
{
	return current_context;
}

void F2X_MakeCurrent(F2X_Context *context)
{
	current_context = context;
}

int F2X_SetParam(F2X_Context *context, unsigned pname, void *data)
{
	if (!context) context = current_context;
	if (!context) return -1;
	return context->set_param(pname, data);
}

int F2X_GetParam(F2X_Context *context, unsigned pname, void *data)
{
	if (!context) context = current_context;
	if (!context) return -1;
	return context->get_param(pname, data);
}
