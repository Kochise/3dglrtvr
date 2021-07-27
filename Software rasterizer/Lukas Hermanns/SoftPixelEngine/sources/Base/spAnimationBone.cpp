/*
 * Animation bone file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Base/spAnimationBone.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;

namespace scene
{


/*
 * AnimationBone class
 */

AnimationBone::AnimationBone(
    u32 &BoneIDCounter, AnimationBone* Parent,
    const dim::vector3df &Translation, const dim::quaternion &Rotation, const dim::vector3df &Scale,
    const io::stringc &Name, bool isGlobal)
    : ID_(++BoneIDCounter), Parent_(Parent)
{
    // General settings
    Name_           = Name;
    
    Enabled_        = true;
    
    Interpolation_  = 0.0f;
    Sequence_       = 0;
    
    // Setup location
    setupFrameLocation(
        Translation_, Rotation_, Scale_,
        Translation, Rotation, Scale, isGlobal
    );
    
    Rotation_.getMatrix(RotationMatrix_);
}
AnimationBone::~AnimationBone()
{
}

u32 AnimationBone::addKeyframe(
    const dim::vector3df &Translation, const dim::quaternion &Rotation, const dim::vector3df &Scale, const f32 Speed, bool isGlobal)
{
    static const f32 fps = 60.0f;
    
    SAnimKeyframe FrameData;
    {
        FrameData.Speed         = 1000.0f / (fps*Speed);
        
        setupFrameLocation(
            FrameData.Translation, FrameData.Rotation, FrameData.Scale,
            Translation, Rotation, Scale, isGlobal
        );
    }
    KeyframeList_.push_back(FrameData);
    
    return KeyframeList_.size() - 1;
}

void AnimationBone::removeKeyframe(u32 FrameIndex)
{
    if (FrameIndex >= KeyframeList_.size())
        return;
    
    std::vector<SAnimKeyframe>::iterator itf = KeyframeList_.begin() + FrameIndex;
    KeyframeList_.erase(itf);
}

void AnimationBone::setKeyframe(u32 Index, const SAnimKeyframe &Keyframe)
{
    if (Index < KeyframeList_.size())
        KeyframeList_[Index] = Keyframe;
}

SAnimKeyframe AnimationBone::getKeyframe(u32 Index) const
{
    return Index < KeyframeList_.size() ? KeyframeList_[Index] : SAnimKeyframe();
}

void AnimationBone::addVertexWeight(video::MeshBuffer* Surface, u32 Index, f32 Weight)
{
    if (Surface)
    {
        SVertexWeight VertWeight;
        {
            VertWeight.Surface  = Surface;
            VertWeight.Index    = Index;
            VertWeight.Weight   = Weight;
            VertWeight.Position = Surface->getVertexCoord(Index);
            VertWeight.Normal   = Surface->getVertexNormal(Index);
        }
        VertexWeights_.push_back(VertWeight);
    }
}

void AnimationBone::setupVertexWeights(const std::vector<SVertexWeight> &VertexWeights)
{
    // Copy vertex weight container
    VertexWeights_ = VertexWeights;
    
    // Setup each vertex weight
    for (std::vector<SVertexWeight>::iterator it = VertexWeights_.begin(); it != VertexWeights_.end();)
    {
        if (it->Surface)
        {
            // Setup original vertex coordiante and its normal
            it->Position    = it->Surface->getVertexCoord(it->Index);
            it->Normal      = it->Surface->getVertexNormal(it->Index);
            
            ++it;
        }
        else
        {
            // This should not happen, but if a surface is not defined the vertex weight cannot be used
            it = VertexWeights_.erase(it);
        }
    }
}

void AnimationBone::updateVertexWeights(bool UpdateVertices, bool UpdateNormals)
{
    if (!UpdateVertices && !UpdateNormals)
        return;
    
    // Loop for each vertex weight
    for (std::vector<SVertexWeight>::iterator it = VertexWeights_.begin(); it != VertexWeights_.end(); ++it)
    {
        if (UpdateVertices)
            it->Position = it->Surface->getVertexCoord(it->Index);
        if (UpdateNormals)
            it->Normal = it->Surface->getVertexNormal(it->Index);
    }
}

void AnimationBone::updateSequence(s32 IndexFrom, s32 IndexTo)
{
    if (IndexFrom >= 0 && IndexFrom < KeyframeList_.size() && IndexTo >= 0 && IndexTo < KeyframeList_.size())
    {
        NextTranslation_    = KeyframeList_[IndexTo].Translation   - KeyframeList_[IndexFrom].Translation;
        NextScale_          = KeyframeList_[IndexTo].Scale         - KeyframeList_[IndexFrom].Scale;
        NextRotation_       = KeyframeList_[IndexTo].Rotation;
    }
}

void AnimationBone::updateInterpolation()
{
    if (Sequence_ >= 0 && Sequence_ < KeyframeList_.size())
    {
        Translation_  = KeyframeList_[Sequence_].Translation    + NextTranslation_  * Interpolation_;
        Scale_        = KeyframeList_[Sequence_].Scale          + NextScale_        * Interpolation_;
        
        Rotation_.slerp(
            KeyframeList_[Sequence_].Rotation, NextRotation_, Interpolation_
        );
    }
}

void AnimationBone::updateTransformation(dim::matrix4f &Transformation, bool isGlobal) const
{
    /* Process parent bone */
    if (isGlobal && Parent_)
        Parent_->updateTransformation(Transformation);
    
    /* Process transformation */
    Transformation.translate(Translation_);
    Transformation *= RotationMatrix_;
    Transformation.scale(Scale_);
}

dim::matrix4f AnimationBone::getGlobalLocation() const
{
    dim::matrix4f Mat;
    updateTransformation(Mat);
    return Mat;
}

void AnimationBone::setSeek(f32 Seek)
{
    s32 Seq = (s32)Seek;
    Seek -= Seq;
    
    if (Seek < 0.f)
        Seek += 1.f;
    
    math::Clamp(Seek, 0.f, 1.f);
    
    Seek *= KeyframeList_.size();
    
    Sequence_       = (s32)Seek;
    Interpolation_  = Seek - (f32)Sequence_;
}

f32 AnimationBone::getSeek() const
{
    return ( (f32)Sequence_ + Interpolation_ ) / KeyframeList_.size();
}

bool AnimationBone::nextFrame(const f32 Speed)
{
    if (Sequence_ >= 0 && Sequence_ < KeyframeList_.size())
        return ( Interpolation_ += KeyframeList_[Sequence_].Speed * Speed ) >= 1.0f;
    return false;
}

void AnimationBone::renderBone(const AnimationBone* Bone, dim::matrix4f Matrix, const f32 Radius)
{
    #define Line(a, b) __spVideoDriver->draw3DLine(Matrix * a, Matrix * b, video::color(0, 0, 255))
    #define LineDirect(a, b, c) __spVideoDriver->draw3DLine(a, b, c)
    
    dim::vector3d< dim::vector3df > LastPos, Pos;
    
    LastPos.X = dim::vector3df(0, Radius, 0);
    LastPos.Y = dim::vector3df(0, 0, Radius);
    LastPos.Z = dim::vector3df(0, 0, Radius);
    
    if (Bone)
        Bone->updateTransformation(Matrix);
    
    #if 0 // !!! (to slow in debug mode)
    for (s32 i = 20; i <= 360; i += 20)
    {
        Pos.X = dim::vector3df(SIN(i)*Radius, COS(i)*Radius, 0);
        Line(LastPos.X, Pos.X);
        LastPos.X = Pos.X;
        
        Pos.Y = dim::vector3df(0, SIN(i)*Radius, COS(i)*Radius);
        Line(LastPos.Y, Pos.Y);
        LastPos.Y = Pos.Y;
        
        Pos.Z = dim::vector3df(SIN(i)*Radius, 0, COS(i)*Radius);
        Line(LastPos.Z, Pos.Z);
        LastPos.Z = Pos.Z;
    }
    #endif
    
    __spVideoDriver->setLineSize(3);
    LineDirect(Matrix.getPosition(), Matrix * dim::vector3df(Radius, 0, 0), video::color(255, 0, 0));
    LineDirect(Matrix.getPosition(), Matrix * dim::vector3df(0, Radius, 0), video::color(0, 255, 0));
    LineDirect(Matrix.getPosition(), Matrix * dim::vector3df(0, 0, Radius), video::color(0, 0, 255));
    __spVideoDriver->setLineSize(1);
    
    #undef Line
    #undef LineDirect
}


/*
 * ======= Private: =======
 */

void AnimationBone::setupFrameLocation(
    dim::vector3df &DestTranslation, dim::quaternion &DestRotation, dim::vector3df &DestScale,
    const dim::vector3df &SrcTranslation, const dim::quaternion &SrcRotation, const dim::vector3df &SrcScale,
    bool isGlobal)
{
    if (isGlobal && Parent_)
    {
        const dim::matrix4f Mat(Parent_->getGlobalLocation().getInverse());
        
        DestTranslation = SrcTranslation + Mat.getPosition();
        DestRotation    = dim::quaternion(Mat.getRotationMatrix()) * SrcRotation;
        DestScale       = Mat.getScale() * SrcScale;
    }
    else
    {
        DestTranslation = SrcTranslation;
        DestRotation    = SrcRotation;
        DestScale       = SrcScale;
    }
}


} // /namespace scene

} // /namespace sp



// ================================================================================
