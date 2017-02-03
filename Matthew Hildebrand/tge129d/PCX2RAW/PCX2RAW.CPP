#include <alloc.h>
#include <dir.h>
#include <process.h>
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

int openPcxFile(char *filename, int *wide, int *deep, uchar *palette);
void closePcxFile(void);
int translatePcxFile(FILE *outFile, void *lineBuf);
void assumeExtension(char *filename, char *extension);
void forceExtension(char *filename, char *extension);

uchar palette[768];


void main(int argc, char *argv[])
{
  char filename[80];
  uchar *lineBuf;
  int wide, deep;
  FILE *outFile;


  //*** Print blurb
  printf("PCX2RAW 1.29·D  Copyright (c) 1993 by Matthew Hildebrand\n\n");

  //*** Ensure enough parameters
  if (argc < 2)
  {
    printf("	Usage:  PCX2RAW pcxfile[.pcx]\n\n"
    	   "	Output:	pcxfile.raw -- bitmap compatible with TGE\n"
           "		pcxfile.pal -- palette compatible with TGE\n\n"
           "	Note:	PCX2RAW will only convert 256-colour PCX files.\n\n");
    exit(1);
  }

  //*** Do the job
  strcpy(filename, argv[1]);
  assumeExtension(filename, ".pcx");
  strupr(filename);

  //*** Open the PCX file
  if (openPcxFile(filename, &wide, &deep, palette) != OK)
  {
    printf("Error processing file %s\n\n", filename);
    exit(1);
  }

  //*** Allocate a line buffer
  if ((lineBuf=(uchar*)malloc(wide)) == NULL)
  {
    closePcxFile();			// close the PCX file
    printf("Out of memory\n\n");
    exit(1);
  }

  //*** Create the output RAW file
  forceExtension(filename, ".RAW");
  if ((outFile=fopen(filename,"wb")) == NULL)
  {
    closePcxFile();
    printf("Error creating image file %s\n\n", filename);
    exit(1);
  }

  //*** Translate the image data
  printf("Converting image data...");
  fwrite(&wide, 2, 1, outFile);
  fwrite(&deep, 2, 1, outFile);
  if (translatePcxFile(outFile, lineBuf) != OK)
  {
    printf("\n\nError translating formats\n\n");
    exit(1);
  }

  //*** Clean up
  free(lineBuf);
  closePcxFile();
  fclose(outFile);
  printf(" Done.\n");

  //*** Open palette data file
  forceExtension(filename, ".PAL");
  if ((outFile=fopen(filename,"wb")) == NULL)
  {
    printf("Error creating palette file %s\n\n", filename);
    exit(1);
  }

  //*** Write palette data
  if (!fwrite(palette, 768, 1, outFile))
  {
    printf("Error writing palette file %s\n\n", filename);
    fclose(outFile);
    exit(1);
  }
  fclose(outFile);
  printf("Writing palette... Done.\n\n");
}


//***
//*** Filename manipulation routines
//***

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