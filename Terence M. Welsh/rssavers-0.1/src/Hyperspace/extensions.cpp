/*
 * Copyright (C) 2005  Terence M. Welsh
 *
 * This file is part of Hyperspace.
 *
 * Hyperspace is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * Hyperspace is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <Hyperspace/extensions.h>



PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
PFNGLBINDPROGRAMARBPROC glBindProgramARB;
PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;



int queryExtension(char* name){
	char* extensions = (char *)glGetString(GL_EXTENSIONS);
	char* start = extensions;
	char* position;
	char* end;
	int quit = 0;

	while(quit == 0){
		position = strstr(start, name);
		if(!position)
			quit = 1;
		else{
			// is name a substring of a larger name?
			end = position + strlen(name);
			if(position == start || *(position-1) == ' '){
				if(*end == ' ' || *end == '\0')
					return 1;
			}
			start = end;
		}
	}

	return 0;
}


void* getProcAddr(char* name){
#ifdef WIN32
	void *addr = (void *)wglGetProcAddress(name);
	return addr;
#else
	void *addr = (void *)glXGetProcAddressARB((char *)name);
	return addr;
#endif
}


int initExtensions(){
	if(queryExtension("GL_ARB_multitexture") && queryExtension("GL_ARB_texture_cube_map")
		&& queryExtension("GL_ARB_vertex_program") && queryExtension("GL_ARB_fragment_program")){
		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)getProcAddr("glActiveTextureARB");
		glGenProgramsARB = (PFNGLGENPROGRAMSARBPROC)getProcAddr("glGenProgramsARB");
		glBindProgramARB = (PFNGLBINDPROGRAMARBPROC)getProcAddr("glBindProgramARB");
		glProgramStringARB = (PFNGLPROGRAMSTRINGARBPROC)getProcAddr("glProgramStringARB");
		return 1;
	}

	return 0;
}
