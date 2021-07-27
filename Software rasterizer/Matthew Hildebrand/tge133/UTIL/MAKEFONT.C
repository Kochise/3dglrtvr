/**************************************************************************
*  File:        MAKEFONT.C     Copyright (c) 1993-1994 by Matthew Hildebrand
*
*  Purpose:     Make a font file from its component RAW files.
**************************************************************************/

#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readOffsetFile(char *filename);
void readPaletteFile(char *filename);

unsigned short offsets[256];
unsigned char palette[768];
const char copyright[] = " Copyright (c) 1993-1994 by Matthew Hildebrand.  All rights reserved.";



/****
***** Program entry point.
****/

void main(int argc, char *argv[])
{
  FILE *inFile, *outFile;
  char filename[80];
  void far *charAddr=NULL;
  int count;
  unsigned short zero=0;
  unsigned long fileSize;

  /*** Ensure filename specified ***/
  printf("MAKEFONT 1.33  Copyright (c) 1993-1994 by Matthew Hildebrand\n\n");
  if (argc < 2)
  {
    printf("        Usage:  MAKEFONT fontname\n\n");
    exit(EXIT_FAILURE);
  }

  /*** Read the offsetFromTop data file ***/
  strcpy(filename, argv[1]);
  strcat(filename, ".off");
  strupr(filename);
  readOffsetFile(filename);

  /*** Read the palette file ***/
  strcpy(filename, argv[1]);
  strcat(filename, ".pal");
  strupr(filename);
  readPaletteFile(filename);

  /*** Open the output file ***/
  strcpy(filename, argv[1]);
  strcat(filename, ".fnt");
  strupr(filename);
  if ((outFile=fopen(filename,"wb")) == NULL)
  {
    printf("Error opening file '%s', aborting.\n\n", filename);
    exit(EXIT_FAILURE);
  }

  /*** Write the font file header ***/
  fwrite("TGEFONT2", 8, 1, outFile);                    /* signature */
  fwrite(copyright, strlen(copyright)+1, 1, outFile);   /* copyright notice */
  fwrite(palette, 768, 1, outFile);                     /* font palette */

  /*** Copy the character data, one character at a time ***/
  for (count=0; count<256; count++)
  {
    /*** Write the character's offsetFromTop entry ***/
    fwrite(&offsets[count], sizeof(short), 1, outFile);

    /*** Try to open the character file ***/
    sprintf(filename, "%d.raw", count);
    if ((inFile=fopen(filename,"rb")) != NULL)  /* file exists */
    {
      /*** Allocate memory for the character file ***/
      fseek(inFile, 0L, SEEK_END);          /* seek to end-of-file */
      fileSize = ftell(inFile);             /* get file size */
      rewind(inFile);                       /* seek to start-of-file */
      if (charAddr != NULL)                 /* free old memory if necessary */
        farfree(charAddr);
      charAddr = farmalloc(fileSize);       /* allocate new memory */

      /*** Read in the character data ***/
      if (!fread(charAddr, (size_t)fileSize, 1, inFile))
      {
        printf("Error reading file '%s', aborting.\n\n", filename);
        exit(EXIT_FAILURE);
      }

      /*** Write the data to the new file ***/
      if (!fwrite(charAddr, (size_t)fileSize, 1, outFile))
      {
        printf("Error writing file '%s', aborting.\n\n", filename);
        exit(EXIT_FAILURE);
      }
      fclose(inFile);
    }
    else                                        /* file doesn't exist */
    {
      /*** Write a blank character entry ***/
      if (!fwrite(&zero, sizeof(short), 1, outFile))  /* zero width */
      {
        printf("Error writing file '%s', aborting.\n\n", filename);
        exit(EXIT_FAILURE);
      }
      if (!fwrite(&zero, sizeof(short), 1, outFile))  /* zero depth */
      {
        printf("Error writing file '%s', aborting.\n\n", filename);
        exit(EXIT_FAILURE);
      }
    }
  }
}



/****
***** Read the characters' offset data.
****/

void readOffsetFile(char *filename)
{
  FILE *offFile;
  int count;
  char curLine[80];

  /*** Open the file ***/
  if ((offFile=fopen(filename,"rb")) == NULL)
  {
    printf("Error opening file '%s', aborting.\n\n", filename);
    exit(EXIT_FAILURE);
  }

  /*** Read in the offsets, one by one ***/
  for (count=0; count<256; count++)
  {
    /*** Read the line ***/
    if (fgets(curLine, 80, offFile) == NULL)
    {
      printf("Error reading file '%s', aborting.\n\n", filename);
      exit(EXIT_FAILURE);
    }
    /*** Convert the text to a number ***/
    offsets[count] = atoi(curLine);
    if (offsets <= 0)
    {
      printf("Error on line %d of file '%s', aborting.\n\n", filename, count);
      exit(EXIT_FAILURE);
    }
  }
}



/****
***** Read the font's palette data file.
****/

void readPaletteFile(char *filename)
{
  FILE *palFile;

  /*** Open the file ***/
  if ((palFile=fopen(filename,"rb")) == NULL)
  {
    printf("Error opening file '%s', aborting.\n\n", filename);
    exit(EXIT_FAILURE);
  }

  /*** Read the data ***/
  if (!fread(palette, 768, 1, palFile))
  {
    printf("Error reading file '%s', aborting.\n\n", filename);
    exit(EXIT_FAILURE);
  }
}
