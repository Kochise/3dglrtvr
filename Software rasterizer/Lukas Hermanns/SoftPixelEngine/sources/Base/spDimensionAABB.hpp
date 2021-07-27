/*
 * Axis-Aligned BoundingBox header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_DIMENSION_AABB_H__
#define __SP_DIMENSION_AABB_H__


#include "Base/spStandard.hpp"
#include "Base/spDimensionVector3D.hpp"
#include "Base/spDimensionLine3D.hpp"


namespace sp
{
namespace dim
{


//! Axis-aligned-bounding-box (AABB) class for bounding volume tests.
template <typename T> class aabbox3d
{
    
    public:
        
        aabbox3d()
        {
        }
        aabbox3d(const vector3d<T> &MinEdge, const vector3d<T> &MaxEdge)
            : Min(MinEdge), Max(MaxEdge)
        {
        }
        aabbox3d(const line3d<T> &Line)
            : Min(Line.Start), Max(Line.End)
        {
        }
        aabbox3d(const aabbox3d<T> &other)
            : Min(other.Min), Max(other.Max)
        {
        }
        ~aabbox3d()
        {
        }
        
        /* === Extra functions === */
        
        //! Returns the center of the bounding box.
        inline vector3d<T> getCenter() const
        {
            return (Min + Max) / 2;
        }
        
        //! Returns the size of the bounding box.
        inline vector3d<T> getSize() const
        {
            return Max - Min;
        }
        
        //! Returns true if the two AABBs are intersecting.
        inline bool getBoxBoxIntersection(const aabbox3d<T> &other) const
        {
            return Min <= other.Max && Max >= other.Min;
        }
        
        //! Repairs the bounding box if any component of "Min" is greater then the corresponding component of "Max".
        inline void repair()
        {
            T tmp;
            
            if (Min.X > Max.X)
                tmp = Min.X, Min.X = Max.X, Max.X = tmp;
            if (Min.Y > Max.Y)
                tmp = Min.Y, Min.Y = Max.Y, Max.Y = tmp;
            if (Min.Z > Max.Z)
                tmp = Min.Z, Min.Z = Max.Z, Max.Z = tmp;
        }
        
        //! Returns the bounding box's volume.
        inline T getBoxVolume() const
        {
            return (Max.X - Min.X).getAbs() * (Max.Y - Min.Y).getAbs() * (Max.Z - Min.Z).getAbs();
        }
        
        //! Returns true if the specified point is inside the box.
        inline bool isPointInside(const vector3d<T> &Point) const
        {
            return (
                Point.X >= Min.X && Point.Y >= Min.Y && Point.Z >= Min.Z &&
                Point.X <= Max.X && Point.Y <= Max.Y && Point.Z <= Max.Z
            );
        }
        
        //! Returns a copy of this box multiplied by the specified size.
        inline aabbox3d<T> getScaled(const vector3d<T> &Size) const
        {
            return aabbox3d<T>(Min * Size, Max * Size);
        }
        
        /*inline plane3d<T> getLeft() const
        {
            return plane3d<T>(vector3d<T>(-1, 0, 0), -Min.X);
        }
        inline plane3d<T> getRight() const
        {
            return plane3d<T>(vector3d<T>(1, 0, 0), -Max.X);
        }
        inline plane3d<T> getTop() const
        {
            return plane3d<T>(vector3d<T>(0, 1, 0), -Max.Y);
        }
        inline plane3d<T> getBottom() const
        {
            return plane3d<T>(vector3d<T>(0, -1, 0), -Min.Y);
        }
        inline plane3d<T> getFront() const
        {
            return plane3d<T>(vector3d<T>(0, 0, -1), -Min.Z);
        }
        inline plane3d<T> getBack() const
        {
            return plane3d<T>(vector3d<T>(0, 0, 1), -Min.Z);
        }*/
        
        /* Members */
        
        vector3d<T> Min;    //!< Left-lower-front corner of the bounding box.
        vector3d<T> Max;    //!< Right-upper-back corner of the bounding box.
        
};

typedef aabbox3d<s32> aabbox3di;
typedef aabbox3d<f32> aabbox3df;


} // /namespace dim

} // /namespace sp


#endif



// ================================================================================
