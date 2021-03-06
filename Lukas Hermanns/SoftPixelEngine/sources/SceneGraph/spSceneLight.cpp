/*
 * Light scene node file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "SceneGraph/spSceneLight.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;

namespace scene
{


bool __spLightIDList[MAX_COUNT_OF_SCENELIGHTS] = { 0 };

const f32 Light::DEF_SLCA_INNER = 30.0f;
const f32 Light::DEF_SLCA_OUTER = 60.0f;

Light::Light() : SceneNode(NODE_LIGHT), LightID_(0)
{
    /* General settings */
    LightingType_       = LIGHT_DIRECTIONAL;
    SpotInnerConeAngle_ = DEF_SLCA_INNER;
    SpotOuterConeAngle_ = DEF_SLCA_OUTER;
    isVolumetric_       = false;
    Direction_          = dim::vector3df(0, 0, 1);
    
    /* Color settings */
    DiffuseColor_       = 200;
    AmbientColor_       = 255;
    SpecularColor_      = 0;
    
    /* Volumetric light */
    AttenuationConstant_    = 1.0f;
    AttenuationLinear_      = 0.1f;
    AttenuationQuadratic_   = 0.4f;
}

Light::Light(const ELightModels Type) : SceneNode(NODE_LIGHT), LightID_(0)
{
    /* General settings */
    LightingType_       = Type;
    SpotInnerConeAngle_ = DEF_SLCA_INNER;
    SpotOuterConeAngle_ = DEF_SLCA_OUTER;
    Direction_          = dim::vector3df(0, 0, 1);
    
    /* Color settings */
    DiffuseColor_   = 200;
    AmbientColor_   = 255;
    SpecularColor_  = 0;
    
    /* Volumetric light settings */
    isVolumetric_           = false;
    AttenuationConstant_    = 1.0f;
    AttenuationLinear_      = 0.1f;
    AttenuationQuadratic_   = 0.4f;
    
    u32 i;
    
    /* Check if the there is still enough space for an additional light */
    for (i = 0; i < MAX_COUNT_OF_SCENELIGHTS; ++i)
    {
        if (!__spLightIDList[i])
        {
            __spLightIDList[i]  = true;
            LightID_            = i;
            break;
        }
    }
    
    /* Check if the light has an identity */
    if (i < MAX_COUNT_OF_SCENELIGHTS)
    {
        /* Add a dynamic light soruce */
        __spVideoDriver->addDynamicLightSource(
            LightID_, LightingType_,
            DiffuseColor_, AmbientColor_, SpecularColor_,
            AttenuationConstant_, AttenuationLinear_, AttenuationQuadratic_
        );
    }
    else
    {
        io::Log::warning(
            "To many lights in use, only " + io::stringc(MAX_COUNT_OF_SCENELIGHTS) + " lights are available"
        );
    }
}

Light::~Light()
{
    /* Enable the light */
    __spVideoDriver->setLightStatus(LightID_, false);
    
    /* Remove the light from the list */
    __spLightIDList[LightID_] = false;
}

/* Lighting type */

void Light::setLightingType(const ELightModels Type)
{
    LightingType_ = Type;
}

/* Lighting color */

void Light::setLightingColor(const video::color &Diffuse, const video::color &Ambient, const video::color &Specular)
{
    /* General settings */
    DiffuseColor_   = Diffuse;
    AmbientColor_   = Ambient;
    SpecularColor_  = Specular;
    
    /* Update lighting colors */
    __spVideoDriver->setLightColor(LightID_, DiffuseColor_, AmbientColor_, SpecularColor_);
}
void Light::getLightingColor(video::color &Diffuse, video::color &Ambient, video::color &Specular) const
{
    Diffuse     = DiffuseColor_;
    Ambient     = AmbientColor_;
    Specular    = SpecularColor_;
}

/* Spot cone */

void Light::setSpotCone(const f32 InnerConeAngle, const f32 OuterConeAngle)
{
    SpotInnerConeAngle_ = InnerConeAngle;
    SpotOuterConeAngle_ = OuterConeAngle;
}

void Light::getSpotCone(f32 &InnerConeAngle, f32 &OuterConeAngle) const
{
    InnerConeAngle = SpotInnerConeAngle_;
    OuterConeAngle = SpotOuterConeAngle_;
}

/* Volumetric */

void Light::setVolumetric(bool isVolumetric)
{
    if (!isVolumetric)
    {
        f32 tmp1 = 1.0f, tmp2 = 0.0f;
        
        /* Update the renderer for the light */
        __spVideoDriver->updateLight(
            LightID_, LightingType_, isVolumetric_,
            Direction_, SpotInnerConeAngle_, SpotOuterConeAngle_,
            tmp1, tmp2, tmp2
        );
    }
    
    isVolumetric_ = isVolumetric;
}

/*
                1
 _________________________________

  Att0 + (Att1 * d) + (Att2 * d�)
*/

void Light::setVolumetricRadius(f32 Radius)
{
    AttenuationConstant_    = 1.0;
    AttenuationLinear_      = 1.0 / Radius;
    AttenuationQuadratic_   = 1.0 / Radius;
}
f32 Light::getVolumetricRadius() const
{
    return 1.0 / AttenuationLinear_;
}

void Light::setVolumetricRange(f32 Constant, f32 Linear, f32 Quadratic)
{
    AttenuationConstant_    = Constant;
    AttenuationLinear_      = Linear;
    AttenuationQuadratic_   = Quadratic;
}
void Light::getVolumetricRange(f32 &Constant, f32 &Linear, f32 &Quadratic) const
{
    Constant    = AttenuationConstant_;
    Linear      = AttenuationLinear_;
    Quadratic   = AttenuationQuadratic_;
}

void Light::setDirection(const dim::vector3df &Direction)
{
    Direction_ = Direction;
    Direction_.normalize();
}
void Light::setDirection(const dim::matrix4f &Matrix)
{
    Direction_ = Matrix * dim::vector3df(0, 0, 1);
    Direction_.normalize();
}

void Light::setVisible(bool isVisible)
{
    /* Set the light stauts */
    __spVideoDriver->setLightStatus(LightID_, isVisible_ = isVisible);
}

Light* Light::copy() const
{
    /* Allocate a new light */
    Light* NewLight = new Light(LightingType_);
    
    /* Copy the root attributes */
    copyRoot(NewLight);
    
    /* Copy the light configurations */
    NewLight->Direction_            = Direction_;
    NewLight->SpotInnerConeAngle_   = SpotInnerConeAngle_;
    NewLight->SpotOuterConeAngle_   = SpotOuterConeAngle_;
    
    NewLight->isVolumetric_         = isVolumetric_;
    NewLight->AttenuationConstant_  = AttenuationConstant_;
    NewLight->AttenuationLinear_    = AttenuationLinear_;
    NewLight->AttenuationQuadratic_ = AttenuationQuadratic_;
    
    NewLight->AmbientColor_         = AmbientColor_;
    NewLight->SpecularColor_        = SpecularColor_;
    
    /* Return the new light */
    return NewLight;
}


/*
 * ======= Updateing =======
 */

void Light::render()
{
    /* Matrix transformation */
    updateGlobalLocation();
    
    /* Update the render matrix */
    __spVideoDriver->updateModelviewMatrix();
    
    /* Update the renderer for the light */
    __spVideoDriver->updateLight(
        LightID_, LightingType_, isVolumetric_,
        Direction_, SpotInnerConeAngle_, SpotOuterConeAngle_,
        AttenuationConstant_, AttenuationLinear_, AttenuationQuadratic_
    );
}


} // /namespace scene

} // /namespace sp



// ================================================================================
 
