
#pragma once

// Class representing a 2-dimensional vector
__declspec(align(16)) class Vector2
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
			};
		};
#else
		float X;
		float Y;
#endif

		Vector2(void);
		Vector2(float v);
		Vector2(float x, float y);

#ifdef USE_SSE
		Vector2(__m128 m);
#endif

		const Vector2 operator -() const;

		const Vector2 operator +(const Vector2& b) const;
		const Vector2 operator -(const Vector2& b) const;
		const Vector2 operator *(const Vector2& b) const;
		const Vector2 operator /(const Vector2& b) const;
		const Vector2 operator *(const float& b) const;
		const Vector2 operator /(const float& b) const;

		Vector2& operator +=(const Vector2& b);
		Vector2& operator -=(const Vector2& b);
		Vector2& operator *=(const Vector2& b);
		Vector2& operator /=(const Vector2& b);
		Vector2& operator *=(const float& b);
		Vector2& operator /=(const float& b);

		float Length(void) const;
		float LengthSquared(void) const;

		// Public Static Helper Methods
		static float Dot(const Vector2& v0, const Vector2& v1);
		static const Vector2 Normalize(const Vector2& v0);
		static const Vector2 Clamp(const Vector2& vec, const Vector2& vMin, const Vector2& vMax);
		static const Vector2 Max(const Vector2& v0, const Vector2& v1);
		static const Vector2 Min(const Vector2& v0, const Vector2& v1);
};