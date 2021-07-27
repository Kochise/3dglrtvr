/*
 * Line 3D header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_DIMENSION_LINE3D_H__
#define __SP_DIMENSION_LINE3D_H__


#include "Base/spStandard.hpp"
#include "Base/spDimensionVector3D.hpp"


namespace sp
{
namespace dim
{


//! 3D line class for intersection tests or 3D drawing information.
template <typename T, template <typename> class Vec> class linekd
{
        
        typedef Vec<T> V;
        typedef linekd<T, Vec> L;
        
    public:
        
        linekd()
        {
        }
        linekd(const V &RayStart, const V &RayEnd)
            : Start(RayStart), End(RayEnd)
        {
        }
        linekd(const L &other)
            : Start(other.Start), End(other.End)
        {
        }
        ~linekd()
        {
        }
        
        /* === Operators === */
        
        inline bool operator == (const L &other)
        {
            return Start == other.Start && End == other.End;
        }
        inline bool operator != (const L &other)
        {
            return Start != other.Start && End != other.End;
        }
        
        inline L operator + (const L &other) const
        {
            return L(Start + other.Start, End + other.End);
        }
        inline L& operator += (const L &other)
        {
            Start += other.Start; End += other.End; return *this;
        }
        
        inline L operator - (const L &other) const
        {
            return L(Start - other.Start, End - other.End);
        }
        inline L& operator -= (const L &other)
        {
            Start -= other.Start; End -= other.End; return *this;
        }
        
        inline L operator / (const L &other) const
        {
            return L(Start / other.Start, End / other.End);
        }
        inline L& operator /= (const L &other)
        {
            Start /= other.Start; End /= other.End; return *this;
        }
        
        inline L operator * (const L &other) const
        {
            return L(Start * other.Start, End * other.End);
        }
        inline L& operator *= (const L &other)
        {
            Start *= other.Start; End *= other.End; return *this;
        }
        
        inline L operator - () const
        {
            return L(-Start, -End);
        }
        
        /* === Extra functions === */
        
        //! Returns the line's center ((Start + End) / 2).
        inline V getCenter() const
        {
            return (Start + End) / 2;
        }
        
        //! Returns the line's direction (End - Start).
        inline V getDirection() const
        {
            return End - Start;
        }
        
        //! Returns true if the two lines (understood as axis-aligned-bounding-boxes) are intersecting.
        inline bool checkBoxBoxIntersection(const L &Line) const
        {
            return Start <= Line.End && End >= Line.Start;
        }
        
        //! Returns true if the specified point lies between the line's start and end point.
        inline bool isPointInside(const V &Point) const
        {
            return Point.isBetweenPoints(Start, End);
        }
        
        //! Returns the closest point on the line between the specfied point and the line.
        inline V getClosestPoint(const V &Point) const
        {
            V Pos = Point - Start;
            V Dir = End - Start;
            
            T Len = (T)Dir.getLength();
            Dir /= Len;
            T Factor = Dir.dot(Pos);
            
            if (Factor < (T)0.0)
                return Start;
            if (Factor > Len)
                return End;
            
            Dir *= Factor;
            
            return Start + Dir;
        }
        
        //! Returns the distance between the line and the specified point.
        inline T getPointDistance(const V &Point) const
        {
            V ClstPnt(getClosestPoint(Point));
            const V DistVec(ClstPnt - Point);
            const T Dist = ( DistVec.X*DistVec.X + DistVec.Y*DistVec.Y + DistVec.Z*DistVec.Z );
            return Dist > (T)0.0 ? (T)sqrt(Dist) : Dist;
        }
        
        /* Members */
        
        V Start, End;
        
};

template <typename T> class line3d : public linekd<T, vector3d>
{
    
    public:
        
        line3d()
        {
        }
        line3d(const vector3d<T> &RayStart, const vector3d<T> &RayEnd) : linekd<T, vector3d>(RayStart, RayEnd)
        {
        }
        line3d(const line3d<T> &other) : linekd<T, vector3d>(other)
        {
        }
        ~line3d()
        {
        }
        
};

typedef line3d<s32> line3di;
typedef line3d<f32> line3df;


} // /namespace dim

} // /namespace sp


#endif



// ================================================================================
