/*****************************************************************************
*       The Graphics Engine version 1.33                                     *
*                                                                            *
*       The Graphics Engine code and documentation are                       *
*       Copyright (c) 1993-1994 by Matthew Hildebrand; all rights reserved.  *
*                                                                            *
*       Unauthorised usage or modification of any or all of The Graphics     *
*       Engine is strictly prohibited.                                       *
*****************************************************************************/

#if !defined(FONTdotH)
#define FONTdotH

#include <stdio.h>



//*****
//***** Bitmapped font services class
//*****

class FixedFont
{
  unsigned charWide, charDeep;		// character cell dimensions
  unsigned realCharWide;		// real character width (before adjustment)
  unsigned char fgColour, bgColour;	// font colours (0=invisible)
  void far *rawData;			// raw character data here
  void far *image;			// buffer for putImageInv()
  unsigned charSize;                    // character cell size in bits
public:
  FixedFont(unsigned char fg=1, unsigned char bg=0);
  FixedFont(char *filename, unsigned char fg=1, unsigned char bg=0);
  FixedFont(unsigned char *filename, unsigned char fg=1, unsigned char bg=0);
  ~FixedFont();
  int load(char *filename);
  int load(unsigned char *filename)              { return load((char*)filename); }
  inline int status(void);		// return status code (0=ERROR)
  unsigned width(char *str);            // string width in pixels
  unsigned width(unsigned char *string)          { return width((char*)string);  }
  inline unsigned height(char *str);    // string depth in pixels
  unsigned height(unsigned char *string)         { return height((char*)string); }
  inline unsigned width(char ch);       // character width in pixels
  unsigned short width(unsigned char ch)         { return width((char)ch);       }
  inline unsigned height(char ch);      // character depth in pixels
  unsigned short height(unsigned char ch)        { return height((char)ch);      }
  inline unsigned maxWidth(void);       // max character width in pixels
  inline unsigned maxHeight(void);      // max character height in pixels
  void put(int x, int y, char *str);	// write a string
  void put(int x, int y, char ch);	// write a single character
  void put(int x, int y, unsigned char *string)  { put(x,y,(char*)string); }
  void put(int x, int y, unsigned char ch)       { put(x,y,(char)ch);      }
  inline void foreground(unsigned c);	// set foreground colour
  inline void background(unsigned c);	// set background colour
  inline unsigned foreground(void);	// get foreground colour
  inline unsigned background(void);	// get background colour
  inline void *charAddr(char ch)                 { return ((void*)(((char*)rawData)+charSize/8*(unsigned)ch)); }
  inline void *charAddr(unsigned char ch)        { return ((void*)(((char*)rawData)+charSize/8*(unsigned)ch)); }
};


inline int FixedFont::status(void)
{
  return (rawData==NULL ? 0 : 1);
}

inline unsigned FixedFont::width(char)
{
  return (realCharWide);
}

inline unsigned FixedFont::maxWidth(void)
{
  return (realCharWide);
}

inline unsigned FixedFont::height(char *)
{
  return (charDeep);
}

inline unsigned FixedFont::height(char)
{
  return (charDeep);
}

inline unsigned FixedFont::maxHeight(void)
{
  return (charDeep);
}

inline void FixedFont::foreground(unsigned colour)
{
  fgColour = colour;
}

inline void FixedFont::background(unsigned colour)
{
  bgColour = colour;
}

inline unsigned FixedFont::foreground(void)
{
  return (fgColour);
}

inline unsigned FixedFont::background(void)
{
  return (bgColour);
}



#endif
