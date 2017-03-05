#include "stdafx.h"
#include "MD2Loader.h"
#include "Vector3.h"
#include "Model.h"
#include "Texture2D.h"

// File reading
#include <iostream>
#include <fstream>

#include "OrderedMap.h"

using namespace std;

// BYTE added in case Windows.h is not included.
typedef unsigned char BYTE; 

// Magic number for MD2 files  "IDP2" or 844121161
const int MD2_IDENT = (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I');

// MS2 version
const int MD2_VERSION = 8;

static Vector3 Normals[] =
{
    Vector3(-0.525731f, 0.000000f, 0.850651f),
    Vector3(-0.442863f, 0.238856f, 0.864188f),
    Vector3(-0.295242f, 0.000000f, 0.955423f),
    Vector3(-0.309017f, 0.500000f, 0.809017f),
    Vector3(-0.162460f, 0.262866f, 0.951056f),
    Vector3(0.000000f, 0.000000f, 1.000000f), 
    Vector3(0.000000f, 0.850651f, 0.525731f),
    Vector3(-0.147621f, 0.716567f, 0.681718f),
    Vector3(0.147621f, 0.716567f, 0.681718f), 
    Vector3(0.000000f, 0.525731f, 0.850651f), 
    Vector3(0.309017f, 0.500000f, 0.809017f), 
    Vector3(0.525731f, 0.000000f, 0.850651f),
    Vector3(0.295242f, 0.000000f, 0.955423f),
    Vector3(0.442863f, 0.238856f, 0.864188f),
    Vector3(0.162460f, 0.262866f, 0.951056f),
    Vector3(-0.681718f, 0.147621f, 0.716567f),
    Vector3(-0.809017f, 0.309017f, 0.500000f),
    Vector3(-0.587785f, 0.425325f, 0.688191f),
    Vector3(-0.850651f, 0.525731f, 0.000000f),
    Vector3(-0.864188f, 0.442863f, 0.238856f),
    Vector3(-0.716567f, 0.681718f, 0.147621f),
    Vector3(-0.688191f, 0.587785f, 0.425325f),
    Vector3(-0.500000f, 0.809017f, 0.309017f),
    Vector3(-0.238856f, 0.864188f, 0.442863f),
    Vector3(-0.425325f, 0.688191f, 0.587785f),
    Vector3(-0.716567f, 0.681718f, -0.147621f),
    Vector3(-0.500000f, 0.809017f, -0.309017f),
    Vector3(-0.525731f, 0.850651f, 0.000000f), 
    Vector3(0.000000f, 0.850651f, -0.525731f), 
    Vector3(-0.238856f, 0.864188f, -0.442863f),
    Vector3(0.000000f, 0.955423f, -0.295242f), 
    Vector3(-0.262866f, 0.951056f, -0.162460f),
    Vector3(0.000000f, 1.000000f, 0.000000f), 
    Vector3(0.000000f, 0.955423f, 0.295242f), 
    Vector3(-0.262866f, 0.951056f, 0.162460f),
    Vector3(0.238856f, 0.864188f, 0.442863f), 
    Vector3(0.262866f, 0.951056f, 0.162460f), 
    Vector3(0.500000f, 0.809017f, 0.309017f), 
    Vector3(0.238856f, 0.864188f, -0.442863f),
    Vector3(0.262866f, 0.951056f, -0.162460f),
    Vector3(0.500000f, 0.809017f, -0.309017f),
    Vector3(0.850651f, 0.525731f, 0.000000f), 
    Vector3(0.716567f, 0.681718f, 0.147621f), 
    Vector3(0.716567f, 0.681718f, -0.147621f),
    Vector3(0.525731f, 0.850651f, 0.000000f), 
    Vector3(0.425325f, 0.688191f, 0.587785f), 
    Vector3(0.864188f, 0.442863f, 0.238856f), 
    Vector3(0.688191f, 0.587785f, 0.425325f), 
    Vector3(0.809017f, 0.309017f, 0.500000f), 
    Vector3(0.681718f, 0.147621f, 0.716567f), 
    Vector3(0.587785f, 0.425325f, 0.688191f), 
    Vector3(0.955423f, 0.295242f, 0.000000f), 
    Vector3(1.000000f, 0.000000f, 0.000000f), 
    Vector3(0.951056f, 0.162460f, 0.262866f), 
    Vector3(0.850651f, -0.525731f, 0.000000f),
    Vector3(0.955423f, -0.295242f, 0.000000f),
    Vector3(0.864188f, -0.442863f, 0.238856f),
    Vector3(0.951056f, -0.162460f, 0.262866f),
    Vector3(0.809017f, -0.309017f, 0.500000f),
    Vector3(0.681718f, -0.147621f, 0.716567f),
    Vector3(0.850651f, 0.000000f, 0.525731f), 
    Vector3(0.864188f, 0.442863f, -0.238856f),
    Vector3(0.809017f, 0.309017f, -0.500000f),
    Vector3(0.951056f, 0.162460f, -0.262866f),
    Vector3(0.525731f, 0.000000f, -0.850651f),
    Vector3(0.681718f, 0.147621f, -0.716567f),
    Vector3(0.681718f, -0.147621f, -0.716567f),
    Vector3(0.850651f, 0.000000f, -0.525731f), 
    Vector3(0.809017f, -0.309017f, -0.500000f),
    Vector3(0.864188f, -0.442863f, -0.238856f),
    Vector3(0.951056f, -0.162460f, -0.262866f),
    Vector3(0.147621f, 0.716567f, -0.681718f), 
    Vector3(0.309017f, 0.500000f, -0.809017f), 
    Vector3(0.425325f, 0.688191f, -0.587785f), 
    Vector3(0.442863f, 0.238856f, -0.864188f), 
    Vector3(0.587785f, 0.425325f, -0.688191f), 
    Vector3(0.688191f, 0.587785f, -0.425325f), 
    Vector3(-0.147621f, 0.716567f, -0.681718f),
    Vector3(-0.309017f, 0.500000f, -0.809017f),
    Vector3(0.000000f, 0.525731f, -0.850651f), 
    Vector3(-0.525731f, 0.000000f, -0.850651f),
    Vector3(-0.442863f, 0.238856f, -0.864188f),
    Vector3(-0.295242f, 0.000000f, -0.955423f),
    Vector3(-0.162460f, 0.262866f, -0.951056f),
    Vector3(0.000000f, 0.000000f, -1.000000f), 
    Vector3(0.295242f, 0.000000f, -0.955423f), 
    Vector3(0.162460f, 0.262866f, -0.951056f), 
    Vector3(-0.442863f, -0.238856f, -0.864188f),
    Vector3(-0.309017f, -0.500000f, -0.809017f),
    Vector3(-0.162460f, -0.262866f, -0.951056f),
    Vector3(0.000000f, -0.850651f, -0.525731f),
    Vector3(-0.147621f, -0.716567f, -0.681718f),
    Vector3(0.147621f, -0.716567f, -0.681718f), 
    Vector3(0.000000f, -0.525731f, -0.850651f), 
    Vector3(0.309017f, -0.500000f, -0.809017f), 
    Vector3(0.442863f, -0.238856f, -0.864188f), 
    Vector3(0.162460f, -0.262866f, -0.951056f), 
    Vector3(0.238856f, -0.864188f, -0.442863f), 
    Vector3(0.500000f, -0.809017f, -0.309017f), 
    Vector3(0.425325f, -0.688191f, -0.587785f), 
    Vector3(0.716567f, -0.681718f, -0.147621f), 
    Vector3(0.688191f, -0.587785f, -0.425325f), 
    Vector3(0.587785f, -0.425325f, -0.688191f), 
    Vector3(0.000000f, -0.955423f, -0.295242f), 
    Vector3(0.000000f, -1.000000f, 0.000000f),
    Vector3(0.262866f, -0.951056f, -0.162460f), 
    Vector3(0.000000f, -0.850651f, 0.525731f), 
    Vector3(0.000000f, -0.955423f, 0.295242f), 
    Vector3(0.238856f, -0.864188f, 0.442863f), 
    Vector3(0.262866f, -0.951056f, 0.162460f), 
    Vector3(0.500000f, -0.809017f, 0.309017f), 
    Vector3(0.716567f, -0.681718f, 0.147621f), 
    Vector3(0.525731f, -0.850651f, 0.000000f), 
    Vector3(-0.238856f, -0.864188f, -0.442863f),
    Vector3(-0.500000f, -0.809017f, -0.309017f),
    Vector3(-0.262866f, -0.951056f, -0.162460f),
    Vector3(-0.850651f, -0.525731f, 0.000000f), 
    Vector3(-0.716567f, -0.681718f, -0.147621f),
    Vector3(-0.716567f, -0.681718f, 0.147621f), 
    Vector3(-0.525731f, -0.850651f, 0.000000f), 
    Vector3(-0.500000f, -0.809017f, 0.309017f), 
    Vector3(-0.238856f, -0.864188f, 0.442863f), 
    Vector3(-0.262866f, -0.951056f, 0.162460f), 
    Vector3(-0.864188f, -0.442863f, 0.238856f), 
    Vector3(-0.809017f, -0.309017f, 0.500000f), 
    Vector3(-0.688191f, -0.587785f, 0.425325f), 
    Vector3(-0.681718f, -0.147621f, 0.716567f), 
    Vector3(-0.442863f, -0.238856f, 0.864188f), 
    Vector3(-0.587785f, -0.425325f, 0.688191f), 
    Vector3(-0.309017f, -0.500000f, 0.809017f), 
    Vector3(-0.147621f, -0.716567f, 0.681718f), 
    Vector3(-0.425325f, -0.688191f, 0.587785f), 
    Vector3(-0.162460f, -0.262866f, 0.951056f), 
    Vector3(0.442863f, -0.238856f, 0.864188f), 
    Vector3(0.162460f, -0.262866f, 0.951056f), 
    Vector3(0.309017f, -0.500000f, 0.809017f), 
    Vector3(0.147621f, -0.716567f, 0.681718f), 
    Vector3(0.000000f, -0.525731f, 0.850651f), 
    Vector3(0.425325f, -0.688191f, 0.587785f), 
    Vector3(0.587785f, -0.425325f, 0.688191f), 
    Vector3(0.688191f, -0.587785f, 0.425325f), 
    Vector3(-0.955423f, 0.295242f, 0.000000f), 
    Vector3(-0.951056f, 0.162460f, 0.262866f), 
    Vector3(-1.000000f, 0.000000f, 0.000000f), 
    Vector3(-0.850651f, 0.000000f, 0.525731f), 
    Vector3(-0.955423f, -0.295242f, 0.000000f),
    Vector3(-0.951056f, -0.162460f, 0.262866f),
    Vector3(-0.864188f, 0.442863f, -0.238856f),
    Vector3(-0.951056f, 0.162460f, -0.262866f),
    Vector3(-0.809017f, 0.309017f, -0.500000f),
    Vector3(-0.864188f, -0.442863f, -0.238856f),
    Vector3(-0.951056f, -0.162460f, -0.262866f),
    Vector3(-0.809017f, -0.309017f, -0.500000f),
    Vector3(-0.681718f, 0.147621f, -0.716567f), 
    Vector3(-0.681718f, -0.147621f, -0.716567f),
    Vector3(-0.850651f, 0.000000f, -0.525731f), 
    Vector3(-0.688191f, 0.587785f, -0.425325f), 
    Vector3(-0.587785f, 0.425325f, -0.688191f), 
    Vector3(-0.425325f, 0.688191f, -0.587785f), 
    Vector3(-0.425325f, -0.688191f, -0.587785f),
    Vector3(-0.587785f, -0.425325f, -0.688191f),
    Vector3(-0.688191f, -0.587785f, -0.425325f)
};



/* Texture coords */
struct Md2TexCoord
{
	short s;
	short t;

	Vector2 ToVector2(const Md2Header& header) const
	{
		return Vector2(
			(float)s / (float)header.skinWidth,
			(float)t / (float)header.skinHeight);
	}
};

struct Md2Triangle
{
	short vertexIndex[3];   // Vertex indices of the triangle
	short uvIndex[3];       // Texture coordinate indices 
};

struct Md2Vertex
{
	BYTE v[3];                // Compressed vertex (x, y, z) coordinates
	BYTE normalIndex;		  // Index to a normal vector for the lighting
};

struct Md2Frame
{
	float       scale[3];       // Scale values
	float       translate[3];   // Translation vector
	char        name[16];       // Frame name
	Md2Vertex   verts[1];       // First vertex of this frame
};

// Texture name
struct Md2Skin
{
	char name[64];              // texture file name
};

MD2Loader::MD2Loader(void)
{
}

MD2Loader::~MD2Loader(void)
{
}

// ----------------------------------------------
// LoadModel() - load model from file.
// ----------------------------------------------

class MD2Temp_Vertex;
class MD2Temp_Face
{
public:
	OrderedMap<int, MD2Temp_Vertex*> Verts;
};
__declspec(align(16)) class MD2Temp_Vertex
{
public:
	DEFINE_NEW_DELETE_ALIGNED

	Vector3 Position;
    Vector3 Normal;
    int Index;
	bool HasUnifiedTexCoord;
    Vector2 UnifiedTexCoord;
	OrderedMap<const Md2TexCoord*, vector<MD2Temp_Face*>> TexCoords;

	MD2Temp_Vertex(void)
	{
		HasUnifiedTexCoord = false;
		Index = 0;
	}
};

static void LoadFrame(const Md2Header& header, const Md2Frame& frame, const Md2Triangle* triangles, const Md2TexCoord* texCoords, vector<Vertex, AlignAlloc<Vertex, 16>>& outVerts, vector<Face, AlignAlloc<Face, 16>>& outFaces)
{
    vector<MD2Temp_Vertex*> vertices;
    vector<MD2Temp_Face*> faces;

    // Vertex array initialization
    for (int i = 0; i < header.numVertices; ++i)
    {
		const Md2Vertex* vert = &frame.verts[i];

        MD2Temp_Vertex* v = new MD2Temp_Vertex();

        // Calculate position
        v->Position = Vector3(
            (vert->v[0] * frame.scale[0]) + frame.translate[0],
            (vert->v[2] * frame.scale[2]) + frame.translate[2],
            (vert->v[1] * frame.scale[1]) + frame.translate[1]);
                
        // Retrieve normal from the normals array
        v->Normal = Vector3(
			Normals[vert->normalIndex].X,
			Normals[vert->normalIndex].Z,
			Normals[vert->normalIndex].Y);

        // Store index
        v->Index = i;

		vertices.push_back(v);
    }

    // Polygon array initialization
    for (int i = 0; i < header.numTriangles; ++i)
    {
        MD2Temp_Face* f = new MD2Temp_Face();
        const Md2Triangle* pTri = &triangles[i];
        
        // Foreach vertex in this face
        for (int k = 0; k < 3; k++)
        {
            // Get the current vertex index
            short vertIndex = pTri->vertexIndex[k];

            // Add this vertex to the face's vertex dictionary
			f->Verts.Add(vertIndex, vertices[vertIndex]);

            // Retrieve the texture coordinate for this face
            const Md2TexCoord* texCoord = &texCoords[pTri->uvIndex[k]];

            // Create list if it doesn't exist
			if (!f->Verts[vertIndex]->TexCoords.ContainsKey(texCoord))
            {
				f->Verts[vertIndex]->TexCoords.Add(texCoord, vector<MD2Temp_Face*>());
            }

            // Add this face to the vertex's face dictionary
			f->Verts[vertIndex]->TexCoords[texCoord].push_back(f);
        }

		faces.push_back(f);
    }

    // Now, calculate the per vertex texture coords,
    // remapping indices if required

    // Foreach vertex in the frame
	UINT size = vertices.size();
	for (UINT i = 0; i < size; i++)
    {
        MD2Temp_Vertex* currentVert = vertices[i];
        if (!currentVert->HasUnifiedTexCoord)
        {
			if (currentVert->TexCoords.Count() > 1)
            {
                // Allocate first tex coord to current vert
				currentVert->UnifiedTexCoord = currentVert->TexCoords.GetKey(0)->ToVector2(header);

                // For all remaining unique coords
				for (int j = 1; j < currentVert->TexCoords.Count(); j++)
                {
					const Md2TexCoord* currentCoord = currentVert->TexCoords.GetKey(j);

                    // Split the vertex into multiple vertices
                    // each with one of the unique coords
                    MD2Temp_Vertex* newVert = new MD2Temp_Vertex();
					newVert->Index = vertices.size();
                    newVert->Normal = currentVert->Normal;
                    newVert->Position = currentVert->Position;
                    newVert->UnifiedTexCoord = currentCoord->ToVector2(header);
					newVert->HasUnifiedTexCoord = true;

					vertices.push_back(newVert);

                    // Then remap indices to use these additional vertices
					const vector<MD2Temp_Face*>& currentFaces = currentVert->TexCoords[currentCoord];
					UINT numCurrentFaces = currentFaces.size();
					for (UINT t = 0; t < numCurrentFaces; t++)
					{
						currentFaces[t]->Verts[currentVert->Index] = newVert;
					}
                }
            }
			else if (currentVert->TexCoords.Count() == 1)
            {
                // Only 1 tex coord for this vert
				currentVert->UnifiedTexCoord = currentVert->TexCoords.GetKey(0)[0].ToVector2(header);
            }
            else
            {
                // No tex coord for this vert
                currentVert->UnifiedTexCoord = Vector2(0, 0);
            }

			currentVert->HasUnifiedTexCoord = true;
        }
    }

	outVerts.clear();
	outFaces.clear();

	size = vertices.size();
	for (UINT i = 0; i < size; i++)
	{
		outVerts.push_back(Vertex(
			vertices[i]->Position,
			vertices[i]->Normal,
			vertices[i]->UnifiedTexCoord));
	}

	size = faces.size();
	for (UINT i = 0, j = 0; j < size; j++)
	{
		Face f;

		for (int x = 0; x < 3; x++, i++)
		{
			f.SetIndex(x, faces[j]->Verts[faces[j]->Verts.GetKey(x)]->Index);
		}

		const int* inds = f.GetIndices();

		// Calculate face normal
		f.SetFaceNormal(Vector3::Normalize(Vector3::Cross(
			outVerts[inds[1]].Position - outVerts[inds[0]].Position,
			outVerts[inds[2]].Position - outVerts[inds[0]].Position)));

		outFaces.push_back(f);
	}

	// Deallocate all temporary structures
	size = vertices.size();
	for (UINT i = 0; i < size; i++)
	{
		delete vertices[i];
	}

	size = faces.size();
	for (UINT i = 0; i < size; i++)
	{
		delete faces[i];
	}
}

bool MD2Loader::LoadModel(const char* filename, UINT maxFramesToLoad, Model** ppModel, Md2Header* pHeader)
{
	ifstream   file;           

	// Try to open MD2 file
	file.open( filename, ios::in | ios::binary );
	if(file.fail())
	{
		return false;
	}
	// Read file header
	Md2Header header;         
	file.read( reinterpret_cast<char*>(&header), sizeof( Md2Header ) );
		
	// Verify that this is a MD2 file (check for the magic number and version number)
	if( (header.indent != MD2_IDENT) && (header.version != MD2_VERSION) )
	{
		// This is not a MD2 model
		file.close();
		return false;
	}

	// Allocate the memory we need
	Md2Triangle* triangles = new Md2Triangle[header.numTriangles];
	Md2TexCoord* texCoords = new Md2TexCoord[header.numTexCoords];
	Md2Skin* skins = new Md2Skin[header.numSkins];

	// We are only interested in the first frame 
	BYTE* frameBuffer = new BYTE[ header.frameSize ];
	Md2Frame* frame = reinterpret_cast<Md2Frame*>(frameBuffer);

	// Read Skin data...
	file.seekg( header.offsetSkins, ios::beg );
	file.read( reinterpret_cast<char*>(skins), header.numSkins * sizeof(Md2Skin) );

	// Read polygon data...
	file.seekg( header.offsetTriangles, ios::beg );
	file.read( reinterpret_cast<char*>(triangles), sizeof(Md2Triangle) * header.numTriangles );	
		
	// Read tex coord data...
	file.seekg( header.offsetTexCoords, ios::beg );
	file.read( reinterpret_cast<char*>(texCoords), header.numTexCoords * sizeof(Md2TexCoord) );

	vector<ModelFrame> modelFrames;

	// Get the number of frames to read from the file
	// If 0 is specified, load all frames
	UINT numFramesToLoad = maxFramesToLoad ? min(maxFramesToLoad, (UINT)header.numFrames) : (UINT)header.numFrames;

	// Seek to frame data
	file.seekg( header.offsetFrames, ios::beg );
	for (UINT i = 0; i < numFramesToLoad; i++)
	{
		// Read frame data...
		file.read( reinterpret_cast<char*>(frame), header.frameSize );

		vector<Vertex, AlignAlloc<Vertex, 16>> verts;
		vector<Face, AlignAlloc<Face, 16>> faces;
		
		LoadFrame(header, *frame, triangles, texCoords, verts, faces);

		modelFrames.push_back(ModelFrame(verts, faces));
	}

	vector<const Texture2D*> textures;
	BYTE* texMem = new BYTE[header.skinWidth * header.skinHeight];
	for (int i = 0; i < header.numSkins; i++)
	{
		Md2Skin& skin = skins[i];

		//bool MD2Loader::LoadPCX(const char* filename, BYTE* texture, DWORD* palette, const Md2Header* md2Header)
		DWORD palette[256];
		if (LoadPCX(skin.name, texMem, palette, &header))
		{
			textures.push_back(new Texture2D(header.skinWidth, header.skinHeight, texMem, palette));
		}
	}
	delete [] texMem;
	
	*ppModel = new Model(modelFrames, textures);
	if (pHeader)
	{
		*pHeader = header;
	}

	// Free dynamically allocated memory
	delete [] skins;
	delete [] triangles; // NOTE: this is 'array' delete. Must be sure to use this
	delete [] texCoords;
	triangles = 0;

	delete [] frameBuffer;
	frameBuffer = 0;
	frame = 0;

	// Close the file 
	file.close();

	return true;
}

struct PcxHeader
{
	BYTE  ID;
	BYTE  Version;
	BYTE  Encoding;
	BYTE  BitsPerPixel;
	short XMin;
	short YMin;
	short XMax;
	short YMax;
	short HRes;
	short VRes;
	BYTE  ClrMap[16*3];
	BYTE  Reserved;
	BYTE  NumPlanes;
	short BytesPerLine;
	short Pal;
	BYTE  Filler[58];
};

// ----------------------------------------------
// LoadPCX() - load pcx texture from file.
//
// assume the caller has allocated enough memory for texture - md2Header->skinWidth * md2Header->skinHeight
// ----------------------------------------------
bool MD2Loader::LoadPCX(const char* filename, BYTE* texture, DWORD* palette, const Md2Header* md2Header)
{
	std::ifstream   file;   // file stream

	// try to open filename
    file.open( filename, std::ios::in | std::ios::binary );

    if( file.fail() )
        return false;

    // read md2 header file
    PcxHeader header;         
	file.read( reinterpret_cast<char*>(&header), sizeof( PcxHeader ) );

    /////////////////////////////////////////////
    //      verify that this is a valid PCX file

	// only handle those with 256 colour palette
	if( (header.Version != 5) || (header.BitsPerPixel != 8) || 
		(header.Encoding != 1) || (header.NumPlanes != 1) ||
		(md2Header && (header.BytesPerLine != md2Header->skinWidth) ) )
    {
        // this is not valid supported PCX
        file.close();
        return false;
    }

    /////////////////////////////////////////////
	//		check dimensions

	int XSize = header.XMax - header.XMin + 1;
	int YSize = header.YMax - header.YMin + 1;
	int Size = XSize * YSize;

	// check matches our MD2 expected texture
	// note. valid size is <= because uses RLE (so potentially smaller)
	if ( md2Header && ( Size > (md2Header->skinHeight * md2Header->skinWidth) ) )
	{
		// doesn't match expected MD2 skin size
		file.close();
		return false;
	}
	
	/////////////////////////////////////////////
    //          reading file data

	BYTE processByte, colourByte;
	int count = 0;
	while(count < Size)
	{
		file.read( reinterpret_cast<char*>(&processByte), 1 );

		// Run length encoding - test if byte is an RLE byte
		if ((processByte & 192) == 192)
		{
			// extract number of times repeated byte
			processByte &= 63;
			file.read( reinterpret_cast<char*>(&colourByte), 1 );
			for (int index = 0; index < processByte; ++index)
			{
				// repeatedly write colour 
				texture[count] = colourByte;
				++count;
			}
		}
		else
		{
			// process byte is the colour
			texture[count] = processByte;
			++count;
		}
	}

	bool returnValue = false;

	// read palette data...
	file.seekg( -769, std::ios::end);	// from this offset from end of file
	file.read( reinterpret_cast<char*>(&processByte), 1 );
	if (processByte == 12)
	{
		BYTE rawPalette[768];
		file.read( reinterpret_cast<char*>(&rawPalette), 768 );

		// build palette
		for (int palIndex = 0; palIndex < 256; ++palIndex)
		{			
			palette[palIndex] = RGB(rawPalette[ palIndex * 3],
									rawPalette[(palIndex * 3) + 1],
									rawPalette[(palIndex * 3) + 2] );
		}

		returnValue = true;
	}

	// all done
	file.close();
	return returnValue;
}
