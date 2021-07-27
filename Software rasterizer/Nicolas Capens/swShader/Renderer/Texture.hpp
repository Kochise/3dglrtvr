#ifndef Texture_hpp
#define Texture_hpp

#include "Bitmap.hpp"

namespace swShader
{
	class Rasterizer;
	class PS20Assembler;

	class Texture : public Bitmap
	{
		friend Rasterizer;
		friend PS20Assembler;

	public:
		Texture();
		Texture(int width, int height, int bitDepth = 32);
		Texture(const char *fileName);

		~Texture();

	protected:
		void createMipmaps();
		int mipmapsSize();
		void swapRedBlue();   // ABGR <-> ARGB

		unsigned int *mipmap[16];
		__int64 uInt[16];
		__int64 vInt[16];
		__int64 uFrac[16];
		__int64 vFrac[16];
		short uHalf[16][4];
		short vHalf[16][4];

	private:
		void initTables();

		static void halveWidth(unsigned int *source, unsigned int *dest, int sourceWidth, int sourceHeight);
		static void halveHeight(unsigned int *source, unsigned int *dest, int sourceWidth, int sourceHeight);
		static void halveWidthHeight(unsigned int *source, unsigned int *dest, int sourceWidth, int sourceHeight);
	};
}

#endif   // Texture_hpp