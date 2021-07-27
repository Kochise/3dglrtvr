
#pragma once

#include "Vector2.h"

// Class representing a 3-dimensional vector
__declspec(align(16)) class Vector3
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
			};
		};
#else
		float X;
		float Y;
		float Z;
#endif

		Vector3(void);
		Vector3(float v);
		Vector3(float x, float y, float z);
		Vector3(const Vector2& v, float z);

#ifdef USE_SSE
		Vector3(__m128 m);
#endif

		const Vector3 operator -() const;

		const Vector3 operator +(const Vector3& b) const;
		const Vector3 operator -(const Vector3& b) const;
		const Vector3 operator *(const Vector3& b) const;
		const Vector3 operator /(const Vector3& b) const;
		const Vector3 operator *(const float& b) const;
		const Vector3 operator /(const float& b) const;

		Vector3& operator +=(const Vector3& b);
		Vector3& operator -=(const Vector3& b);
		Vector3& operator *=(const Vector3& b);
		Vector3& operator /=(const Vector3& b);
		Vector3& operator *=(const float& b);
		Vector3& operator /=(const float& b);

		float Length(void) const;
		float LengthSquared(void) const;

		// Public Static Helper Methods
		static float Dot(const Vector3& v0, const Vector3& v1);
		static const Vector3 Cross(const Vector3& v0, const Vector3& v1);
		static const Vector3 Normalize(const Vector3& v0);
		static const Vector3 Clamp(const Vector3& vec, const Vector3& vMin, const Vector3& vMax);
		static const Vector3 Max(const Vector3& v0, const Vector3& v1);
		static const Vector3 Min(const Vector3& v0, const Vector3& v1);
};