
#include "stdafx.h"
#include "Vector3.h"
#include <math.h>

Vector3::Vector3(void)
{
#ifdef USE_SSE
	m = _mm_setzero_ps();
#else
	this->X = this->Y = this->Z = 0.0f;
#endif
}
Vector3::Vector3(float v)
{
#ifdef USE_SSE
	m = _mm_set_ps1(v);
#else
	this->X = v;
	this->Y = v;
	this->Z = v;
#endif
}
Vector3::Vector3(float x, float y, float z)
{
#ifdef USE_SSE
	m = _mm_set_ps(0.0f, z, y, x);
#else
	this->X = x;
	this->Y = y;
	this->Z = z;
#endif
}
Vector3::Vector3(const Vector2& v, float z)
{
#ifdef USE_SSE
	m = _mm_shuffle_ps(_mm_set_ps(0.0f, z, 0.0f, 0.0f), v.m, _MM_SHUFFLE(3, 2, 1, 0));
#else
	this->X = v.X;
	this->Y = v.Y;
	this->Z = z;
#endif
}

#ifdef USE_SSE
Vector3::Vector3(__m128 m)
{
	this->m = m;
}
#endif

const Vector3 Vector3::operator -() const
{
#ifdef USE_SSE
	return Vector3(_mm_sub_ps(_mm_setzero_ps(), m));
#else
	return Vector3(-X, -Y, -Z);
#endif
}

const Vector3 Vector3::operator +(const Vector3& b) const
{
	return Vector3(*this) += b;
}
const Vector3 Vector3::operator -(const Vector3& b) const
{
	return Vector3(*this) -= b;
}
const Vector3 Vector3::operator *(const Vector3& b) const
{
	return Vector3(*this) *= b;
}
const Vector3 Vector3::operator /(const Vector3& b) const
{
	return Vector3(*this) /= b;
}
const Vector3 Vector3::operator *(const float& b) const
{
	return Vector3(*this) *= b;
}
const Vector3 Vector3::operator /(const float& b) const
{
	return Vector3(*this) /= b;
}

Vector3& Vector3::operator +=(const Vector3& b)
{
#ifdef USE_SSE
	m = _mm_add_ps(m, b.m);
#else
	this->X += b.X;
	this->Y += b.Y;
	this->Z += b.Z;
#endif

	return *this;
}
Vector3& Vector3::operator -=(const Vector3& b)
{
#ifdef USE_SSE
	m = _mm_sub_ps(m, b.m);
#else
	this->X -= b.X;
	this->Y -= b.Y;
	this->Z -= b.Z;
#endif

	return *this;
}
Vector3& Vector3::operator *=(const Vector3& b)
{
#ifdef USE_SSE
	m = _mm_mul_ps(m, b.m);
#else
	this->X *= b.X;
	this->Y *= b.Y;
	this->Z *= b.Z;
#endif

	return *this;
}
Vector3& Vector3::operator /=(const Vector3& b)
{
#ifdef USE_SSE
	m = _mm_div_ps(m, b.m);
#else
	this->X /= b.X;
	this->Y /= b.Y;
	this->Z /= b.Z;
#endif

	return *this;
}
Vector3& Vector3::operator *=(const float& b)
{
#ifdef USE_SSE
	m = _mm_mul_ps(m, _mm_set_ps1(b));
#else
	this->X *= b;
	this->Y *= b;
	this->Z *= b;
#endif

	return *this;
}
Vector3& Vector3::operator /=(const float& b)
{
#ifdef USE_SSE
	m = _mm_div_ps(m, _mm_set_ps1(b));
#else
	this->X /= b;
	this->Y /= b;
	this->Z /= b;
#endif

	return *this;
}

float Vector3::Length(void) const
{
	return sqrt(Vector3::Dot(*this, *this));
}
float Vector3::LengthSquared(void) const
{
	return Vector3::Dot(*this, *this);
}

float Vector3::Dot(const Vector3& v0, const Vector3& v1)
{
#ifdef USE_SSE
	float result;
	_mm_store_ss(&result, _mm_dp_ps(v0.m, v1.m, 0x71));
	return result;
#else
	return v0.X * v1.X +
		v0.Y * v1.Y +
		v0.Z * v1.Z;
#endif
}
const Vector3 Vector3::Cross(const Vector3& v0, const Vector3& v1)
{
#ifdef USE_SSE
	// Source: http://fastcpp.blogspot.com/2011/04/vector-cross-product-using-sse-code.html
	return Vector3(_mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(v0.m, v0.m, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(v1.m, v1.m, _MM_SHUFFLE(3, 1, 0, 2))),
		_mm_mul_ps(_mm_shuffle_ps(v0.m, v0.m, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(v1.m, v1.m, _MM_SHUFFLE(3, 0, 2, 1))))); 
#else
	return Vector3(
		(v0.Y * v1.Z) - (v0.Z * v1.Y),
		(v0.Z * v1.X) - (v0.X * v1.Z),
		(v0.X * v1.Y) - (v0.Y * v1.X));
#endif
}
const Vector3 Vector3::Normalize(const Vector3& v0)
{
#ifdef USE_SSE
	__m128 temp1 = _mm_dp_ps(v0.m, v0.m, 0x7F);
	__m128 temp2 = _mm_sqrt_ps(temp1);
	__m128 result = _mm_div_ps(v0.m, temp2);
	return Vector3(result);
#else
	float length = v0.Length();
	return v0 / length;
#endif
}

const Vector3 Vector3::Clamp(const Vector3& vec, const Vector3& vMin, const Vector3& vMax)
{
#ifdef USE_SSE
	return Vector3(_mm_max_ps(_mm_min_ps(vec.m, vMax.m), vMin.m));
#else
	return Vector3(
		max(min(vec.X, vMax.X), vMin.X),
		max(min(vec.Y, vMax.Y), vMin.Y),
		max(min(vec.Z, vMax.Z), vMin.Z));
#endif
}

const Vector3 Vector3::Max(const Vector3& v0, const Vector3& v1)
{
#ifdef USE_SSE
	return Vector3(_mm_max_ps(v0.m, v1.m));
#else
	return Vector3(
		max(v0.X, v1.X),
		max(v0.Y, v1.Y),
		max(v0.Z, v1.Z));
#endif
}
const Vector3 Vector3::Min(const Vector3& v0, const Vector3& v1)
{
#ifdef USE_SSE
	return Vector3(_mm_min_ps(v0.m, v1.m));
#else
	return Vector3(
		min(v0.X, v1.X),
		min(v0.Y, v1.Y),
		min(v0.Z, v1.Z));
#endif
}