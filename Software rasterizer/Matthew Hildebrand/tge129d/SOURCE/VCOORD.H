/*****************************************************************************
*       The Graphics Engine version 1.29áD                                   *
*                                                                            *
*       The Graphics Engine code and documentation are Copyright (c) 1993    *
*       by Matthew Hildebrand.                                               *
*                                                                            *
*       Unauthorised usage or modification of any or all of The Graphics     *
*       Engine is strictly prohibited.                                       *
*****************************************************************************/

#ifndef VCOORDdotH
#define VCOORDdotH


//*****
//***** Virtual coordinate handling class
//*****

class VirtualCoord
{
  unsigned virtMaxX, virtMaxY, realMaxX, realMaxY;
public:
  inline VirtualCoord(void) { ; }
  inline VirtualCoord(unsigned virtMaxx, unsigned virtMaxy, unsigned
  	realMaxx, unsigned realMaxy);
  inline void virtParams(unsigned virtMaxx, unsigned virtMaxy);
  inline void realParams(unsigned realMaxx, unsigned realMaxy);
  inline void virtParams(unsigned *virtMaxx, unsigned *virtMaxy);
  inline void realParams(unsigned *realMaxx, unsigned *realMaxy);
  inline void realCoords(unsigned virtX, unsigned virtY, unsigned *realX,
  	unsigned *realY);
  inline unsigned realX(unsigned virtX);
  inline unsigned realY(unsigned virtY);
  inline void virtCoords(unsigned realX, unsigned realY, unsigned *virtX,
  	unsigned *virtY);
  inline unsigned virtX(unsigned realX);
  inline unsigned virtY(unsigned realY);
};


//*****
//***** Constructor
//*****

inline VirtualCoord::VirtualCoord(unsigned virtMaxx, unsigned virtMaxy,
	unsigned realMaxx, unsigned realMaxy)
{
  virtMaxX = virtMaxx;
  virtMaxY = virtMaxy;
  realMaxX = realMaxx;
  realMaxY = realMaxy;
}


//*****
//***** Routines to set the virtual and real maximums
//*****

inline void VirtualCoord::virtParams(unsigned virtMaxx, unsigned virtMaxy)
{
  virtMaxX = virtMaxx;
  virtMaxY = virtMaxy;
}

inline void VirtualCoord::realParams(unsigned realMaxx, unsigned realMaxy)
{
  realMaxX = realMaxx;
  realMaxY = realMaxy;
}


//*****
//***** Routines to get the virtual and real maximums
//*****

inline void VirtualCoord::virtParams(unsigned *virtMaxx, unsigned *virtMaxy)
{
  *virtMaxx = virtMaxX;
  *virtMaxy = virtMaxY;
}

inline void VirtualCoord::realParams(unsigned *realMaxx, unsigned *realMaxy)
{
  *realMaxx = realMaxX;
  *realMaxy = realMaxY;
}


//*****
//***** Calculate the real coordinates, given the virtual coordinates.
//*****

inline void VirtualCoord::realCoords(unsigned virtX, unsigned virtY,
	unsigned *realX, unsigned *realY)
{
  //*** x-coordinate
  *realX = (unsigned) (((long)virtX * (long)realMaxX) / (long)virtMaxX);
  if ((((long)virtX*realMaxX)%virtMaxX >= virtMaxX/2) && (*realX<realMaxX))
    *realX++;

  //*** y-coordinate
  *realY = (unsigned) (((long)virtY * (long)realMaxY) / (long)virtMaxY);
  if ((((long)virtY*realMaxY)%virtMaxY >= virtMaxY/2) && (*realY<realMaxY))
    *realY++;
}


//*****
//***** Calculate the real x-coordinate, given the virtual x-coordinate.
//*****

inline unsigned VirtualCoord::realX(unsigned virtX)
{
  register unsigned realX;

  realX = (unsigned) (((long)virtX * (long)realMaxX) / (long)virtMaxX);
  if ((((long)virtX*realMaxX)%virtMaxX >= virtMaxX/2) && (realX<realMaxX))
    realX++;

  return (realX);
}


//*****
//***** Calculate the real y-coordinate, given the virtual y-coordinate.
//*****

inline unsigned VirtualCoord::realY(unsigned virtY)
{
  register unsigned realY;

  realY = (unsigned) (((long)virtY * (long)realMaxY) / (long)virtMaxY);
  if ((((long)virtY*realMaxY)%virtMaxY >= virtMaxY/2) && (realY<realMaxY))
    realY++;

  return (realY);
}


//*****
//***** Calculate the virtual coordinates, given the real coordinates.
//*****

inline void VirtualCoord::virtCoords(unsigned realX, unsigned realY,
	unsigned *virtX, unsigned *virtY)
{
  //*** x-coordinate
  *virtX = (unsigned) (((long)realX * (long)virtMaxX) / (long)realMaxX);
  if ((((long)realX*virtMaxX)%realMaxX >= realMaxX/2) && (*virtX<virtMaxX))
    *virtX++;

  //*** y-coordinate
  *virtY = (unsigned) (((long)realY * (long)virtMaxY) / (long)realMaxY);
  if ((((long)realY*virtMaxY)%realMaxY >= realMaxY/2) && (*virtY<virtMaxY))
    *virtY++;
}


//*****
//***** Calculate the virtual x-coordinate, given the real x-coordinate
//*****

inline unsigned VirtualCoord::virtX(unsigned realX)
{
  register unsigned virtX;

  virtX = (unsigned) (((long)realX * (long)virtMaxX) / (long)realMaxX);
  if ((((long)realX*virtMaxX)%realMaxX >= realMaxX/2) && (virtX<virtMaxX))
    virtX++;

  return (virtX);
}


//*****
//***** Calculate the virtual y-coordinate, given the real y-coordinate
//*****

inline unsigned VirtualCoord::virtY(unsigned realY)
{
  register unsigned virtY;

  virtY = (unsigned) (((long)realY * (long)virtMaxY) / (long)realMaxY);
  if ((((long)realY*virtMaxY)%realMaxY >= realMaxY/2) && (virtY<virtMaxY))
    virtY++;

  return (virtY);
}


#endif