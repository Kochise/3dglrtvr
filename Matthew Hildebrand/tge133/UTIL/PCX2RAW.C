/**************************************************************************
*  File:        PCX2RAW.C     Copyright (c) 1993-1994 by Matthew Hildebrand
*
*  Purpose:     Convert PCX files to RAW and PAL files.
**************************************************************************/

#include <alloc.h>
#include <dir.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GENERAL_ERR     0
#define OK		1
#define OPEN_ERR	2
#define SEEK_ERR	3
#define READ_ERR	4
#define WRITE_ERR	5
#define MEM_ERR		6

typedef unsigned char   uchar;
typedef unsigned int    uint;
typedef unsigned long   ulong;
typedef struct
{
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
} PcxHeader;

void convertFile(char *filename);
int openPcxFile(char *filename, int *wide, int *deep, uchar *palette);
void closePcxFile(void);
int translatePcxFile(FILE *outFile, void *lineBuf);
void assumeExtension(char *filename, char *extension);
void forceExtension(char *filename, char *extension);
int readPcxLine(uchar *p, FILE *inFile);

uchar palette[768];
PcxHeader header;
int width, depth, bytes;
FILE *inFile;



/****
***** Program entry point.
****/

void main(int argc, char *argv[])
{
  char fileDrive[MAXDRIVE];
  char fileDir[MAXDIR];
  char fileFile[MAXFILE];
  char fileExt[MAXEXT];
  char path[MAXPATH];
  struct ffblk ffblk;
  int done;

  /*** Ensure filespec specified ***/
  printf("PCX2RAW 1.33  Copyright (c) 1993-1994 by Matthew Hildebrand\n\n");
  if (argc < 2)
  {
    printf("        Usage:  PCX2RAW pcxfile[.pcx]\n\n"
           "       Output:  pcxfile.raw -- RAW bitmap file\n"
           "                pcxfile.pal -- PAL palette file\n\n"
           "         Note:  PCX2RAW will convert only 256-colour PCX files.\n\n");
    exit(EXIT_FAILURE);
  }

  /*** Loop through, converting each file ***/
  fnsplit(argv[1], fileDrive, fileDir, fileFile, fileExt);
  assumeExtension(argv[1], ".PCX");
  if ((done=findfirst(argv[1], &ffblk, 0)) != 0)
    exit(EXIT_SUCCESS);
  while (!done)
  {
    /*** Get the current filename ***/
    strcpy(path, fileDrive);
    strcat(path, fileDir);
    strcat(path, ffblk.ff_name);

    /*** Convert the file ***/
    strupr(path);
    printf("\n\nProcessing file %s:\n", path);
    convertFile(path);

    /*** Get the next filename ***/
    done = findnext(&ffblk);
  }

  printf("\n\n");
}



/****
***** Convert a PCX file to a RAW file
****/

void convertFile(char *filename)
{
  uchar *lineBuf;
  int wide, deep;
  FILE *outFile;

  /*** Open the PCX file ***/
  if (openPcxFile(filename, &wide, &deep, palette) != OK)
  {
    printf("Error processing file %s\n\n", filename);
    exit(EXIT_FAILURE);
  }

  /*** Allocate a line buffer ***/
  if ((lineBuf=(uchar*)malloc(wide)) == NULL)
  {
    closePcxFile();                         /* close the PCX file */
    printf("Out of memory\n\n");
    exit(EXIT_FAILURE);
  }

  /*** Create the output RAW file ***/
  forceExtension(filename, ".RAW");
  if ((outFile=fopen(filename,"wb")) == NULL)
  {
    closePcxFile();
    printf("Error creating image file %s\n\n", filename);
    exit(EXIT_FAILURE);
  }

  /*** Translate the image data ***/
  printf("Converting image data...");
  fwrite(&wide, 2, 1, outFile);
  fwrite(&deep, 2, 1, outFile);
  if (translatePcxFile(outFile, lineBuf) != OK)
  {
    printf("\n\nError translating formats\n\n");
    exit(EXIT_FAILURE);
  }

  /*** Clean up ***/
  free(lineBuf);
  closePcxFile();
  fclose(outFile);
  printf(" Done.\n");

  /*** Open palette data file ***/
  forceExtension(filename, ".PAL");
  if ((outFile=fopen(filename,"wb")) == NULL)
  {
    printf("Error creating palette file %s\n\n", filename);
    exit(EXIT_FAILURE);
  }

  /*** Write palette data ***/
  if (!fwrite(palette, 768, 1, outFile))
  {
    printf("Error writing palette file %s\n\n", filename);
    fclose(outFile);
    exit(EXIT_FAILURE);
  }
  fclose(outFile);
  printf("Writing palette... Done.");
}



/****
***** Filename manipulation routines.
****/

void assumeExtension(char *filename, char *defaultExt)
{
  char fileDrive[MAXDRIVE];
  char fileDir[MAXDIR];
  char fileFile[MAXFILE];
  char fileExt[MAXEXT];
  char newPath[MAXPATH];
  int status;

  status = fnsplit(filename, fileDrive, fileDir, fileFile, fileExt);

  if (!(status&EXTENSION))
    strcpy(fileExt, defaultExt);

  fnmerge(newPath, fileDrive, fileDir, fileFile, fileExt);
  strcpy(filename, newPath);
}

void forceExtension(char *filename, char *extention)
{
  char fileDrive[MAXDRIVE];
  char fileDir[MAXDIR];
  char fileFile[MAXFILE];
  char fileExt[MAXEXT];

  fnsplit(filename, fileDrive, fileDir, fileFile, fileExt);

  if (strcmpi(fileExt, extention))
    fnmerge(filename, fileDrive, fileDir, fileFile, extention);
}



/****
***** Open a PCX file.
****/

int openPcxFile(char *filename, int *wide, int *deep, uchar *palette)
{
  /*** Open the file ***/
  if ((inFile=fopen(filename,"rb")) == NULL)
    return (OPEN_ERR);

  /*** Read in the header ***/
  if (!fread((char *)&header, sizeof(PcxHeader), 1, inFile))
  {
    fclose(inFile);
    return (READ_ERR);
  }

  /*** Check to make sure it's a PCX ***/
  if (header.manufacturer!=0x0A || header.version!=5)
  {
    fclose(inFile);
    return (GENERAL_ERR);
  }

  /*** Find the palette ***/
  if (fseek(inFile, -769L, SEEK_END))
  {
    fclose(inFile);
    return (SEEK_ERR);
  }

  /*** Read in the palette data ***/
  if (fgetc(inFile)!=0x0C || !fread(palette, 1, 768, inFile)==768)
  {
    fclose(inFile);
    return (READ_ERR);
  }

  /*** Seek to start of image data ***/
  fseek(inFile, 128L, SEEK_SET);

  /*** Pass information back to caller ***/
  width = (header.xmax - header.xmin) + 1;
  depth = (header.ymax - header.ymin) + 1;
  bytes = header.bytesPerLine;
  *wide = width;
  *deep = depth;
  return (OK);
}



/****
***** Translate a PCX file to a RAW file.
****/

int translatePcxFile(FILE *outFile, void *lineBuf)
{
  int i, status;

  /*** Convert the file, one line at a time ***/
  for (i=0; i<depth; i++)
  {
    status = readPcxLine((uchar *)lineBuf, inFile);
    if (status == OK)
      fwrite(lineBuf, bytes, 1, outFile);   /* write the line */
    else
      return (status);                      /* return error code */
  }

  /*** Return success or error code ***/
  if (!ferror(outFile))
    return (OK);
  else
    return (WRITE_ERR);
}



/****
***** Read a single line of a PCX file.
****/

int readPcxLine(uchar *p, FILE *fp)
{
  int n=0;
  register int c, i;

  /*** Null the buffer ***/
  do
  {
    c = fgetc(fp) & 0xFF;                   /* get a key byte */
    /*** If it's a run of bytes field ***/
    if ((c&0xC0) == 0xC0)
    {
      i = c & 0x3F;                         /* AND off high bits */
      c = fgetc(fp);                        /* get the run byte */
      while (i--)                           /* run the byte */
        p[n++] = c;
    }
    /*** Else just store it ***/
    else
      p[n++] = c;
  }
  while (n < bytes);

  return (ferror(fp) ? GENERAL_ERR : OK);
}



/****
***** Close the PCX file.
****/

void closePcxFile(void)
{
  fclose(inFile);
}
