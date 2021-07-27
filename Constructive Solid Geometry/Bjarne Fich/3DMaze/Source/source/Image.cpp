#include <fstream.h>

#include "Log.hpp"
#include "Image.hpp"

bool LoadTGA(Image *image, char *filename);
bool LoadRAW(Image *image, char *filename);
bool LoadBMP(Image *image, char *filename);

Image::Image(char *filename)
{
  char *extension;
  imageData = NULL;
  bpp = width = height = 0;

  extension = &filename[strlen(filename) - 4];
  if (strcmp (".raw", extension) == 0)
    LoadRAW(this, filename);
  else if (strcmp (".tga", extension) == 0)
    LoadTGA(this, filename);
  else if (strcmp (".bmp", extension) == 0)
    LoadBMP(this, filename);
  else
    Log_Exit("Unknown image fileformat '%s', expected '.tga' or '.raw' in file '%s'", extension, filename);
}

Image::~Image()
{
  if (imageData!=0) free(imageData);
}

/******************************************************/
/*                                                    */
/* TGA                                                */
/*                                                    */
/******************************************************/

typedef struct TGAHeader
{
	unsigned char Header[12];									// TGA File Header
} TGAHeader;

typedef struct TGA
{
	unsigned char		header[6];								// First 6 Useful Bytes From The Header
	unsigned int 		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	unsigned int 		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	unsigned int 		temp;									// Temporary Variable
	unsigned int 		type;
	unsigned int 		Height;									//Height of Image
	unsigned int 		Width;									//Width ofImage
	unsigned int 		Bpp;									// Bits Per Pixel
} TGA;

TGAHeader tgaheader;									// TGA header
TGA tga;												// TGA image data

unsigned char uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
unsigned char cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header
bool LoadUncompressedTGA(Image *, char *, FILE *);	// Load an Uncompressed file
bool LoadCompressedTGA(Image *, char *, FILE *);		// Load a Compressed file

bool LoadTGA(Image *image, char *filename)				// Load a TGA file
{
	FILE * fTGA;												// File pointer to texture file
	fTGA = fopen(filename, "rb");								// Open file for reading

	if(fTGA == NULL)											// If it didn't open....
	{
    Log_Exit("Can't open tga image file '%s'", filename);
		return false;														// Exit function
	}

	if(fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0)					// Attempt to read 12 byte header from file
	{
    Log_Exit("Can't read tga image file header in '%s'", filename);
		if(fTGA != NULL)													// Check to seeiffile is still open
		{
			fclose(fTGA);													// If it is, close it
		}
		return false;														// Exit function
	}

	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)				// See if header matches the predefined header of 
	{																		// an Uncompressed TGA image
		LoadUncompressedTGA(image, filename, fTGA);						// If so, jump to Uncompressed TGA loading code
	}
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)		// See if header matches the predefined header of
	{																		// an RLE compressed TGA image
		LoadCompressedTGA(image, filename, fTGA);							// If so, jump to Compressed TGA loading code
	}
	else																	// If header matches neither type
	{
    Log_Exit("Invalid tga image file '%s' (Type 2 or 10)", filename);
		fclose(fTGA);
		return false;																// Exit function
	}

  LOG("Load TGA image '%s' (w:%i, h:%i, d:%i)", filename, image->width, image->height, image->bpp);
	return true;															// All went well, continue on
}

bool LoadUncompressedTGA(Image * image, char * filename, FILE * fTGA)	// Load an uncompressed TGA (note, much of this code is based on NeHe's
{																			// TGA Loading code nehe.gamedev.net)
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Read TGA header
	{										
    Log_Exit("Invalid tga image info header in file '%s'", filename);
		if(fTGA != NULL)													// if file is still open
		{
			fclose(fTGA);													// Close it
		}
		return false;														// Return failular
	}	

	image->width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	image->height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	image->bpp	= tga.header[4];										// Determine the bits per pixel
	tga.Width		= image->width;										// Copy width into local structure
	tga.Height		= image->height;										// Copy height into local structure
	tga.Bpp			= image->bpp;											// Copy BPP into local structure

	if((image->width <= 0) || (image->height <= 0) || ((image->bpp != 24) && (image->bpp !=32)))	// Make sure all information is valid
	{
    Log_Exit("Invalid tga image file '%s' (w:%i, h:%i, d:%i)", filename, image->width, image->height, image->bpp);
		if(fTGA != NULL)													// Check if file is still open
		{
			fclose(fTGA);													// If so, close it
		}
		return false;														// Return failed
	}

	tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute the number of BYTES per pixel
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute the total amout ofmemory needed to store data
	image->imageData	= (unsigned char *)malloc(tga.imageSize);					// Allocate that much memory

	if(image->imageData == NULL)											// If no space was allocated
	{
    Log_Exit("Could not allocate memory to load tga file '%s'", filename);
		fclose(fTGA);														// Close the file
		return false;														// Return failed
	}

	if(fread(image->imageData, 1, tga.imageSize, fTGA) != tga.imageSize)	// Attempt to read image data
	{
    Log_Exit("Could not read image data from tga file '%s'", filename);
		if(image->imageData != NULL)										// If imagedata has data in it
		{
			free(image->imageData);										// Delete data from memory
		}
		fclose(fTGA);														// Close file
		return false;														// Return failed
	}

	// Byte Swapping Optimized By Steve Thomas
	for(unsigned int cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		image->imageData[cswap] ^= image->imageData[cswap+2] ^=
		image->imageData[cswap] ^= image->imageData[cswap+2];
	}

	fclose(fTGA);															// Close file
	return true;															// Return success
}

bool LoadCompressedTGA(Image * image, char * filename, FILE * fTGA)		// Load COMPRESSED TGAs
{ 
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)					// Attempt to read header
	{
    Log_Exit("Invalid tga image info header in file '%s'", filename);
		if(fTGA != NULL)													// If file is open
		{
			fclose(fTGA);													// Close it
		}
		return false;														// Return failed
	}

	image->width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	image->height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	image->bpp	= tga.header[4];										// Determine Bits Per Pixel
	tga.Width		= image->width;										// Copy width to local structure
	tga.Height	= image->height;										// Copy width to local structure
	tga.Bpp			= image->bpp;											// Copy width to local structure

	if((image->width <= 0) || (image->height <= 0) || ((image->bpp != 24) && (image->bpp !=32)))	//Make sure all texture info is ok
	{
    Log_Exit("Invalid tga image file '%s' (w:%i, h:%i, d:%i)", filename, image->width, image->height, image->bpp);
		if(fTGA != NULL)													// Check if file is open
		{
			fclose(fTGA);													// Ifit is, close it
		}
		return false;														// Return failed
	}

	tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute BYTES per pixel
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute amout of memory needed to store image
	image->imageData	= (unsigned char *)malloc(tga.imageSize);					// Allocate that much memory

	if(image->imageData == NULL)											// If it wasnt allocated correctly..
	{
    Log_Exit("Could not allocate memory to load tga file '%s'", filename);
		fclose(fTGA);														// Close file
		return false;														// Return failed
	}

	unsigned int pixelcount	= tga.Height * tga.Width;							// Nuber of pixels in the image
	unsigned int currentpixel	= 0;												// Current pixel being read
	unsigned int currentbyte	= 0;												// Current byte
	unsigned char * colorbuffer = (unsigned char *)malloc(tga.bytesPerPixel);			// Storage for 1 pixel

	do
	{
		unsigned char chunkheader = 0;											// Storage for "chunk" header

		if(fread(&chunkheader, sizeof(unsigned char), 1, fTGA) == 0)				// Read in the 1 byte header
		{
      Log_Exit("Invalid tga image RLE header in file '%s'", filename);
			if(fTGA != NULL)												// If file is open
			{
				fclose(fTGA);												// Close file
			}
			if(image->imageData != NULL)									// If there is stored image data
			{
				free(image->imageData);									// Delete image data
			}
			return false;													// Return failed
		}

		if(chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
		{																	// that follow the header
			chunkheader++;													// add 1 to get number of following color values
			for(short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
			{
				if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel) // Try to read 1 pixel
				{
          Log_Exit("Could not read image data from tga file '%s'", filename);

					if(fTGA != NULL)													// See if file is open
					{
						fclose(fTGA);													// If so, close file
					}

					if(colorbuffer != NULL)												// See if colorbuffer has data in it
					{
						free(colorbuffer);												// If so, delete it
					}

					if(image->imageData != NULL)										// See if there is stored Image data
					{
						free(image->imageData);										// If so, delete it too
					}

					return false;														// Return failed
				}
																						// write to memory
				image->imageData[currentbyte		] = colorbuffer[2];				    // Flip R and B vcolor values around in the process
				image->imageData[currentbyte + 1	] = colorbuffer[1];
				image->imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												// if its a 32 bpp image
				{
					image->imageData[currentbyte + 3] = colorbuffer[3];				// copy the 4th byte
				}

				currentbyte += tga.bytesPerPixel;										// Increase thecurrent byte by the number of bytes per pixel
				currentpixel++;															// Increase current pixel by 1

				if(currentpixel > pixelcount)											// Make sure we havent read too many pixels
				{
          Log_Exit("TGA error, Too many pixels read");

					if(fTGA != NULL)													// If there is a file open
					{
						fclose(fTGA);													// Close file
					}	

					if(colorbuffer != NULL)												// If there is data in colorbuffer
					{
						free(colorbuffer);												// Delete it
					}

					if(image->imageData != NULL)										// If there is Image data
					{
						free(image->imageData);										// delete it
					}

					return false;														// Return failed
				}
			}
		}
		else																			// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
		{
			chunkheader -= 127;															// Subteact 127 to get rid of the ID bit
			if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)		// Attempt to read following color values
			{	
        Log_Exit("TGA error, Could not read from file");

				if(fTGA != NULL)														// If thereis a file open
				{
					fclose(fTGA);														// Close it
				}

				if(colorbuffer != NULL)													// If there is data in the colorbuffer
				{
					free(colorbuffer);													// delete it
				}

				if(image->imageData != NULL)											// If thereis image data
				{
					free(image->imageData);											// delete it
				}

				return false;															// return failed
			}

			for(short counter = 0; counter < chunkheader; counter++)					// copy the color into the image data as many times as dictated 
			{																			// by the header
				image->imageData[currentbyte		] = colorbuffer[2];					// switch R and B bytes areound while copying
				image->imageData[currentbyte + 1	] = colorbuffer[1];
				image->imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												// If TGA images is 32 bpp
				{
					image->imageData[currentbyte + 3] = colorbuffer[3];				// Copy 4th byte
				}

				currentbyte += tga.bytesPerPixel;										// Increase current byte by the number of bytes per pixel
				currentpixel++;															// Increase pixel count by 1

				if(currentpixel > pixelcount)											// Make sure we havent written too many pixels
				{
          Log_Exit("TGA error, Too many pixels rea");

					if(fTGA != NULL)													// If there is a file open
					{
						fclose(fTGA);													// Close file
					}	

					if(colorbuffer != NULL)												// If there is data in colorbuffer
					{
						free(colorbuffer);												// Delete it
					}

					if(image->imageData != NULL)										// If there is Image data
					{
						free(image->imageData);										// delete it
					}

					return false;														// Return failed
				}
			}
		}
	}

	while(currentpixel < pixelcount);													// Loop while there are still pixels left
	fclose(fTGA);																		// Close the file
	return true;																		// return success
}

/******************************************************/
/*                                                    */
/* RAW                                                */
/*                                                    */
/******************************************************/

bool LoadRAW(Image *image, char *filename)
{
  long int filesize;
  ifstream fin;

  fin.open(filename, ios::binary | ios::in);
  if (fin.good() == false) Log_Exit("Can't open file: '%s'", filename);
  fin.seekg(0, ios::end);
  filesize = fin.tellg();
  fin.seekg(0, ios::beg);

  // Guess format size=x*y*d
  // ...asuming square and size can be div by 2.

  image->width = 8;
  image->height = 8;
  image->bpp = 24;

  while ((filesize!=(image->width*image->height*image->bpp/8)) && (image->width<=2048))
  {
    image->width *= 2;
    image->height *= 2;
    if ((image->width > 2048) && (image->bpp!=32))
    {
      image->width = 8;
      image->height = 8;
      image->bpp = 32;
    }
  }


  if (image->width > 2048)
    Log_Exit("Can't guess raw data format. Tried filesize (%i) = size x size x depth in file '%s'", filesize, filename);

  image->imageData = (BYTE *)malloc(filesize);

  fin.read((char *)image->imageData, filesize);
  fin.close();

  LOG("Load RAW image '%s' (w:%i, h:%i, d:%i) size:%i", filename, image->width, image->height, image->bpp, filesize);
  return true;
}


///////////////////////////////////////////////////////////////////////////////
// Downloaded from:
//   http://www.starstonesoftware.com/OpenGL/
// This function loads a 24-bit color Windows bitmap and sets it as the current
// texture. The bitmap must be 24-bit color, and the dimensions must be powers
// of 2 (no additional checks are performed).
bool LoadBMP(Image *image, char *filename) {
	HANDLE hFileHandle;
	BITMAPINFO *pBitmapInfo = NULL;
	unsigned long lInfoSize = 0;
	unsigned long lBitSize = 0;

	// Open the Bitmap file
	hFileHandle = CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,
		NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);

	// Check for open failure (most likely file does not exist).
	if(hFileHandle == INVALID_HANDLE_VALUE)
		return FALSE;

	// File is Open. Read in bitmap header information
	BITMAPFILEHEADER	bitmapHeader;
	DWORD dwBytes;
	ReadFile(hFileHandle,&bitmapHeader,sizeof(BITMAPFILEHEADER),	
		&dwBytes,NULL);

//	__try {
		if(dwBytes != sizeof(BITMAPFILEHEADER))
			return FALSE;

		// Check format of bitmap file
/*
		if(bitmapHeader.bfType != 'MB')
			return FALSE;
*/

		// Read in bitmap information structure
		lInfoSize = bitmapHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
		pBitmapInfo = (BITMAPINFO *) new BYTE[lInfoSize];

		ReadFile(hFileHandle,pBitmapInfo,lInfoSize,&dwBytes,NULL);

		if(dwBytes != lInfoSize)
			return FALSE;

		image->width = pBitmapInfo->bmiHeader.biWidth;
		image->height = pBitmapInfo->bmiHeader.biHeight;
		lBitSize = pBitmapInfo->bmiHeader.biSizeImage;

    image->bpp = pBitmapInfo->bmiHeader.biBitCount;

		if(lBitSize == 0)
			lBitSize = (image->width * pBitmapInfo->bmiHeader.biBitCount + 7) / 8 * abs((long int)image->height);
	
		// Allocate space for the actual bitmap
    image->imageData = (BYTE *)malloc(lBitSize);

		// Read in the bitmap bits
		ReadFile(hFileHandle,image->imageData,lBitSize,&dwBytes,NULL);

		if(lBitSize != dwBytes)
			{
			if(image->imageData)
				free(image->imageData);
			image->imageData = NULL;
			
			return FALSE;
			}
//		}
//	__finally // Fail or success, close file and free working memory
//		{
		CloseHandle(hFileHandle);

		if(pBitmapInfo != NULL)
			delete [] (BYTE *)pBitmapInfo;
//		}

   
	// This is specific to the binary format of the data read in.
  /*
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, nTextureWidth, nTextureHeight, 0,
                 GL_BGR_EXT, GL_UNSIGNED_BYTE, pBitmapData);
  */

	return TRUE;
}
