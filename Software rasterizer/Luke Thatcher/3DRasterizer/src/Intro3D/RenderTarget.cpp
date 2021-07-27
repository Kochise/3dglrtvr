
#include "stdafx.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(UINT width, UINT height)
{
	this->width = width;
	this->height = height;
	this->size = this->width * this->height;
	CreateHBitmap();
}
RenderTarget::RenderTarget(const RenderTarget& renderTarget)
{
	this->width = renderTarget.width;
	this->height = renderTarget.height;
	this->size = this->width * this->height;
	CreateHBitmap();

	// Copy the other target's data
	memcpy(this->pBitmapBits, renderTarget.pBitmapBits, sizeof(DWORD) * this->size);
}
RenderTarget::~RenderTarget(void)
{
	// Free the bitmap
	if (pBitmapBits)
	{
		pBitmapBits = NULL;
		DeleteObject(hBitmap);
	}
}

void RenderTarget::CreateHBitmap(void)
{
	// Create a bitmap
	BITMAPINFO info;
	ZeroMemory(&info, sizeof(BITMAPINFO));
	info.bmiHeader.biWidth = width;
	info.bmiHeader.biHeight = height;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biCompression = BI_RGB;
	hBitmap = CreateDIBSection(NULL, &info, DIB_RGB_COLORS, (void**)&pBitmapBits, NULL, 0);
}

void RenderTarget::Clear(DWORD color)
{
	for (UINT i = 0; i < size; i++)
	{
		this->pBitmapBits[i] = color;
	}
}

HBITMAP RenderTarget::GetHBitmap(void) const
{
	return this->hBitmap;
}

UINT RenderTarget::GetWidth(void) const
{
	return this->width;
}
UINT RenderTarget::GetHeight(void) const
{
	return this->height;
}

DWORD RenderTarget::GetPixel(UINT pixelIndex) const
{
	return this->pBitmapBits[pixelIndex];
}
void RenderTarget::SetPixel(UINT pixelIndex, DWORD color)
{
	this->pBitmapBits[pixelIndex] = color;
}