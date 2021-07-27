
#include "stdafx.h"
#include "DepthBuffer.h"
#include <math.h>

DepthBuffer::DepthBuffer(UINT width, UINT height)
{
	this->width = width;
	this->height = height;
	this->size = this->width * this->height;
	this->data = new float[this->size];
	this->dims = Vector2((float)(this->width - 1), (float)(this->height - 1));
}
DepthBuffer::DepthBuffer(const DepthBuffer& depthBuffer)
{
	this->width = depthBuffer.width;
	this->height = depthBuffer.height;
	this->size = this->width * this->height;
	this->data = new float[this->size];
	this->dims = Vector2((float)(this->width - 1), (float)(this->height - 1));

	// Copy the other buffer's data
	memcpy(this->data, depthBuffer.data, this->size * sizeof(float));
}
DepthBuffer::~DepthBuffer(void)
{
	if (this->data)
	{
		delete [] this->data;
		this->data = NULL;
	}
}

float DepthBuffer::GetValue(UINT pixelIndex) const
{
	return this->data[pixelIndex];
}
void DepthBuffer::SetValue(UINT pixelIndex, float value)
{
	this->data[pixelIndex] = value;
}

void DepthBuffer::Clear(float value)
{
	for (UINT i = 0; i < size; i++)
	{
		this->data[i] = value;
	}
}

UINT DepthBuffer::GetWidth(void) const
{
	return this->width;
}
UINT DepthBuffer::GetHeight(void) const
{
	return this->height;
}

float DepthBuffer::SamplePoint(const Vector2& texCoord) const
{
	float x = fmod(texCoord.X, 1.0f);
	float y = fmod(texCoord.Y, 1.0f);
	Vector2 pixelPos = (dims * Vector2(x,y)) + Vector2(0.5f, 0.5f);
	pixelPos = Vector2::Clamp(pixelPos, Vector2(0, 0), dims);
	return this->data[(((int)floor(pixelPos.Y)) * this->width) + ((int)floor(pixelPos.X))];
}