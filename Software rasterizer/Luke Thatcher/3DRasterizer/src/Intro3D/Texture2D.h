
#pragma once

#include <Windows.h>
#include "Vector4.h"

struct Md2Header;

__declspec(align(16)) class Texture2D
{
public:
	DEFINE_NEW_DELETE_ALIGNED

	Texture2D(UINT width, UINT height);
	Texture2D(UINT width, UINT height, const BYTE* textureData, const DWORD* paletteData);
	Texture2D(UINT width, UINT height, const Vector4* pixelData);
	Texture2D(UINT width, UINT height, const DWORD* textureData);
	Texture2D(const Texture2D& tex);
	~Texture2D(void);

	Vector4 Sample(int x, int y) const;
	Vector4 SamplePoint(const Vector2& texCoord) const;
	Vector4 SampleBilinear(const Vector2& texCoord) const;

	static bool FromPCX(const char* filename, const Md2Header* header, Texture2D** ppTex);
	static bool FromBitmap(const char* filename, Texture2D** ppTex);

private:
	UINT width;
	UINT height;
	Vector2 size;
	Vector4* texData;

	void Init(UINT width, UINT height);
};