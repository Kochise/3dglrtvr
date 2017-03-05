
#include "stdafx.h"
#include "Matrix4x4.h"

#define _USE_MATH_DEFINES
#include <math.h>

// Constructors

Matrix4x4::Matrix4x4(void)
{
#ifdef USE_SSE
	// Initialize to identity matrix
	col0 = _mm_set_ps(0, 0, 0, 1);
	col1 = _mm_set_ps(0, 0, 1, 0);
	col2 = _mm_set_ps(0, 1, 0, 0);
	col3 = _mm_set_ps(1, 0, 0, 0);
#else
	// Initialize to identity matrix
	_m[0][0] = _m[1][1] = _m[2][2] = _m[3][3] = 1.0f;
	
	_m[0][1] = _m[0][2] = _m[0][3] = 0.0f;
	_m[1][0] = _m[1][2] = _m[1][3] = 0.0f;
	_m[2][0] = _m[2][1] = _m[2][3] = 0.0f;
	_m[3][0] = _m[3][1] = _m[3][2] = 0.0f;
#endif
}

Matrix4x4::Matrix4x4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
{
#ifdef USE_SSE
	col0 = _mm_set_ps(m30, m20, m10, m00);
	col1 = _mm_set_ps(m31, m21, m11, m01);
	col2 = _mm_set_ps(m32, m22, m12, m02);
	col3 = _mm_set_ps(m33, m23, m13, m03);
#else
	_m[0][0] = m00; _m[0][1] = m01; _m[0][2] = m02; _m[0][3] = m03;
	_m[1][0] = m10; _m[1][1] = m11; _m[1][2] = m12; _m[1][3] = m13;
	_m[2][0] = m20; _m[2][1] = m21; _m[2][2] = m22; _m[2][3] = m23;
	_m[3][0] = m30; _m[3][1] = m31; _m[3][2] = m32; _m[3][3] = m33;
#endif
}

#ifdef USE_SSE
Matrix4x4::Matrix4x4(
		__m128* col0,
		__m128* col1,
		__m128* col2,
		__m128* col3)
{
	this->col0 = *col0;
	this->col1 = *col1;
	this->col2 = *col2;
	this->col3 = *col3;
}
#endif

// Operators
Matrix4x4& Matrix4x4::operator *=(const Matrix4x4& b)
{
	*this = *this * b;
	return *this;
}

const Matrix4x4 Matrix4x4::operator *(const Matrix4x4& b) const
{
#ifdef USE_SSE

	__m128 tmp3, tmp2, tmp1, tmp0, row0, row1, row2, row3;

	// Tranpose this matrix
	tmp0 = _mm_shuffle_ps(col0, col1, _MM_SHUFFLE(1,0,1,0));
	tmp2 = _mm_shuffle_ps(col0, col1, _MM_SHUFFLE(3,2,3,2));
	tmp1 = _mm_shuffle_ps(col2, col3, _MM_SHUFFLE(1,0,1,0));
	tmp3 = _mm_shuffle_ps(col2, col3, _MM_SHUFFLE(3,2,3,2));
	row0 = _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(2,0,2,0));
	row1 = _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(3,1,3,1));
	row2 = _mm_shuffle_ps(tmp2, tmp3, _MM_SHUFFLE(2,0,2,0));
	row3 = _mm_shuffle_ps(tmp2, tmp3, _MM_SHUFFLE(3,1,3,1));

	__m128 m00 = _mm_dp_ps(row3, b.col0, 0xF8);
	__m128 m01 = _mm_dp_ps(row3, b.col1, 0xF8);
	__m128 m02 = _mm_dp_ps(row3, b.col2, 0xF8);
	__m128 m03 = _mm_dp_ps(row3, b.col3, 0xF8);

	__m128 m10 = _mm_dp_ps(row2, b.col0, 0xF4);
	__m128 m11 = _mm_dp_ps(row2, b.col1, 0xF4);
	__m128 m12 = _mm_dp_ps(row2, b.col2, 0xF4);
	__m128 m13 = _mm_dp_ps(row2, b.col3, 0xF4);

	__m128 m20 = _mm_dp_ps(row1, b.col0, 0xF2);
	__m128 m21 = _mm_dp_ps(row1, b.col1, 0xF2);
	__m128 m22 = _mm_dp_ps(row1, b.col2, 0xF2);
	__m128 m23 = _mm_dp_ps(row1, b.col3, 0xF2);

	__m128 m30 = _mm_dp_ps(row0, b.col0, 0xF1);
	__m128 m31 = _mm_dp_ps(row0, b.col1, 0xF1);
	__m128 m32 = _mm_dp_ps(row0, b.col2, 0xF1);
	__m128 m33 = _mm_dp_ps(row0, b.col3, 0xF1);

	__m128 finalCol0 = _mm_add_ps(_mm_add_ps(_mm_add_ps(m00, m10), m20), m30);
	__m128 finalCol1 = _mm_add_ps(_mm_add_ps(_mm_add_ps(m01, m11), m21), m31);
	__m128 finalCol2 = _mm_add_ps(_mm_add_ps(_mm_add_ps(m02, m12), m22), m32);
	__m128 finalCol3 = _mm_add_ps(_mm_add_ps(_mm_add_ps(m03, m13), m23), m33);

	return Matrix4x4(&finalCol0, &finalCol1, &finalCol2, &finalCol3);
#else
	return Matrix4x4(
    (((_m[0][0] * b._m[0][0]) + (_m[0][1] * b._m[1][0])) + (_m[0][2] * b._m[2][0])) + (_m[0][3] * b._m[3][0]),
    (((_m[0][0] * b._m[0][1]) + (_m[0][1] * b._m[1][1])) + (_m[0][2] * b._m[2][1])) + (_m[0][3] * b._m[3][1]),
    (((_m[0][0] * b._m[0][2]) + (_m[0][1] * b._m[1][2])) + (_m[0][2] * b._m[2][2])) + (_m[0][3] * b._m[3][2]),
    (((_m[0][0] * b._m[0][3]) + (_m[0][1] * b._m[1][3])) + (_m[0][2] * b._m[2][3])) + (_m[0][3] * b._m[3][3]),

    (((_m[1][0] * b._m[0][0]) + (_m[1][1] * b._m[1][0])) + (_m[1][2] * b._m[2][0])) + (_m[1][3] * b._m[3][0]),
    (((_m[1][0] * b._m[0][1]) + (_m[1][1] * b._m[1][1])) + (_m[1][2] * b._m[2][1])) + (_m[1][3] * b._m[3][1]),
    (((_m[1][0] * b._m[0][2]) + (_m[1][1] * b._m[1][2])) + (_m[1][2] * b._m[2][2])) + (_m[1][3] * b._m[3][2]),
    (((_m[1][0] * b._m[0][3]) + (_m[1][1] * b._m[1][3])) + (_m[1][2] * b._m[2][3])) + (_m[1][3] * b._m[3][3]),

    (((_m[2][0] * b._m[0][0]) + (_m[2][1] * b._m[1][0])) + (_m[2][2] * b._m[2][0])) + (_m[2][3] * b._m[3][0]),
    (((_m[2][0] * b._m[0][1]) + (_m[2][1] * b._m[1][1])) + (_m[2][2] * b._m[2][1])) + (_m[2][3] * b._m[3][1]),
    (((_m[2][0] * b._m[0][2]) + (_m[2][1] * b._m[1][2])) + (_m[2][2] * b._m[2][2])) + (_m[2][3] * b._m[3][2]),
    (((_m[2][0] * b._m[0][3]) + (_m[2][1] * b._m[1][3])) + (_m[2][2] * b._m[2][3])) + (_m[2][3] * b._m[3][3]),

    (((_m[3][0] * b._m[0][0]) + (_m[3][1] * b._m[1][0])) + (_m[3][2] * b._m[2][0])) + (_m[3][3] * b._m[3][0]),
    (((_m[3][0] * b._m[0][1]) + (_m[3][1] * b._m[1][1])) + (_m[3][2] * b._m[2][1])) + (_m[3][3] * b._m[3][1]),
    (((_m[3][0] * b._m[0][2]) + (_m[3][1] * b._m[1][2])) + (_m[3][2] * b._m[2][2])) + (_m[3][3] * b._m[3][2]),
    (((_m[3][0] * b._m[0][3]) + (_m[3][1] * b._m[1][3])) + (_m[3][2] * b._m[2][3])) + (_m[3][3] * b._m[3][3]));
#endif
}

const Vector4 Matrix4x4::operator*(const Vector4& v) const
{
#ifdef USE_SSE
	__m128 line0 = _mm_dp_ps(v.m, col0, 0xF1);
	__m128 line1 = _mm_dp_ps(v.m, col1, 0xF2);
	__m128 line2 = _mm_dp_ps(v.m, col2, 0xF4);
	__m128 line3 = _mm_dp_ps(v.m, col3, 0xF8);

	__m128 result = _mm_add_ps(line2, line3);
	result = _mm_add_ps(result, line1);
	result = _mm_add_ps(result, line0);

	return Vector4(result);

#else
	return Vector4(
		(((v.X * _m[0][0]) + (v.Y * _m[1][0])) + (v.Z * _m[2][0])) + (v.W * _m[3][0]),
		(((v.X * _m[0][1]) + (v.Y * _m[1][1])) + (v.Z * _m[2][1])) + (v.W * _m[3][1]),
		(((v.X * _m[0][2]) + (v.Y * _m[1][2])) + (v.Z * _m[2][2])) + (v.W * _m[3][2]),
		(((v.X * _m[0][3]) + (v.Y * _m[1][3])) + (v.Z * _m[2][3])) + (v.W * _m[3][3]));
#endif
}

const Vector3 Matrix4x4::operator*(const Vector3& v) const
{
#ifdef USE_SSE
	// Build a vector from v with W = 1
	__m128 temp = _mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(0, 1, 2, 3));
	temp = _mm_move_ss(temp, _mm_set_ss(1.0f));
	temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(0, 1, 2, 3));

	// Do Vector4 transformation
	__m128 line0 = _mm_dp_ps(temp, col0, 0xF1);
	__m128 line1 = _mm_dp_ps(temp, col1, 0xF2);
	__m128 line2 = _mm_dp_ps(temp, col2, 0xF4);
	__m128 line3 = _mm_dp_ps(temp, col3, 0xF8);

	// Sum result
	__m128 result = _mm_add_ps(_mm_add_ps(_mm_add_ps(line3, line2), line1), line0);

	// Drop the W component from the result
	__m128 final = _mm_shuffle_ps(result, result, _MM_SHUFFLE(0, 1, 2, 3));
	final = _mm_move_ss(final, _mm_setzero_ps());
	final = _mm_shuffle_ps(final, final, _MM_SHUFFLE(0, 1, 2, 3));

	return Vector3(final);
#else
	return Vector3(
		(((v.X * _m[0][0]) + (v.Y * _m[1][0])) + (v.Z * _m[2][0])) + _m[3][0],
		(((v.X * _m[0][1]) + (v.Y * _m[1][1])) + (v.Z * _m[2][1])) + _m[3][1],
		(((v.X * _m[0][2]) + (v.Y * _m[1][2])) + (v.Z * _m[2][2])) + _m[3][2]);
#endif
}

const Vector3 Matrix4x4::TransformNormal(const Vector3& normal) const
{
#ifdef USE_SSE
	__m128 line0 = _mm_dp_ps(normal.m, col0, 0x71);
	__m128 line1 = _mm_dp_ps(normal.m, col1, 0x72);
	__m128 line2 = _mm_dp_ps(normal.m, col2, 0x74);

	return Vector3(_mm_add_ps(_mm_add_ps(line2, line1), line0));
#else
    return Vector3(
		((normal.X * _m[0][0]) + (normal.Y * _m[1][0])) + (normal.Z * _m[2][0]),
		((normal.X * _m[0][1]) + (normal.Y * _m[1][1])) + (normal.Z * _m[2][1]),
		((normal.X * _m[0][2]) + (normal.Y * _m[1][2])) + (normal.Z * _m[2][2]));
#endif
}


Matrix4x4 Matrix4x4::CreateTranslation(const Vector3& v)
{
	return CreateTranslation(v.X, v.Y, v.Z);
}
Matrix4x4 Matrix4x4::CreateTranslation(float x, float y, float z)
{
	return Matrix4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1);
}

Matrix4x4 Matrix4x4::CreateRotationX(float angle)
{
	float s = sin(angle);
	float c = cos(angle);

	return Matrix4x4(
		1, 0, 0, 0,
		0, c, s, 0,
		0, -s, c, 0,
		0, 0, 0, 1);
}
Matrix4x4 Matrix4x4::CreateRotationY(float angle)
{
	float c = cos(angle);
	float s = sin(angle);

	return Matrix4x4(
		c, 0, -s, 0,
		0, 1, 0, 0,
		s, 0, c, 0,
		0, 0, 0, 1);
}
Matrix4x4 Matrix4x4::CreateRotationZ(float angle)
{
	float c = cos(angle);
	float s = sin(angle);

	return Matrix4x4(
		c, s, 0, 0,
		-s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::CreateScale(const Vector3& scale)
{
	return Matrix4x4(
		scale.X, 0, 0, 0,
		0, scale.Y, 0, 0,
		0, 0, scale.Z, 0,
		0, 0, 0, 1);
}
Matrix4x4 Matrix4x4::CreateScale(float scale)
{
	return Matrix4x4(
		scale, 0, 0, 0,
		0, scale, 0, 0,
		0, 0, scale, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::CreateLookAt(const Vector3& camPos, const Vector3& camTarget, const Vector3& camUp)
{
    Vector3 v0 = Vector3::Normalize(camPos - camTarget);
    Vector3 v1 = Vector3::Normalize(Vector3::Cross(camUp, v0));
    Vector3 v2 = Vector3::Cross(v0, v1);

	return Matrix4x4(
		 v1.X, v2.X, v0.X, 0.0f,
		 v1.Y, v2.Y, v0.Y, 0.0f,
		 v1.Z, v2.Z, v0.Z, 0.0f,

		 -Vector3::Dot(v1, camPos),
		 -Vector3::Dot(v2, camPos),
		 -Vector3::Dot(v0, camPos),
		 1.0f);
}

Matrix4x4 Matrix4x4::CreatePerspectiveFieldOfView(float fov, float aspect, float nearPlane, float farPlane)
{
	float m11 = 1.0f / ((float)tan((double)(fov * 0.5f)));
    float m00 = m11 / aspect;
	float m22 = farPlane / (nearPlane - farPlane);
	float m32 = (nearPlane * farPlane) / (nearPlane - farPlane);

    return Matrix4x4(
		m00, 0, 0, 0,
		0, m11, 0, 0,
		0, 0, m22, -1.0f,
		0, 0, m32, 0);
}

Vector3 Matrix4x4::GetRotationFromMatrix(const Matrix4x4& mat)
{
	// Reference: http://www.gregslabaugh.name/publications/euler.pdf
	float theta, psi, phi;

#ifdef USE_SSE
	if (mat._m[0][2] != 1.0f && mat._m[0][2] != -1.0f)
	{
		float theta1 = -asin(mat._m[0][2]);
		float theta2 = (float)M_PI - theta1;

		float cosTheta1 = cos(theta1);
		float cosTheta2 = cos(theta2);

		float psi1 = atan2(mat._m[1][2] / cosTheta1, mat._m[2][2] / cosTheta1);
		float psi2 = atan2(mat._m[1][2] / cosTheta2, mat._m[2][2] / cosTheta2);
		
		float phi1 = atan2(mat._m[0][1] / cosTheta1, mat._m[0][0] / cosTheta1);
		float phi2 = atan2(mat._m[0][1] / cosTheta2, mat._m[0][0] / cosTheta2);

		if (phi1 < M_PI)
		{
			theta = theta1;
			psi = psi1;
			phi = phi1;
		}
		else
		{
			theta = theta2;
			psi = psi2;
			phi = phi2;
		}
	}
	else
	{
		phi = 0.0f;
		if (mat._m[0][2] == -1.0f)
		{
			theta = ((float)M_PI) / 2.0f;
			psi = atan2(mat._m[1][0], mat._m[2][0]);
		}
		else
		{
			theta = -((float)M_PI) / 2.0f;
			psi = atan2(-mat._m[1][0], -mat._m[2][0]);
		}
	}
#else
	if (mat._m[2][0] != 1.0f && mat._m[2][0] != -1.0f)
	{
		float theta1 = -asin(mat._m[2][0]);
		float theta2 = (float)M_PI - theta1;

		float cosTheta1 = cos(theta1);
		float cosTheta2 = cos(theta2);

		float psi1 = atan2(mat._m[2][1] / cosTheta1, mat._m[2][2] / cosTheta1);
		float psi2 = atan2(mat._m[2][1] / cosTheta2, mat._m[2][2] / cosTheta2);
		
		float phi1 = atan2(mat._m[1][0] / cosTheta1, mat._m[0][0] / cosTheta1);
		float phi2 = atan2(mat._m[1][0] / cosTheta2, mat._m[0][0] / cosTheta2);

		if (phi1 < M_PI)
		{
			theta = theta1;
			psi = psi1;
			phi = phi1;
		}
		else
		{
			theta = theta2;
			psi = psi2;
			phi = phi2;
		}
	}
	else
	{
		phi = 0.0f;
		if (mat._m[2][0] == -1.0f)
		{
			theta = ((float)M_PI) / 2.0f;
			psi = atan2(mat._m[0][1], mat._m[0][2]);
		}
		else
		{
			theta = -((float)M_PI) / 2.0f;
			psi = atan2(-mat._m[0][1], -mat._m[0][2]);
		}
	}
#endif

	return Vector3(psi, theta, phi);
}