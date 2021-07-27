#include "Bitmap.hpp"

#include "File.hpp"
#include "String.hpp"
#include "Color.hpp"
#include "Error.hpp"

#include "cdjpeg.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

namespace swShader
{
	Bitmap::Bitmap()
	{
		colorBuffer = 0;
		width = 0;
		height = 0;
		bitDepth = 0;
	}

	Bitmap::Bitmap(int width, int height, int bitDepth)
	{
		colorBuffer = malloc(width * height * bitDepth / 8);
		this->width = width;
		this->height = height;
		this->bitDepth = bitDepth;
	}

	Bitmap::Bitmap(const char *fileName)
	{
		colorBuffer = 0;
		width = 0;
		height = 0;
		bitDepth = 0;

		load(fileName);
	}

	Bitmap::~Bitmap()
	{
		free(colorBuffer);
		colorBuffer = 0;
		width = 0;
		height = 0;
		bitDepth = 0;
	}

	void Bitmap::load(const char *fileName)
	{
		free(colorBuffer);   // Clear any previous image
		colorBuffer = 0;

		try
		{
			File bitmapFile(fileName);

			if(bitmapFile.hasExtension("tga"))
			{
				loadTGA(bitmapFile);
			}
			else if(bitmapFile.hasExtension("jpg"))
			{
				loadJPG_32(bitmapFile);
			}
			else
			{
				throw Error("Bitmap extension not supported: %s", bitmapFile.getFullPath());
			}
		}
		catch(...)
		{
			free(colorBuffer);

			colorBuffer = malloc(4);
			*(Color<byte>*)colorBuffer = 0x00000000;
			width = 1;
			height = 1;
			bitDepth = 32;
		}
	}

	void Bitmap::loadTGA(const File &TGAFile)
	{
		byte *fileBuffer = (byte*)TGAFile.getBuffer();

		width = ((short*)fileBuffer)[12 / 2];
		height = ((short*)fileBuffer)[14 / 2];
		bitDepth = fileBuffer[16];

		switch(bitDepth)
		{
		case 8:
			loadTGA8_32(fileBuffer);
			break;
		case 16:
			loadTGA16_32(fileBuffer);
			break;
		case 24:
			loadTGA24_32(fileBuffer);
			break;
		case 32:
			loadTGA32_32(fileBuffer);
			break;
		default:
			throw Error("Bitmap bit depth not supported: %d", bitDepth);
		}

		delete[] fileBuffer;
	}

	void Bitmap::loadTGA32_32(byte *fileBuffer)
	{
		colorBuffer = malloc(4 * width * height);

		byte *file = fileBuffer + 18;
		byte *dest = (byte*)colorBuffer;

		unsigned int texel = width * height;

		if(fileBuffer[2] != 10)   // Uncompressed
		{
			while(texel-- > 0)
			{
				*dest++ = file[0];
				*dest++ = file[1];
				*dest++ = file[2];
				*dest++ = file[3];

				file += 4;
			}
		}
		else
		{
			while(texel > 0)
			{			
				int repeat = *file++;

				if(repeat > 127)
				{
					repeat -= 127;
					texel -= repeat;

					while(repeat-- > 0)
					{				
						*dest++ = file[0];
						*dest++ = file[1];
						*dest++ = file[2];
						*dest++ = file[3];
					}

					file += 4;
				}
				else
				{
					repeat++;
					texel -= repeat;

					while(repeat-- > 0)
					{
						*dest++ = file[0];
						*dest++ = file[1];
						*dest++ = file[2];
						*dest++ = file[3];

						file += 4;
					}
				}		
			}
		}
	}

	void Bitmap::loadTGA24_32(byte *fileBuffer)
	{
		colorBuffer = malloc(4 * width * height);

		byte *file = fileBuffer + 18;
		byte *dest = (byte*)colorBuffer;

		unsigned int texel = width * height;

		if(fileBuffer[2] != 10)   // Uncompressed
		{		
			while(texel-- > 0)
			{
				*dest++ = file[0];
				*dest++ = file[1];
				*dest++ = file[2];
				*dest++ = 255;

				file += 3;
			}
		}
		else
		{
			while(texel > 0)
			{			
				int repeat = *file++;

				if(repeat > 127)
				{
					repeat -= 127;
					texel -= repeat;

					while(repeat-- > 0)
					{				
						*dest++ = file[0];
						*dest++ = file[1];
						*dest++ = file[2];
						*dest++ = 255;
					}

					file += 3;
				}
				else
				{
					repeat++;
					texel -= repeat;

					while(repeat-- > 0)
					{
						*dest++ = file[0];
						*dest++ = file[1];
						*dest++ = file[2];
						*dest++ = 255;

						file += 3;
					}
				}		
			}
		}
	}

	void Bitmap::loadTGA16_32(byte *fileBuffer)
	{
		colorBuffer = malloc(4 * width * height);

		byte *file = fileBuffer + 18;
		byte *dest = (byte*)colorBuffer;
		
		unsigned int texel = width * height;

		if(fileBuffer[2] != 10)   // Uncompressed
		{
			while(texel-- > 0)
			{
				unsigned short C = file[1] * 256 + file[0];
				
				*dest++ = (C & 31) << 3;
				*dest++ = ((C >> 5) & 31) << 3;
				*dest++ = ((C >> 10) & 31) << 3;
				*dest++ = 255;
				
				file += 2;
			}
		}
		else
		{
			while(texel > 0)
			{			
				int repeat = *file++;

				if(repeat > 127)
				{
					repeat -= 127;
					texel -= repeat;

					unsigned short C = file[1] * 256 + file[0];

					while (repeat-- > 0)
					{				
						*dest++ = (C & 31) << 3;
						*dest++ = ((C >> 5) & 31) << 3;
						*dest++ = ((C >> 10) & 31) << 3;
						*dest++ = 255;
					}
					
					file += 2;
				}
				else
				{
					repeat ++;
					texel -= repeat;

					while (repeat-- > 0)
					{
						unsigned short C = file[1] * 256 + file[0];
						
						*dest++ = (C & 31) << 3;
						*dest++ = ((C >> 5) & 31) << 3;
						*dest++ = ((C >> 10) & 31) << 3;
						*dest++ = 255;
						
						file += 2;
					}
				}		
			}
		}
	}

	void Bitmap::loadTGA8_32(byte *fileBuffer)
	{
		colorBuffer = malloc(4 * width * height);

		byte *palette = new byte[3 * 256];
		memcpy(palette, fileBuffer + 18, 3 * 256);

		byte *file = fileBuffer + 18 + 3 * 256;
		byte *dest = (byte*)colorBuffer;

		unsigned int texel = width * height;

		if(fileBuffer[2] == 1)   // Uncompressed
		{
			while(texel-- > 0)
			{
				unsigned char C = *file++;

				*dest++ = palette[C * 3 + 0];
				*dest++ = palette[C * 3 + 1];
				*dest++ = palette[C * 3 + 2];
				*dest++ = 255;
			}
		}
		else
		{
			while(texel > 0)
			{
				int repeat = *file++;
				
				if(repeat > 127) 
				{
					repeat -= 127; 
					texel -= repeat;

					unsigned char C = *file++;

					while(repeat-- > 0)
					{
						*dest++ = palette[C * 3 + 0];
						*dest++ = palette[C * 3 + 1];
						*dest++ = palette[C * 3 + 2];
						*dest++ = 255;		
					}
				}
				else
				{
					repeat++;
					texel -= repeat;

					while (repeat-- > 0)
					{
						unsigned char C = *file++;

						*dest++ = palette[C * 3 + 0];
						*dest++ = palette[C * 3 + 1];
						*dest++ = palette[C * 3 + 2];
						*dest++ = 255;
					}
				}
			}
		}

		delete[] palette;
	}

	void Bitmap::loadJPG_32(const File &JPGFile)
	{
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;
		JSAMPARRAY scanline;
		int i;

		int jmpret = setjmp(*jerror_env());

		char *mfbuf = (char*)JPGFile.getBuffer();
		int fsize = JPGFile.length();

		if(jmpret) throw Error("Invalid JPEG file");

		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);

		jpeg_stdio_src(&cinfo, mfbuf, fsize);
		jpeg_read_header(&cinfo, TRUE);

		jpeg_calc_output_dimensions(&cinfo);

		if((cinfo.output_components != 3) && (cinfo.out_color_space != JCS_GRAYSCALE))
		{
			jpeg_destroy_decompress(&cinfo);
			throw Error("JPEG format not supported");
		}

		bitDepth = cinfo.output_components * 8;

		int pitch = cinfo.output_width * cinfo.output_components;
		width = cinfo.output_width;
		height = cinfo.output_height;

		if(cinfo.out_color_space == JCS_GRAYSCALE)
		{
			bitDepth *= 3;
		}

		colorBuffer = malloc(4 * width * height);

		if(!colorBuffer)
		{
			jpeg_destroy_decompress(&cinfo);
			throw INTERNAL_ERROR;
		}

		scanline = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, pitch, 1);
		(void)jpeg_start_decompress(&cinfo);

		while(cinfo.output_scanline < cinfo.output_height)
		{
			byte *dest = (byte*)colorBuffer + 4 * (height - cinfo.output_scanline - 1) * width;

			jpeg_read_scanlines(&cinfo, scanline, 1);
			if(cinfo.out_color_space == JCS_GRAYSCALE)
			{
				for(i = 0; i < pitch; i++)
				{
					*dest++ = *(scanline[0] + i);
					*dest++ = *(scanline[0] + i);
	 				*dest++ = *(scanline[0] + i);
					*dest++ = 255;
				}
			}
			else
			{
				for(i = 0; i < pitch; i += 3)
				{
					*dest++ = *(scanline[0] + i + 0);
					*dest++ = *(scanline[0] + i + 1);
	 				*dest++ = *(scanline[0] + i + 2);
					*dest++ = 255;
				}
			}
		}

		bitDepth = 32;

		(void)jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);

		delete[] mfbuf;
	}

	void Bitmap::scaleLine(unsigned int *newData, unsigned int *oldData, int increment, int oldLength, int newLength)
	{
		int step = increment * (oldLength / newLength);
		int errorStep = oldLength % newLength;
		int error = 0;

		for(int i = 0; i < newLength; i++)
		{
			*newData = *oldData;

			newData += increment;
			oldData += step;
			error += errorStep;

			if(error >= newLength)
			{
				oldData += increment;
				error -= newLength;
			}
		}
	}

	void Bitmap::scaleHorizontal(int newWidth)
	{
		if(width == newWidth)
		{
			return;
		}

		unsigned int *newData = (unsigned int*)malloc(4 * newWidth *height);

		for(int i = 0; i < height; i++)
		{
			scaleLine(newData + i * newWidth, (unsigned int*)colorBuffer + i * width, 1, width, newWidth);
		}

		width = newWidth;
		free(colorBuffer);
		colorBuffer = newData;
	}

	void Bitmap::scaleVertical(int newHeight)
	{
		if(height == newHeight)
		{
			return;
		}

		unsigned int *newData = (unsigned int*)malloc(4 * width *newHeight);

		for(int i = 0; i < width; i++)
		{
			scaleLine(newData + i, (unsigned int*)colorBuffer + i, width, height, newHeight);
		}

		height = newHeight;
		free(colorBuffer);
		colorBuffer = newData;
	}

	void Bitmap::stretchNearest(int newWidth, int newHeight)
	{
		scaleHorizontal(newWidth);
		scaleVertical(newHeight);
	}

	void Bitmap::stretchBilerp(int newWidth, int newHeight)
	{
		unsigned int *oldData = (unsigned int*)colorBuffer;
		unsigned int *newData = (unsigned int*)malloc(4 * newWidth *newHeight);

		for(int v = 0; v < newHeight; v++)
		{
			for(int u = 0; u < newWidth; u++)
			{			
				float fx = (float)(u * width) / newWidth;
				float fy = (float)(v * height) / newHeight;
				int x = (int)fx;
				int y = (int)fy;
				float dx = fx - x;
				float dy = fy - y;

				Color<float> c1 = oldData[(y + 0) * width + (x + 0)];
				Color<float> c2 = oldData[(y + 0) * width + (x + 1)];
				Color<float> c3 = oldData[(y + 1) * width + (x + 0)];
				Color<float> c4 = oldData[(y + 1) * width + (x + 1)];

				float d1 = (1 - dx) * (1 - dy);
				float d2 = dx * (1 - dy);
				float d3 = (1 - dx) * dy;
				float d4 = dx * dy;

				Color<byte> c = d1 * c1 + d2 * c2 + d3 * c3 + d4 * c4;

				newData[v * newWidth + u] = c;
			}
		}

		width = newWidth;
		height = newHeight;

		free(colorBuffer);
		colorBuffer = newData;
	}

	void Bitmap::resize(int bytes)
	{
		colorBuffer = realloc(colorBuffer, bytes);
	}

	void Bitmap::resize(int width, int height)
	{
		if(!colorBuffer) throw Error("Cannot resize bitmap");

		resize(width * height * bitDepth / 8);
	}

	void *Bitmap::getColorBuffer()
	{
		return colorBuffer;
	}

	int Bitmap::getWidth()
	{
		return width;
	}

	int Bitmap::getHeight()
	{
		return height;
	}

	int Bitmap::getBitDepth()
	{
		return bitDepth;
	}
}
