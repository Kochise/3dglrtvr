
#pragma once

#include "Vector3.h"

__declspec(align(16)) class Face
{
public:
	DEFINE_NEW_DELETE_ALIGNED

	Face(void);
	Face(const int indices[3], const Vector3& faceNormal);
	Face(int i0, int i1, int i2, const Vector3& faceNormal);

	inline Vector3 GetFaceNormal(void) const
	{
		return faceNormal;
	}
	inline const int* GetIndices(void) const
	{
		return indices;
	}

	inline void SetFaceNormal(const Vector3& normal)
	{
		this->faceNormal = normal;
	}
	inline void SetIndex(int i, int value)
	{
		this->indices[i] = value;
	}

private:
	Vector3 faceNormal;
	int indices[3];
};