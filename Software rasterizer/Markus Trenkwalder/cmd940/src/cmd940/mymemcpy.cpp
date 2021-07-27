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

#include "mymemcpy.h"

#define UNALIGNED(X) ((long)X & (sizeof(long) - 1))

namespace cmd940i {

	void * mymemcpy( void * s1, const void * s2, size_t n )
	{
		char * dest = (char *) s1;
		const char * src = (const char *) s2;

		if (n >= sizeof(long) && !(UNALIGNED(s1) | UNALIGNED(s2))) 
		{
			long *ldest = (long*)s1;
			const long *lsrc = (const long*)s2;
			
			while (n >= sizeof(long) * 4) {
				*ldest++ = *lsrc++;
				*ldest++ = *lsrc++;
				*ldest++ = *lsrc++;
				*ldest++ = *lsrc++;
				n -= sizeof(long) * 4;
			}
			
			while (n >= sizeof(long)) {
				*ldest++ = *lsrc++;
				n -= sizeof(long);
			}
			
			dest = (char*)ldest;
			src = (const char*)lsrc;
		}
		
		while (n--) *dest++ = *src++;

		return s1;
	}
}
