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

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "scene1.h"
#include "scene2.h"
#include "scene3.h"
#include "scene4.h"
#include "scene5.h"
#include "scene6.h"
#include "scene7.h"
#include "democommon.h"

#ifdef WIN32
	#include <windows.h>
	#include <tchar.h>
#endif

#ifdef GP2X

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstdlib>

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
const int SYSCLKENREG = 0x0904;

static const int DUALINT920REG = 0x3B40;
static const int DUALINT940REG = 0x3B42;
static const int DUALCTRL940REG = 0x3B48;

static const int MEMTIMEX0 = 0x3802;
static const int MEMTIMEX1 = 0x3804;

// structures for register contents
union dualctrl940reg_t {
	struct {
		unsigned int bank     : 7;
		unsigned int reset    : 1;
		unsigned int reserved : 8;
	} r;
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
	} r;
	unsigned short i;
};

union memtimex0_t {
	struct {
		unsigned int tRCD : 4; // default = 2
		unsigned int tRP  : 4; // default = 15
		unsigned int tRFC : 4; // default = 4
		unsigned int tMRD : 4; // default = 4
	} s;
	unsigned short i;
};

union memtimex1_t {
	struct {
		unsigned int tWR  : 4; // default = 2
		unsigned int tRAS : 4; // default = 7
		unsigned int tRC  : 4; // default = 10
		unsigned int LAT  : 1; // default = 1; apparently readonly
		unsigned int reserved : 2; // readonly
		
		// Write: Set 1 to set new SDRAM mode parameter; 
		// Read: 0 = complete, 1 = busy
		unsigned int ModeStatusSet : 1; 
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

void ramhack() 
{
	memtimex0_t t0;
	t0.i = memreg16(MEMTIMEX0);
	t0.s.tRCD = 1;
	t0.s.tRP = 1;
	t0.s.tRFC = 0;
	t0.s.tMRD = 0;
	memreg16(MEMTIMEX0) = t0.i;
	
	memtimex1_t t1;
	t1.i = memreg16(MEMTIMEX1);
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

#endif

int main(int ac, char *av[])
{
	Uint32 fs = 0;

#ifdef WIN32
	if (IDYES == MessageBox(0, _T("Run in fullscreen 320x240?"), _T("Fullscreen?"), MB_YESNO))
		fs = SDL_FULLSCREEN;
#endif

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);
	SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE | fs);
	SDL_JoystickOpen(0);

	if (fs)	SDL_ShowCursor(SDL_DISABLE);

#ifdef GP2X
	SDL_ShowCursor(SDL_DISABLE);

	mmuhack();
	atexit(mmuunhack);

	mmsp2_regs_init();
	atexit(mmsp2_regs_deinit);
	ramhack();

#endif

	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 512);
	atexit(Mix_CloseAudio);

	Mix_Music *mus = Mix_LoadMUS("data/enback.ogg");
	Mix_PlayMusic(mus, 1);

	scene7();
	scene1();
	scene2();
	scene3();
	scene4();
	scene5();
	scene6();

	Mix_FadeOutMusic(1000);
	while (Mix_PlayingMusic()) SDL_Delay(10);

	return 0;
}
