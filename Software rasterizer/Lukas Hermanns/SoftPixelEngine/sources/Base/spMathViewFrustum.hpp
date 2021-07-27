/*
 * ViewFrustum header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_MATH_VIEWFRUSTUM_H__
#define __SP_MATH_VIEWFRUSTUM_H__


#include "Base/spStandard.hpp"
#include "Base/spDimensionMatrix4.hpp"
#include "Base/spDimensionVector3D.hpp"
#include "Base/spDimensionLine3D.hpp"
#include "Base/spDimensionPlane3D.hpp"
#include "Base/spDimensionAABB.hpp"


namespace sp
{
namespace math
{


//! View frustum plane sides
enum EViewFrustumPlanes
{
    VIEWFRUSTUM_NEAR = 0,
    VIEWFRUSTUM_FAR,
    VIEWFRUSTUM_LEFT,
    VIEWFRUSTUM_RIGHT,
    VIEWFRUSTUM_BOTTOM,
    VIEWFRUSTUM_TOP,
    
    VIEWFRUSTUM_PLANE_COUNT,
};


/**
View frustum for Camera objects. Each Camera object has one view frustum which defines the whole area
which is visible for the user with all its near- and far clippings planes etc.
The view frustum can be used for "Frustum Culling" optimization where each object which is outside the view frustum
will not be rendered. This safes a lot of time because the renderer does not need to pass each triangle of the mesh
when it's anyway completly invisible.
\see Node::setBoundingType, Node::setBoundingBox, Node::setBoundingSphere
*/
class SP_EXPORT ViewFrustum
{
    
    public:
        
        ViewFrustum();
        ViewFrustum(const dim::matrix4f &Matrix);
        ~ViewFrustum();
        
        /* Functions */
        
        //! Transforms the whole frustum by the specified 4x4 matrix.
        void setFrustum(const dim::matrix4f &Matrix);
        
        //! \return 3D direction vector along the left-up frustum edge.
        dim::vector3df getLeftUp() const;
        
        //! \return 3D direction vector along the left-down frustum edge.
        dim::vector3df getLeftDown() const;
        
        //! \return 3D direction vector along the right-up frustum edge.
        dim::vector3df getRightUp() const;
        
        //! \return 3D direction vector along the right-down frustum edge.
        dim::vector3df getRightDown() const;
        
        //! \return True if the specified 3D point is inside the frustum.
        bool isPointInside(const dim::vector3df &Point, const f32 Radius = 0.0f) const;
        
        //! \return True if the specified box is inside the frustum.
        bool isBoundBoxInside(const dim::aabbox3df &Box, const dim::matrix4f &Matrix) const;
        
        /* Inline functions */
        
        //! \return Reference the the specified frustum plane.
        inline dim::plane3df& getPlane(const EViewFrustumPlanes Type)
        {
            return Planes[Type];
        }
        
    private:
        
        /* Members */
        
        dim::plane3df Planes[VIEWFRUSTUM_PLANE_COUNT];
        
};


} // /namespace dim

} // /namespace sp


#endif



// ================================================================================
