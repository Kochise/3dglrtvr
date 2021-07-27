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

#ifndef RINGBUFFER_453A57DD_68AE_4266_B482_22CAB7C3DFBA
#define RINGBUFFER_453A57DD_68AE_4266_B482_22CAB7C3DFBA

#ifndef ARM940
#include <cstring>
#define mymemcpy memcpy
#else
#include "../cmd940/mymemcpy.h"
#endif

#ifdef __GNUC__
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif

namespace cmd940i {
	template <int SIZE>
	class RingBuffer {
		static const size_t DATA_SIZE = SIZE - sizeof(size_t) * 2;

		volatile size_t read_pos_;
		volatile size_t write_pos_;
		volatile char data_[DATA_SIZE];

	public:
		void reset()
		{
			read_pos_ = 0;
			write_pos_ = 0;
		}

		bool can_read(size_t size)
		{
			size_t read_pos = read_pos_;
			size_t write_pos = write_pos_;
			size_t avail = write_pos >= read_pos ?
				write_pos - read_pos : 
				DATA_SIZE - read_pos + write_pos;
			return avail >= size;
		}

		bool can_write(size_t size)
		{
			size_t read_pos = read_pos_;
			size_t write_pos = write_pos_;
			size_t avail = read_pos > write_pos ?
				read_pos - write_pos - 1:
				DATA_SIZE - write_pos + read_pos - 1;
			return avail >= size;
		}

		void NOINLINE write(void *src, size_t size);
		void NOINLINE read(void *dst, size_t size);
	};

	// Defining the functions outside the class and additionally making them 
	// NOINLINE should definitly prevent them to be inlined. These functions are 
	// used in every stub function and inlining them would only increase the
	// code size.

	template <int SIZE>
	void NOINLINE RingBuffer<SIZE>::write(void *src, size_t size)
	{
		// cache volatile in local variable
		size_t write_pos = write_pos_;

		// Position after operation
		size_t tpos = (write_pos + size) % DATA_SIZE;
		
		// Wait until we can write
		while (!can_write(size));	

		if (tpos >= write_pos)
			mymemcpy((void*)(&data_[write_pos]), src, size);
		else {
			// Wraparound case
			size_t step = DATA_SIZE - write_pos;
			mymemcpy((void*)(&data_[write_pos]), src, step);
			mymemcpy((void*)&data_[0], (const char*)src + step, tpos);
		}

		write_pos_ = tpos;
	}

	template <int SIZE>
	void NOINLINE RingBuffer<SIZE>::read(void *dst, size_t size)
	{
		// cache volatile in local variable
		size_t read_pos = read_pos_;

		// Wait until we can read
		while (!can_read(size));

		// Position after operation
		size_t tpos = (read_pos + size) % DATA_SIZE;
			
		if (tpos >= read_pos)
			mymemcpy(dst, (const void*)(&data_[read_pos]), size);
		else {
			// Wraparound case
			size_t step = DATA_SIZE - read_pos;
			mymemcpy(dst, (const void*)(&data_[read_pos]), step);
			mymemcpy((char*)dst + step, (const void*)&data_[0], tpos);
		}

		read_pos_ = tpos;
	}
}

#undef NOINLINE

#ifdef mymemcpy
#undef mymemcpy
#endif

#endif
