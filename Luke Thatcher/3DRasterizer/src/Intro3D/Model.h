
#pragma once

#include "Vertex.h"
#include "Face.h"
#include "Texture2D.h"
#include "vector.h"
#include "AlignAlloc.h"

using namespace std;

class ModelFrame
{
public:
	ModelFrame(const vector<Vertex, AlignAlloc<Vertex, 16>>& vertices, const vector<Face, AlignAlloc<Face, 16>>& faces);

	const vector<Vertex, AlignAlloc<Vertex, 16>>& GetVertices(void) const;
	const vector<Face, AlignAlloc<Face, 16>>& GetFaces(void) const;

private:
	vector<Vertex, AlignAlloc<Vertex, 16>> vertices;
	vector<Face, AlignAlloc<Face, 16>> faces;
};

class Model
{
public:
	Model(const vector<ModelFrame>& frames, const vector<const Texture2D*>& textures);
	~Model(void);

	const vector<ModelFrame>& GetFrames(void) const;
	const vector<const Texture2D*>& GetTextures(void) const;

	static Model* CreatePlane(int gridWidth, int gridHeight, float cellWidth, float cellHeight, const Vector2& texCoordCoeff);
	static Model* CreateSphere(int width, int height, float radius);

private:
	vector<ModelFrame> frames;
	vector<const Texture2D*> textures;
};