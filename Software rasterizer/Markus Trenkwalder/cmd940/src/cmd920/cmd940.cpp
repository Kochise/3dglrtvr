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

#include "globals.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <fstream>

namespace {
	// constanst for various registers
	const int SYSCLKENREG = 0x0904;

	const int DUALINT920REG = 0x3B40;
	const int DUALINT940REG = 0x3B42;
	const int DUALCTRL940REG = 0x3B48;

	// structures for register contents
	union dualctrl940reg_t {
		struct {
			unsigned int bank     : 7;
			unsigned int reset    : 1;
			unsigned int reserved : 8;
		} s;
		unsigned short i;
	};

	union sysclkenreg_t {
		struct {
			unsigned int a940tclk  : 1;
			unsigned int memaclk   : 1;
			unsigned int reserved1 : 1;
			unsigned int memcclk   : 1;
			unsigned int reserved2 : 1;
			unsigned int gpioclk   : 1;
			unsigned int periclk   : 1;
			unsigned int timerclk  : 1;
			unsigned int reserved3 : 1;
			unsigned int dmaclk    : 1;
			unsigned int reserved4 : 1;
			unsigned int reserved5 : 1;
			unsigned int pwmclk    : 1;
			unsigned int adcclk    : 1;
			unsigned int esysclk   : 1;
			unsigned int reserved6 : 1;
		} s;
		unsigned short i;
	};

	volatile void *mem_regs = 0;

	// functions to access the registers
	inline volatile unsigned short& memreg16(int offset) 
	{
		return *((volatile unsigned short*)((char*)mem_regs + (offset)));
	}

	inline volatile unsigned int& memreg32(int offset)
	{
		return *((volatile unsigned int*)((char*)mem_regs + (offset)));
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
	
	void *upper_mem = 0;

	void map_upper_mem()
	{
		int fd = open("/dev/mem", O_RDWR);
		if (fd == -1) return;
		upper_mem = mmap(0, 32 * 1024 * 1024, PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, 0x02000000);
		close(fd);
		if (upper_mem == MAP_FAILED)
			upper_mem = 0;
	}

	void unmap_upper_mem()
	{
		munmap(upper_mem, 32 * 1024 * 1024);
		upper_mem = 0;
	}

	void arm940_terminate()
	{
		// reset 940
		dualctrl940reg_t dc;
		dc.s.bank = 2;
		dc.s.reset = 1;
		dc.s.reserved = 0;
		memreg16(DUALCTRL940REG) = dc.i;

		// pause 940
		sysclkenreg_t sc; 
		sc.i = memreg16(SYSCLKENREG);
		sc.s.a940tclk = 0; 
		memreg16(SYSCLKENREG) = sc.i;
	}

	bool arm940_execute(const char* filename)
	{
		arm940_terminate();

		// disable interrups between 920 and 940
		memreg16(DUALINT920REG) = 0;
		memreg16(DUALINT940REG) = 0;

		using namespace std;

		// write the program for the second CPU into memory
		char * um = static_cast<char*>(upper_mem);
		try {
			ifstream f;
			f.exceptions(ios::failbit | ios::badbit);
			f.open(filename);
			f.seekg(0, ios::end);
			ios::pos_type size = f.tellg();
			f.seekg(0, ios::beg);
			f.read(um, size);
			f.close();
		} 
		catch (...) {
			return false;
		}

		// start the 940
		sysclkenreg_t sc;
		sc.i = memreg16(SYSCLKENREG);
		sc.s.a940tclk = 1;
		memreg16(SYSCLKENREG) = sc.i;

		// take the 940 out of reset
		dualctrl940reg_t dc;
		dc.s.bank = 2;
		dc.s.reset = 0;
		dc.s.reserved = 0;
		memreg16(DUALCTRL940REG) = dc.i;

		return true;
	}

	bool initialized = false;
}

bool cmd940_init(const char *codefile)
{
	// No double initialization.
	if (initialized) return false;

	// Get upper memory and memory registers.
	map_upper_mem();
	if (!upper_mem) return false;

	if (!mmsp2_regs_init()) {
		unmap_upper_mem();
		return false;
	}

	// Initialize the ring buffers
	char *um = static_cast<char*>(upper_mem);
	cmd940i::write_rb = reinterpret_cast<cmd940i::RingBuffer<cmd940i::RING_BUFFER_SIZE_920_TO_940>*>(
		um + cmd940i::RING_BUFFER_LOCATION);
	cmd940i::read_rb  = reinterpret_cast<cmd940i::RingBuffer<cmd940i::RING_BUFFER_SIZE_940_TO_920>*>(
		um + cmd940i::RING_BUFFER_LOCATION + cmd940i::RING_BUFFER_SIZE_920_TO_940);

	// Make definitly sure the 940 isn't running. This could e.g. happen if
	// a previous invocation of the program didn't finish up correctly.
	arm940_terminate();

	cmd940i::write_rb->reset();
	cmd940i::read_rb->reset();

	initialized = arm940_execute(codefile); 
	return initialized;
}

void cmd940_deinit()
{
	// don't deinitialize when not initialized
	if (!initialized) return;

	arm940_terminate();

	mmsp2_regs_deinit();
	unmap_upper_mem();

	initialized = false;
}




