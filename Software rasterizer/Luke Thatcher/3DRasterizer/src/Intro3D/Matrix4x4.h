
#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include <intrin.h>

__declspec(align(16)) class Matrix4x4
{
public:
	DEFINE_NEW_DELETE_ALIGNED
#ifdef USE_SSE
	union
	{
		float _m[4][4];
		struct
		{
			__m128 col0;
			__m128 col1;
			__m128 col2;
			__m128 col3;
		};
	};
#else
	float _m[4][4];
#endif

	Matrix4x4(void);
	Matrix4x4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

#ifdef USE_SSE
	Matrix4x4(
		__m128* col0,
		__m128* col1,
		__m128* col2,
		__m128* col3);
#endif

	Matrix4x4& operator *=(const Matrix4x4& b);
	const Matrix4x4 operator *(const Matrix4x4& b) const;
	const Vector3 operator *(const Vector3& v) const;
	const Vector4 operator *(const Vector4& b) const;

	const Vector3 TransformNormal(const Vector3& normal) const;

	static Matrix4x4 CreateTranslation(const Vector3& v);
	static Matrix4x4 CreateTranslation(float x, float y, float z);
	static Matrix4x4 CreateRotationX(float angle);
	static Matrix4x4 CreateRotationY(float angle);
	static Matrix4x4 CreateRotationZ(float angle);
	static Matrix4x4 CreateScale(const Vector3& scale);
	static Matrix4x4 CreateScale(float scale);
	static Matrix4x4 CreateLookAt(const Vector3& camPos, const Vector3& camTarget, const Vector3& camUp);
	static Matrix4x4 CreatePerspectiveFieldOfView(float fov, float aspect, float nearPlane, float farPlane);

	static Vector3 GetRotationFromMatrix(const Matrix4x4& mat);
};