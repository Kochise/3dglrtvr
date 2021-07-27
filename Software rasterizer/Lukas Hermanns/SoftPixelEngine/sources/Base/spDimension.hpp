/*
 * Dimensions header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_DIMENSION_H__
#define __SP_DIMENSION_H__


/*
 * All dimension classes:
 * point2d, size2d, rect2d, vector3d, line3d, triangle3d, plane3d, matrix2, matrix4, matrix4, quaternion, view-frustum, array, universal-buffer
 */

/* 2nd dimension */
#include "Base/spDimensionPoint2D.hpp"
#include "Base/spDimensionSize2D.hpp"
#include "Base/spDimensionRect2D.hpp"

/* 3rd dimension */
#include "Base/spDimensionVector3D.hpp"
#include "Base/spDimensionLine3D.hpp"
#include "Base/spDimensionTriangle3D.hpp"
#include "Base/spDimensionPlane3D.hpp"

/* Matrix arithmetic */
#include "Base/spDimensionMatrix2.hpp"
#include "Base/spDimensionMatrix3.hpp"
#include "Base/spDimensionMatrix4.hpp"

/* Other dimension classes */
#include "Base/spDimensionQuaternion.hpp"
#include "Base/spDimensionAABB.hpp"
#include "Base/spDimensionOBB.hpp"
#include "Base/spDimensionContainer4.hpp"

/* Container class */
#include "Base/spDimensionSecureList.hpp"
#include "Base/spDimensionUniversalBuffer.hpp"


/* Extended defintions */ 

namespace sp
{
namespace dim
{


template <typename T> vector3d<T>::vector3d(const point2d<T> &other) : X(other.X), Y(other.Y), Z(0)
{
}

template <typename T> point2d<T>::point2d(const vector3d<T> &other) : X(other.X), Y(other.Y)
{
}


} // /namespace dim

} // /namespace sp


#endif



// ================================================================================
