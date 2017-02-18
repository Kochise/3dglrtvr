/***************************************************************************
                         Transform.h  -  description
                             -------------------
    begin                : Tue Sep 23 2003
    copyright            : (C) 2003 by pennere
    email                : pennere@cpsc.ucalgary.ca

 The ultimate (waste of time) Transform class.
   - Templated for any internal type.
   - Appropriate operators are overloaded.


  The transform class should be thought of as a transformation 
  rather than a matrix. This means that transformations occur 
  in the order they are specified.
  
  Example:   
  
  D = A * B * C;
  D.TransformPoint(P);  
  
  In this case the point P will be transformed by A, then B and then C.

 ***************************************************************************/
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Matrix.h"
#include "Vector.h"
#include <vector>
#include <math.h>
#define PI 3.14159265



namespace Geometry
{

void TestTransforms();


template<typename InternalType = float>
class CTransform : public CMatrix<InternalType,4,4>
{
public:
	
	CTransform():CMatrix<InternalType,4,4>(){}
	CTransform(const InternalType* pData):CMatrix<InternalType,4,4>(pData){}
	virtual ~CTransform(){}
	
	static CTransform<InternalType> Identity();
	static CTransform<InternalType> Translation(const CVector3<InternalType>& OffsetVector);
	static CTransform<InternalType> Scaling(const InternalType& X,const InternalType& Y,const InternalType& Z);
	static CTransform<InternalType> Rotation(const InternalType& Angle,CVector3<InternalType> RotationAxis);	
	static CTransform<InternalType> Ortho(const InternalType& l, const InternalType& r, const InternalType& b,const InternalType& t,const InternalType& n,const InternalType& f);
	static CTransform<InternalType> Frustum(const InternalType& l,const InternalType& r,const InternalType& b,const InternalType& t,const InternalType& n,const InternalType& f);
	
	void Translate(const CVector3f& OffsetVector)
	{
		(*this) = (*this) * Translation(OffsetVector);
	}
	
	void Scale(const InternalType& X,const InternalType& Y,const InternalType& Z)
	{
		(*this) = (*this) * Scaling(X,Y,Z);
	}
	
	void Rotate(const InternalType& Angle,CVector3<InternalType> RotationAxis)
	{
		(*this) = (*this) * Rotation(Angle,RotationAxis);
	}
	
	void Ortho(InternalType& l, InternalType& r, InternalType& b, InternalType& t, InternalType& n, InternalType& f) 
	{
		(*this) = (*this) * Ortho(l,r,b,t,n,f);
	}
	
	void Frustum(InternalType& l, InternalType& r, InternalType& b, InternalType& t, InternalType& n, InternalType& f)
	{
		(*this) = (*this) * Frustum(l,r,b,t,n,f);
	}

	
	CPoint3<InternalType> TransformPoint(CPoint3<InternalType> P)
	{
		CMatrix<InternalType,4,1> PointMatrix;
		
		PointMatrix.SetValue(0,0,P.GetX());
		PointMatrix.SetValue(1,0,P.GetY());
		PointMatrix.SetValue(2,0,P.GetZ());
		PointMatrix.SetValue(3,0,1);
		
		PointMatrix = (*this) * PointMatrix;
		
		InternalType W = PointMatrix.GetValue(0,3);
		
		return CPoint3<InternalType>(PointMatrix.GetValue(0,0)/W,PointMatrix.GetValue(1,0)/W,PointMatrix.GetValue(2,0)/W);
	}
	
	CPoint3<InternalType> TransformVector(CPoint3<InternalType> P)
	{
		CMatrix<InternalType,4,1> PointMatrix;
		
		PointMatrix.SetValue(0,0,P.GetX());
		PointMatrix.SetValue(1,0,P.GetY());
		PointMatrix.SetValue(2,0,P.GetZ());
		PointMatrix.SetValue(3,0,0);
		
		PointMatrix = (*this) * PointMatrix;
		
		return CPoint3<InternalType>(PointMatrix.GetValue(0,0),PointMatrix.GetValue(1,0),PointMatrix.GetValue(2,0));
	}
};



typedef CTransform<float>  CTransformf;
typedef CTransform<double> CTransformd;
typedef CTransform<int>    CTransformi;


template<typename InternalType>
CTransform<InternalType> CTransform<InternalType>::Identity()
{
	InternalType Array[16] = { 1, 0, 0, 0,
		                        0, 1, 0, 0,
		                        0, 0, 1, 0,
		                        0, 0, 0, 1 };
	
	return  CTransform<InternalType>(Array);
}

template<typename InternalType>
CTransform<InternalType> CTransform<InternalType>::Translation(const CVector3<InternalType>& OffsetVector)
{
	InternalType x = OffsetVector.GetX();
	InternalType y = OffsetVector.GetY();
	InternalType z = OffsetVector.GetZ();
	
	InternalType Array[16] = { 1, 0, 0, x,
		                        0, 1, 0, y,
		                        0, 0, 1, z,
		                        0, 0, 0, 1 };
	
	return  CTransform<InternalType>(Array);
}

template<typename InternalType>
CTransform<InternalType> CTransform<InternalType>::Scaling(const InternalType& X,const InternalType& Y,const InternalType& Z)
{
	InternalType Array[16] = { X, 0, 0, 0,
		                        0, Y, 0, 0,
		                        0, 0, Z, 0,
		                        0, 0, 0, 1 };
	
	return  CTransform<InternalType>(Array);
}

template<typename InternalType>
CTransform<InternalType> CTransform<InternalType>::Rotation(const InternalType& Angle,CVector3<InternalType> RotationAxis)
{
	if(RotationAxis.MagnitudeSquared() != 1.0)
	{
		RotationAxis = RotationAxis.Normalize();
	}
	
	InternalType theta = Angle*PI/180;
	InternalType c = cos(theta);
	InternalType s = sin(theta);
	
	InternalType x = RotationAxis.GetX();
	InternalType y = RotationAxis.GetY();
	InternalType z = RotationAxis.GetZ();
	
	InternalType Array[16] = { x*x*(1-c)+c  , x*y*(1-c)-z*s , x*z*(1-c)+y*s , 0,
		                        y*x*(1-c)+z*s, y*y*(1-c)+c   , y*z*(1-c)-x*s , 0,
		                        x*z*(1-c)-y*s, y*z*(1-c)+x*s , z*z*(1-c)+c   , 0,
		                        0            , 0             ,  0            , 1 };
	
	return  CTransform<InternalType>(Array);
}

template<typename InternalType>
CTransform<InternalType> CTransform<InternalType>::Ortho(const InternalType& l,  //Left
	                                                            const InternalType& r,  //Right
	                                                            const InternalType& b,  //Bottom
	                                                            const InternalType& t,  //Top
	                                                            const InternalType& n,  //Near
	                                                            const InternalType& f)  //Far
{
	InternalType Array[16] = { 2/(r-l), 0      ,  0       ,   (r+l)/(r-l) ,
		                        0      , 2/(t-b),  0       ,   (t+b)/(t-b) ,
		                        0      , 0      , -2/(f-n) ,  -(f+n)/(f-n) ,
		                        0      , 0      ,  0       ,   1            };
	
	return  CTransform<InternalType>(Array);
}

template<typename InternalType>
static CTransform<InternalType> CTransform<InternalType>::Frustum(const InternalType& l,  //Left
	                                                              const InternalType& r,  //Right
	                                                              const InternalType& b,  //Bottom
	                                                              const InternalType& t,  //Top
	                                                              const InternalType& n,  //Near
	                                                              const InternalType& f)  //Far
{
	InternalType Array[16] = { (2*n)/(r-l), 0          ,  (r+l)/(r-l) ,  0            ,
		                        0          , (2*n)/(t-b),  (t+b)/(t-b) ,  0            ,
		                        0          , 0          , -(f+n)/(f-n) , -(2*f*n)/(f-n),
		                        0          , 0          , -1           ,  0             };
	
	return  CTransform<InternalType>(Array);
}


template<typename InternalType>
const CTransform<InternalType> operator*(const CTransform<InternalType>& M1, const CTransform<InternalType>& M2)
{
	CTransform<InternalType> NewTransform;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			InternalType tValue = 0;
			
			for(int k = 0; k < 4; k++)
				tValue += M2.GetValue(i,k) * M1.GetValue(k,j);  //Order is reversed so transforms occur in order
			
			NewTransform.SetValue(i,j,tValue);
		}
	}
	
	return NewTransform;
}






   

/*

	static CTransform     RotateXMatrix(InternalType Angle)
	{
		Angle = Angle*PI/180;
		InternalType c = cos(Angle);
		InternalType s = sin(Angle);

		InternalType Array[16] = { 1, 0 ,  0 , 0,
		                           0, c , -s , 0,
		                           0, s ,  c , 0,
		                           0, 0 ,  0 , 1 };

		return  CTransform<InternalType>(Array);
	}

	static CTransform     RotateYMatrix(InternalType Angle)
	{
		Angle = Angle*PI/180;
		InternalType c = cos(Angle);
		InternalType s = sin(Angle);

		InternalType Array[16] = {  c , 0, s , 0,
		                            0 , 1, 0 , 0,
		                           -s , 0, c , 0,
		                            0 , 0, 0 , 1 };

		return  CTransform<InternalType>(Array);
	}

	static CTransform     RotateZMatrix(InternalType Angle)
	{
		Angle = Angle*PI/180;
		InternalType c = cos(Angle);
		InternalType s = sin(Angle);

		InternalType Array[16] = { c ,-s , 0, 0,
		                           s , c , 0, 0,
		                           0 , 0 , 1, 0,
		                           0 , 0 , 0, 1 };

		return  CTransform<InternalType>(Array);
	}
*/

}  //namespace Geometry

#endif




