#ifndef Types_hpp
#define Types_hpp

namespace swShader
{
	#define ALIGN(x) __declspec(align(x))

	typedef unsigned __int8 byte;
	typedef unsigned __int16 word;
	typedef unsigned __int32 dword;
	typedef unsigned __int64 qword;
	typedef unsigned __int8 byte8[8];
	typedef unsigned __int16 word4[4];
	typedef unsigned __int32 dword2[2];
	typedef ALIGN(16) float float4[4];
	typedef ALIGN(16) int int4[4];
}

#endif   // Types_hpp