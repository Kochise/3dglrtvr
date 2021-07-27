/*
 * Camera scene node file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "SceneGraph/spSceneCamera.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;

namespace scene
{


Camera::Camera() : SceneNode(NODE_CAMERA)
{
    /* General settings */
    Viewport_       = dim::rect2di(0, 0, __Screen_Width, __Screen_Height);
    NearRange_      = CAMERADEF_RANGE_NEAR;
    FarRange_       = CAMERADEF_RANGE_FAR;
    FieldOfView_    = DEF_PERSPECTIVE_FOV;
    isOrtho_        = false;
    isMirror_       = false;
    
    /* Initialize the perspective matrix */
    setPerspective(Viewport_, NearRange_, FarRange_, FieldOfView_);
}
Camera::~Camera()
{
}

void Camera::updateControl()
{
    // do nothing
}
void Camera::drawMenu()
{
    // do nothing
}

void Camera::updatePerspective()
{
    /* Check which projection matrix is used */
    if (isOrtho_)
    {
        /* Check which projection matrix the renderer is using */
        /*if (__spVideoDriver->getProjectionMatrixType() == dim::MATRIX_LEFTHANDED)
        {*/
            PerspectiveMatrix_.setOrthoLH(
                static_cast<f32>(Viewport_.Left) / FieldOfView_,
                static_cast<f32>(Viewport_.Left + Viewport_.Right) / FieldOfView_,
                static_cast<f32>(Viewport_.Top) / FieldOfView_,
                static_cast<f32>(Viewport_.Top + Viewport_.Bottom) / FieldOfView_,
                NearRange_, FarRange_
            );
        /*}
        else
        {
            PerspectiveMatrix_.setOrthoRH(
                static_cast<f32>(Viewport_.Left) / FieldOfView_,
                static_cast<f32>(Viewport_.Left + Viewport_.Right) / FieldOfView_,
                static_cast<f32>(Viewport_.Top) / FieldOfView_,
                static_cast<f32>(Viewport_.Top + Viewport_.Bottom) / FieldOfView_,
                NearRange_, FarRange_
            );
        }*/
    }
    else
    {
        /* Temporary constants */
        const f32 AspectRatio = (f32)Viewport_.Right / (f32)Viewport_.Bottom;
        
        #if 0
        PerspectiveMatrix_.setPerspectiveLH(
            FieldOfView_, (f32)Viewport_.Right / (f32)Viewport_.Bottom, NearRange_, FarRange_
        );
        #else
        /* Check which projection matrix the renderer is using */
        if (__spVideoDriver->getProjectionMatrixType() == dim::MATRIX_LEFTHANDED)
            PerspectiveMatrix_.setPerspectiveLH(FieldOfView_, AspectRatio, NearRange_, FarRange_);
        else
            PerspectiveMatrix_.setPerspectiveRH(FieldOfView_, AspectRatio, NearRange_, FarRange_);
        #endif
    }
    
    /* Set the viewport */
    __spVideoDriver->setViewport(
        dim::point2di(Viewport_.Left, Viewport_.Top), dim::size2di(Viewport_.Right, Viewport_.Bottom)
    );
    __spVideoDriver->setClippingRange(NearRange_, FarRange_);
    
    /* Update the render matrices */
    spProjectionMatrix = PerspectiveMatrix_;
    spViewMatrix.reset();
    spWorldMatrix.reset();
}

void Camera::setPerspective(const dim::rect2di &Viewport, f32 NearRange, f32 FarRange, f32 FieldOfView)
{
    /* General settings */
    Viewport_       = Viewport;
    NearRange_      = NearRange;
    FarRange_       = FarRange;
    FieldOfView_    = FieldOfView;
    
    /* Update the perspective */
    updatePerspective();
}

void Camera::getPerspective(dim::rect2di &Viewport, f32 &NearRange, f32 &FarRange, f32 &FieldOfView)
{
    Viewport    = Viewport_;
    NearRange   = NearRange_;
    FarRange    = FarRange_;
    FieldOfView = FieldOfView_;
}

dim::point2di Camera::getProjection(dim::vector3df Position) const
{
    /* Generate the line coordinates in dependent to the frustum culling */
    Position = getGlobalLocationInverse() * Position;
    
    if (Position.Z <= 0)
        return dim::point2di(-10000, -10000);
    
    dim::point2df ScreenCoord;
    
    /* Compute the 2d coordinates */
    if (isOrtho_)
    {
        ScreenCoord.set(
              Position.X * FieldOfView_ + Viewport_.Right /2 + Viewport_.Left,
            - Position.Y * FieldOfView_ + Viewport_.Bottom/2 + Viewport_.Top
        );
    }
    else
    {
        const f32 Aspect = (f32)Viewport_.Right / (f32)Viewport_.Bottom;
        
        ScreenCoord.set(
              Position.X / Position.Z * f32(Viewport_.Right/2) * math::STDASPECT / Aspect + Viewport_.Right /2 + Viewport_.Left,
            - Position.Y / Position.Z * f32(Viewport_.Right/2) * math::STDASPECT / Aspect + Viewport_.Bottom/2 + Viewport_.Top
        );
    }
    
    return dim::point2di((s32)ScreenCoord.X, (s32)ScreenCoord.Y);
}

dim::line3df Camera::getPickingLine(const dim::point2di &Position, f32 Length) const
{
    if (Length < 0.0f)
        Length = FarRange_;
    
    /* Temporary variables */
    dim::line3df Line;
    dim::point2df Coord((f32)Position.X - Viewport_.Left, (f32)Position.Y - Viewport_.Top);
    
    const dim::matrix4f Mat(getGlobalLocation());
    
    if (isOrtho_)
    {
        dim::point2df Origin(
            Coord.X - f32(Viewport_.Right/2), Coord.Y - f32(Viewport_.Bottom/2)
        );
        
        Origin /= FieldOfView_;
        
        Line.Start  = Mat * dim::vector3df(Origin.X, -Origin.Y, 0.0f);
        Line.End    = Mat * dim::vector3df(Origin.X, -Origin.Y, Length);
    }
    else
    {
        Coord.make3DFrustum(Viewport_.Right, Viewport_.Bottom);
        
        Line.Start  = Mat.getPosition();
        Line.End    = Mat * ( dim::vector3df(Coord.X, Coord.Y, 1.0f).normalize() * Length );
    }
    
    /* Return the result */
    return Line;
}

void Camera::lookAt(dim::vector3df Position, bool isGlobal)
{
    /* Settings */
    dim::vector3df Pos(getPosition(isGlobal));
    dim::vector3df Rot(getRotation(isGlobal));
    
    /* Calculate rotation */
    Rot.X = -ASIN( (Position.Y - Pos.Y) / math::getDistance(Pos, Position) );
    Rot.Y = -ASIN( (Position.X - Pos.X) / math::getDistance(Pos.X, Pos.Z, Position.X, Position.Z) ) + 180.0f;
    Rot.Z = 0.0f;
    
    if (Pos.Z < Position.Z)
        Rot.Y = 180.0f - Rot.Y;
    
    /* Final rotation */
    setRotation(Rot, isGlobal);
}

void Camera::updateViewmode()
{
    /* Viewport */
    __spVideoDriver->setViewport(
        dim::point2di(Viewport_.Left, Viewport_.Top), dim::size2di(Viewport_.Right, Viewport_.Bottom)
    );
    
    /* Projection matrix */
    __spVideoDriver->setProjectionMatrix(PerspectiveMatrix_);
}

void Camera::updateModelviewMatrix()
{
    /* Work with an identified matrix */
    spWorldMatrix.reset();
    
    /* Compute the inverse location */
    updateGlobalLocationInverse();
    
    /* Apply the possible mirror matrix */
    if (isMirror_)
        spWorldMatrix *= MirrorMatrix_;
    
    /* Use the right matrix */
    __spVideoDriver->setViewMatrix(spWorldMatrix);
    spWorldMatrix.reset();
    
    /* Update the view-frustum */
    updateViewFrustum();
}

Camera* Camera::copy() const
{
    /* Allocate a new camera */
    Camera* NewCamera = new Camera();
    
    /* Copy the root attributes */
    copyRoot(NewCamera);
    
    /* Copy the camera configurations */
    NewCamera->PerspectiveMatrix_   = PerspectiveMatrix_;
    NewCamera->MirrorMatrix_        = MirrorMatrix_;
    NewCamera->ViewFrustum_         = ViewFrustum_;
    NewCamera->Viewport_            = Viewport_;
    
    NewCamera->NearRange_           = NearRange_;
    NewCamera->FarRange_            = FarRange_;
    NewCamera->FieldOfView_         = FieldOfView_;
    
    NewCamera->isOrtho_             = isOrtho_;
    NewCamera->isMirror_            = isMirror_;
    
    /* Return the new sprite */
    return NewCamera;
}


/*
 * ======= Protected: =======
 */

void Camera::updateViewFrustum()
{
    const f32 AspectRatio = (f32)Viewport_.Right / (f32)Viewport_.Bottom;
    dim::matrix4f ViewFrustumMatrix(PerspectiveMatrix_);
    
    dim::matrix4f Mat(spViewMatrix);
    Mat.setPosition(-Mat.getPosition());
    
    ViewFrustumMatrix *= Mat;
    
    ViewFrustum_.setFrustum(ViewFrustumMatrix);
}


} // /namespace scene

} // /namespace sp



// ================================================================================
