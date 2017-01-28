#include "Model3DS.hpp"

#include "File.hpp"
#include "Error.hpp"
#include "FVF.hpp"

namespace swShader
{
	Model3DS::ChunkHeader::ChunkHeader()
	{
		ID = 0;
		size = 0;
	}

	Model3DS::ChunkHeader::~ChunkHeader()
	{
	}

	Model3DS::Model3DS(const char *fileName) : VertexBuffer(FVF_POSITION | FVF_TEX1)
	{
		file = 0;
		load(fileName);
	}

	Model3DS::~Model3DS()
	{
		delete file;
	}

	void Model3DS::load(const char *fileName)
	{
		if(!fileName) throw Error("No 3DS file specified");

		try
		{
			file = new File(fileName);

			ChunkHeader main;
			readChunkHeader(main);
			readChunk(main);
		}
		catch(const Error &error)
		{
			throw Error("Could not load 3DS file '%s'", fileName) << error;
		}
		catch(...)
		{
			throw Error("Could not load 3DS file '%s'", fileName);
		}
	}

	void Model3DS::readChunk(ChunkHeader &parentChunk)
	{
		if(parentChunk.ID != 0x4D4D) throw INTERNAL_ERROR;

		do
		{
			ChunkHeader currentChunk;
			readChunkHeader(currentChunk);
			parentChunk.size -= currentChunk.size + 6;

			switch(currentChunk.ID)
			{
			case 0x3D3D:
				readModelDataChunk(currentChunk);
				break;
			default:
				skipChunk(currentChunk);
				break;
			}

			if(currentChunk.size != 0) throw INTERNAL_ERROR;
		}
		while(parentChunk.size > 0);

		if(parentChunk.size != 0) throw INTERNAL_ERROR;
	}

	void Model3DS::readModelDataChunk(ChunkHeader &parentChunk)
	{
		if(parentChunk.ID != 0x3D3D) throw INTERNAL_ERROR;

		do
		{
			ChunkHeader currentChunk;
			readChunkHeader(currentChunk);
			parentChunk.size -= currentChunk.size + 6;

			switch(currentChunk.ID)
			{
			case 0x4000:
				readNamedObjectChunk(currentChunk);
				break;
			default:
				skipChunk(currentChunk);
				break;
			}

			if(currentChunk.size != 0) throw INTERNAL_ERROR;
		}
		while(parentChunk.size > 0);

		if(parentChunk.size != 0) throw INTERNAL_ERROR;
	}

	void Model3DS::readNamedObjectChunk(ChunkHeader &parentChunk)
	{
		if(parentChunk.ID != 0x4000) throw INTERNAL_ERROR;

		char objectName[256];
		unsigned int nameLength;

		nameLength = file->readString(objectName);
		parentChunk.size -= nameLength + 1;

		do
		{
			ChunkHeader currentChunk;
			readChunkHeader(currentChunk);
			parentChunk.size -= currentChunk.size + 6;

			switch(currentChunk.ID)
			{
			case 0x4100:
				readMeshChunk(currentChunk);
				break;
			default:
				skipChunk(currentChunk);
				break;
			}

			if(currentChunk.size != 0) throw INTERNAL_ERROR;
		}
		while(parentChunk.size > 0);

		if(parentChunk.size != 0) throw INTERNAL_ERROR;
	}

	void Model3DS::readMeshChunk(ChunkHeader &parentChunk)
	{
		if(parentChunk.ID != 0x4100) throw INTERNAL_ERROR;

		do
		{
			ChunkHeader currentChunk;
			readChunkHeader(currentChunk);
			parentChunk.size -= currentChunk.size + 6;

			switch(currentChunk.ID)
			{
			case 0x4110:
				readVertexPositionChunk(currentChunk);
				break;
			case 0x4120:
				readFaceChunk(currentChunk);
				break;
			case 0x4140:
				readTexCoordChunk(currentChunk);
				break;
			default:
				skipChunk(currentChunk);
				break;
			}

			if(currentChunk.size != 0) throw INTERNAL_ERROR;
		}
		while(parentChunk.size > 0);

		if(parentChunk.size != 0) throw INTERNAL_ERROR;
	}

	void Model3DS::readVertexPositionChunk(ChunkHeader &parentChunk)
	{
		if(parentChunk.ID != 0x4110) throw INTERNAL_ERROR;

		int n = VertexBuffer::size();
		int m = (int)file->readWord();
		parentChunk.size -= 2;

		VertexBuffer::realloc(n + m);

		for(int i = n; i < n + m; i++)
		{
			Point P = readVector();
			parentChunk.size -= 12;

			Px[i] = P.x;
			Py[i] = P.y;
			Pz[i] = P.z;
		}

		if(parentChunk.size != 0) throw INTERNAL_ERROR;
	}

	void Model3DS::readFaceChunk(ChunkHeader &parentChunk)
	{
		if(parentChunk.ID != 0x4120) throw INTERNAL_ERROR;

		int n = IndexBuffer::size();
		int m = (int)file->readWord();
		parentChunk.size -= 2;

		IndexBuffer::realloc(n + m);

		for(int i = n; i < n + m; i++)
		{
			face[i][0] = file->readWord();
			face[i][1] = file->readWord();
			face[i][2] = file->readWord();
			unsigned short faceFlag = file->readWord();
			parentChunk.size -= 8;
		}

		do
		{
			ChunkHeader currentChunk;
			readChunkHeader(currentChunk);
			parentChunk.size -= currentChunk.size + 6;

			switch(currentChunk.ID)
			{
			case 0x4150:
				skipChunk(currentChunk);
				break;
			default:
				skipChunk(currentChunk);
				break;
			}

			if(currentChunk.size != 0) throw INTERNAL_ERROR;
		}
		while(parentChunk.size > 0);

		if(parentChunk.size != 0) throw INTERNAL_ERROR;
	}

	void Model3DS::readTexCoordChunk(ChunkHeader &parentChunk)
	{
		if(parentChunk.ID != 0x4140) throw INTERNAL_ERROR;

		int n = VertexBuffer::size();
		int m = (int)file->readWord();
		parentChunk.size -= 2;

		if(m > n) throw INTERNAL_ERROR;

		// HACK: Generally can't assume same number of uv than vertices or even correstponding
		for(int i = n - m; i < n; i++)
		{
			Tu[0][i] = file->readFloat();
			Tv[0][i] = file->readFloat();
			parentChunk.size -= 8;
		}

		if(parentChunk.size != 0) throw INTERNAL_ERROR;
	}

	Color<float> Model3DS::readColorChunk(ChunkHeader &parentChunk)
	{
		Color<float> c;

		ChunkHeader currentChunk;
		readChunkHeader(currentChunk);
		parentChunk.size -= currentChunk.size + 6;

		switch(currentChunk.ID)
		{
		case 0x0010:
			c.r = file->readFloat();
			c.g = file->readFloat();
			c.b = file->readFloat();
			currentChunk.size -= 12;
			break;
		case 0x0011:
			c.r = (float)file->readByte() / 255.0f;
			c.g = (float)file->readByte() / 255.0f;
			c.b = (float)file->readByte() / 255.0f;
			currentChunk.size -= 3;
			break;
		case 0x0012:
			c.r = (float)file->readByte() / 255.0f;
			c.g = (float)file->readByte() / 255.0f;
			c.b = (float)file->readByte() / 255.0f;
			currentChunk.size -= 3;
			break;
		case 0x0013:
			c.r = file->readFloat();
			c.g = file->readFloat();
			c.b = file->readFloat();
			currentChunk.size -= 12;
			break;
		default:
			skipChunk(currentChunk);
			break;
		}
		
		if(parentChunk.size > 0)   // More color chunks might follow...
		{
			skipChunk(parentChunk);   // ...just skip them.
		}

		return c;
	}

	float Model3DS::readPercentage(ChunkHeader &parentChunk)
	{
		float p;

		ChunkHeader currentChunk;
		readChunkHeader(currentChunk);
		parentChunk.size -= currentChunk.size + 6;

		switch(currentChunk.ID)
		{
		case 0x0030:
			p = (float)file->readWord();
			currentChunk.size -= 12;
			break;
		case 0x0031:
			p = file->readFloat();
			currentChunk.size -= 4;
			break;
		default:
			skipChunk(currentChunk);
			break;
		}
		
		if(parentChunk.size != 0) throw INTERNAL_ERROR;

		return p;
	}

	void Model3DS::skipChunk(ChunkHeader &chunkHeader)
	{
		if(chunkHeader.size < 0) throw INTERNAL_ERROR;

		file->seekCur(chunkHeader.size);
		chunkHeader.size = 0;
	}

	void Model3DS::readChunkHeader(ChunkHeader &currentChunk)
	{
		file->read(&currentChunk.ID, sizeof(currentChunk.ID), 1);
		file->read(&currentChunk.size, sizeof(currentChunk.size), 1);
		currentChunk.size -= 6;
	}

	Vector Model3DS::readVector()
	{
		Vector v;

		v.x = file->readFloat();
		v.y = file->readFloat();
		v.z = file->readFloat();

		return v;
	}
}