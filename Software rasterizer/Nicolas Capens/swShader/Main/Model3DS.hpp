#ifndef Model3DS_hpp
#define Model3DS_hpp

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

#include "Color.hpp"
#include "Vector.hpp"

namespace swShader
{
	class File;

	class Model3DS : public VertexBuffer, public IndexBuffer
	{
	public:
		Model3DS(const char *fileName);

		~Model3DS();

	private:
		struct ChunkHeader
		{
		public:
			ChunkHeader();

			~ChunkHeader();

			unsigned short ID;
			int size;
		};

		File *file;
		void load(const char *fileName);

		void readChunk(ChunkHeader &parentChunk);
		void readModelDataChunk(ChunkHeader &parentChunk);
		void readNamedObjectChunk(ChunkHeader &parentChunk);
		void readMeshChunk(ChunkHeader &parentChunk);
		void readVertexPositionChunk(ChunkHeader &parentChunk);
		void readFaceChunk(ChunkHeader &parentChunk);
		void readTexCoordChunk(ChunkHeader &parentChunk);
	
		void skipChunk(ChunkHeader &chunkHeader);
		void readChunkHeader(ChunkHeader &chunkHeader);

		Color<float> readColorChunk(ChunkHeader &parentChunk);
		float readPercentage(ChunkHeader &parentChunk);

		Vector readVector();
	};
}

#endif   // Model3DS_hpp