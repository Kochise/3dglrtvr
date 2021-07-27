// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <intrin.h>
#include <exception>
#include <GdiPlus.h>

#define DEFINE_NEW_DELETE_ALIGNED \
	void* operator new[](size_t size) \
	{ \
		void* pMem = _aligned_malloc(size, 16); \
		if (!pMem) \
			throw bad_alloc(); \
			\
		return pMem; \
	} \
	void* operator new(size_t size) \
	{ \
		void* pMem = _aligned_malloc(size, 16); \
		if (!pMem) \
			throw bad_alloc(); \
			\
		return pMem; \
	} \
	void operator delete[] (void* ptr) \
	{ \
		_aligned_free(ptr); \
	} \
	void operator delete(void* mem) \
	{ \
		_aligned_free(mem); \
	} \
	void* operator new (size_t size, void* ptr) throw() \
	{ \
		return ptr; \
	}

using namespace std;