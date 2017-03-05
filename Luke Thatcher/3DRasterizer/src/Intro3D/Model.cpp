
#include "stdafx.h"
#include "Model.h"

#define _USE_MATH_DEFINES
#include <math.h>

Model::Model(const vector<ModelFrame>& frames, const vector<const Texture2D*>& textures)
{
	this->frames = vector<ModelFrame>(frames);
	this->textures = vector<const Texture2D*>(textures);
}
Model::~Model(void)
{
	UINT size = this->textures.size();
	for (UINT i = 0; i < size; i++)
	{
		delete this->textures[i];
	}
	this->textures.clear();
}

const vector<ModelFrame>& Model::GetFrames(void) const
{
	return this->frames;
}
const vector<const Texture2D*>& Model::GetTextures(void) const
{
	return this->textures;
}

ModelFrame::ModelFrame(const vector<Vertex, AlignAlloc<Vertex, 16>>& vertices, const vector<Face, AlignAlloc<Face, 16>>& faces)
{
	this->vertices = vector<Vertex, AlignAlloc<Vertex, 16>>(vertices);
	this->faces = vector<Face, AlignAlloc<Face, 16>>(faces);
}

const vector<Vertex, AlignAlloc<Vertex, 16>>& ModelFrame::GetVertices(void) const
{
	return this->vertices;
}
const vector<Face, AlignAlloc<Face, 16>>& ModelFrame::GetFaces(void) const
{
	return this->faces;
}

Model* Model::CreatePlane(int gridWidth, int gridHeight, float cellWidth, float cellHeight, const Vector2& texCoordCoeff)
{
	Vector3 gridHalfDims = Vector3(gridWidth * cellWidth, 0, gridHeight * cellHeight) * 0.5f;
	Vector2 gridTexDims = Vector2(1.0f / gridWidth, 1.0f / gridHeight);

	// Generate a list of vertices for the plane
	vector<Vertex, AlignAlloc<Vertex, 16>> planeVerts;
	for (int gridY = 0; gridY <= gridHeight; gridY++)
	{
		for (int gridX = 0; gridX <= gridWidth; gridX++)
		{
			Vector3 pos = Vector3(gridX * cellWidth, 0, gridY * cellHeight) - gridHalfDims;
			Vector2 tex = Vector2((float)gridX, (float)gridY) * gridTexDims;
			tex *= texCoordCoeff;

			planeVerts.push_back(Vertex(pos, Vector3(0, 1, 0), tex));
		}
	}

	// Generate faces for the plane
	vector<Face, AlignAlloc<Face, 16>> planeFaces;
	for (int gridY = 0; gridY < gridHeight; gridY++)
	{
		for (int gridX = 0; gridX < gridWidth; gridX++)
		{
			int topLeft = gridY * (gridWidth + 1) + gridX;
			int topRight = topLeft + 1;
			int bottomLeft = topLeft + gridWidth + 1;
			int bottomRight = bottomLeft + 1;

			planeFaces.push_back(Face(topLeft, bottomRight, topRight, Vector3(0, 1, 0)));
			planeFaces.push_back(Face(topLeft, bottomLeft, bottomRight, Vector3(0, 1, 0)));
		}
	}

	// Generate 1 model frame and return the plane model
	vector<ModelFrame> frames;
	frames.push_back(ModelFrame(planeVerts, planeFaces));
	return new Model(frames, vector<const Texture2D*>());
}

Model* Model::CreateSphere(int width, int height, float radius)
{
	// Reference: http://www.gamedev.net/topic/537269-procedural-sphere-creation/
	float theta, phi;
    int i, j, t, ntri, nvec;

    nvec = (height - 2) * width + 2;
    ntri = (height - 2) * (width - 1) * 2;

    vector<Vertex, AlignAlloc<Vertex, 16>> verts;
    vector<Face, AlignAlloc<Face, 16>> faces;

    for (t = 0, j = 1; j < height - 1; j++)
    {
        for (i = 0; i < width; i++)
        {
            theta = ((float)j) / ((float)(height - 1)) * (float)M_PI;
            phi = ((float)i) / ((float)(width - 1)) * ((float)M_PI * 2.0f);

            Vector3 normal = Vector3::Normalize(Vector3(
                (float)(sin(theta) * cos(phi)),
                (float)cos(theta),
                (float)(-sin(theta) * sin(phi))));
            Vector3 pos = normal * radius;

			verts.push_back(Vertex(pos, normal, Vector2(((float)i) / ((float)(width - 1)), ((float)j) / ((float)(height - 1)))));
        }
    }

	verts.push_back(Vertex(Vector3(0.0f, radius, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)));
	verts.push_back(Vertex(Vector3(0.0f, -radius, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)));

    for (t = 0, j = 0; j < height - 3; j++)
    {
        for (i = 0; i < width - 1; i++)
        {
			int i0 = (j) * width + i;
			int i1 = (j + 1) * width + i + 1;
			int i2 = (j) * width + i + 1;

            int i3 = (j) * width + i;
            int i4 = (j + 1) * width + i;
            int i5 = (j + 1) * width + i + 1;

			Vector3 faceNormal0 = Vector3::Normalize(Vector3::Cross(verts[i1].Position - verts[i0].Position, verts[i2].Position - verts[i0].Position));
			Vector3 faceNormal1 = Vector3::Normalize(Vector3::Cross(verts[i4].Position - verts[i3].Position, verts[i5].Position - verts[i3].Position));

			faces.push_back(Face(i0, i1, i2, faceNormal0));
			faces.push_back(Face(i3, i4, i5, faceNormal1));
        }
    }

    for (i = 0; i < width - 1; i++)
    {
        int i0 = (height - 2) * width;
        int i1 = i;
        int i2 = i + 1;

        int i3 = (height - 2) * width + 1;
        int i4 = (height - 3) * width + i + 1;
        int i5 = (height - 3) * width + i;

		Vector3 faceNormal0 = Vector3::Normalize(Vector3::Cross(verts[i1].Position - verts[i0].Position, verts[i2].Position - verts[i0].Position));
		Vector3 faceNormal1 = Vector3::Normalize(Vector3::Cross(verts[i4].Position - verts[i3].Position, verts[i5].Position - verts[i3].Position));

		faces.push_back(Face(i0, i1, i2, faceNormal0));
		faces.push_back(Face(i3, i4, i5, faceNormal1));
    }

	vector<ModelFrame> frames;
	frames.push_back(ModelFrame(verts, faces));

	return new Model(frames, vector<const Texture2D*>());
}