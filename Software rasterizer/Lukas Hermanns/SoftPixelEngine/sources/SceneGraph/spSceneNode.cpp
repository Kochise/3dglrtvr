/*
 * Scene node file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "SceneGraph/spSceneNode.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;
extern scene::SceneGraph* __spSceneManager;

namespace scene
{


/*
 * ======= Internal matrices =======
 */

dim::matrix4f spProjectionMatrix;
dim::matrix4f spViewMatrix;
dim::matrix4f spViewInvMatrix;
dim::matrix4f spWorldMatrix;
dim::matrix4f spTextureMatrix[MAX_COUNT_OF_TEXTURES];
dim::matrix4f spColorMatrix;


/*
 * ======= Constructor & destructor =======
 */

SceneNode::SceneNode(const ENodeTypes Type)
    : Node(), Type_(Type), SceneParent_(0)
{
    BoundingType_   = BOUNDING_NONE;
    BoundingSphere_ = 0.0f;
}
SceneNode::~SceneNode()
{
    clearAnimations();
}


/* === Detailed localisation === */

void SceneNode::setPositionMatrix(const dim::matrix4f &Position, bool isGlobal)
{
    if (!isGlobal || !SceneParent_)
    {
        Position_ = Position;
        return;
    }
    
    Position_ = dim::getPositionMatrix(
        (SceneParent_->getGlobalLocationInverse() * Position).getPosition()
    );
}
dim::matrix4f SceneNode::getPositionMatrix(bool isGlobal) const
{
    if (!isGlobal || !SceneParent_)
        return Position_;
    
    return dim::getPositionMatrix(
        (SceneParent_->getGlobalLocation() * Position_).getPosition()
    );
}

void SceneNode::setRotationMatrix(const dim::matrix4f &Rotation, bool isGlobal)
{
    if (!isGlobal || !SceneParent_)
    {
        Rotation_ = Rotation;
        return;
    }
    
    Rotation_ = SceneParent_->getRotationMatrix(true).getInverse() * Rotation;
}
dim::matrix4f SceneNode::getRotationMatrix(bool isGlobal) const
{
    if (!isGlobal || !SceneParent_)
        return Rotation_;
    
    return SceneParent_->getRotationMatrix(true) * Rotation_;
}

void SceneNode::setScaleMatrix(const dim::matrix4f &Scale, bool isGlobal)
{
    if (!isGlobal || !SceneParent_)
    {
        Scale_ = Scale;
        return;
    }
    
    Scale_ = SceneParent_->getScaleMatrix(true).getInverse() * Scale;
}
dim::matrix4f SceneNode::getScaleMatrix(bool isGlobal) const
{
    if (!isGlobal || !SceneParent_)
        return Scale_;
    
    return SceneParent_->getScaleMatrix(true) * Scale_;
}


void SceneNode::setPosition(const dim::vector3df &Position, bool isGlobal)
{
    setPositionMatrix(dim::getPositionMatrix(Position), isGlobal);
}
dim::vector3df SceneNode::getPosition(bool isGlobal) const
{
    return getPositionMatrix(isGlobal).getPosition();
}

void SceneNode::setRotation(const dim::vector3df &Rotation, bool isGlobal)
{
    setRotationMatrix(dim::getRotationMatrix(Rotation), isGlobal);
}
dim::vector3df SceneNode::getRotation(bool isGlobal) const
{
    return getRotationMatrix(isGlobal).getRotation();
}

void SceneNode::setScale(const dim::vector3df &Scale, bool isGlobal)
{
    setScaleMatrix(dim::getScaleMatrix(Scale), isGlobal);
}
dim::vector3df SceneNode::getScale(bool isGlobal) const
{
    return getScaleMatrix(isGlobal).getScale();
}


void SceneNode::move(const dim::vector3df &Direction)
{
    translate(Rotation_ * Direction);
}
void SceneNode::turn(const dim::vector3df &Rotation)
{
    dim::matrix4f Mat;
    Mat.setRotation(Rotation);
    Rotation_ *= Mat;
}
void SceneNode::translate(const dim::vector3df &Direction)
{
    Position_.setPosition(Position_.getPosition() + Direction);
}
void SceneNode::transform(const dim::vector3df &Size)
{
    Scale_.setScale(Scale_.getScale() + Size);
}


void SceneNode::lookAt(const dim::vector3df &Position, bool isGlobal)
{
    /* Temporary variables */
    dim::vector3df Pos(getPosition(isGlobal));
    dim::vector3df Rot;
    
    /* Calculate rotation */
    if (!math::Equal(Position.Y, Pos.Y))
        Rot.X = ASIN( (Position.Y - Pos.Y) / math::getDistance(Pos, Position) );
    if (!math::Equal(Position.X, Pos.X))
        Rot.Y = -ASIN( (Position.X - Pos.X) / math::getDistance(Pos.X, Pos.Z, Position.X, Position.Z) );
    
    if (Pos.Z < Position.Z)
        Rot.Y = 180.0f - Rot.Y;
    
    setRotation(Rot, isGlobal);
}


/* === Animation === */

Animation* SceneNode::addAnimation(const EAnimationTypes Type, const io::stringc &Name)
{
    /* Check the animation type */
    if (Type != ANIMATION_NODE)
    {
        io::Log::error("Invalid animation type for node");
        return 0;
    }
    
    /* Create a new animation */
    Animation* NewAnim = new AnimationNode(this);
    NewAnim->setName(Name);
    
    /* Add the animation to the node and scene list */
    AnimationList_.push_back(NewAnim);
    __spSceneManager->AnimationList_.push_back(NewAnim);
    
    return NewAnim;
}

void SceneNode::removeAnimation(Animation* Anim)
{
    if (!Anim)
        return;
    
    std::list<Animation*>::iterator it;
    
    /* Remove the animation from the node list */
    for (it = AnimationList_.begin(); it != AnimationList_.end(); ++it)
    {
        if (*it == Anim)
        {
            AnimationList_.erase(it);
            break;
        }
    }
    
    /* Remove the animation from the scene list */
    for (it = __spSceneManager->AnimationList_.begin(); it != __spSceneManager->AnimationList_.end(); ++it)
    {
        if (*it == Anim)
        {
            __spSceneManager->AnimationList_.erase(it);
            break;
        }
    }
    
    /* Delete the animation */
    MemoryManager::deleteMemory(Anim);
}

void SceneNode::clearAnimations()
{
    std::list<Animation*>* AnimList = &__spSceneManager->AnimationList_;
    
    for (std::list<Animation*>::iterator it = AnimationList_.begin(), itSub; it != AnimationList_.end(); ++it)
    {
        if (*it)
        {
            for (itSub = AnimList->begin(); itSub != AnimList->end(); ++itSub)
            {
                if (*itSub == *it)
                {
                    AnimList->erase(itSub);
                    break;
                }
            }
            
            switch ((*it)->getType())
            {
                case ANIMATION_NODE:
                    delete static_cast<AnimationNode*>(*it); break;
                case ANIMATION_MORPHTARGET:
                    delete static_cast<AnimationMorphTarget*>(*it); break;
                case ANIMATION_SKELETAL:
                    delete static_cast<AnimationSkeletal*>(*it); break;
            }
        }
    }
}

Animation* SceneNode::getAnimation(const io::stringc Name) const
{
    for (std::list<Animation*>::const_iterator it = AnimationList_.begin(); it != AnimationList_.end(); ++it)
    {
        if ((*it)->getName() == Name)
            return *it;
    }
    
    return 0;
}

Animation* SceneNode::getAnimation(u32 Index) const
{
    return Index < AnimationList_.size() ? (*AnimationList_.begin() + sizeof(Animation) * Index) : 0;
}


/* === Collision === */

bool SceneNode::checkContact(Collision* CollisionHandle)
{
    for (std::list<SCollisionContact>::iterator it = CollisionContactList_.begin(); it != CollisionContactList_.end(); ++it)
    {
        if (it->CollisionHandle == CollisionHandle)
            return true;
    }
    
    return false;
}

bool SceneNode::getContact(SCollisionContact &NextContact, Collision* CollisionHandle)
{
    for (std::list<SCollisionContact>::iterator it = CollisionContactList_.begin(); it != CollisionContactList_.end(); ++it)
    {
        if (it->CollisionHandle == CollisionHandle)
        {
            NextContact = *it;
            CollisionContactList_.erase(it);
            return true;
        }
    }
    
    return false;
}

bool SceneNode::getNextContact(SCollisionContact &NextContact)
{
    if (CollisionContactList_.size())
    {
        NextContact = *CollisionContactList_.rbegin();
        CollisionContactList_.pop_back();
        return true;
    }
    return false;
}


/* === Parent system === */

bool SceneNode::getVisible(bool isGlobal) const
{
    if (isGlobal && SceneParent_)
        return SceneParent_->getVisible(true);
    return isVisible_;
}

void SceneNode::setParent(SceneNode* Parent, bool isGlobal)
{
    if (isGlobal)
    {
        const dim::matrix4f MatPos = getPositionMatrix(true);
        const dim::matrix4f MatRot = getRotationMatrix(true);
        const dim::matrix4f MatScl = getScaleMatrix(true);
        
        SceneParent_ = Parent;
        
        setPositionMatrix(MatPos, true);
        setRotationMatrix(MatRot, true);
        setScaleMatrix(MatScl, true);
    }
    else
        SceneParent_ = Parent;
}

void SceneNode::updateParent() const
{
    if (SceneParent_)
    {
        /* Update the parent of this node */
        SceneParent_->updateParent();
        
        /* Matrix transformation */
        spWorldMatrix *= SceneParent_->Position_;
        spWorldMatrix *= SceneParent_->Rotation_;
        spWorldMatrix *= SceneParent_->Scale_;
    }
}

dim::matrix4f SceneNode::getGlobalLocation() const
{
    spWorldMatrix.reset();
    updateGlobalLocation();
    return spWorldMatrix;
}

dim::matrix4f SceneNode::getGlobalLocationInverse() const
{
    spWorldMatrix.reset();
    updateGlobalLocationInverse();
    return spWorldMatrix;
}

void SceneNode::updateGlobalLocation() const
{
    /* Parent location */
    updateParent();
    
    /* Matrix transformation */
    spWorldMatrix *= Position_;
    spWorldMatrix *= Rotation_;
    spWorldMatrix *= Scale_;
}

void SceneNode::updateGlobalLocationInverse() const
{
    /* Compute the global location */
    updateGlobalLocation();
    
    /* Inverse the matrix */
    spWorldMatrix.setInverse();
}    

dim::matrix4f SceneNode::getLocalLocation() const
{
    return Position_ * Rotation_ * Scale_;
}

dim::matrix4f SceneNode::getLocalLocationInverse() const
{
    return (Position_ * Rotation_ * Scale_).getInverse();
}

void SceneNode::updateLocalLocation() const
{
    /* Matrix transformation */
    spWorldMatrix *= Position_;
    spWorldMatrix *= Rotation_;
    spWorldMatrix *= Scale_;
}
void SceneNode::updateLocalLocationInverse() const
{
    /* Matrix transformation */
    spWorldMatrix *= Position_;
    spWorldMatrix *= Rotation_;
    spWorldMatrix *= Scale_;
    
    /* Inverse the matrix */
    spWorldMatrix.setInverse();
}

void SceneNode::setMatrix(const dim::matrix4f &Matrix)
{
    /* Setup position and rotation */
    Position_ = Matrix.getPositionMatrix();
    Rotation_ = Matrix.getRotationMatrix();
    
    /* Setup scaling matrix */
    const dim::vector3df Scale(Matrix.getScale());
    
    Scale_.reset();
    Scale_[ 0] = Scale.X;
    Scale_[ 5] = Scale.Y;
    Scale_[10] = Scale.Z;
}

dim::matrix4f SceneNode::getMatrix() const
{
    return Position_ * Rotation_ * Scale_;
}

void SceneNode::updateModelviewMatrix()
{
    if (__spSceneManager->hasChildTree())
        updateLocalLocation();
    else
        updateGlobalLocation();
}

void SceneNode::updateTransformation()
{
    updateModelviewMatrix();
    Transformation_ = spWorldMatrix;
}

void SceneNode::loadTransformation()
{
    if (__spSceneManager->hasChildTree())
        spWorldMatrix *= Transformation_;
    else
        spWorldMatrix = Transformation_;
}

SceneNode* SceneNode::copy() const
{
    SceneNode* NewNode = new SceneNode(NODE_BASICNODE);
    copyRoot(NewNode);
    return NewNode;
}


/* === Children === */

void SceneNode::addChild(SceneNode* Child)
{
    SceneChildren_.push_back(Child);
    __spSceneManager->removeRootNode(Child);
}

void SceneNode::addChildren(const std::list<SceneNode*> &Children)
{
    for (std::list<SceneNode*>::const_iterator it = SceneChildren_.begin(); it != SceneChildren_.end(); ++it)
    {
        SceneChildren_.push_back(*it);
        __spSceneManager->removeRootNode(*it);
    }
}

bool SceneNode::removeChild(SceneNode* Child)
{
    for (std::list<SceneNode*>::iterator it = SceneChildren_.begin(); it != SceneChildren_.end(); ++it)
    {
        if (*it == Child)
        {
            __spSceneManager->addRootNode(Child);
            SceneChildren_.erase(it);
            return true;
        }
    }
    return false;
}

bool SceneNode::removeChild()
{
    if (!SceneChildren_.empty())
    {
        __spSceneManager->addRootNode(*SceneChildren_.begin());
        SceneChildren_.erase(SceneChildren_.begin());
        return true;
    }
    return false;
}

u32 SceneNode::removeChildren(const std::list<SceneNode*> &Children)
{
    u32 CountRemoved = 0;
    
    for (std::list<SceneNode*>::const_iterator it = Children.begin(); it != Children.end(); ++it)
    {
        if (removeChild(*it))
            ++CountRemoved;
    }
    
    return CountRemoved;
}

void SceneNode::removeChildren()
{
    for (std::list<SceneNode*>::iterator it = SceneChildren_.begin(); it != SceneChildren_.end(); ++it)
        __spSceneManager->addRootNode(*it);
    SceneChildren_.clear();
}


/*
 * ======= Protected functions =======
 */

void SceneNode::copyRoot(SceneNode* NewNode) const
{
    if (!NewNode)
        return;
    
    /* Copy the root attributes */
    NewNode->Type_              = Type_;
    NewNode->Name_              = Name_;
    
    NewNode->Position_          = Position_;
    NewNode->Rotation_          = Rotation_;
    NewNode->Scale_             = Scale_;
    
    NewNode->isVisible_         = isVisible_;
    
    NewNode->Parent_            = Parent_;
    NewNode->SceneParent_       = SceneParent_;
    
    NewNode->BoundingType_      = BoundingType_;
    NewNode->BoundingBox_       = BoundingBox_;
    NewNode->BoundingSphere_    = BoundingSphere_;
    
    NewNode->setUserData(getUserData());
    
    /* Copy the animations */
    scene::Animation* NewAnim = 0;
    
    for (std::list<Animation*>::const_iterator it = AnimationList_.begin(); it != AnimationList_.end(); ++it)
    {
        NewAnim = (*it)->copy(NewNode);
        
        NewNode->AnimationList_.push_back(NewAnim);
        __spSceneManager->AnimationList_.push_back(NewAnim);
    }
}


} // /namespace scene

} // /namespace sp



// ================================================================================
