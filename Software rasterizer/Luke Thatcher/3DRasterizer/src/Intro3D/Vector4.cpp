
#include "stdafx.h"
#include "Vector4.h"
#include <math.h>

Vector4::Vector4(void)
{
#ifdef USE_SSE
	m = _mm_set_ps1(0);
#else
	this->X = this->Y = this->Z = this->W = 0.0f;
#endif
}
Vector4::Vector4(float v)
{
#ifdef USE_SSE
	m = _mm_set_ps1(v);
#else
	this->X = v;
	this->Y = v;
	this->Z = v;
	this->W = v;
#endif
}

#ifdef USE_SSE
Vector4::Vector4(__m128 m)
{
	this->m = m;
}
#endif

Vector4::Vector4(float x, float y, float z, float w)
{
#ifdef USE_SSE
	m = _mm_set_ps(w, z, y, x);
#else
	this->X = x;
	this->Y = y;
	this->Z = z;
	this->W = w;
#endif
}
Vector4::Vector4(const Vector3& v, float w)
{
#ifdef USE_SSE
	m = _mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(0, 1, 2, 3));
	m = _mm_move_ss(m, _mm_set_ss(w));
	m = _mm_shuffle_ps(m, m, _MM_SHUFFLE(0, 1, 2, 3));
#else
	this->X = v.X;
	this->Y = v.Y;
	this->Z = v.Z;
	this->W = w;
#endif
}
Vector4::Vector4(const Vector2& v, float z, float w)
{
#ifdef USE_SSE
	m = _mm_shuffle_ps(v.m, _mm_set_ps(w, z, 0.0f, 0.0f), _MM_SHUFFLE(3, 2, 1, 0));
#else
	this->X = v.X;
	this->Y = v.Y;
	this->Z = z;
	this->W = w;
#endif
}

const Vector4 Vector4::operator -() const
{
#ifdef USE_SSE
	__m128 zero = _mm_setzero_ps();
	return Vector4(_mm_sub_ps(zero, this->m));
#else
	return Vector4(-X, -Y, -Z, -W);
#endif
}

const Vector4 Vector4::operator +(const Vector4& b) const
{
	return Vector4(*this) += b;
}
const Vector4 Vector4::operator -(const Vector4& b) const
{
	return Vector4(*this) -= b;
}
const Vector4 Vector4::operator *(const Vector4& b) const
{
	return Vector4(*this) *= b;
}
const Vector4 Vector4::operator /(const Vector4& b) const
{
	return Vector4(*this) /= b;
}
const Vector4 Vector4::operator *(const float& b) const
{
	return Vector4(*this) *= b;
}
const Vector4 Vector4::operator /(const float& b) const
{
	return Vector4(*this) /= b;
}

Vector4& Vector4::operator +=(const Vector4& b)
{
#ifdef USE_SSE
	m = _mm_add_ps(m, b.m);
#else
	this->X += b.X;
	this->Y += b.Y;
	this->Z += b.Z;
	this->W += b.W;
#endif

	return *this;
}
Vector4& Vector4::operator -=(const Vector4& b)
{
#ifdef USE_SSE
	m = _mm_sub_ps(m, b.m);
#else
	this->X -= b.X;
	this->Y -= b.Y;
	this->Z -= b.Z;
	this->W -= b.W;
#endif

	return *this;
}
Vector4& Vector4::operator *=(const Vector4& b)
{
#ifdef USE_SSE
	m = _mm_mul_ps(m, b.m);
#else
	this->X *= b.X;
	this->Y *= b.Y;
	this->Z *= b.Z;
	this->W *= b.W;
#endif

	return *this;
}
Vector4& Vector4::operator /=(const Vector4& b)
{
#ifdef USE_SSE
	m = _mm_div_ps(m, b.m);
#else
	this->X /= b.X;
	this->Y /= b.Y;
	this->Z /= b.Z;
	this->W /= b.W;
#endif

	return *this;
}
Vector4& Vector4::operator *=(const float& b)
{
#ifdef USE_SSE
	m = _mm_mul_ps(m, _mm_set_ps1(b));
#else
	this->X *= b;
	this->Y *= b;
	this->Z *= b;
	this->W *= b;
#endif

	return *this;
}
Vector4& Vector4::operator /=(const float& b)
{
#ifdef USE_SSE
	m = _mm_div_ps(m, _mm_set_ps1(b));
#else
	this->X /= b;
	this->Y /= b;
	this->Z /= b;
	this->W /= b;
#endif

	return *this;
}

float Vector4::Length(void) const
{
	return sqrt(Vector4::Dot(*this, *this));
}
float Vector4::LengthSquared(void) const
{
	return Vector4::Dot(*this, *this);
}
bool Vector4::IsInView(void) const
{
	return (-W < X && X < W &&
		-W < Y && Y < W &&
		0 < Z && Z < W);
}

float Vector4::Dot(const Vector4& v0, const Vector4& v1)
{
#ifdef USE_SSE
	float result;
	_mm_store_ss(&result, _mm_dp_ps(v0.m, v1.m, 0xF1));
	return result;
#else
	return v0.X * v1.X +
		v0.Y * v1.Y +
		v0.Z * v1.Z +
		v0.W * v1.W;
#endif
}
const Vector4 Vector4::Normalize(const Vector4& v0)
{
#ifdef USE_SSE
	__m128 temp1 = _mm_dp_ps(v0.m, v0.m, 0xFF);
	__m128 temp2 = _mm_sqrt_ps(temp1);
	__m128 result = _mm_div_ps(v0.m, temp2);
	return Vector4(result);
#else
	float length = v0.Length();
	return v0 / length;
#endif
}

const Vector4 Vector4::Clamp(const Vector4& vec, const Vector4& vMin, const Vector4& vMax)
{
#ifdef USE_SSE
	__m128 temp = _mm_max_ps(vec.m, vMin.m);
	__m128 result = _mm_min_ps(temp, vMax.m);
	return Vector4(result);
#else
	return Vector4(
		max(min(vec.X, vMax.X), vMin.X),
		max(min(vec.Y, vMax.Y), vMin.Y),
		max(min(vec.Z, vMax.Z), vMin.Z),
		max(min(vec.W, vMax.W), vMin.W));
#endif
}

const Vector4 Vector4::Max(const Vector4& v0, const Vector4& v1)
{
#ifdef USE_SSE
	return Vector4(_mm_max_ps(v0.m, v1.m));
#else
	return Vector4(
		max(v0.X, v1.X),
		max(v0.Y, v1.Y),
		max(v0.Z, v1.Z),
		max(v0.W, v1.W));
#endif
}
const Vector4 Vector4::Min(const Vector4& v0, const Vector4& v1)
{
#ifdef USE_SSE
	return Vector4(_mm_min_ps(v0.m, v1.m));
#else
	return Vector4(
		min(v0.X, v1.X),
		min(v0.Y, v1.Y),
		min(v0.Z, v1.Z),
		min(v0.W, v1.W));
#endif
}

Vector3& Vector4::AsVector3(void) const
{
	return *((Vector3*)this);
}
Vector2& Vector4::AsVector2(void) const
{
	return *((Vector2*)this);
}