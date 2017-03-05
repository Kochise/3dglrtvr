/*
 * Triangle 3D header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_DIMENSION_TRIANGLE3D_H__
#define __SP_DIMENSION_TRIANGLE3D_H__


#include "Base/spStandard.hpp"
#include "Base/spDimensionVector3D.hpp"
#include "Base/spDimensionLine3D.hpp"


namespace sp
{
namespace dim
{


template < typename T, class C = vector3d<T> > class triangle3d
{
    
    public:
        
        triangle3d()
        {
        }
        triangle3d(const C &NewPointA, const C &NewPointB, const C &NewPointC)
        {
            PointA = NewPointA; PointB = NewPointB; PointC = NewPointC;
        }
        triangle3d(const triangle3d<T> &other)
        {
            PointA = other.PointA; PointB = other.PointB; PointC = other.PointC;
        }
        triangle3d(const triangle3d<T, C*> &other)
        {
            PointA = *other.PointA; PointB = *other.PointB; PointC = *other.PointC;
        }
        ~triangle3d()
        {
        }
        
        /* === Operators === */
        
        inline void operator = (const triangle3d<T, C*> &other)
        {
            PointA = *other.PointA; PointB = *other.PointB; PointC = *other.PointC;
        }
        
        inline triangle3d<T> operator + (const vector3d<T> &Vector) const
        {
            return triangle3d<T>(PointA + Vector, PointB + Vector, PointC + Vector);
        }
        inline triangle3d<T>& operator += (const vector3d<T> &Vector)
        {
            PointA += Vector; PointB += Vector; PointC += Vector; return *this;
        }
        
        inline triangle3d<T> operator - (const vector3d<T> &Vector) const
        {
            return triangle3d<T>(PointA - Vector, PointB - Vector, PointC - Vector);
        }
        inline triangle3d<T>& operator -= (const vector3d<T> &Vector)
        {
            PointA -= Vector; PointB -= Vector; PointC -= Vector; return *this;
        }
        
        inline triangle3d<T> operator * (const vector3d<T> &Vector) const
        {
            return triangle3d<T>(PointA*Vector, PointB*Vector, PointC*Vector);
        }
        inline triangle3d<T>& operator *= (const vector3d<T> &Vector)
        {
            PointA *= Vector; PointB *= Vector; PointC *= Vector; return *this;
        }
        
        inline triangle3d<T> operator / (const vector3d<T> &Vector) const
        {
            return triangle3d<T>(PointA/Vector, PointB/Vector, PointC/Vector);
        }
        inline triangle3d<T>& operator /= (const vector3d<T> &Vector)
        {
            PointA /= Vector; PointB /= Vector; PointC /= Vector; return *this;
        }
        
        inline triangle3d<T> operator - () const
        {
            return triangle3d<T>(-PointA, -PointB, -PointC);
        }
        
        /* === Additional operators === */
        
        inline const vector3d<T> operator [] (s32 i) const
        {
            switch (i)
            {
                case 0: return PointA;
                case 1: return PointB;
                case 2: return PointC;
            }
            return vector3d<T>();
        }
        
        inline vector3d<T>& operator [] (s32 i)
        {
            return *(&PointA + i);
        }
        
        /* === Extra functions === */
        
        inline vector3d<T> getNormal() const
        {
            return dim::vector3d<T>( (PointB - PointA).cross(PointC - PointA) ).normalize();
        }
        
        inline vector3d<T> getCenter() const
        {
            return (PointA + PointB + PointC) / 3;
        }
        
        inline T getArea() const
        {
            return dim::vector3d<T>( (PointB - PointA).cross(PointC - PointA) ).getLength() / 2;
        }
        
        inline bool isPointInside(const vector3d<T> &Vector) const
        {
            return
                isSameSide(Vector, PointA, PointB, PointC) &&
                isSameSide(Vector, PointB, PointA, PointC) &&
                isSameSide(Vector, PointC, PointA, PointB);
        }
        
        inline bool isSameSide(
            const vector3d<T> &OtherPoint1, const vector3d<T> &OtherPoint2,
            const vector3d<T> &OtherPointA, const vector3d<T> &OtherPointB) const
        {
            vector3d<T> Difference = OtherPointB - OtherPointA;
            vector3d<T> OtherPoint3 = Difference.cross(OtherPoint1 - OtherPointA);
            vector3d<T> OtherPoint4 = Difference.cross(OtherPoint2 - OtherPointA);
            return OtherPoint3.dot(OtherPoint4) >= (T)0;
        }
        
        inline bool isFrontSide() const
        {
            return (
                (PointB.X - PointA.X)*(PointC.Y - PointA.Y) - (PointB.Y - PointA.Y)*(PointC.X - PointA.X)
            ) >= (T)0;
        }
        
        inline triangle3d<T> getSwapedTriangle() const
        {
            return triangle3d<T>(PointC, PointB, PointA);
        }
        
        inline line3d<T> getBox() const
        {
            line3d<T> TriangleBox;
            
            TriangleBox.Start.X = getMin(PointA.X, PointB.X, PointC.X);
            TriangleBox.Start.Y = getMin(PointA.Y, PointB.Y, PointC.Y);
            TriangleBox.Start.Z = getMin(PointA.Z, PointB.Z, PointC.Z);
            
            TriangleBox.End.X   = getMax(PointA.X, PointB.X, PointC.X);
            TriangleBox.End.Y   = getMax(PointA.Y, PointB.Y, PointC.Y);
            TriangleBox.End.Z   = getMax(PointA.Z, PointB.Z, PointC.Z);
            
            return TriangleBox;
        }
        
        inline bool equal(const triangle3d<T> &other, f32 Precision = 1.0e-6) const
        {
            return
                PointA.equal(other.PointA, Precision) &&
                PointB.equal(other.PointB, Precision) &&
                PointC.equal(other.PointC, Precision);
        }
        inline bool empty() const
        {
            return PointA.empty() && PointB.empty() && PointC.empty();
        }
        
        inline bool isFaceVisible() const
        {
            return (PointB.X - PointA.X)*(PointC.Y - PointA.Y) - (PointB.Y - PointA.Y)*(PointC.X - PointA.X) >= 0.0f;
        }
        
        template <typename T2, class C2> inline triangle3d<T2, C2> cast() const
        {
            triangle3d<T2, C2> Tri;
            
            Tri.PointA = PointA.cast<T2>();
            Tri.PointB = PointB.cast<T2>();
            Tri.PointC = PointC.cast<T2>();
            
            return Tri;
        }
        
        /* Members */
        
        C PointA, PointB, PointC;

    private:
        
        /* Functions */
        
        inline T getMin(const T &a, const T &b, const T &c) const
        {
            if (a <= b && a <= c) return a;
            if (b <= a && b <= c) return b;
            return c;
        }
        inline T getMax(const T &a, const T &b, const T &c) const
        {
            if (a >= b && a >= c) return a;
            if (b >= a && b >= c) return b;
            return c;
        }
        
};

typedef triangle3d<s32> triangle3di;
typedef triangle3d<f32> triangle3df;

typedef triangle3d<s32, vector3di*> ptriangle3di;
typedef triangle3d<f32, vector3df*> ptriangle3df;


} // /namespace dim

} // /namespace sp


#endif



// ================================================================================
