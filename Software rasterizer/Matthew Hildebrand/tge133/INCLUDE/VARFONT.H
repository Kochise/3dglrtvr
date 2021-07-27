/*****************************************************************************
*       The Graphics Engine version 1.33                                     *
*                                                                            *
*       The Graphics Engine code and documentation are                       *
*       Copyright (c) 1993-1994 by Matthew Hildebrand; all rights reserved.  *
*                                                                            *
*       Unauthorised usage or modification of any or all of The Graphics     *
*       Engine is strictly prohibited.                                       *
*****************************************************************************/

#if !defined(VARFONTdotH)
#define VARFONTdotH



//*****
//***** Structure used by the VariableFont class.
//*****

#pragma option -a-

struct FontCharInfo
{
  unsigned short offsetFromTop;
  unsigned short wide, deep;
  void far *bitmap;
};

#pragma option -a



//*****
//***** Class for dealing with variable-sized, 256-colour, loadable fonts.
//*****

class VariableFont
{
  unsigned char fontPalette[768], colourTranslate[256];
  FontCharInfo characters[256];
  void far *characterData, *charBuf;
  unsigned short maxWide, maxHigh;
  unsigned spaceBetweenChars;
public:
  VariableFont(void);
  ~VariableFont(void);
  int load(char *filename);                     // returns 0 on error
  int load(unsigned char *filename)              { return load((char*)filename); }
  void put(int x, int y, char ch);
  void put(int x, int y, char *string);
  void put(int x, int y, unsigned char *string)  { put(x,y,(char*)string); }
  void put(int x, int y, unsigned char ch)       { put(x,y,(char)ch);      }
  unsigned width(char *string);
  unsigned width(unsigned char *string)          { return width((char*)string);  }
  unsigned height(char *string);
  unsigned height(unsigned char *string)         { return height((char*)string); }
  inline unsigned short width(char ch);
  unsigned short width(unsigned char ch)         { return width((char)ch);       }
  inline unsigned short height(char ch);
  unsigned short height(unsigned char ch)        { return height((char)ch);      }
  inline unsigned short maxWidth(void);
  inline unsigned short maxHeight(void);
  void matchColours(void);
  void palette(void *palette);
  void palette(unsigned char palReg, unsigned char red, unsigned char green,
               unsigned char blue);
  void palette(unsigned char palReg, unsigned char *red, unsigned char *green,
               unsigned char *blue);
  inline void *palette(void);
  inline void spacing(unsigned numPixels);
  inline unsigned spacing(void);
};



/**************************************************************************
*  Function:    VariableFont::width
*
*  Purpose:     Return the width, in pixels, of a single character.
*
*  Entry:       ch = Character of which to find the width.
*
*  Exit:        Returns the width of the character in pixels, not
*               accounting for blank space on either side.
**************************************************************************/

inline unsigned short VariableFont::width(char ch)
{
  return (characters[(unsigned)ch].wide);
}



/**************************************************************************
*  Function:    VariableFont::height
*
*  Purpose:     Return the height, in pixels, of a single character's
*               bitmap.  The offsetFromTop value is ignored.
*
*  Entry:       ch = Character of which to find the height.
*
*  Exit:        Returns the height of the character in pixels, not
*               accounting for blank space above or below.
**************************************************************************/

inline unsigned short VariableFont::height(char ch)
{
  return (characters[(unsigned)ch].deep);
}



/**************************************************************************
*  Function:    VariableFont::maxWidth
*
*  Purpose:     Return the width, in pixels, of the widest character.
*
*  Entry:       N/A
*
*  Exit:        Returns the width of the widest character in pixels, not
*               accounting for blank space used between characters.
**************************************************************************/

inline unsigned short VariableFont::maxWidth(void)
{
  return (maxWide);
}



/**************************************************************************
*  Function:    VariableFont::maxHeight
*
*  Purpose:     Return the height, in pixels, of the tallest character.
*
*  Entry:       N/A
*
*  Exit:        Returns the height of the tallest character in pixels, not
*               accounting for blank space above or below.
**************************************************************************/

inline unsigned short VariableFont::maxHeight(void)
{
  return (maxHigh);
}



/**************************************************************************
*  Function:    VariableFont::palette
*
*  Purpose:     Return the current font palette.
*
*  Entry:       N/A
*
*  Exit:        Returns the address of the current font palette.
**************************************************************************/

inline void *VariableFont::palette(void)
{
  return (fontPalette);
}



/**************************************************************************
*  Function:    VariableFont::spacing
*
*  Purpose:     Set the spacing, in pixels, between characters.
*
*  Entry:       numPixels = New spacing value, in pixels.
*
*  Exit:        N/A
**************************************************************************/

inline void VariableFont::spacing(unsigned numPixels)
{
  spaceBetweenChars = numPixels;            // change spacing value
}



/**************************************************************************
*  Function:    VariableFont::spacing
*
*  Purpose:     Return the spacing, in pixels, between characters.
*
*  Entry:       N/A
*
*  Exit:        Returns the spacing in pixels.
**************************************************************************/

inline unsigned VariableFont::spacing(void)
{
  return (spaceBetweenChars);               // return spacing value
}
 


#endif
