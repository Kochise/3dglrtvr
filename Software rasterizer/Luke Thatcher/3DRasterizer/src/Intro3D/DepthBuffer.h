
#pragma once

#include <Windows.h>
#include "Vector2.h"

__declspec(align(16)) class DepthBuffer
{
public:
	DEFINE_NEW_DELETE_ALIGNED

	DepthBuffer(UINT width, UINT height);
	DepthBuffer(const DepthBuffer& depthBuffer);
	~DepthBuffer(void);

	UINT GetWidth(void) const;
	UINT GetHeight(void) const;

	void Clear(float value);

	float GetValue(UINT pixelIndex) const;
	void SetValue(UINT pixelIndex, float value);

	float SamplePoint(const Vector2& texCoord) const;

private:
	UINT width;
	UINT height;
	UINT size;
	Vector2 dims;
	float* data;
};