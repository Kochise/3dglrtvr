
#pragma once

#include "Vertex.h"

struct Md2Header
{
	int indent;               // The magic number used to identify the file.
	int version;              // The file version number (must be 8).
	int skinWidth;            // The width in pixels of our image.
	int skinHeight;           // The height in pixels of our image.
	int frameSize;            // The size in bytes the frames are.
	int numSkins;             // The number of skins associated with the model.
	int numVertices;		  // The number of vertices.
	int numTexCoords;		  // The number of texture coordinates.
	int numTriangles;		  // The number of faces (polygons).
	int numGlCommands;        // The number of gl commands.
	int numFrames;			  // The number of animated frames.
	int offsetSkins;		  // The offset in the file for the skin data.
	int offsetTexCoords;	  // The offset in the file for the texture data.
	int offsetTriangles;	  // The offset in the file for the face data.
	int offsetFrames;		  // The offset in the file for the frames data.
	int offsetGlCommands;	  // The offset in the file for the gl commands data.
	int offsetEnd;            // The end of the file offset.
};

class Model;

class MD2Loader
{
public:
	MD2Loader(void);
	~MD2Loader(void);
	static bool LoadModel(const char* md2Filename, UINT maxFramesToLoad, Model** ppModel, Md2Header* pHeader);
	static bool LoadPCX(const char* filename, BYTE* texture, DWORD* palette, const Md2Header* md2Header);
};
