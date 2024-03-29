/*
 * Extended toolset header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_TOOL_EXTENSIONSET_H__
#define __SP_TOOL_EXTENSIONSET_H__


#include "Base/spStandard.hpp"
#include "SceneGraph/spSceneCamera.hpp"
#include "Framework/Tools/spToolScriptLoader.hpp"
#include "Framework/Tools/spToolModelCombiner.hpp"
#include "Framework/Tools/spToolLightmapGenerator.hpp"
#include "Framework/Tools/spToolTextureManipulator.hpp"
#include "Framework/Tools/spToolParticleAnimator.hpp"
#include "Framework/Tools/spToolXMLParser.hpp"
#include "Framework/Tools/spToolPathFinder.hpp"


namespace sp
{
namespace tool
{


/**
Extended toolset class. Here you will find some useful functions when you are making just small
applications or test programs.
*/
namespace Toolset
{

/**
Moves a Camera object free. This function is normally used for debuging while the project is still in progress.
Use the arrow keys for W/A/S/D like in first-person-shooters.
\param Cam: Camera object which is to be moved.
\param MoveSpeed: Speed which is to be used for moving around.
\param TurnSpeed: Speed which is to be used for looking around.
\param MaxTurnDegree: Maximal degree for the X-Axis in rotation.
\param UseArrowKeys: If true the arrow keys are enabled. Set it to false if the arrow keys are used for other operations.
*/
SP_EXPORT void moveCameraFree(
    scene::Camera* Cam = 0,
    const f32 MoveSpeed = 0.25f, const f32 TurnSpeed = 0.25f, const f32 MaxTurnDegree = 90.0f,
    bool UseArrowKeys = true
);

} // /namespace Toolset


} // /namespace tool

} // /namespace sp


#endif



// ================================================================================
