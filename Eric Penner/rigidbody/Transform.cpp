/***************************************************************************
                          Transform.cpp  -  description
                             -------------------
    begin                : Fri Nov 7 2003
    copyright            : (C) 2003 by Eric Penner
    email                : pennere@cpsc.ucalgary.ca
 ***************************************************************************/

#include "Transform.h"

//The implementation is all in the .h file, since this is a template class.

namespace Geometry
{


void TestTransforms()
{
	CTransformf test;
	test = CTransformf::Identity();
	test = test * test;
}


} //namespace Geometry

