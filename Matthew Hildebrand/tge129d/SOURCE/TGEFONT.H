/*****************************************************************************
*       The Graphics Engine version 1.29·D                                   *
*                                                                            *
*       The Graphics Engine code and documentation are Copyright (c) 1993    *
*       by Matthew Hildebrand.                                               *
*                                                                            *
*       Unauthorised usage or modification of any or all of The Graphics     *
*       Engine is strictly prohibited.                                       *
*****************************************************************************/

#ifndef FONTdotH
#define FONTdotH


//*****
//***** Bitmapped font services class
//*****

class Font
{
  unsigned charWide, charDeep;		// character cell dimensions
  unsigned realCharWide;		// real character width (before adjustment)
  unsigned char fgColour, bgColour;	// font colours (0=invisible)
  void far *rawData;			// raw character data here
  void far *image;			// buffer for putImageInv()
  unsigned charSize;			// character image size in bytes
public:
  Font(char *filename, unsigned char fg=1, unsigned char bg=0);
  ~Font();
  inline int status(void);		// return status code (0=ERROR)
  unsigned wide(char *str);		// string width in pixels
  inline unsigned wide(char ch);	// character width in pixels
  inline unsigned maxWide(void);    	// max character width in pixels
  inline unsigned deep(char *str);	// string depth in pixels
  inline unsigned deep(char ch);    	// character depth in pixels
  inline unsigned maxDeep(void);	// max character depth in pixels
  void put(int x, int y, char *str);	// write a string
  void put(int x, int y, char ch);	// write a single character
  inline void foreground(unsigned c);	// set foreground colour
  inline void background(unsigned c);	// set background colour
  inline unsigned foreground(void);	// get foreground colour
  inline unsigned background(void);	// get background colour
};


inline int Font::status(void)
{
  return (rawData==NULL ? 0 : 1);
}

inline unsigned Font::wide(char)
{
  return (realCharWide);
}

inline unsigned Font::maxWide(void)
{
  return (realCharWide);
}

inline unsigned Font::deep(char *)
{
  return (charDeep);
}

inline unsigned Font::deep(char)
{
  return (charDeep);
}

inline unsigned Font::maxDeep(void)
{
  return (charDeep);
}

inline void Font::foreground(unsigned colour)
{
  fgColour = colour;
}

inline void Font::background(unsigned colour)
{
  bgColour = colour;
}

inline unsigned Font::foreground(void)
{
  return (fgColour);
}

inline unsigned Font::background(void)
{
  return (bgColour);
}

#endif