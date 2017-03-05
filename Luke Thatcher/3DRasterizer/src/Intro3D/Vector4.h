
#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include <intrin.h>

// Class representing a 4-dimensional vector
__declspec(align(16)) class Vector4
{
	public:
		DEFINE_NEW_DELETE_ALIGNED

#ifdef USE_SSE
		union
		{
			__m128 m;
			struct
			{
				float X;
				float Y;
				float Z;
				float W;
			};
		};
#else
		float X;
		float Y;
		float Z;
		float W;
#endif

		Vector4(void);
		Vector4(float v);
		Vector4(float x, float y, float z, float w);
		Vector4(const Vector2& v, float z, float w);
		Vector4(const Vector3& v, float w);
#ifdef USE_SSE
		Vector4(__m128 m);
#endif

		const Vector4 operator -() const;

		const Vector4 operator +(const Vector4& b) const;
		const Vector4 operator -(const Vector4& b) const;
		const Vector4 operator *(const Vector4& b) const;
		const Vector4 operator /(const Vector4& b) const;
		const Vector4 operator *(const float& b) const;
		const Vector4 operator /(const float& b) const;

		Vector4& operator +=(const Vector4& b);
		Vector4& operator -=(const Vector4& b);
		Vector4& operator *=(const Vector4& b);
		Vector4& operator /=(const Vector4& b);
		Vector4& operator *=(const float& b);
		Vector4& operator /=(const float& b);

		Vector3& AsVector3(void) const;
		Vector2& AsVector2(void) const;

		float Length(void) const;
		float LengthSquared(void) const;
		bool IsInView(void) const;

		// Public Static Helper Methods
		static float Dot(const Vector4& v0, const Vector4& v1);
		static const Vector4 Normalize(const Vector4& v0);
		static const Vector4 Clamp(const Vector4& vec, const Vector4& vMin, const Vector4& vMax);
		static const Vector4 Max(const Vector4& v0, const Vector4& v1);
		static const Vector4 Min(const Vector4& v0, const Vector4& v1);
};