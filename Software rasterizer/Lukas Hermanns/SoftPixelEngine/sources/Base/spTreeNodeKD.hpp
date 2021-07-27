/*
 * Tree node KD header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_TREENODEKD_H__
#define __SP_TREENODEKD_H__


#include "Base/spStandard.hpp"
#include "Base/spTreeNode.hpp"


namespace sp
{
namespace scene
{


//! kd-Tree node axles.
enum EKDTreeAxles
{
    KDTREE_XAXIS = 0,   //!< X axis.
    KDTREE_YAXIS,       //!< Y axis.
    KDTREE_ZAXIS,       //!< Z axis.
};


//! kd-Tree node class for universal usage.
class SP_EXPORT KDTreeNode : public TreeNode
{
    
    public:
        
        KDTreeNode(TreeNode* Parent = 0);
        ~KDTreeNode();
        
        /* === Functions === */
        
        bool isLeafNode() const;
        
        void addChildren();
        void addChildren(const EKDTreeAxles Axis, f32 Distance);
        
        void removeChildren();
        
        const TreeNode* findTreeNode(const dim::vector3df &Point) const;
        
        /* === Inline functions === */
        
        inline KDTreeNode* getChildNear() const
        {
            return ChildNear_;
        }
        inline KDTreeNode* getChildFar() const
        {
            return ChildFar_;
        }
        
        inline void setAxis(const EKDTreeAxles Axis)
        {
            Axis_ = Axis;
        }
        inline EKDTreeAxles getAxis() const
        {
            return Axis_;
        }
        
        inline void setDistance(f32 Distance)
        {
            Distance_ = Distance;
        }
        inline f32 getDistance() const
        {
            return Distance_;
        }
        
        inline void setBox(const dim::aabbox3df &Box)
        {
            Box_ = Box;
        }
        inline dim::aabbox3df getBox() const
        {
            return Box_;
        }
        
    private:
        
        /* Functions */
        
        void setupBox();
        
        /* Members */
        
        KDTreeNode* ChildNear_;
        KDTreeNode* ChildFar_;
        
        EKDTreeAxles Axis_;
        f32 Distance_;
        
        dim::aabbox3df Box_;
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
