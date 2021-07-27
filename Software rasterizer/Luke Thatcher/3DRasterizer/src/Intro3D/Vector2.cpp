
#include "stdafx.h"
#include "Vector2.h"
#include <math.h>

Vector2::Vector2(void)
{
#ifdef USE_SSE
	m = _mm_setzero_ps();
#else
	this->X = this->Y = 0.0f;
#endif
}
Vector2::Vector2(float v)
{
#ifdef USE_SSE
	m = _mm_set_ps1(v);
#else
	this->X = v;
	this->Y = v;
#endif
}
Vector2::Vector2(float x, float y)
{
#ifdef USE_SSE
	m = _mm_set_ps(0, 0, y, x);
#else
	this->X = x;
	this->Y = y;
#endif
}

#ifdef USE_SSE
Vector2::Vector2(__m128 m)
{
	this->m = m;
}
#endif

const Vector2 Vector2::operator -() const
{
#ifdef USE_SSE
	return Vector2(_mm_sub_ps(_mm_setzero_ps(), m));
#else
	return Vector2(-X, -Y);
#endif
}

const Vector2 Vector2::operator +(const Vector2& b) const
{
	return Vector2(*this) += b;
}
const Vector2 Vector2::operator -(const Vector2& b) const
{
	return Vector2(*this) -= b;
}
const Vector2 Vector2::operator *(const Vector2& b) const
{
	return Vector2(*this) *= b;
}
const Vector2 Vector2::operator /(const Vector2& b) const
{
	return Vector2(*this) /= b;
}
const Vector2 Vector2::operator *(const float& b) const
{
	return Vector2(*this) *= b;
}
const Vector2 Vector2::operator /(const float& b) const
{
	return Vector2(*this) /= b;
}

Vector2& Vector2::operator +=(const Vector2& b)
{
#ifdef USE_SSE
	m = _mm_add_ps(m, b.m);
#else
	this->X += b.X;
	this->Y += b.Y;
#endif

	return *this;
}
Vector2& Vector2::operator -=(const Vector2& b)
{
#ifdef USE_SSE
	m = _mm_sub_ps(m, b.m);
#else
	this->X -= b.X;
	this->Y -= b.Y;
#endif

	return *this;
}
Vector2& Vector2::operator *=(const Vector2& b)
{
#ifdef USE_SSE
	m = _mm_mul_ps(m, b.m);
#else
	this->X *= b.X;
	this->Y *= b.Y;
#endif

	return *this;
}
Vector2& Vector2::operator /=(const Vector2& b)
{
#ifdef USE_SSE
	m = _mm_div_ps(m, b.m);
#else
	this->X /= b.X;
	this->Y /= b.Y;
#endif

	return *this;
}
Vector2& Vector2::operator *=(const float& b)
{
#ifdef USE_SSE
	m = _mm_mul_ps(m, _mm_set_ps1(b));
#else
	this->X *= b;
	this->Y *= b;
#endif

	return *this;
}
Vector2& Vector2::operator /=(const float& b)
{
#ifdef USE_SSE
	m = _mm_div_ps(m, _mm_set_ps1(b));
#else
	this->X /= b;
	this->Y /= b;
#endif

	return *this;
}

float Vector2::Length(void) const
{
	return sqrt(Vector2::Dot(*this, *this));
}
float Vector2::LengthSquared(void) const
{
	return Vector2::Dot(*this, *this);
}

float Vector2::Dot(const Vector2& v0, const Vector2& v1)
{
#ifdef USE_SSE
	float result;
	_mm_store_ss(&result, _mm_dp_ps(v0.m, v1.m, 0x31));
	return result;
#else
	return v0.X * v1.X +
		v0.Y * v1.Y;
#endif
}
const Vector2 Vector2::Normalize(const Vector2& v0)
{
#ifdef USE_SSE
	__m128 temp1 = _mm_dp_ps(v0.m, v0.m, 0x33);
	__m128 temp2 = _mm_sqrt_ps(temp1);
	__m128 result = _mm_div_ps(v0.m, temp2);
	return Vector2(result);
#else
	float length = v0.Length();
	return v0 / length;
#endif
}

const Vector2 Vector2::Clamp(const Vector2& vec, const Vector2& vMin, const Vector2& vMax)
{
#ifdef USE_SSE
	__m128 temp = _mm_max_ps(vec.m, vMin.m);
	__m128 result = _mm_min_ps(temp, vMax.m);
	return Vector2(result);
#else
	return Vector2(
		max(min(vec.X, vMax.X), vMin.X),
		max(min(vec.Y, vMax.Y), vMin.Y));
#endif
}

const Vector2 Vector2::Max(const Vector2& v0, const Vector2& v1)
{
#ifdef USE_SSE
	return Vector2(_mm_max_ps(v0.m, v1.m));
#else
	return Vector2(
		max(v0.X, v1.X),
		max(v0.Y, v1.Y));
#endif
}
const Vector2 Vector2::Min(const Vector2& v0, const Vector2& v1)
{
#ifdef USE_SSE
	return Vector2(_mm_min_ps(v0.m, v1.m));
#else
	return Vector2(
		min(v0.X, v1.X),
		min(v0.Y, v1.Y));
#endif
}