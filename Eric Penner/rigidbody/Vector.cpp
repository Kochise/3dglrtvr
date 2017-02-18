/***************************************************************************
                          point.cpp  -  description
                             -------------------
    begin                : Tue Sep 23 2003
    copyright            : (C) 2003 by pennere
    email                : pennere@cpsc.ucalgary.ca

***************************************************************************/

#include "Vector.h"

//The implementation is all in the .h file, since this is a template class.

namespace Geometry
{

void TestVectors()
{
	CVector3f test(1,2,3);

	test = test * 3;
	test = test / 3;

	test = CVector3f::Cross(test,test);

	float length = CVector3f::Dot(test,test);
}



}  //namespace Geometry