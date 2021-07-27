#ifndef Bitmap_hpp
#define Bitmap_hpp

#include "ColorBuffer.hpp"
#include "Types.hpp"

namespace swShader
{
	class File;

	class Bitmap : public ColorBuffer
	{
	public:
		Bitmap();
		Bitmap(int width, int height, int bitDepth);
		Bitmap(const char *fileName);

		virtual ~Bitmap();

		Bitmap &operator=(const Bitmap &bitmap);

		void *getColorBuffer();
		int getWidth();
		int getHeight();
		int getBitDepth();

		void load(const char *fileName);

		void stretchNearest(int newWidth, int newHeight);
		void stretchBilerp(int newWidth, int newHeight);

	protected:
		void loadTGA(const File &TGAFile);
		void loadTGA32_32(byte *fileBuffer);
		void loadTGA24_32(byte *fileBuffer);
		void loadTGA16_32(byte *fileBuffer);
		void loadTGA8_32(byte *fileBuffer);

		void loadJPG_32(const File &JPGFile);

		void scaleHorizontal(int newWidth);
		void scaleVertical(int newHeight);
		void scaleLine(unsigned int *newData, unsigned int *oldData, int oldIncrement, int oldLength, int newLength);

		void resize(int bytes);
		void resize(int width, int height);

		void *colorBuffer;
		int width;
		int height;
		int bitDepth;
	};
}

#endif   // Bitmap_hpp