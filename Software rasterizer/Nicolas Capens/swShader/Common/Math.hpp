#ifndef Math_hpp
#define Math_hpp

#include <math.h>

namespace swShader
{
	extern const float M_PI;
	extern const float M_PI_180;
	extern const float M_180_PI;
	extern const float M_2PI;
	extern const float M_PI_2;

	inline float rad(float deg)
	{
		return deg * M_PI_180;
	}

	inline float deg(float rad)
	{
		return rad * M_180_PI;
	}

	inline float sin(float x)
	{
		return sinf(x);
	}

	inline float asin(float x)
	{
		return asinf(x);
	}

	inline float sinh(float x)
	{
		return sinhf(x);
	}

	inline float cos(float x)
	{
		return cosf(x);
	}

	inline float acos(float x)
	{
		return acosf(x);
	}

	inline float cosh(float x)
	{
		return coshf(x);
	}

	inline float tan(float x)
	{
		return tanf(x);
	}

	inline float cot(float x)
	{
		return 1.0f / tan(x);
	}

	inline float atan(float x)
	{
		return atanf(x);
	}

	inline float tanh(float x)
	{
		return tanhf(x);
	}

	inline float sqrt(float x)
	{
		return sqrtf(x);
	}

	// Fast but less precise square root
	inline float fsqrt(float x)
	{
		__asm
		{
			movss	xmm0, x
			rsqrtss xmm0, xmm0
			rcpss	xmm0, xmm0
			movss	x, xmm0
		}
		
		return x;
	}

	inline float exp(float x)
	{
		return expf(x);
	}

	inline float exp2(float x)
	{
		static const float ln2 = logf(2);

		return exp(ln2 * x);
	}

	inline float pow(float x, float y)
	{
		return powf(x, y);
	}

	inline float pow2(float x)
	{
		return x * x;
	}

	inline float abs(float x)
	{
		return fabsf(x);
	}

	#undef min
	#undef max

	template<class T>
	inline T max(T a, T b)
	{
		return a > b ? a : b;
	}

	template<class T>
	inline T min(T a, T b) 
	{
		return a < b ? a : b;
	}

	template<class T>
	inline T max(T a, T b, T c)
	{
		return a > b ? (a > c ? a : c) : (b > c ? b : c);
	}

	template<class T>
	inline T min(T a, T b, T c)
	{
		return a < b ? (a < c ? a : c) : (b < c ? b : c);
	}

	template<class T>
	inline void swap(T &a, T &b)
	{
		T t = a;
		a = b;
		b = t;
	}

	inline int iround(float x)
	{
		int t;

		__asm
		{
			fld   x
			fistp t
		}

		return t;
	}

	inline int ifloor(float x)
	{
		const float h = 0.5f;

		int t;

		__asm
		{
			fld   x
			fsub	h
			fistp t
		}

		return t;
	}

	/*
		---- ---- --XX XXXX = MCW_EM - exception masks (1=handle exception internally, 0=fault)
		---- ---- ---- ---X = EM_INVALID - invalid operation
		---- ---- ---- --X- = EM_DENORMAL - denormal operand
		---- ---- ---- -X-- = EM_ZERODIVIDE - divide by zero
		---- ---- ---- X--- = EM_OVERFLOW - overflow
		---- ---- ---X ---- = EM_UNDERFLOW - underflow
		---- ---- --X- ---- = EM_INEXACT - rounding was required
		---- --XX ---- ---- = MCW_PC - precision control
		---- --00 ---- ---- = PC_24 - single precision
		---- --10 ---- ---- = PC_53 - double precision
		---- --11 ---- ---- = PC_64 - extended precision
		---- XX-- ---- ---- = MCW_RC - rounding control
		---- 00-- ---- ---- = RC_NEAR - round to nearest
		---- 01-- ---- ---- = RC_DOWN - round towards -Inf
		---- 10-- ---- ---- = RC_UP - round towards +Inf
		---- 11-- ---- ---- = RC_CHOP - round towards zero
		---X ---- ---- ---- = MCW_IC - infinity control (obsolete, always affine)
		---0 ---- ---- ---- = IC_AFFINE - -Inf < +Inf
		---1 ---- ---- ---- = IC_PROJECTIVE - -Inf == +Inf
	*/
	
	inline unsigned short boostFPU()
	{
		unsigned short OldCW;
		unsigned short Load;
 
		__asm
		{
			fstcw   OldCW												; store copy of CW
			mov     ax, OldCW											; get it in ax
			and     eax, 0FCFFh										; 24 bit precision
			mov     Load, ax											; store it
			fldcw   Load												; load the FPU
		}

		return OldCW;
	}

	inline unsigned short tweakFPU(unsigned short newCW)
	{
		unsigned short oldCW;
 
		__asm
		{
			fstcw   oldCW												; store copy of CW
			fldcw   newCW												; load the FPU
		}

		return oldCW;
	}

	inline void RestoreFPU(unsigned short oldCW)
	{
		__asm fldcw oldCW;
	}

	class FPUSingle
	{
	private:
		unsigned short oldCW;

	public:
		FPUSingle()
		{
			oldCW = boostFPU();
		}

		~FPUSingle()
		{
			RestoreFPU(oldCW);
		}
	};
	
	inline int ceilFix4(int x)
	{
		return (x + 0xF) & 0xFFFFFFF0;
	}

	inline int ceilInt4(int x)
	{
		return (x + 0xF) >> 4;
	}

	inline int log2(int x)
	{
		__asm
		{
			bsr eax, x
			mov x, eax
		}

		return x;
	}

	inline int exp2(int x)
	{
		return 1 << x;
	}

	inline int ilog2(float x)
	{
		unsigned int y = *(unsigned int*)&x;

		return ((y & 0x7F800000) >> 23) - 127;
	}

	inline float log2(float x)
	{
		unsigned int y = (*(unsigned int*)&x);

		return (float)((y & 0x7F800000) >> 23) - 127 + (float)((*(unsigned int*)&x) & 0x007FFFFF) / 16777216.0f;
	}

	inline bool isPow2(int x)
	{
		return (x & -x) == x;
	}

	inline int nextPow2(int x)
	{
		return 1 << (log2(x) + 1);
	}

	template<class T>
	inline T clamp(T &x, T a, T b)
	{
		if(x < a) x = a;
		if(x > b) x = b;

		return x;
	}

	inline int ceilPow2(int x)
	{
		int i = 1;

		while(i < x)
		{
			i <<= 1;
		}

		return i;
	}

	inline float rsq(float x)
	{
	//	float xHalf = x * 0.5f; 
	//	int i = *(int*)&x; 
	//	i = 0x5F400000 - (i >> 1);
	//	x = *(float*)&i;
	//	return x * (1.5f - xHalf * x * x); 

		__asm
		{
			movss	xmm0, x
			rsqrtss	xmm0, xmm0
			movss	x, xmm0
		}

		return x;
	}

	// Mathematically correct flooring divide
	inline int floorDiv(int a, int b)
	{
		if(b == 0)
		{
			return 0x80000000;   // Prevent exception
		}

		int q;   // Quotient
		int r;   // Remainder

		if(a >= 0)
		{
			q = a / b;
		}
		else
		{
			q = -(-a / b);
			r = -a % b;

			if(r)
			{
				q--;
			}
		}

		return q;
	}

	// Mathematically correct ceiling divide
	inline int ceilDiv(int a, int b)
	{
		if(b == 0)
		{
			return 0x80000000;   // Prevent exception
		}

		a += - 1 + b;

		int q;   // Quotient
		int r;   // Remainder

		if(a >= 0)
		{
			q = a / b;
		}
		else
		{
			q = -(-a / b);
			r = -a % b;

			if(r)
			{
				q--;
			}
		}

		return q;
	}

	// Mathematically correct flooring modulo
	inline int floorMod(int a, int b)
	{
		if(b == 0)
		{
			return 0x80000000;   // Prevent exception
		}

		int r;   // Remainder

		if(a >= 0)
		{
			r = a % b;
		}
		else
		{
			r = -a % b;

			if(r)
			{
				r = b - r;
			}
		}

		return r;
	}

	// Mathematically correct ceiling modulo
	inline int ceilMod(int a, int b)
	{
		if(b == 0)
		{
			return 0x80000000;   // Prevent exception
		}

		a += - 1 + b;

		int r;   // Remainder

		if(a >= 0)
		{
			r = a % b;
		}
		else
		{
			r = -a % b;

			if(r)
			{
				r = b - r;
			}
		}

		return r + 1 - b;
	}
}

#endif   // Math_hpp