#include "Texture.hpp"

#include "Color.hpp"
#include "Math.hpp"

#include <stdlib.h>

namespace swShader
{
	Texture::Texture()
	{
		Color<byte>::setGamma(2.2f);
	}

	Texture::Texture(int width, int height, int bitDepth) : Bitmap(width, height, bitDepth)
	{
		Color<byte>::setGamma(2.2f);
		createMipmaps();
		swapRedBlue();
	}

	Texture::Texture(const char *fileName) : Bitmap(fileName)
	{
		Color<byte>::setGamma(2.2f);
		createMipmaps();
		swapRedBlue();
	}

	Texture::~Texture()
	{
	}

	void Texture::createMipmaps()
	{
		const int size = mipmapsSize();
		resize(size * bitDepth / 8);
		initTables();

		// Start position in subtexture matrix
		int m = 16 - log2(width);
		int n = 16 - log2(height);

		for(int i = m + 1, j = n + 1; i < 16 && j < 16; i++, j++)
		{
			int ij = max(i, j);
			int mn = max(m, n);

			int width = exp2(16 - m);
			int height = exp2(16 - n);

			mipmap[ij] = mipmap[mn] + width * height;
			uInt[ij] = 16 - i;
			vInt[ij] = 16 - j;
			uFrac[ij] = i;
			vFrac[ij] = j;

			short __11[4] = {-1, -1, 1, 1};
			short _1_1[4] = {-1, 1, -1, 1};

			uHalf[ij][0] = _1_1[0] << (short)(uFrac[ij] - 1);
			uHalf[ij][1] = _1_1[1] << (short)(uFrac[ij] - 1);
			uHalf[ij][2] = _1_1[2] << (short)(uFrac[ij] - 1);
			uHalf[ij][3] = _1_1[3] << (short)(uFrac[ij] - 1);

			vHalf[ij][0] = __11[0] << (short)(vFrac[ij] - 1);
			vHalf[ij][1] = __11[1] << (short)(vFrac[ij] - 1);
			vHalf[ij][2] = __11[2] << (short)(vFrac[ij] - 1);
			vHalf[ij][3] = __11[3] << (short)(vFrac[ij] - 1);

			if(i < 16 && j < 16)
			{
				halveWidthHeight(mipmap[mn], mipmap[ij], width, height);
			}
			else if(i < 16)
			{
				halveWidth(mipmap[mn], mipmap[ij], width, height);
			}
			else
			{
				halveHeight(mipmap[mn], mipmap[ij], width, height);
			}

			m = i;
			n = j;
		}
	}

	int Texture::mipmapsSize()
	{
		// Start position in subtexture matrix
		int m = 16 - log2(width);
		int n = 16 - log2(height);

		int size = width * height;

		for(int i = m + 1, j = n + 1; i < 16 && j < 16; i++, j++)
		{
			int ij = max(i, j);
			int mn = max(m, n);

			size += exp2(16 - i) * exp2(16 - j);

			m = i;
			n = j;
		}

		return size;
	}

	void Texture::swapRedBlue()
	{
		const int size = mipmapsSize();

		for(int i = 0; i < size; i++)
		{
			Color<byte> s = ((dword*)colorBuffer)[i];
			Color<byte> d = Color<byte>(s.b, s.g, s.r, s.a);
			((dword*)colorBuffer)[i] = d;
		}
	}

	void Texture::halveWidth(unsigned int *source, unsigned int *dest, int sourceWidth, int sourceHeight)
	{
		int destWidth = sourceWidth / 2;
		int destHeight = sourceHeight;

		for(int u = 0; u < destWidth; u++)
		{
			for(int v = 0; v < destHeight; v++)
			{
				Color<byte> c1 = source[(2 * u + 0) + (2 * v + 0) * sourceWidth];
				Color<byte> c2 = source[(2 * u + 1) + (2 * v + 0) * sourceWidth];

				dest[u + v * destWidth] = Color<byte>::average(c1, c2);
			}
		}
	}

	void Texture::halveHeight(unsigned int *source, unsigned int *dest, int sourceWidth, int sourceHeight)
	{
		int destWidth = sourceWidth;
		int destHeight = sourceHeight / 2;

		for(int u = 0; u < destWidth; u++)
		{
			for(int v = 0; v < destHeight; v++)
			{
				Color<byte> c1 = source[(2 * u + 0) + (2 * v + 0) * sourceWidth];
				Color<byte> c2 = source[(2 * u + 0) + (2 * v + 1) * sourceWidth];

				dest[u + v * destWidth] = Color<byte>::average(c1, c2);
			}
		}
	}

	void Texture::halveWidthHeight(unsigned int *source, unsigned int *dest, int sourceWidth, int sourceHeight)
	{
		int destWidth = sourceWidth / 2;
		int destHeight = sourceHeight / 2;

		for(int u = 0; u < destWidth; u++)
		{
			for(int v = 0; v < destHeight; v++)
			{
				Color<byte> c1 = source[(2 * u + 0) + (2 * v + 0) * sourceWidth];
				Color<byte> c2 = source[(2 * u + 1) + (2 * v + 0) * sourceWidth];
				Color<byte> c3 = source[(2 * u + 0) + (2 * v + 1) * sourceWidth];
				Color<byte> c4 = source[(2 * u + 1) + (2 * v + 1) * sourceWidth];

				dest[u + v * destWidth] = Color<byte>::average(Color<byte>::average(c1, c2), Color<byte>::average(c3, c4));
			}
		}
	}

	void Texture::initTables()
	{
		// Make all subtextures equal to top level
		for(int i = 0; i < 16; i++)
		{
			mipmap[i] = (unsigned int*)colorBuffer;
			uInt[i] = log2(width);
			vInt[i] = log2(height);
			uFrac[i] = 16 - log2(width);
			vFrac[i] = 16 - log2(height);

			short __11[4] = {-1, -1, 1, 1};
			short _1_1[4] = {-1, 1, -1, 1};

			uHalf[i][0] = _1_1[0] << (short)(uFrac[i] - 1);
			uHalf[i][1] = _1_1[1] << (short)(uFrac[i] - 1);
			uHalf[i][2] = _1_1[2] << (short)(uFrac[i] - 1);
			uHalf[i][3] = _1_1[3] << (short)(uFrac[i] - 1);

			vHalf[i][0] = __11[0] << (short)(vFrac[i] - 1);
			vHalf[i][1] = __11[1] << (short)(vFrac[i] - 1);
			vHalf[i][2] = __11[2] << (short)(vFrac[i] - 1);
			vHalf[i][3] = __11[3] << (short)(vFrac[i] - 1);
		}
	}
}
