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

#include "cmd940/cmd940.h"

#define CONTROL_REGION(REGION, ENABLE, BASE, SIZE, ISCODE)                     \
	{                                                                          \
		unsigned int cw = ENABLE | (SIZE << 1) | ((BASE) & ~0xfff);            \
		asm volatile(                                                          \
			"mcr p15, 0, %0, c6, c" #REGION ", " #ISCODE                       \
			: /* no output */                                                  \
			: "r"(cw));                                                        \
	}

#define CONTROL_CACHE(REGIONMASK, ISCODE)                                      \
	{                                                                          \
		unsigned int cw = REGIONMASK;                                          \
		asm volatile (                                                         \
			"mcr p15, 0, %0, c2, c0, " #ISCODE                                 \
			: /* no output */                                                  \
			: "r"(cw));                                                        \
	}

#define CONTROL_WRITEBUFFER(REGIONMASK)                                        \
	{                                                                          \
		unsigned int cw = REGIONMASK;                                          \
		asm volatile (                                                         \
			"mcr p15, 0, %0, c3, c0, 0"                                        \
			: /* no output */                                                  \
			: "r"(cw));                                                        \
	}

#define CONTROL_PROTECTION(PERMISSIONS, ISCODE)                                \
	{                                                                          \
		unsigned int cw = PERMISSIONS;                                         \
		asm volatile (                                                         \
		"mcr p15, 0, %0, c5, c0, " #ISCODE                                     \
			: /* no output */                                                  \
			: "r"(cw));                                                        \
	}

#define PROTECTION_MASK(r0, r1, r2, r3, r4, r5, r6, r7) \
	(r0 | (r1 << 2) | (r2 << 4) | (r3 << 6) | (r4 << 8) | (r5 << 10) \
		| (r6 << 12) | (r7 << 14))

#define REGION_MASK(r0, r1, r2, r3, r4, r5, r6, r7) \
	(r0 | (r1 << 1) | (r2 << 2) | (r3 << 3) | (r4 << 4) | (r5 << 5) | (r6 << 6)\
		| (r7 << 7))

namespace internal {
	union cp15_ctrl_reg_t {
		struct {
			unsigned int protection_enable : 1;
			unsigned int reserved1 : 1;
			unsigned int dcache_enable: 1;
			unsigned int reserved2 : 4;
			unsigned int big_end : 1;
			unsigned int reserved3 : 4;
			unsigned int icache_enable: 1;
			unsigned int except_base : 1;
			unsigned int reserved4 : 16;
			unsigned int fastbus : 1;
			unsigned int async : 1;
		} s;
		unsigned int i;
	};

	// The entry point for the 940 must be the first function defined so that 
	// the entry point address will end up beeing 0!
	// Declaring both of these functions inline and implementing them before
	// the entry point also seemed to work but I play safe here and only declare 
	// them here and define them after the entry point.

	inline cp15_ctrl_reg_t cp15_get_ctrl_reg();
	inline void cp15_set_ctrl_reg(const cp15_ctrl_reg_t& reg);
}

using namespace internal;

extern void rtm_init();

extern "C" void __attribute__((naked)) _start()
{
	// setup irq vector table
	asm volatile (
		"b __init\n" // Reset
		"b __undefined\n" // Undefined instruction
		"b __undefined\n" // Software interrupt
		"b __undefined\n" // Prefetch abort
		"b __undefined\n" // Data abort
		"b __undefined\n" // Reserved
		"b __undefined\n" // IRQ vector
		"b __undefined\n" // FIQ vector

		"__undefined:\n"
		"b __undefined\n" // infinite loop

		"__init:\n");

	// set up a 1M stack frame
	asm volatile (
		"mov sp, #0x100000\n"
		"sub sp, sp, #4" ::: "sp"); 

	// Define memory regions and enable caches. I had some weird problems with
	// this when building without optimization. The program would hang when run
	// the first time after booting but after that it would work. With 
	// optimization it always seemed to work. So, for now I only do this if 
	// optimization is enabled. I suspect this could be cauesd by compiler 
	// problems but it may also be a bug in the framework.

#ifdef __OPTIMIZE__
	// Define and enable memory region 1 to be the whole 4 GB address space.
	CONTROL_REGION(0, 1, 0, 31, 0); // Data region
	CONTROL_REGION(0, 1, 0, 31, 1); // Code region

	// Define 16M memory region
	CONTROL_REGION(1, 1, 0, 23, 0);
	CONTROL_REGION(1, 1, 0, 23, 1);

	// Define 1M region for the ringbuffer
	CONTROL_REGION(2, 1, 0x100000, 19, 0);
	CONTROL_REGION(2, 1, 0x100000, 19, 1);

	// Enable the cache only for the 16M region
	unsigned int m = REGION_MASK(0, 1, 0, 0, 0, 0, 0, 0);
	CONTROL_CACHE(m, 0);
	CONTROL_CACHE(m, 1);

	// Enable the write buffer for the 16M region
	CONTROL_WRITEBUFFER(m);

	// Allow full access to the first three defined regions
	unsigned int p = PROTECTION_MASK(3, 3, 3, 0, 0, 0, 0, 0);
	CONTROL_PROTECTION(p, 0);
	CONTROL_PROTECTION(p, 1);

	// enable protection unit and caches
	cp15_ctrl_reg_t cw = cp15_get_ctrl_reg();
	cw.s.protection_enable = 1;
	cw.s.dcache_enable = 1;
	cw.s.icache_enable = 1;
	cw.s.async = 1;
	cw.s.fastbus = 1;
	cp15_set_ctrl_reg(cw);
#endif

	// Initialize global variables and call constructors.
	rtm_init();

	cmd940_dispatch_commands();
}

namespace internal {
	inline cp15_ctrl_reg_t cp15_get_ctrl_reg()
	{
		cp15_ctrl_reg_t reg;
		asm volatile ("mrc p15, 0, %0, c1, c0, 0" : "=r"(reg.i));
		return reg;
	}

	inline void cp15_set_ctrl_reg(const cp15_ctrl_reg_t& reg) 
	{
		asm volatile ("mcr p15, 0, %0, c1, c0, 0" : /*no output*/ : "r"(reg.i));
	}
}
