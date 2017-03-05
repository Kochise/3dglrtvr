/*
 * Morph target animation file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Base/spAnimationMorphTarget.hpp"
#include "SceneGraph/spSceneMesh.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{

extern scene::SceneGraph* __spSceneManager;

namespace scene
{


AnimationMorphTarget::AnimationMorphTarget(Mesh* ObjMesh)
    : Animation(ANIMATION_MORPHTARGET), Mesh_(ObjMesh), PrevSeq_(0), NextSeq_(0), VertexWeightsSize_(0)
{
}
AnimationMorphTarget::~AnimationMorphTarget()
{
}


void AnimationMorphTarget::addSequence(const SAnimMorphTargetSequence &Sequence)
{
    if (checkDataSize(Sequence.Vertices.size()))
        SequenceList_.push_back(Sequence);
}

void AnimationMorphTarget::removeSequence(u32 SeqIndex)
{
    if (SeqIndex >= SequenceList_.size())
        return;
    
    const std::vector<SAnimMorphTargetSequence>::iterator it = SequenceList_.begin() + SeqIndex;
    
    SequenceList_.erase(it);
}

void AnimationMorphTarget::setSequence(u32 SeqIndex, const SAnimMorphTargetSequence &Sequence)
{
    if (SeqIndex < SequenceList_.size())
        SequenceList_[SeqIndex] = Sequence;
}
SAnimMorphTargetSequence AnimationMorphTarget::getSequence(u32 SeqIndex) const
{
    if (SeqIndex < SequenceList_.size())
        return SequenceList_[SeqIndex];
    
    SAnimMorphTargetSequence TmpSeq;
    TmpSeq.Speed = 0.0f;
    
    return TmpSeq;
}

void AnimationMorphTarget::setVertexWeightsDefault()
{
    if (!SequenceList_.size())
        return;
    
    SAnimMorphTargetVertexWeight VertexWeight;
    
    for (s32 s = 0, i, j = 0; s < Mesh_->getMeshBufferCount(); ++s)
    {
        for (i = 0; i < Mesh_->getMeshBuffer(s)->getVertexCount() && j < SequenceList_[0].Vertices.size(); ++i, ++j)
        {
            VertexWeight.Index      = j;
            VertexWeight.Surface    = s;
            VertexWeights_.push_back(VertexWeight);
        }
    }
}
void AnimationMorphTarget::setVertexWeights(const std::vector<SAnimMorphTargetVertexWeight> &VertexWeights)
{
    if (checkDataSize(VertexWeights.size()))
        VertexWeights_ = VertexWeights;
}
std::vector<SAnimMorphTargetVertexWeight> AnimationMorphTarget::getVertexWeights() const
{
    return VertexWeights_;
}

s32 AnimationMorphTarget::getLength() const
{
    return SequenceList_.size();
}


void AnimationMorphTarget::updateAnimation(const bool DirectUpdate)
{
    if (!DirectUpdate && AnimMode_ == ANIM_STOP)
        return;
    
    updateSequence(NextSeq_->Speed);
    
    /* Check for frustum culling */
    if (__spSceneManager->getActiveCamera() &&
        !Mesh_->checkFrustumCulling(__spSceneManager->getActiveCamera()->getViewFrustum(), Mesh_->getGlobalLocation()))
    {
        return;
    }
    
    /* Transform each vertex (used by the vertex weights) */
    s32 i = 0;
    video::MeshBuffer* Surf = 0;
    SAnimMorphTargetVertex * PrevVert = 0, * NextVert = 0;
    
    for (std::vector<SAnimMorphTargetVertexWeight>::iterator it = VertexWeights_.begin(); it != VertexWeights_.end(); ++it, ++i)
    {
        Surf = Mesh_->getMeshBuffer(it->Surface);
        
        PrevVert = &PrevSeq_->Vertices[i];
        NextVert = &NextSeq_->Vertices[i];
        
        Surf->setVertexCoord(
            it->Index, PrevVert->Coord + ( NextVert->Coord - PrevVert->Coord ) * Interpolation_
        );
        Surf->setVertexNormal(
            it->Index, PrevVert->Normal + ( NextVert->Normal - PrevVert->Normal ) * Interpolation_
        );
    }
    
    Mesh_->updateVertexBuffer();
}


/*
 * ======= Private: =======
 */

void AnimationMorphTarget::updateNextSequence()
{
    s32 NextSeqIndex = Sequence_ + AnimMode_;
    
    if (Sequence_ + AnimMode_ > End_)
        NextSeqIndex = Start_;
    else if (Sequence_ + AnimMode_ < 0)
        NextSeqIndex = End_;
    
    PrevSeq_ = &(SequenceList_[Sequence_]);
    NextSeq_ = &(SequenceList_[NextSeqIndex]);
}

bool AnimationMorphTarget::checkDataSize(s32 Size)
{
    if (VertexWeightsSize_ > 0 && Size != VertexWeightsSize_)
    {
        io::Log::error("Morph-target animation sequences must have equal count of vertex data");
        return false;
    }
    
    VertexWeightsSize_ = Size;
    
    return true;
}

Animation* AnimationMorphTarget::copy(SceneNode* NewNode) const
{
    if (NewNode->getType() != scene::NODE_MESH)
        return 0;
    
    AnimationMorphTarget* NewAnim = new AnimationMorphTarget((Mesh*)NewNode);
    
    copyRoot(NewAnim);
    
    NewAnim->VertexWeightsSize_ = VertexWeightsSize_;
    NewAnim->SequenceList_      = SequenceList_;
    NewAnim->VertexWeights_     = VertexWeights_;
    
    return NewAnim;
}


} // /namespace scene

} // /namespace sp



// ================================================================================
