#include <stdio.h>
#include <string.h>
#include "types.h"

#define GENERAL_ERR	0
#define OK		1
#define OPEN_ERR	2
#define SEEK_ERR	3
#define READ_ERR	4
#define WRITE_ERR	5
#define MEM_ERR		6


typedef struct {
  char manufacturer;
  char version;
  char encoding;
  char bitsPerPixel;
  int xmin, ymin;
  int xmax, ymax;
  int hres, vres;
  char palette[48];
  char reserved;
  char colourPlanes;
  int bytesPerLine;
  int paletteType;
  char filler[58];
} PCXHEAD;

PCXHEAD header;

static int width, depth, bytes;
static FILE *inFile;

int openPcxFile(char *filename, int *wide, int *deep, uchar *palette);
void closePcxFile(void);
int translatePcxFile(FILE *outFile, void *lineBuf);
int readPcxLine(uchar *p, FILE *inFile);

int openPcxFile(char *filename, int *wide, int *deep, uchar *palette)
{
  // open the file
  if ((inFile=fopen(filename,"rb")) == NULL)
    return (OPEN_ERR);

  // read in the header
  if (!fread((char *)&header, sizeof(PCXHEAD), 1, inFile))
  {
    fclose(inFile);
    return (READ_ERR);
  }

  // check to make sure it's a .PCX
  if (header.manufacturer!=0x0A || header.version!=5)
  {
    fclose(inFile);
    return (GENERAL_ERR);
  }

  // find the palette
  if (fseek(inFile, -769L, SEEK_END))
  {
    fclose(inFile);
    return (SEEK_ERR);
  }

  // read in the palette data
  if (fgetc(inFile)!=0x0C || !fread(palette, 1, 768, inFile)==768)
  {
    fclose(inFile);
    return (READ_ERR);
  }

  // seek to start of image data
  fseek(inFile, 128L, SEEK_SET);

  width = (header.xmax - header.xmin) + 1;
  depth = (header.ymax - header.ymin) + 1;
  bytes = header.bytesPerLine;
  *wide = width;
  *deep = depth;

  return (OK);
}

int translatePcxFile(FILE *outFile, void *lineBuf)
{
  register int i, status;

  for (i=0; i<depth; i++)
  {
    status = readPcxLine((uchar *)lineBuf, inFile);
    if (status == OK)
      fwrite(lineBuf, bytes, 1, outFile);	// write the line
    else
      return (status);				// return error code
  }

  if (!ferror(outFile))
    return (OK);
  else
    return (WRITE_ERR);
}

int readPcxLine(uchar *p, FILE *fp)
{
  int n=0;
  register int c, i;

  // null the buffer
  do
  {
    // get a key byte
    c = fgetc(fp) & 0xFF;
    // if it's a run of bytes field
    if ((c&0xC0) == 0xC0)
    {
      // and off the high bits
      i = c & 0x3F;
      // get the run byte
      c = fgetc(fp);
      // run the byte
      while (i--)
        p[n++] = c;
    }
    // else just store it
    else
      p[n++] = c;
  }
  while (n < bytes);

  return (ferror(fp) ? GENERAL_ERR : OK);
}

void closePcxFile(void)
{
  fclose(inFile);
}