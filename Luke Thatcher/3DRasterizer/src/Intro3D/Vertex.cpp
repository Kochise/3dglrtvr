
#include "stdafx.h"
#include "Vertex.h"

Vertex::Vertex(void)
{
}

Vertex::Vertex(const Vector3& Position, const Vector3& Normal, const Vector2& TexCoord)
{
	this->Position = Position;
	this->Normal = Normal;
	this->TexCoord = TexCoord;
}