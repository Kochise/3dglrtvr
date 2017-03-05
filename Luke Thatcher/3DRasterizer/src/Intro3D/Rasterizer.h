
#pragma once

#include "stdafx.h"
#include "Vector4.h"
#include "Vertex.h"
#include "Face.h"
#include "Matrix4x4.h"
#include "vector.h"
#include "AlignAlloc.h"
#include "RenderTarget.h"
#include "DepthBuffer.h"
#include <GdiPlus.h>

using namespace std;

#define MAX_USER_SLOTS 4
#define TOTAL_VERTEX_SLOTS (MAX_USER_SLOTS + 1)

class AppEngine;

__declspec(align(16)) struct VS_Output
{
	DEFINE_NEW_DELETE_ALIGNED
	Vector4 Position;
	Vector4 Slots[MAX_USER_SLOTS];
};

class DemoEngine;
typedef void (DemoEngine::*VertexShaderCallback)(const Vertex& input, VS_Output& output) const;
typedef void (DemoEngine::*PixelShaderCallback)(const VS_Output& input, Vector4& output) const;

enum RasterFillMode
{
	Wireframe,
	Solid
};
enum RasterCullMode
{
	CullNone,
	CullCounterClockwise,
	CullClockwise
};

__declspec(align(16)) class Rasterizer
{
public:
	DEFINE_NEW_DELETE_ALIGNED

	Rasterizer(DemoEngine* appEngine);
	~Rasterizer(void);

	RasterCullMode GetCullMode(void) const;
	RasterFillMode GetFillMode(void) const;
	void SetCullMode(RasterCullMode cullMode);
	void SetFillMode(RasterFillMode fillMode);

	void SetTargets(RenderTarget* pTarget, DepthBuffer* pDepthBuffer);

	void BeginTransformVertsWorkItem(const vector<Vertex, AlignAlloc<Vertex, 16>>& vertices, const vector<Face, AlignAlloc<Face, 16>>& faces, bool usePerVertexNormals, UINT numVector4Slots, VertexShaderCallback vertexShader);
	void BeginDrawPrimitivesWorkItem(PixelShaderCallback pixelShader);

	void RunTransformVertsWorkItem(void);
	void RunDrawPrimitivesWorkItem(void);

private:
	DemoEngine* appEngine;

	// Pointers to the curretn active targets
	RenderTarget* currentRenderTarget;
	DepthBuffer* currentDepthBuffer;

	// Rasterizer state variables
	UINT width;
	UINT height;
	Matrix4x4 screenMatrix;
	RasterCullMode cullMode;
	RasterFillMode fillMode;

	// Buffer for transformed vertices
	VS_Output* xformVertsBuffer;
	UINT xformVertsBufferLength;

	// Indexer values used in atomic increments
	// Controls which threads access which array elements
	UINT* currentWorkingThreadIndexer;

	// General work item info
	int currentWorkingNumVector4Slots;
	bool currentWorkingUsePerVertexNormals;

	// Counts of vertices and faces
	UINT currentWorkingNumFaces;
	UINT currentWorkingNumVertices;

	// References to the current vertex and pixel shader
	VertexShaderCallback currentWorkingVertexShader;
	PixelShaderCallback currentWorkingPixelShader;

	// References to the current vertex and face lists
	const vector<Vertex, AlignAlloc<Vertex, 16>>* currentWorkingVertices;
	const vector<Face, AlignAlloc<Face, 16>>* currentWorkingFaces;

	void DispatchPolygon(VS_Output& A, VS_Output& B, VS_Output& C);
	void FillPolygon(const VS_Output* A, const VS_Output* B, const VS_Output* C);
	void DrawPolygon(const VS_Output* A, const VS_Output* B, const VS_Output* C);
	void DrawLine(const VS_Output* A, const VS_Output* B);
};