
#include "stdafx.h"
#include "Rasterizer.h"
#include "Matrix4x4.h"
#include <math.h>
#include <float.h>
#include "DemoEngine.h"

double round(double r)
{
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5); 
}
float round(float r)
{
    return (r > 0.0f) ? floor(r + 0.5f) : ceil(r - 0.5f); 
}

Rasterizer::Rasterizer(DemoEngine* appEngine)
{
	this->appEngine = appEngine;
	this->xformVertsBuffer = NULL;
	this->xformVertsBufferLength = 0;
	this->cullMode = CullCounterClockwise;
	this->fillMode = Solid;
	this->width = 0;
	this->height = 0;

	// Allocate a 32-bit boundary aligned UINT for threading
	this->currentWorkingThreadIndexer = (UINT*)_aligned_malloc(sizeof(UINT), 32);
}
Rasterizer::~Rasterizer(void)
{
	if (xformVertsBuffer)
	{
		delete [] xformVertsBuffer;
		xformVertsBuffer = NULL;
		xformVertsBufferLength = 0;
	}
	if (currentWorkingThreadIndexer)
	{
		_aligned_free(currentWorkingThreadIndexer);
		currentWorkingThreadIndexer = NULL;
	}
}

RasterCullMode Rasterizer::GetCullMode(void) const
{
	return this->cullMode;
}
RasterFillMode Rasterizer::GetFillMode(void) const
{
	return this->fillMode;
}
void Rasterizer::SetCullMode(RasterCullMode cullMode)
{
	this->cullMode = cullMode;
}
void Rasterizer::SetFillMode(RasterFillMode fillMode)
{
	this->fillMode = fillMode;
}

void Rasterizer::SetTargets(RenderTarget* pTarget, DepthBuffer* pDepthBuffer)
{
	this->currentRenderTarget = pTarget;
	this->currentDepthBuffer = pDepthBuffer;

	if (this->currentRenderTarget)
	{
		this->width = this->currentRenderTarget->GetWidth();
		this->height = this->currentRenderTarget->GetHeight();
	}
	else if (this->currentDepthBuffer)
	{
		this->width = this->currentDepthBuffer->GetWidth();
		this->height = this->currentDepthBuffer->GetHeight();
	}

	// Create the screen matrix to map vertices to the window
	this->screenMatrix = Matrix4x4(
		width / 2.0f, 0, 0, 0,
		0, (height / 2.0f), 0, 0,
		0, 0, 1, 0,
		width / 2.0f, height / 2.0f, 0, 1);
}

void Rasterizer::BeginTransformVertsWorkItem(const vector<Vertex, AlignAlloc<Vertex, 16>>& vertices, const vector<Face, AlignAlloc<Face, 16>>& faces, bool usePerVertexNormals, UINT numVector4Slots, VertexShaderCallback vertexShader)
{
	this->currentWorkingNumVertices = usePerVertexNormals ? vertices.size() : faces.size() * 3;

	// Create or grow the xformVertsBuffer to fit the vertices vector
	if (this->xformVertsBufferLength < currentWorkingNumVertices)
	{
		if (this->xformVertsBuffer)
		{
			delete [] this->xformVertsBuffer;
		}

		this->xformVertsBuffer = new VS_Output[currentWorkingNumVertices];
		this->xformVertsBufferLength = currentWorkingNumVertices;
	}

	// Store the vertices and faces for the current work item
	this->currentWorkingVertices = &vertices;
	this->currentWorkingFaces = &faces;

	// Store vertex shader info
	this->currentWorkingUsePerVertexNormals = usePerVertexNormals;
	this->currentWorkingNumVector4Slots = numVector4Slots;
	this->currentWorkingVertexShader = vertexShader;
	this->currentWorkingNumFaces = faces.size();

	// Reset the thread indexer to ((UINT)-1)
	*this->currentWorkingThreadIndexer = ((UINT)-1);
}
void Rasterizer::RunTransformVertsWorkItem(void)
{
	if (currentWorkingUsePerVertexNormals)
	{
		// Apply transformations on a per vertex basis
		UINT vertIndex = 0;
		while((vertIndex = InterlockedIncrement(this->currentWorkingThreadIndexer)) < currentWorkingNumVertices)
		{
			(appEngine->*currentWorkingVertexShader)((*currentWorkingVertices)[vertIndex], xformVertsBuffer[vertIndex]);

			// Divide all slot values by Position.W (for perspective correction)
			float w = xformVertsBuffer[vertIndex].Position.W;
			for (int j = 0; j < currentWorkingNumVector4Slots; j++)
			{
				xformVertsBuffer[vertIndex].Slots[j] /= w;
			}
		}
	}
	else
	{
		// Apply transfromations in batches of 3 on a per face basis
		UINT faceIndex = 0;
		while((faceIndex = InterlockedIncrement(this->currentWorkingThreadIndexer)) < currentWorkingNumFaces)
		{
			// Get current face
			UINT vertsBaseIndex = faceIndex * 3;
			const Face& currentFace = (*currentWorkingFaces)[faceIndex];
			const int* faceIndices = currentFace.GetIndices();

			// Extract batch of 3 output vertices
			VS_Output& v0 = xformVertsBuffer[vertsBaseIndex];
			VS_Output& v1 = xformVertsBuffer[vertsBaseIndex+1];
			VS_Output& v2 = xformVertsBuffer[vertsBaseIndex+2];

			// Extract the input vertices
			Vertex input0 = (*currentWorkingVertices)[faceIndices[0]];
			Vertex input1 = (*currentWorkingVertices)[faceIndices[1]];
			Vertex input2 = (*currentWorkingVertices)[faceIndices[2]];

			// Overwrite the vertex normal with the face normal
			input0.Normal = input1.Normal = input2.Normal = currentFace.GetFaceNormal();

			// Run the vertices through the vertex shader
			(appEngine->*currentWorkingVertexShader)(input0, v0);
			(appEngine->*currentWorkingVertexShader)(input1, v1);
			(appEngine->*currentWorkingVertexShader)(input2, v2);

			// Divide all slot values by Position.W (for perspective correction)
			for (int j = 0; j < currentWorkingNumVector4Slots; j++)
			{
				v0.Slots[j] /= v0.Position.W;
				v1.Slots[j] /= v1.Position.W;
				v2.Slots[j] /= v2.Position.W;
			}
		}
	}
}

/*static inline bool IsInside(const Vector4* S, const Vector4* plane)
{
    return Vector4::Dot(*S, *plane) > 0 && (S->W > 0);
}
static inline Vector4 ComputeIntersection(const Vector4* start, const Vector4* target, const Vector4* plane)
{
    // Compute the intersection between the line joining start and target, and the plane
    // In the following formula:
    // Q:q = (start.XYZ : start.W)
    // P:p = (target.XYZ : target.W)
    // N:n = (plane.XYZ : plane.W)

	const Vector3& startV3 = start->AsVector3();
	const Vector3& targetV3 = target->AsVector3();
	const Vector3& planeV3 = plane->AsVector3();

    // Reference: http://www.acm.org/tog/resources/RTNews/html/rtnv11n1.html#art3
    //@ L = {qP-pQ:PxQ}, for (P:p) and (Q:q) distinct homogeneous points on L.
    Vector3 U = (targetV3 * start->W) - (startV3 * target->W);    //qP-pQ
    Vector3 V = Vector3::Cross(targetV3, startV3);           //PxQ

    //@ (VxN-Un:U.N) is the point where L intersects plane [N:n] not parallel to L.
    return Vector4(
        Vector3::Cross(V, planeV3) - U * plane->W,  // VxN-Un
        Vector3::Dot(U, planeV3));                 // U.N
}
UINT Rasterizer::ClipPolygon(VS_Output points[7])
{
	const static Vector4 planes[] = 
	{
		Vector4(0, 0, 1, -0.4f),	// Near Plane
		Vector4(0, 0, -1, 1),	// Far Plane
		Vector4(1, 0, 0, 1),	// Left Plane
		Vector4(-1, 0, 0, 1),	// Right Plane
		Vector4(0, -1, 0, 1),	// Top Plane
		Vector4(0, 1, 0, 1),	// Bottom Plane
	};

    // Reference: http://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm

    // Initialize the output list with the original points
	Vector4 list0[7];
	Vector4 list1[7];
	for (UINT i = 0; i < 3; i++)
	{
		list0[i] = points[i].Position;
	}

	Vector4* inputList = list1;
	Vector4* outputList = list0;

    int numInputVerts = 3;
    int numOutputVerts = 3;

    // Loop over each clipping plane
    for(UINT j = 0; j < 1; j++)
    {
		const Vector4* clipPlane = &planes[j];

        // Stop if no more vertices are left
        if (numOutputVerts == 0)
            break;

		// Swap input and output lists
		Vector4* temp = outputList;
		outputList = inputList;
		inputList = temp;

        numInputVerts = numOutputVerts;
        numOutputVerts = 0;

        // Get the first point (last point in list)
        const Vector4* S = &inputList[numInputVerts - 1];

        // Loop over each point (from first to last)
        for (int i = 0; i < numInputVerts; i++)
        {
            const Vector4* E = &inputList[i];

            // If the end point is inside the clipping plane...
            if (IsInside(E, clipPlane))
            {
                // If the start point is outside the clipping plane
                if (!IsInside(S, clipPlane))
                {
                    // Save the intersection between the start and end points, and the plane
					outputList[numOutputVerts] = ComputeIntersection(S, E, clipPlane);
                    numOutputVerts++;
                }

                // Save the end point (it is inside the non-clipped region)
				outputList[numOutputVerts] = *E;
                numOutputVerts++;
            }
            // Otherwise, if the start point is inside the clipping plane...
            else if (IsInside(S, clipPlane))
            {
                // Save the intersection between the start and end points, and the plane
                outputList[numOutputVerts] = ComputeIntersection(S, E, clipPlane);
                numOutputVerts++;
            }

            // Advance the current point
            S = E;
        }

        for (int i = 0; i < numOutputVerts; i++)
            outputList[i] /= outputList[i].W;
    }

	if (numOutputVerts > 2)
	{
		// outputList contains a list of vertex positions
		
		for (int i = 0; i < numOutputVerts; i++)
			points[i].Position = outputList[i];

		if (numOutputVerts > 6)
		{
			int h = numOutputVerts;
			printf("%i\r\n", h);
		}

		// Calculate lerp values for the original 3 input vertices
		UINT numVector4Slots = currentWorkingNumVector4Slots + 1;

		Vector4* A = (Vector4*)&points[0];
		Vector4* B = (Vector4*)&points[1];
		Vector4* C = (Vector4*)&points[2];

		*A /= A->W;
		*B /= B->W;
		*C /= C->W;

		// Precalculate gradient values for interpolation
		Vector4 lerpA_minus_C[TOTAL_VERTEX_SLOTS];
		Vector4 lerpB_minus_C[TOTAL_VERTEX_SLOTS];
		for (UINT i = 0; i < numVector4Slots; i++)
		{
			lerpA_minus_C[i] = (A[i] - C[i]);
			lerpB_minus_C[i] = (B[i] - C[i]);
		}

		float detT = (((B->Y - C->Y) * (A->X - C->X)) + ((C->X - B->X) * (A->Y - C->Y)));

		for (UINT i = 0; i < numOutputVerts; i++)
		{
			Vector4* currentVert = (Vector4*)&points[i];

			currentVert[0] = outputList[i];

			float u = (((B->Y - C->Y) * (currentVert->X - C->X)) + ((C->X - B->X) * (currentVert->Y - C->Y))) / detT;
			float v = (((C->Y - A->Y) * (currentVert->X - C->X)) + ((A->X - C->X) * (currentVert->Y - C->Y))) / detT;

			// First, interpolate position
			//currentVert[0] = C[0] + lerpA_minus_C[0] * u + lerpB_minus_C[0] * v;

			// Extract the interpolated zRecip value from position
			float zRecip = currentVert[0].W;

			// Interpolate remaining values
			for (UINT i = 1; i < numVector4Slots; i++)
			{
				currentVert[i] = (C[i] + lerpA_minus_C[i] * u + lerpB_minus_C[i] * v) / zRecip;
			}
		}
	}

	return numOutputVerts;
}*/

void Rasterizer::BeginDrawPrimitivesWorkItem(PixelShaderCallback pixelShader)
{
	// Reset the thread indexer to ((UINT)-1)
	*this->currentWorkingThreadIndexer = ((UINT)-1);

	// Store the current pixel shader for later
	this->currentWorkingPixelShader = pixelShader;
}
void Rasterizer::RunDrawPrimitivesWorkItem(void)
{
	// Cancel rendering if no depth buffer is set
	if (!currentDepthBuffer)
		return;

	// Loop over each face in the model
	UINT i = 0;
	while((i = InterlockedIncrement(this->currentWorkingThreadIndexer)) < currentWorkingNumFaces)
	{
		VS_Output v0, v1, v2;

		// Array of 3 vertices
		// 7 is the maximum possible vertices outputted by the clipper
		//VS_Output verts[3];

		if (currentWorkingUsePerVertexNormals)
		{
			// Select vertices using current face indices
			const Face& currentFace = (*currentWorkingFaces)[i];
			const int* faceIndices = currentFace.GetIndices();
			v0 = xformVertsBuffer[faceIndices[0]];
			v1 = xformVertsBuffer[faceIndices[1]];
			v2 = xformVertsBuffer[faceIndices[2]];
		}
		else
		{
			// Select vertices in batches of 3, using i
			UINT vertexBaseIndex = i * 3;
			v0 = xformVertsBuffer[vertexBaseIndex];
			v1 = xformVertsBuffer[vertexBaseIndex + 1];
			v2 = xformVertsBuffer[vertexBaseIndex + 2];
		}

		// Check if the polygon is completely within the view frustum
		if (v0.Position.IsInView() ||
			v1.Position.IsInView() ||
			v2.Position.IsInView())
		{
			// The polygon can be rendered completely, as it is fully within the view frustum
			DispatchPolygon(v0, v1, v2);
		}
		/*else
		{
			// The polygon is either completely outside the view frustum, 
			// or partially intersecting it
			// Run the vertices through the polygon clipper
			UINT numPoints = ClipPolygon(verts);
			if (numPoints > 2)
			{
				// Dispatch the polygons as a triangle fan
				UINT numTriangles = numPoints - 2;
				for (UINT i = 0, j = 1; i < numTriangles; i++, j++)
				{
					VS_Output v0 = verts[0];
					VS_Output v1 = verts[j];
					VS_Output v2 = verts[j + 1];

					// Dispatch this clipped polygon for rendering
					DispatchPolygon(v0, v1, v2);
				}
			}
		}*/
	}
}
void Rasterizer::DispatchPolygon(VS_Output& A, VS_Output& B, VS_Output& C)
{
	// Calculate zRecip, per vertex, for perspective correction
	float z0 = 1.0f / A.Position.W;
	float z1 = 1.0f / B.Position.W;
	float z2 = 1.0f / C.Position.W;

	// Dehomogenise the positions
	A.Position /= A.Position.W;
	B.Position /= B.Position.W;
	C.Position /= C.Position.W;

	// Cull back facing polygons
	if (cullMode != CullNone)
	{
		// Calculate the face normal direction
		float zVal = Vector3::Cross(
			*((Vector3*)&C.Position) - *((Vector3*)&A.Position),
			*((Vector3*)&B.Position) - *((Vector3*)&A.Position)).Z;

		// Skip polygon if culling condition is not met
		switch(cullMode)
		{
		case CullCounterClockwise: if (zVal > 0) return; break;
		case CullClockwise: if (zVal < 0) return; break;
		}
	}

	// Apply the screen transformation
	A.Position = screenMatrix * A.Position;
	B.Position = screenMatrix * B.Position;
	C.Position = screenMatrix * C.Position;

	// Store the zRecip values in the (now useless) W component
	// These will be interpolated with the position value
	A.Position.W = z0;
	B.Position.W = z1;
	C.Position.W = z2;

	// Draw the polygon
	switch(fillMode)
	{
	case Solid:
		FillPolygon(&A, &B, &C);
		break;
	case Wireframe:
		DrawPolygon(&A, &B, &C);
		break;
	}
}
void Rasterizer::FillPolygon(const VS_Output* A, const VS_Output* B, const VS_Output* C)
{
	// Order verts by Y value
    if (B->Position.Y < A->Position.Y) { const VS_Output* temp = A; A = B; B = temp; }
    if (C->Position.Y < B->Position.Y) { const VS_Output* temp = B; B = C; C = temp; }
    if (B->Position.Y < A->Position.Y) { const VS_Output* temp = A; A = B; B = temp; }

	// Pointer used in interpolation later
	Vector4* pC = (Vector4*)C;

	// Get rounded limits
    int minY = (int)round(A->Position.Y);
    int midY = (int)round(B->Position.Y);
    int maxY = (int)round(C->Position.Y);
    int Ax = (int)round(A->Position.X);
    int Bx = (int)round(B->Position.X);
    int Cx = (int)round(C->Position.X);

	// Skip triangle if it has no height
    if (minY == maxY)
        return;

    float aToBGrad = (B->Position.X - A->Position.X) / (B->Position.Y - A->Position.Y);
    float aToCGrad = (C->Position.X - A->Position.X) / (C->Position.Y - A->Position.Y);
    float bToCGrad = (C->Position.X - B->Position.X) / (C->Position.Y - B->Position.Y);

    float aToCX = (float)Ax;
    float bToCX = (float)Bx;
    float aToBX = aToCX;

    // Calculate constants for barycentric coords
	float Cx_m_Bx = (float)(Cx - Bx);
    float Ax_m_Cx = (float)(Ax - Cx);
    float uvDetT = (midY - maxY) * Ax_m_Cx + Cx_m_Bx * (minY - maxY);
	if (uvDetT == 0)
		return;

	float oneOverUVDetT = 1.0f / uvDetT;
	float midY_m_maxY_over_detT = ((float)(midY - maxY)) * oneOverUVDetT;
	float maxY_m_minY_over_detT = ((float)(maxY - minY)) * oneOverUVDetT;
	float Cx_m_Bx_over_uvDetT = Cx_m_Bx * oneOverUVDetT;
	float Ax_m_Cx_over_uvDetT = Ax_m_Cx * oneOverUVDetT;

    // Initialize values for first loop
    int currentMinY = minY;
    int currentMaxY = midY;
    float p0ToP2_X = aToBX;
    float p0ToP2_XGrad = aToBGrad;

	UINT numVector4Slots = currentWorkingNumVector4Slots + 1;

	// Precalculate gradient values for interpolation
	Vector4 lerpA_minus_C[TOTAL_VERTEX_SLOTS];
	Vector4 lerpB_minus_C[TOTAL_VERTEX_SLOTS];
	for (UINT i = 0; i < numVector4Slots; i++)
	{
		lerpA_minus_C[i] = (((Vector4*)A)[i] - ((Vector4*)C)[i]);
		lerpB_minus_C[i] = (((Vector4*)B)[i] - ((Vector4*)C)[i]);
	}

    // Loop twice (for each half of the triangle)
    for (int i = 0; i < 2; i++)
    {
        // Skip this half if its height is 0
        if (currentMinY != currentMaxY)
        {
			// Loop over each scanline
            for (int yUV = currentMinY - maxY; currentMinY < currentMaxY; currentMinY++, yUV++)
            {
				if (currentMinY >= 0 && currentMinY < (int)height)
				{
					float Cx_m_Bx_times_yUV_over_detT = yUV * Cx_m_Bx_over_uvDetT;
					float Ax_m_Cx_times_yUV_over_detT = yUV * Ax_m_Cx_over_uvDetT;

					// Get rounded scanline start and end points
					int x0 = (int)round(aToCX);
					int x1 = (int)round(p0ToP2_X);

					// Ensure x0 <= x1
					if (x0 > x1) { int temp = x0; x0 = x1; x1 = temp; }

					// Loop over each pixel in the scanline
					for (int x0UV = x0 - Cx; x0 < x1; x0++, x0UV++)
					{
						if (x0 >= 0 && x0 < (int)width)
						{
							// Calculate pixel index
							UINT pixelIndex = currentMinY * width + x0;

							// Calculate pixel's barycentric coords
							float u = x0UV * midY_m_maxY_over_detT + Cx_m_Bx_times_yUV_over_detT;
							float v = x0UV * maxY_m_minY_over_detT + Ax_m_Cx_times_yUV_over_detT;

							// Interpolate values across the triangle
							VS_Output pixelInput;
							Vector4* pPixelInput = (Vector4*)&pixelInput;

							// First, interpolate position
							pPixelInput[0] = pC[0] + lerpA_minus_C[0] * u + lerpB_minus_C[0] * v;

							// Extract the interpolated zRecip value from position
							float zRecip = pPixelInput[0].W;

							// Check depth buffer, skip occluded pixels
							if (pixelInput.Position.Z > currentDepthBuffer->GetValue(pixelIndex))
								continue;

							// Store the new depth value in the depth buffer
							currentDepthBuffer->SetValue(pixelIndex, pixelInput.Position.Z);

							// Check we have a render target available
							if (currentRenderTarget)
							{
								// Interpolate remaining values
								for (UINT i = 1; i < numVector4Slots; i++)
								{
									pPixelInput[i] = (pC[i] + lerpA_minus_C[i] * u + lerpB_minus_C[i] * v) / zRecip;
								}

								// Call the pixel shader with the final VS_Output structure
								Vector4 pixelColor;
								(appEngine->*currentWorkingPixelShader)(pixelInput, pixelColor);

								// Clamp the color to 0.0f -> 1.0f
								pixelColor = Vector4::Clamp(pixelColor, Vector4(0.0f), Vector4(1.0f));
					
								pixelColor *= 255.0f;

								// Convert color floats to bytes values
								UINT a = (UINT)(pixelColor.W);
								UINT r = (UINT)(pixelColor.X);
								UINT g = (UINT)(pixelColor.Y);
								UINT b = (UINT)(pixelColor.Z);

								// Logical OR the colors together
								UINT col = (a << 24) | (r << 16) | (g << 8) | b;

								// Draw the pixel to the frame buffer
								//pBitmapBits[currentMinY * width + x0] = col;
								currentRenderTarget->SetPixel(currentMinY * width + x0, col);
							}
						}
					}
				}
				// Increment x coordinates
                aToCX += aToCGrad;
                p0ToP2_X += p0ToP2_XGrad;
            }
        }

        // Initialize values for second loop
        currentMinY = midY;
        currentMaxY = maxY;
        p0ToP2_X = bToCX;
        p0ToP2_XGrad = bToCGrad;
    }
}
void Rasterizer::DrawPolygon(const VS_Output* A, const VS_Output* B, const VS_Output* C)
{
	// Draw the three lines of the polygon
	DrawLine(A, B);
	DrawLine(B, C);
	DrawLine(C, A);
}

void Rasterizer::DrawLine(const VS_Output* A, const VS_Output* B)
{
	UINT numVector4Slots = currentWorkingNumVector4Slots + 1;

	int p0X = (int)round(A->Position.X);
    int p0Y = (int)round(A->Position.Y);
    int p1X = (int)round(B->Position.X);
    int p1Y = (int)round(B->Position.Y);

    int dx = abs(p1X - p0X);
    int dy = abs(p1Y - p0Y);
    int sx, sy;
    if (p0X < p1X) { sx = 1; } else { sx = -1; }
    if (p0Y < p1Y) { sy = 1; } else { sy = -1; }
    int err = dx - dy;

    while (true)
    {
        // Set pixel at x0, y0
        if (p0X >= 0 && p0X < (int)width &&
            p0Y >= 0 && p0Y < (int)height)
        {
			// Calculate pixel index
			UINT pixelIndex = p0Y * width + p0X;
			if (pixelIndex >= 0 && pixelIndex < (width * height))
			{
				// Lerp values in VS_Output structs across the line
				float t = (p0X - A->Position.X) / (B->Position.X - A->Position.X);

				// Interpolate values across the triangle
				VS_Output pixelInput;
				Vector4* pPixelInput = (Vector4*)&pixelInput;

				// First, interpolate position
				pPixelInput[0] = ((Vector4*)A)[0] * (1.0f - t) + ((Vector4*)B)[0] * t;

				// Extract the interpolated zRecip value from position
				float zRecip = pPixelInput[0].W;

				// Check depth buffer, skip occluded pixels
				if (pixelInput.Position.Z <= currentDepthBuffer->GetValue(pixelIndex))
				{
					// Store the new depth value in the depth buffer
					currentDepthBuffer->SetValue(pixelIndex, pixelInput.Position.Z);

					// Check we have a render target available
					if (currentRenderTarget)
					{
						// Interpolate remaining values
						for (UINT i = 1; i < numVector4Slots; i++)
						{
							pPixelInput[i] = (((Vector4*)A)[i] * (1.0f - t) + ((Vector4*)B)[i] * t) / zRecip;
						}

						// Call the pixel shader with the final VS_Output structure
						Vector4 pixelColor;
						(appEngine->*currentWorkingPixelShader)(pixelInput, pixelColor);

						// Clamp the color to 0.0f -> 1.0f
						pixelColor = Vector4::Clamp(pixelColor, Vector4(0.0f), Vector4(1.0f));
					
						pixelColor *= 255.0f;

						// Convert color floats to bytes values
						UINT a = (UINT)(pixelColor.W);
						UINT r = (UINT)(pixelColor.X);
						UINT g = (UINT)(pixelColor.Y);
						UINT b = (UINT)(pixelColor.Z);

						// Logical OR the colors together
						UINT col = (a << 24) | (r << 16) | (g << 8) | b;

						// Draw the pixel to the frame buffer
						currentRenderTarget->SetPixel(p0Y * width + p0X, col);
					}
				}
			}
        }

        if ((p0X == p1X) && (p0Y == p1Y)) break;
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err = err - dy;
            p0X = p0X + sx;
        }
        if (e2 < dx)
        {
            err = err + dx;
            p0Y = p0Y + sy;
        }
    }
}