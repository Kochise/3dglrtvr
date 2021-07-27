/*
 * Skeletal animation header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Base/spAnimationSkeletal.hpp"
#include "Base/spAnimationBone.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;
extern scene::SceneGraph* __spSceneManager;

namespace scene
{


AnimationSkeletal::AnimationSkeletal(Mesh* ObjMesh)
    : Animation(ANIMATION_SKELETAL), Mesh_(ObjMesh)
{
    /* General settings */
    SkeletonBoneID_     = 0;
    isFrustumCulling_   = false;
}
AnimationSkeletal::~AnimationSkeletal()
{
    MemoryManager::deleteList(BoneList_);
}

AnimationBone* AnimationSkeletal::addBone(
    AnimationBone* ParentBone,
    const dim::vector3df &Translation, const dim::quaternion &Rotation, const dim::vector3df &Scale,
    const io::stringc &Name, bool isGlobal)
{
    /* Create a new bone */
    AnimationBone* NewBone = new AnimationBone(
        SkeletonBoneID_, ParentBone, Translation, Rotation, Scale, Name, isGlobal
    );
    
    BoneList_.push_back(NewBone);
    
    /* Add the child to the parent bone */
    if (ParentBone)
        ParentBone->Children_.push_back(NewBone);
    else
        BoneRootList_.push_back(NewBone);
    
    return NewBone;
}

void AnimationSkeletal::removeBone(AnimationBone* &Bone, bool DeleteChildren)
{
    /* Temporary variables */
    std::vector<AnimationBone*>::iterator it, itp;
    
    /* Delete the children bones */
    if (DeleteChildren)
    {
        std::vector<AnimationBone*> BoneList = Bone->Children_;
        for (it = BoneList.begin(); it != BoneList.end(); ++it)
            removeBone(*it, true);
        Bone->Children_.clear();
    }
    
    /* Remove the bone from the root list */
    MemoryManager::removeElement(BoneRootList_, Bone);
    
    /* Release the parent of each bone when the parent is this bone */
    for (it = BoneList_.begin(); it != BoneList_.end(); ++it)
    {
        if ((*it)->Parent_ == Bone)
            (*it)->Parent_ = 0;
    }
    
    /* Remove the child from the parent bone */
    if (Bone->Parent_)
        MemoryManager::removeElement(Bone->Parent_->Children_, Bone);
    
    /* Remove the bone from the generall list */
    MemoryManager::removeElement(BoneList_, Bone, true);
}

void AnimationSkeletal::play(
    const EAnimMethods AnimMethod, const EAnimModes AnimMode, s32 Start, s32 End, bool UseIndividualRange)
{
    if (!BoneList_.size())
        return;
    
    /* General settings */
    AnimMethod_ = AnimMethod;
    AnimMode_   = AnimMode;
    
    s32 FrameStart, FrameEnd;
    
    /* Reset each bone */
    for (std::vector<AnimationBone*>::iterator it = BoneList_.begin(); it != BoneList_.end(); ++it)
    {
        FrameStart  = (Start == -1 ? 0 : Start);
        FrameEnd    = (End == -1 ? (*it)->KeyframeList_.size() - 1 : End);
        
        math::Clamp(FrameStart, 0, (s32)(*it)->KeyframeList_.size() - 1);
        math::Clamp(FrameEnd, 0, (s32)(*it)->KeyframeList_.size() - 1);
        
        (*it)->Sequence_       = FrameStart;
        (*it)->Interpolation_  = 0.0f;
        
        if (!UseIndividualRange)
            (*it)->setSequenceRange(FrameStart, FrameEnd);
    }
    
    /* Process the first sequence */
    for (std::vector<AnimationBone*>::iterator it = BoneList_.begin(); it != BoneList_.end(); ++it)
    {
        if ((*it)->KeyframeList_.size() >= 2)
            (*it)->updateSequence((*it)->Sequence_, (*it)->Sequence_ + 1);
    }
}

void AnimationSkeletal::record(const f32 Speed)
{
    for (std::vector<AnimationBone*>::iterator it = BoneList_.begin(); it != BoneList_.end(); ++it)
        (*it)->addKeyframe((*it)->getTranslation(), (*it)->getRotation(), (*it)->getScale(), Speed);
}

void AnimationSkeletal::setSeek(s32 Sequence, f32 Interpolation)
{
    for (std::vector<AnimationBone*>::iterator it = BoneList_.begin(); it != BoneList_.end(); ++it)
    {
        (*it)->setSequence(Sequence);
        (*it)->updateSequence(Sequence, Sequence < (*it)->KeyframeList_.size() - 1 ? Sequence + 1 : 0);
        
        (*it)->setInterpolation(Interpolation);
        (*it)->updateInterpolation();
    }
    
    // Update bone transformations
    AnimMode_ = ANIM_FORWARDS;
    setBonesEnable(false);
    //updateAnimation();
    setBonesEnable(true);
    AnimMode_ = ANIM_STOP;
}
void AnimationSkeletal::getSeek(s32 &Sequence, f32 &Interpolation) const
{
    if (BoneList_.size())
    {
        Sequence        = BoneList_[0]->getSequence();
        Interpolation   = BoneList_[0]->getInterpolation();
    }
}

void AnimationSkeletal::setSeek(f32 State)
{
    // !TODO!
}
f32 AnimationSkeletal::getSeek() const
{
    return 0.0f; // !TODO!
}

s32 AnimationSkeletal::getLength() const
{
    return BoneList_.size() ? BoneList_[0]->KeyframeList_.size() : 0;
}

AnimationBone* AnimationSkeletal::getBone(u32 BoneID) const
{
    if (BoneID)
    {
        for (std::vector<AnimationBone*>::const_iterator it = BoneList_.begin(); it != BoneList_.end(); ++it)
        {
            if ((*it)->getID() == BoneID)
                return *it;
        }
    }
    return 0;
}

void AnimationSkeletal::setBonesEnable(bool Enable)
{
    for (std::vector<AnimationBone*>::iterator it = BoneList_.begin(); it != BoneList_.end(); ++it)
        (*it)->setEnable(Enable);
}

/*
 * Calculates the origin matrix transformation for all bones
 * (Call this function after all bone creations and settings)
 */
void AnimationSkeletal::updateSkeleton()
{
    AnimationBone* CurBone = 0;
    
    /* Update each bone */
    for (std::vector<AnimationBone*>::iterator it = BoneList_.begin(); it != BoneList_.end(); ++it)
    {
        CurBone = *it;
        
        CurBone->MatrixOrigin_.reset();
        
        /* Update rotation matrix */
        CurBone->Rotation_.getMatrix(CurBone->RotationMatrix_);
        CurBone->RotationMatrix_.setInverse();
        
        /* Get the inverse matrix for the origin matrices */
        CurBone->updateTransformation(CurBone->MatrixOrigin_);
        CurBone->MatrixOrigin_.setInverse();
    } // next bone
}

void AnimationSkeletal::renderSkeleton(const f32 BoneRadius, bool isSolid) const
{
    /* Temporary variables */
    dim::matrix4f ParentMatrix;
    dim::matrix4f Matrix;
    
    const dim::matrix4f MeshMat(Mesh_->getGlobalLocation());
    
    /* Draw the bones */
    for (std::vector<AnimationBone*>::const_iterator it = BoneList_.begin(); it != BoneList_.end(); ++it)
        AnimationBone::renderBone(*it, MeshMat, BoneRadius);
    
    /* Draw the bone connections */
    for (std::vector<AnimationBone*>::const_iterator it = BoneList_.begin(); it != BoneList_.end(); ++it)
    {
        if (!(*it)->getParent())
            continue;
        
        // Parent bone matrix transformation
        ParentMatrix.reset();
        (*it)->getParent()->updateTransformation(ParentMatrix);
        
        // Bone matrix transformation
        Matrix.reset();
        (*it)->updateTransformation(Matrix);
        
        // Final matrix transformations
        f32 Size = math::getDistance(ParentMatrix.getPosition(), Matrix.getPosition()) / 10;
        
        Matrix = dim::getDirectionMatrix(
            ParentMatrix.getPosition(), Matrix.getPosition()
        );
        
        Matrix.scale(Size);
        
        // Draw the bone
        renderBoneConnection(MeshMat * Matrix, isSolid);
    }
}

void AnimationSkeletal::updateAnimation(const bool DirectUpdate)
{
    if (!DirectUpdate && AnimMode_ == ANIM_STOP)
        return;
    
    isFrustumCulling_ = checkFrustumCulling();
    
    Matrix_.reset();
    NormalMatrix_.reset();
    
    /* Reset vertex coordinate and normal (to support vertex weights) */
    if (!isFrustumCulling_)
    {
        std::vector<SVertexWeight>::iterator itv;
        for (std::vector<AnimationBone*>::iterator it = BoneList_.begin(); it != BoneList_.end(); ++it)
        {
            for (itv = (*it)->VertexWeights_.begin(); itv != (*it)->VertexWeights_.end(); ++itv)
            {
                itv->Surface->setVertexCoord(itv->Index, 0.0f);
                itv->Surface->setVertexNormal(itv->Index, 0.0f);
            }
        }
    }
    
    /* Update each bone animation */
    for (std::vector<AnimationBone*>::iterator it = BoneRootList_.begin(); it != BoneRootList_.end(); ++it)
        updateBone(*it);
    
    /* Update vertex buffer */
    if (!isFrustumCulling_)
        Mesh_->updateVertexBuffer();
}


/*
 * ======= Private: =======
 */

void AnimationSkeletal::updateBone(AnimationBone* CurBone)
{
    if (CurBone->getEnable())
    {
        /*
         * Update sequence:
         * Next step for the seek status until the next frame has been arived
         */
        if (CurBone->nextFrame(Speed_))
        {
            // Subtract the seek status in dependet of the out-of-range factor
            updateSeekStatus(CurBone->Sequence_, CurBone->Interpolation_);
            
            // Check the end of animation
            if (CurBone->Sequence_ >= CurBone->End_ + 1)
            {
                while (CurBone->Sequence_ >= CurBone->End_ + 1)
                    CurBone->Sequence_ = CurBone->Start_ + (CurBone->Sequence_ - CurBone->End_ - 1);
                
                if (AnimMethod_ == ANIM_ONELOOP)
                    AnimMode_ = ANIM_STOP;
                
                CurBone->updateSequence(CurBone->Sequence_, CurBone->Sequence_ + 1);
            }
            else if (CurBone->Sequence_ >= CurBone->End_)
            {
                if (AnimMethod_ == ANIM_ONESHOT)
                    AnimMode_ = ANIM_STOP;
                
                CurBone->updateSequence(CurBone->Sequence_, CurBone->Start_);
            }
            else
                CurBone->updateSequence(CurBone->Sequence_, CurBone->Sequence_ + 1);
        }
        
        // Keyframe interpolation
        CurBone->updateInterpolation();
    }
    
    // Transform the bone matrix
    CurBone->Rotation_.getMatrix(CurBone->RotationMatrix_);
    CurBone->updateTransformation(Matrix_, false);
    
    // Temporary memory matrices
    dim::matrix4f ParentMatrix, ParentNormalMatrix;
    
    if (!isFrustumCulling_)
    {
        // Store old matrices on the stack
        ParentMatrix        = Matrix_;
        ParentNormalMatrix  = NormalMatrix_;
        
        // Update the normal matrix
        NormalMatrix_ = dim::getRotationMatrix(Matrix_);
        
        // Multiply the bone matrix by the origin matrix
        Matrix_         *= CurBone->MatrixOrigin_;
        NormalMatrix_   *= CurBone->MatrixOrigin_.getRotationMatrix();
        
        // Update each vertex- coordinate and normal
        for (std::vector<SVertexWeight>::iterator itv = CurBone->VertexWeights_.begin();
             itv != CurBone->VertexWeights_.end(); ++itv)
        {
            itv->Surface->setVertexCoord(
                itv->Index,
                itv->Surface->getVertexCoord(itv->Index) + (Matrix_ * itv->Position) * itv->Weight
            );
            itv->Surface->setVertexNormal(
                itv->Index,
                itv->Surface->getVertexNormal(itv->Index) + (NormalMatrix_ * itv->Normal) * itv->Weight
            );
        }
    }
    
    // Update all bone children
    for (std::vector<AnimationBone*>::iterator itc = CurBone->Children_.begin(); itc != CurBone->Children_.end(); ++itc)
    {
        if (!isFrustumCulling_)
        {
            // Reset the memoriezed matrices
            Matrix_         = ParentMatrix;
            NormalMatrix_   = ParentNormalMatrix;
        }
        
        // Update the skeleton bone for the current child
        updateBone(*itc);
    }
}

void AnimationSkeletal::renderBoneConnection(const dim::matrix4f &Matrix, bool isSolid) const
{
    #define Line(a, b) __spVideoDriver->draw3DLine(a, b, video::color(255, 255, 0))
    #define Face(a, b, c, rgb) __spVideoDriver->draw3DTriangle(0, dim::triangle3df(c, b, a), rgb)
    
    const dim::vector3df PosP(Matrix.getPosition());
    const dim::vector3df PosQ(Matrix * dim::vector3df(0, 0, -10));
    const dim::vector3df PosA(Matrix * dim::vector3df(-1,  1, -1));
    const dim::vector3df PosB(Matrix * dim::vector3df( 1,  1, -1));
    const dim::vector3df PosC(Matrix * dim::vector3df( 1, -1, -1));
    const dim::vector3df PosD(Matrix * dim::vector3df(-1, -1, -1));
    
    if (isSolid)
    {
        const video::color ClrA(255, 255, 0), ClrB(255, 222, 0);
        
        Face(PosP, PosA, PosB, ClrA); Face(PosP, PosB, PosC, ClrB); Face(PosP, PosC, PosD, ClrA); Face(PosP, PosD, PosA, ClrB);
        Face(PosQ, PosB, PosA, ClrB); Face(PosQ, PosC, PosB, ClrA); Face(PosQ, PosD, PosC, ClrB); Face(PosQ, PosA, PosD, ClrA);
    }
    else
    {
        Line(PosP, PosA); Line(PosP, PosB); Line(PosP, PosC); Line(PosP, PosD);
        Line(PosA, PosB); Line(PosB, PosC); Line(PosC, PosD); Line(PosD, PosA);
        Line(PosQ, PosA); Line(PosQ, PosB); Line(PosQ, PosC); Line(PosQ, PosD);
    }
    
    #undef Line
    #undef Face
}

bool AnimationSkeletal::checkFrustumCulling() const
{
    /* Check frustum culling for each visible camera */
    if (!__spSceneManager)
        return false;
    
    const dim::matrix4f Transformation(Mesh_->getGlobalLocation());
    
    for (std::list<Camera*>::const_iterator it = __spSceneManager->CameraList_.begin(); it != __spSceneManager->CameraList_.end(); ++it)
    {
        if ((*it)->getVisible() && !Mesh_->checkFrustumCulling((*it)->getViewFrustum(), Transformation))
            return true;
    }
    
    return false;
}

Animation* AnimationSkeletal::copy(SceneNode* NewNode) const
{
    if (NewNode->getType() != scene::NODE_MESH)
        return 0;
    
    // Create new animation
    Mesh* NewMesh = (Mesh*)NewNode;
    
    AnimationSkeletal* NewAnim = new AnimationSkeletal(NewMesh);
    
    copyRoot(NewAnim);
    
    AnimationBone* NewBone = 0;
    std::vector<AnimationBone*>::const_iterator it, itChild;
    std::vector<AnimationBone*>::iterator itNew;
    
    // Copy bones
    for (it = BoneList_.begin(); it != BoneList_.end(); ++it)
    {
        NewBone = NewAnim->addBone(
            (*it)->getParent() ? NewAnim->getBone((*it)->getID()) : 0,
            (*it)->getTranslation(),
            (*it)->getRotation(),
            (*it)->getScale(),
            (*it)->getName()
        );
        
        NewBone->ID_                = (*it)->ID_;
        NewBone->Enabled_           = (*it)->Enabled_;
        NewBone->MatrixOrigin_      = (*it)->MatrixOrigin_;
        NewBone->RotationMatrix_    = (*it)->RotationMatrix_;
        NewBone->VertexWeights_     = (*it)->VertexWeights_;
        NewBone->KeyframeList_      = (*it)->KeyframeList_;
    }
    
    // Copy bones' children
    for (it = BoneList_.begin(), itNew = NewAnim->BoneList_.begin(); it != BoneList_.end(); ++it, ++itNew)
    {
        for (itChild = (*it)->Children_.begin(); itChild != (*it)->Children_.end(); ++itChild)
            (*itNew)->Children_.push_back(NewAnim->getBone((*itChild)->getID()));
    }
    
    // Copy root bones
    for (it = BoneRootList_.begin(); it != BoneRootList_.end(); ++it)
        NewAnim->BoneRootList_.push_back(NewAnim->getBone((*it)->getID()));
    
    return NewAnim;
}


} // /namespace scene

} // /namespace sp



// ================================================================================
