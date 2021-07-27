
#pragma once

#include "Vector2.h"
#include "Vector3.h"

__declspec(align(16)) class Vertex
{
public:
	DEFINE_NEW_DELETE_ALIGNED

	Vector3 Position;
	Vector3 Normal;
	Vector2 TexCoord;

	Vertex(void);
	Vertex(const Vector3& Position, const Vector3& Normal, const Vector2& TexCoord);
};