/***************************************************************************
                          Vector.h  -  description
                             -------------------
    begin                : Tue Sep 23 2003
    copyright            : (C) 2003 by pennere
    email                : pennere@cpsc.ucalgary.ca

 The ultimate (waste of time) Vector class.
   - Templated for any internal type.
   - Appropriate operators are overloaded.
   - Cast to pointer of internal type (for OpenGL).
   - Cast from any point type to any other point type (Compiler warnings are
     given for inappropriate casts eg. double to int).
   - Addition / Subtraction won't occur unless points are of the same type.
    
 ***************************************************************************/

#ifndef VECTOR_H
#define VECTOR_H


namespace Geometry
{


void TestVectors();


template<typename InternalType = float>
class CVector3
{
private:

    //Internal Data
	InternalType X,Y,Z;

public:
  
    ~CVector3(){}
    CVector3(InternalType X=0,InternalType Y=0,InternalType Z=0) 
    {
      SetVector(X,Y,Z);
    }
  
    void SetVector(InternalType X=0, InternalType Y=0,InternalType Z=0){SetX(X); SetY(Y); SetZ(Z);}
  
    void SetX(InternalType X){this->X = X;}
    void SetY(InternalType Y){this->Y = Y;}
    void SetZ(InternalType Z){this->Z = Z;}

    InternalType GetX() const {return X;}
    InternalType GetY() const {return Y;}
    InternalType GetZ() const {return Z;}

    //Note: We could add a template function to add any two types of points, but then we would
    //      need to determine the type for the resulting point (first or second point type). Currently,
    //      adding points of different types results in a compiler error, so we need to cast
    //      one of the points first. I like this more.
    
    friend CVector3<InternalType> operator+(const CVector3<InternalType>& P1,const CVector3<InternalType>& P2)
    {
        return CVector3<InternalType>(P1.X + P2.X , P1.Y + P2.Y , P1.Z + P2.Z);
    }

    friend CVector3<InternalType> operator-(const CVector3<InternalType>& P1,const CVector3<InternalType>& P2)
    {
        return CVector3<InternalType>(P1.X - P2.X , P1.Y - P2.Y , P1.Z - P2.Z);
    }

    friend CVector3<InternalType> operator-(const CVector3<InternalType>& P)
    {
        return CVector3<InternalType>(-P.X , -P.Y , -P.Z);
    }    
  
    friend CVector3<InternalType> operator*(const CVector3<InternalType>& P ,const InternalType Scalar)
    {
        return CVector3<InternalType>(P.X * Scalar , P.Y * Scalar , P.Z * Scalar);
    }

    friend CVector3<InternalType> operator*(const InternalType Scalar, const CVector3<InternalType>& P)
    {
        return CVector3<InternalType>(P.X * Scalar , P.Y * Scalar , P.Z * Scalar);
    }

    friend InternalType operator*(const CVector3<InternalType>& P1 ,const CVector3<InternalType>& P2)
    {
        return P1.X * P2.X + P1.Y * P2.X + P1.Z * P2.X;
    }

    friend CVector3<InternalType> operator/(const CVector3<InternalType>& P,const InternalType Scalar)
    {
        return CVector3<InternalType>(P.X / Scalar , P.Y / Scalar , P.Z / Scalar);
    }

    static CVector3<InternalType> Cross(const CVector3<InternalType>& P1 ,const CVector3<InternalType>& P2)
    {
        return CVector3<InternalType>(P1.Y * P2.Z - P1.Z * P2.Y, 
									  P1.Z * P2.X - P1.X * P2.Z,
									  P1.X * P2.Y - P1.Y * P2.X);
    }

    static InternalType Dot(const CVector3<InternalType>& P1 ,const CVector3<InternalType>& P2)
    {
		return  P1.X * P2.X  +  P1.Y * P2.Y  +  P1.Z + P2.Z;
    }

    //Cast operator to return an array of the internal type. Mainly for OpenGL
    operator InternalType* const()
    {
        return (InternalType*) this;
    }

    template<typename T> operator CVector3<T>() const 
    {
        return CVector3<T>(X,Y,Z);
    }
    
    static CVector3<InternalType> Abs(CVector3<InternalType>& P)  
    {
		return CVector3<InternalType>((P.X<0)?-P.X:P.X,(P.Y<0)?-P.Y:P.Y,(P.Z<0)?-P.Z:P.Z);
    }

    static InternalType Length(CVector3<InternalType>& P) 
    {
        return sqrt(P.X*P.X + P.Y*P.Y + P.Z*P.Z);
    }

    static InternalType LengthSq(CVector3<InternalType>& P) 
    {
        return P.X*P.X + P.Y*P.Y + P.Z*P.Z;
    }

    static CVector3<InternalType> Normalize(CVector3<InternalType>& P)
    {
        InternalType TLength = Length(P);
        return P/( (TLength != 0)? TLength : 1 );
    }

};



//NOTE: Due to things like affine combinations, points need basically all the operations
//      vectors do, in order for ease for use. For this reason it is left up to the user
//      of this class to name things as points or vectors.

typedef CVector3<float>   CPoint3f;
typedef CVector3<double>  CPoint3d;
typedef CVector3<int>     CPoint3i;


//typedef CPoint3 CVector3;  //doesn't work

#define CPoint3 CVector3

typedef CVector3<float>   CVector3f;
typedef CVector3<double>  CVector3d;
typedef CVector3<int>     CVector3i;


}
//namespace Geometry

#endif
