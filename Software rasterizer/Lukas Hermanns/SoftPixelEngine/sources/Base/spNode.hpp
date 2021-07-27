/*
 * Basic node header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_SCENE_NODE_H__
#define __SP_SCENE_NODE_H__


#include "Base/spStandard.hpp"
#include "Base/spDimension.hpp"
#include "Base/spBasicObject.hpp"
#include "Base/spInputOutputString.hpp"

#include <list>
#include <algorithm>


namespace sp
{


class SP_EXPORT Node : public BasicObject
{
    
    public:
        
        /* === Header === */
        
        Node();
        virtual ~Node();
        
        /* === Visibility === */
        
        //! Enables or disables the node.
        virtual inline void setVisible(bool isVisible)
        {
            isVisible_ = isVisible;
        }
        
        //! Returns true if this node is visible (or rather enabled).
        virtual inline bool getVisible() const
        {
            return isVisible_;
        }
        
        /* === Identification === */
        
        //! Sets the node's name.
        virtual inline void setName(const io::stringc &Name)
        {
            Name_ = Name;
        }
        
        //! Returns the node's name.
        virtual inline io::stringc getName() const
        {
            return Name_;
        }
        
        /* === Parents === */
        
        //! Sets the parent node.
        virtual inline void setParent(Node* Parent)
        {
            Parent_ = Parent;
        }
        
        //! Returns the parent node.
        virtual inline Node* getParent() const
        {
            return Parent_;
        }
        
        /* === Children === */
        
        //! Adds the specified child.
        virtual void addChild(Node* Child);
        
        //! Adds the specified children.
        virtual void addChildren(const std::list<Node*> &Children);
        
        //! Removes the specified child and returns true if the child could be removed.
        virtual bool removeChild(Node* Child);
        
        //! Removes the first child and returns true if a child could be removed.
        virtual bool removeChild();
        
        //! Removes the specified children and returns the count of removed children.
        virtual u32 removeChildren(const std::list<Node*> &Children);
        
        //! Removes all children.
        virtual void removeChildren();
        
        //! Returns the children list.
        virtual inline const std::list<Node*> &getChildren() const
        {
            return Children_;
        }
        
    protected:
        
        /* Members */
        
        io::stringc Name_;
        bool isVisible_;
        
        Node* Parent_;
        std::list<Node*> Children_;
        
};


} // /namespace sp


#endif



// ================================================================================
