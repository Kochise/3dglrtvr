/*
Copyright (c) 2007, 2008 Markus Trenkwalder

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

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstdlib>

namespace {
	int mmuhack(void)
	{
		int mmufd;
		system("/sbin/rmmod mmuhack");
		system("/sbin/insmod mmuhack.o");
		mmufd = open("/dev/mmuhack", O_RDWR);
		if(mmufd < 0) return 0;

		close(mmufd);
		return 1;
	}

	void mmuunhack(void)
	{
		system("/sbin/rmmod mmuhack");
	}

	// constanst for various registers
	static const unsigned MEMTIMEX0 = 0x3802;
	static const unsigned MEMTIMEX1 = 0x3804;

	static const unsigned INTMASK = 0x0808;
	static const unsigned FPLLSETVREG = 0x0910;
	static const unsigned CLKCHGSTREG = 0x0902;

	union memtimex0_t {
		struct {
			unsigned tRCD : 4; // default = 2
			unsigned tRP  : 4; // default = 15
			unsigned tRFC : 4; // default = 4
			unsigned tMRD : 4; // default = 4
		} s;
		unsigned short i;
	};

	union memtimex1_t {
		struct {
			unsigned tWR  : 4; // default = 2
			unsigned tRAS : 4; // default = 7
			unsigned tRC  : 4; // default = 10
			unsigned LAT  : 1; // default = 1; apparently readonly
			unsigned reserved : 2; // readonly
			
			// Write: Set 1 to set new SDRAM mode parameter; 
			// Read: 0 = complete, 1 = busy
			unsigned ModeStatusSet : 1; 
		} s;
		unsigned short i;
	};

	// pointer to the registers in memory
	volatile void *mem_regs;

	// functions to access the registers
	inline volatile unsigned short& memreg16(int offset) 
	{
		return *((volatile unsigned short*)((char*)mem_regs + (offset)));
	}

	inline volatile unsigned& memreg32(int offset)
	{
		return *((volatile unsigned*)((char*)mem_regs + (offset)));
	}

	int  mmsp2_regs_init()
	{
		int fd = open("/dev/mem", O_RDWR);
		if (fd == -1) return 0;
		mem_regs = mmap(0, 0x10000, PROT_READ | PROT_WRITE, MAP_SHARED,
			fd, 0xC0000000);
		close(fd);
		if (MAP_FAILED == mem_regs) {
			mem_regs = 0; 
			return 0;
		}
		return 1;
	}

	void mmsp2_regs_deinit()
	{
		munmap((void*)mem_regs, 0x10000);
		mem_regs = 0;
	}

	memtimex0_t restore_t0;
	memtimex1_t restore_t1;

	void ramhack() 
	{
		memtimex0_t t0;
		t0.i = memreg16(MEMTIMEX0);
		restore_t0 = t0;
		t0.s.tRCD = 1;
		t0.s.tRP = 1;
		t0.s.tRFC = 0;
		t0.s.tMRD = 0;
		memreg16(MEMTIMEX0) = t0.i;
		
		memtimex1_t t1;
		t1.i = memreg16(MEMTIMEX1);
		restore_t1 = t1;
		t1.s.tWR = 0;
		t1.s.tRAS = 3;
		t1.s.tRC = 5;
		
		// The docs say to write 1 to set new mode 
		// parameter but seems to work without?
		t1.s.ModeStatusSet = 1; 
		memreg16(MEMTIMEX1) = t1.i;
		
		// Wait until mode is set. May not be necessary?
		do {
			t1.i = memreg16(MEMTIMEX1);
		} while (t1.s.ModeStatusSet);
	}

	void ramunhack()
	{
		memreg16(MEMTIMEX0) = restore_t0.i;
		
		// The docs say to write 1 to set new mode 
		// parameter but seems to work without?
		restore_t1.s.ModeStatusSet = 1; 
		memreg16(MEMTIMEX1) = restore_t1.i;
		
		// Wait until mode is set. May not be necessary?
		do {
			restore_t1.i = memreg16(MEMTIMEX1);
		} while (restore_t1.s.ModeStatusSet);

	}

	void set_cpu_frequency(unsigned mhz)
	{
		const unsigned SYS_CLK_FREQ = 7372800;

		unsigned v;
		unsigned mdiv, pdiv = 3, scale = 0;
		mhz *= 1000000;
		mdiv = (mhz * pdiv) / SYS_CLK_FREQ;

		mdiv = ((mdiv - 8) << 8) & 0xff00;
		pdiv = ((pdiv - 2) << 2) & 0xfc;
		scale &= 3;
		v = mdiv | pdiv | scale;
		
		// get interupt flags
		unsigned l = memreg32(INTMASK); 
		// turn off interrupts
		memreg32(INTMASK) = 0xFF8FFFE7; 
		// set cpu frequency
		memreg16(FPLLSETVREG) = v; 
		// wait for the frequency to be adjusted
		while(memreg16(CLKCHGSTREG) & 1); 
		// turn on interrupts
		memreg32(INTMASK) = l;            
	}

	void reset_cpu_frequency()
	{
		set_cpu_frequency(200);
	}
}


void gp2x_init()
{
	mmuhack();
	atexit(mmuunhack);

	mmsp2_regs_init();
	atexit(mmsp2_regs_deinit);

	ramhack();
	atexit(ramunhack);

	set_cpu_frequency(240);
	atexit(reset_cpu_frequency);
}

void __attribute__((naked)) flush_uppermem_cache(void *start_address, void *end_address, int flags)
{
	asm volatile(
		"swi #0x9f0002\n"
		"bx lr\n"
	);
}
