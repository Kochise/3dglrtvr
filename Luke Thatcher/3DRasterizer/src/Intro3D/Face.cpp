
#include "stdafx.h"
#include "Face.h"

Face::Face(void)
{
	this->faceNormal = Vector3();
	memset(this->indices, 0, sizeof(this->indices));
}

Face::Face(const int indices[3], const Vector3& faceNormal)
{
	this->faceNormal = faceNormal;
	memcpy(this->indices, indices, sizeof(this->indices));
}

Face::Face(int i0, int i1, int i2, const Vector3& faceNormal)
{
	this->faceNormal = faceNormal;
	this->indices[0] = i0;
	this->indices[1] = i1;
	this->indices[2] = i2;
}