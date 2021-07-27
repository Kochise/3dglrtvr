/*
 * Orientated BoundingBox header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_DIMENSION_OBB_H__
#define __SP_DIMENSION_OBB_H__


#include "Base/spStandard.hpp"
#include "Base/spDimensionVector3D.hpp"
#include "Base/spDimensionAABB.hpp"


namespace sp
{
namespace dim
{


template <typename T> class obbox3d
{
    
    public:
        
        obbox3d()
        {
        }
        obbox3d(const vector3d<T> &Min, const vector3d<T> &Max)
        {
            vector3d<T> HalfSize(Max - Min);
            Center = (Min + Max) / 2;
            Axis.X = vector3d<T>(HalfSize.X, 0, 0);
            Axis.Y = vector3d<T>(0, HalfSize.Y, 0);
            Axis.Z = vector3d<T>(0, 0, HalfSize.Z);
        }
        obbox3d(const vector3d<T> &BoxCenter, const vector3d<T> &AxisX, const vector3d<T> &AxisY, const vector3d<T> &AxisZ)
        {
            Center = BoxCenter;
            Axis.X = AxisX;
            Axis.Y = AxisY;
            Axis.Z = AxisZ;
            updateHalfSize();
        }
        obbox3d(const obbox3d<T> &other)
        {
            Center      = other.Center;
            Axis        = other.Axis;
            HalfSize    = other.HalfSize;
        }
        ~obbox3d()
        {
        }
        
        /* === Extra functions === */
        
        inline void updateHalfSize()
        {
            HalfSize.X  = Axis.X.getLength();
            HalfSize.Y  = Axis.Y.getLength();
            HalfSize.Z  = Axis.Z.getLength();
        }
        
        /* Members */
        
        vector3d<T> Center;
        vector3d< vector3d<T> > Axis;
        vector3d<T> HalfSize; // (HalfSize.X is the length of Axis.X vector)
        
};

typedef obbox3d<s32> obbox3di;
typedef obbox3d<f32> obbox3df;


} // /namespace dim

} // /namespace sp


#endif



// ================================================================================
