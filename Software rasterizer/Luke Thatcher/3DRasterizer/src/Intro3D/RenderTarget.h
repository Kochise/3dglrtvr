
#pragma once

#include <Windows.h>

class RenderTarget
{
public:
	RenderTarget(UINT width, UINT height);
	RenderTarget(const RenderTarget& renderTarget);
	~RenderTarget(void);

	void Clear(DWORD color);

	UINT GetWidth(void) const;
	UINT GetHeight(void) const;
	HBITMAP GetHBitmap(void) const;

	DWORD GetPixel(UINT pixelIndex) const;
	void SetPixel(UINT pixelIndex, DWORD color);

private:
	UINT width;
	UINT height;
	UINT size;
	DWORD* pBitmapBits;
	HBITMAP hBitmap;

	void CreateHBitmap(void);
};