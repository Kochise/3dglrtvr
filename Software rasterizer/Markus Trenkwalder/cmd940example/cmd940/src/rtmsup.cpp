/*
Copyright (c) 2007, Markus Trenkwalder

All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

* Neither the name of the library's copyright owner nor the names of its 
  contributors may be used to endorse or promote products derived from this 
  software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "config.h"

extern "C" int __bss_start__[];
extern "C" int __bss_end__[];
extern "C" void (*__CTOR_LIST__)();

void rtm_init()
{
	// Initialize .bss data segment. This comprises for instance all global 
	// variables.
	for (int *p = __bss_start__; p < __bss_end__; ++p)
		*p = 0;

	// call the global constructors
	void (**ctors)() = &__CTOR_LIST__ ;
	while(*ctors) (*ctors++)(); 
}

// The following handles undefined references from 
// __static_initialization_and_destruction_0(int, int) when using global 
// instances of classes or structs with destructors. The following adds dummy 
// implementations since we never exit from the program.

void *__dso_handle = (void*) &__dso_handle;

extern "C" int __cxa_atexit(void (*func) (void *), void *arg, void *d)  
{ return 0; }

// This method declaration is needed when we have pure virtual functions.
extern "C" void __cxa_pure_virtual() { while(1); }

// You can uncomment this line if you ever get linker errors because of this.
//extern "C" void __cxa_finalize(void *d) {}

#ifdef PROVIDE_DUMMY_DELETE
// Provide dummy implementation of operator delete just for the sake of making 
// everything link.
void operator delete (void *ptr) {}
#endif

#if STATIC_CLASS_GUARDS
__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire( __guard* g )
{
	return !*(char*)(g);
}

extern "C" void __cxa_guard_release( __guard* g )
{
	*(char*)g = 1;
}

extern "C" void __cxa_guard_abort( __guard* g )
{
}
#endif
