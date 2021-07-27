/*
 * ViewFrustum file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Base/spMathViewFrustum.hpp"


namespace sp
{
namespace math
{


ViewFrustum::ViewFrustum()
{
}
ViewFrustum::ViewFrustum(const dim::matrix4f &Matrix)
{
    setFrustum(Matrix);
}
ViewFrustum::~ViewFrustum()
{
}

void ViewFrustum::setFrustum(const dim::matrix4f &Matrix)
{
    // Near plane
    Planes[VIEWFRUSTUM_NEAR     ].Normal.X = Matrix[ 3] + Matrix[ 2];
    Planes[VIEWFRUSTUM_NEAR     ].Normal.Y = Matrix[ 7] + Matrix[ 6];
    Planes[VIEWFRUSTUM_NEAR     ].Normal.Z = Matrix[11] + Matrix[10];
    Planes[VIEWFRUSTUM_NEAR     ].Distance = Matrix[15] + Matrix[14];
    
    // Far plane
    Planes[VIEWFRUSTUM_FAR      ].Normal.X = Matrix[ 3] - Matrix[ 2];
    Planes[VIEWFRUSTUM_FAR      ].Normal.Y = Matrix[ 7] - Matrix[ 6];
    Planes[VIEWFRUSTUM_FAR      ].Normal.Z = Matrix[11] - Matrix[10];
    Planes[VIEWFRUSTUM_FAR      ].Distance = Matrix[15] - Matrix[14];
    
    // Bottom plane
    Planes[VIEWFRUSTUM_BOTTOM   ].Normal.X = Matrix[ 3] + Matrix[ 1];
    Planes[VIEWFRUSTUM_BOTTOM   ].Normal.Y = Matrix[ 7] + Matrix[ 5];
    Planes[VIEWFRUSTUM_BOTTOM   ].Normal.Z = Matrix[11] + Matrix[ 9];
    Planes[VIEWFRUSTUM_BOTTOM   ].Distance = Matrix[15] + Matrix[13];
    
    // Top plane
    Planes[VIEWFRUSTUM_TOP      ].Normal.X = Matrix[ 3] - Matrix[ 1];
    Planes[VIEWFRUSTUM_TOP      ].Normal.Y = Matrix[ 7] - Matrix[ 5];
    Planes[VIEWFRUSTUM_TOP      ].Normal.Z = Matrix[11] - Matrix[ 9];
    Planes[VIEWFRUSTUM_TOP      ].Distance = Matrix[15] - Matrix[13];
    
    // Left plane
    Planes[VIEWFRUSTUM_LEFT     ].Normal.X = Matrix[ 3] + Matrix[ 0];
    Planes[VIEWFRUSTUM_LEFT     ].Normal.Y = Matrix[ 7] + Matrix[ 4];
    Planes[VIEWFRUSTUM_LEFT     ].Normal.Z = Matrix[11] + Matrix[ 8];
    Planes[VIEWFRUSTUM_LEFT     ].Distance = Matrix[15] + Matrix[12];
    
    // Right plane
    Planes[VIEWFRUSTUM_RIGHT    ].Normal.X = Matrix[ 3] - Matrix[ 0];
    Planes[VIEWFRUSTUM_RIGHT    ].Normal.Y = Matrix[ 7] - Matrix[ 4];
    Planes[VIEWFRUSTUM_RIGHT    ].Normal.Z = Matrix[11] - Matrix[ 8];
    Planes[VIEWFRUSTUM_RIGHT    ].Distance = Matrix[15] - Matrix[12];
    
    // Normalize all planes
    for (s32 i = 0; i < VIEWFRUSTUM_PLANE_COUNT; ++i)
    {
        const f32 Len = -1.0f / Planes[i].Normal.getLength();
        
        Planes[i].Normal    *= Len;
        Planes[i].Distance  *= Len;
    }
}

dim::vector3df ViewFrustum::getLeftUp() const
{
    dim::vector3df Point;
    Planes[VIEWFRUSTUM_FAR].checkMultiplePlaneIntersection(Planes[VIEWFRUSTUM_TOP], Planes[VIEWFRUSTUM_LEFT], Point);
    return Point;
}

dim::vector3df ViewFrustum::getLeftDown() const
{
    dim::vector3df Point;
    Planes[VIEWFRUSTUM_FAR].checkMultiplePlaneIntersection(Planes[VIEWFRUSTUM_BOTTOM], Planes[VIEWFRUSTUM_LEFT], Point);
    return Point;
}

dim::vector3df ViewFrustum::getRightUp() const
{
    dim::vector3df Point;
    Planes[VIEWFRUSTUM_FAR].checkMultiplePlaneIntersection(Planes[VIEWFRUSTUM_TOP], Planes[VIEWFRUSTUM_RIGHT], Point);
    return Point;
}

dim::vector3df ViewFrustum::getRightDown() const
{
    dim::vector3df Point;
    Planes[VIEWFRUSTUM_FAR].checkMultiplePlaneIntersection(Planes[VIEWFRUSTUM_BOTTOM], Planes[VIEWFRUSTUM_RIGHT], Point);
    return Point;
}

bool ViewFrustum::isPointInside(const dim::vector3df &Point, const f32 Radius) const
{
    return !(
        Planes[VIEWFRUSTUM_NEAR     ].getPointDistance(Point) < -Radius ||
        Planes[VIEWFRUSTUM_FAR      ].getPointDistance(Point) < -Radius ||
        Planes[VIEWFRUSTUM_LEFT     ].getPointDistance(Point) < -Radius ||
        Planes[VIEWFRUSTUM_RIGHT    ].getPointDistance(Point) < -Radius ||
        Planes[VIEWFRUSTUM_BOTTOM   ].getPointDistance(Point) < -Radius ||
        Planes[VIEWFRUSTUM_TOP      ].getPointDistance(Point) < -Radius
    );
}

bool ViewFrustum::isBoundBoxInside(const dim::aabbox3df &Box, const dim::matrix4f &Matrix) const
{
    const dim::matrix4f InvMatrix(Matrix.getInverse());
    
    return !(
        (InvMatrix * Planes[VIEWFRUSTUM_NEAR    ]).getAABBoxRelation(Box) == dim::RELATION_BACK ||
        (InvMatrix * Planes[VIEWFRUSTUM_FAR     ]).getAABBoxRelation(Box) == dim::RELATION_BACK ||
        (InvMatrix * Planes[VIEWFRUSTUM_LEFT    ]).getAABBoxRelation(Box) == dim::RELATION_BACK ||
        (InvMatrix * Planes[VIEWFRUSTUM_RIGHT   ]).getAABBoxRelation(Box) == dim::RELATION_BACK ||
        (InvMatrix * Planes[VIEWFRUSTUM_BOTTOM  ]).getAABBoxRelation(Box) == dim::RELATION_BACK ||
        (InvMatrix * Planes[VIEWFRUSTUM_TOP     ]).getAABBoxRelation(Box) == dim::RELATION_BACK
    );
}


} // /namespace dim

} // /namespace sp



// ================================================================================
