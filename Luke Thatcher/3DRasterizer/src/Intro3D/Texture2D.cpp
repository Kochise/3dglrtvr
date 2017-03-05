
#include "stdafx.h"
#include "Texture2D.h"
#include "MD2Loader.h"
#include <math.h>

#include <cstring>
#include <GdiPlus.h>

using namespace Gdiplus;

Texture2D::Texture2D(UINT width, UINT height)
{
	Init(width, height);
}
Texture2D::Texture2D(UINT width, UINT height, const BYTE* textureData, const DWORD* paletteData)
{
	Init(width, height);

	// Initialize pixel data
	for (UINT y = 0, i = 0; y < height; y++)
	{
		for (UINT x = 0; x < width; x++, i++)
		{
			BYTE palIndex = textureData[i];

			this->texData[i] = (Vector4(
				(float)(paletteData[palIndex] & 0x000000FF),
				(float)((paletteData[palIndex] & 0x0000FF00) >> 8),
				(float)((paletteData[palIndex] & 0x00FF0000) >> 16),
				(float)((paletteData[palIndex] & 0xFF000000) >> 24))) / 255.0f;
		}
	}
}
Texture2D::Texture2D(UINT width, UINT height, const Vector4* pixelData)
{
	Init(width, height);
	memcpy(this->texData, pixelData, sizeof(Vector4) * width * height);
}
Texture2D::Texture2D(UINT width, UINT height, const DWORD* textureData)
{
	Init(width, height);

	// Initialize pixel data
	for (UINT y = 0, i = 0; y < height; y++)
	{
		for (UINT x = 0; x < width; x++, i++)
		{
			this->texData[i] = (Vector4(
				(float)((textureData[i] & 0x00FF0000) >> 16),
				(float)((textureData[i] & 0x0000FF00) >> 8),
				(float)(textureData[i] & 0x000000FF),
				(float)((textureData[i] & 0xFF000000) >> 24))) / 255.0f;
		}
	}
}
Texture2D::Texture2D(const Texture2D& tex)
{
	Init(tex.width, tex.height);

	// Copy data from other texture
	memcpy(texData, tex.texData, sizeof(Vector4) * width * height);
}
Texture2D::~Texture2D(void)
{
	delete [] this->texData;
}

void Texture2D::Init(UINT width, UINT height)
{
	this->width = width;
	this->height = height;
	this->texData = new Vector4[width * height];
	this->size = Vector2((float)(this->width - 1), (float)(this->height - 1));
}

bool Texture2D::FromPCX(const char* filename, const Md2Header* header, Texture2D** ppTex)
{
	BYTE* texMem = new BYTE[header->skinWidth * header->skinHeight];
	DWORD palette[256];

	bool result;
	if ((result = MD2Loader::LoadPCX(filename, texMem, palette, header)))
	{
		*ppTex = new Texture2D(header->skinWidth, header->skinHeight, texMem, palette);
	}
	delete [] texMem;

	return result;
}
bool Texture2D::FromBitmap(const char* filename, Texture2D** ppTex)
{
	if (!ppTex)
		return false;
	
	// Load the bitmap file
	wchar_t buffer[MAX_PATH];
	size_t numChars = 0;
	mbstowcs_s<MAX_PATH>(&numChars, buffer, filename, MAX_PATH);
	Bitmap* pBitmap = Bitmap::FromFile(buffer);

	if (!pBitmap)
		return false;

	bool success = false;

	Rect lockRect = Rect(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
	BitmapData data;
	if (pBitmap->LockBits(&lockRect, ImageLockModeRead, PixelFormat32bppARGB, &data) == Ok)
	{
		*ppTex = new Texture2D(data.Width, data.Height, (DWORD*)data.Scan0);
		success = true;

		pBitmap->UnlockBits(&data);
	}

	delete pBitmap;

	return success;
}

	
Vector4 Texture2D::SampleBilinear(const Vector2& tex) const
{
	// Reference: http://en.wikipedia.org/wiki/Bilinear_filtering

	float xt = fmod(tex.X, 1.0f);
	float yt = fmod(tex.Y, 1.0f);

	Vector2 texCoord = Vector2::Clamp(Vector2(xt, yt) * size, Vector2(0, 0), size);
	int x = (int)floor(texCoord.X);
	int y = (int)floor(texCoord.Y);
	float u_ratio = texCoord.X - x;
	float v_ratio = texCoord.Y - y;
	float u_opposite = 1 - u_ratio;
	float v_opposite = 1 - v_ratio;
	Vector4 result =
		(Sample(x, y)   * u_opposite  + Sample(x + 1, y)   * u_ratio) * v_opposite + 
        (Sample(x, y + 1) * u_opposite  + Sample(x + 1, y + 1) * u_ratio) * v_ratio;

	return result;
}
Vector4 Texture2D::SamplePoint(const Vector2& texCoord) const
{
	float x = fmod(texCoord.X, 1.0f);
	float y = fmod(texCoord.Y, 1.0f);
	Vector2 pixelPos = (size * Vector2(x,y)) + Vector2(0.5f, 0.5f);
	pixelPos = Vector2::Clamp(pixelPos, Vector2(0, 0), size);
	return this->texData[(((int)floor(pixelPos.Y)) * this->width) + ((int)floor(pixelPos.X))];
}
Vector4 Texture2D::Sample(int x, int y) const
{
	x = max(min(x, ((INT)width) - 1), 0);
	y = max(min(y, ((INT)height) - 1), 0);

	return this->texData[y * width + x];
}