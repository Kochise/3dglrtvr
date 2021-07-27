/*****************************************************************************
*       The Graphics Engine version 1.29·D                                   *
*                                                                            *
*       The Graphics Engine code and documentation are Copyright (c) 1993    *
*       by Matthew Hildebrand.                                               *
*                                                                            *
*       Unauthorised usage or modification of any or all of The Graphics     *
*       Engine is strictly prohibited.                                       *
*****************************************************************************/

#include <alloc.h>
#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include "tgefont.h"
#include "tge.h"


//*****
//***** Constructor
//*****

Font::Font(char *filename, unsigned char fg, unsigned char bg)
{
  char signature[8];
  unsigned long fileLen;
  FILE *inFile;

  //*** Initialize member variables
  rawData = NULL;
  fgColour = fg;
  bgColour = bg;

  //*** Open font file
  if ((inFile=fopen(filename,"rb")) == NULL)
    return;

  //*** Parse header
  if (!fread(signature, 8, 1, inFile))		// read in signature
    return;
  if (memcmp(signature, "TGEFONT1", 8))		// is signature there?
    return;					// no, quit

  if (!fread(&realCharWide, 2, 1, inFile))  	// read in character width
    return;
  if (!fread(&charDeep, 2, 1, inFile))	   	// read in character depth
    return;

  if (realCharWide % 8)				// ensure even multiple of 8
    charWide = realCharWide + 8-(realCharWide%8);
  else
    charWide = realCharWide;
  charSize = charWide * charDeep;		// precalculate for speed

  //*** Ensure format is correct (ie., file size is charWide*charDeep+12)
  fseek(inFile, 0L, SEEK_END);			// seek to end of file
  fileLen = ftell(inFile);			// get file length
  fseek(inFile, 12L, SEEK_SET);			// seek past header

  fileLen -= 12L;				// remove header length
  if (fileLen != (charSize/8)*256L)	   	// verify size OK
    return;

  //*** Read in character data
  rawData = farmalloc(fileLen);			// allocate some RAM
  if (rawData == NULL)				// was allocation successful?
    return;					// no, quit

  if (!fread(rawData, (unsigned)fileLen, 1, inFile)) // read in font data
  {						// clean up and quit on error
    farfree(rawData);
    return;
  }
  fclose(inFile);				// close the file

  image = farmalloc(charSize + 4L);		// allocate the image buffer
  if (image == NULL)				// was function successful?
  {						// no, clean up and quit
    farfree(rawData);
    return;
  }
  else
  {
    ((unsigned*)image)[0] = charWide;		// fill in dimensions
    ((unsigned*)image)[1] = charDeep;
  }
}


//*****
//***** Destructor
//*****

Font::~Font(void)
{
  if (rawData)					// was memory allocated?
    farfree(rawData);				// yes, free it
  if (image)					// same procedure
    farfree(image);
}


//*****
//***** Compute the width, in pixels, of the given string
//*****

unsigned Font::wide(char *str)
{
  return (realCharWide * strlen(str));
}


//*****
//***** Write a string at the specified location
//*****

void Font::put(int x, int y, char *str)
{
  unsigned count, length;

  length = strlen(str);		       		// store for speed
  for (count=0; count<length; count++)		// loop for each character
    put(x+count*realCharWide, y, str[count]);	// put the character
}


//*****
//***** Write a single character at the specified location
//*****

void Font::put(int x, int y, char ch)
{
  register unsigned count;
  unsigned char huge *in, huge *out;
  register unsigned char byte;

  //*** Initialize pointers
  (void far*)in = rawData;			// pointer to bit-image buffer
  in += (charSize/8) * (unsigned)ch;		// point to correct bit-image
  (void far*)out = image;			// pointer to image buffer
  out += 4;					// bump past dimension info

  //*** Convert font data from 1 bit/pixel format to 1 byte/pixel format
  for (count=0; count<charSize/8; count++)
  {
    byte = *in;					// get current byte

    if ((byte>>7) & 1)
      *out = fgColour;
    else
      *out = bgColour;
    out++;

    if ((byte>>6) & 1)
      *out = fgColour;
    else
      *out = bgColour;
    out++;

    if ((byte>>5) & 1)
      *out = fgColour;
    else
      *out = bgColour;
    out++;

    if ((byte>>4) & 1)
      *out = fgColour;
    else
      *out = bgColour;
    out++;

    if ((byte>>3) & 1)
      *out = fgColour;
    else
      *out = bgColour;
    out++;

    if ((byte>>2) & 1)
      *out = fgColour;
    else
      *out = bgColour;
    out++;

    if ((byte>>1) & 1)
      *out = fgColour;
    else
      *out = bgColour;
    out++;

    if ((byte>>0) & 1)
      *out = fgColour;
    else
      *out = bgColour;
    out++;

    in++;
  }

  putImageInv(x, y, image);		  	// draw the character
}