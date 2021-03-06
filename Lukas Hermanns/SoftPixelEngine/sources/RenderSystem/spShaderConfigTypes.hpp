/*
 * Shader configuration types header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_SHADER_CONFIG_TYPES_H__
#define __SP_SHADER_CONFIG_TYPES_H__


#include "Base/spStandard.hpp"

#include <boost/function.hpp>


namespace sp
{

namespace scene { class MaterialNode; };

namespace video
{


/*
 * Enumerations
 */

enum EShaderTypes
{
    SHADER_DUMMY,           //!< Dummy shader when shaders are not supported.
    SHADER_VERTEX_PROGRAM,  //!< Vertex program since Direct3D 8 and OpenGL 1.3.
    SHADER_PIXEL_PROGRAM,   //!< Pixel program since Direct3D 8 and OpenGL 1.3.
    SHADER_VERTEX,          //!< Vertex shader since Direct3D 9 and OpenGL 2.
    SHADER_PIXEL,           //!< Pixel shader since Direct3D 9 and OpenGL 2.
    SHADER_GEOMETRY,        //!< Geometry shader since Direct3D 10 and OpenGL 3.2.
    SHADER_HULL,            //!< Hull shader (for tessellation) since Direct3D 11 and OpenGL 4.
    SHADER_DOMAIN,          //!< Domain shader (for tessellation) since Direct3D 11 and OpenGL 4.
    SHADER_COMPUTE,         //!< Compute shader since Direct3D 11 and OpenGL 4.
};

enum EConstantTypes
{
    CONSTANT_UNKNOWN,   //!< Unknown constant type.
    CONSTANT_BOOL,      //!< Single Boolean.
    CONSTANT_INT,       //!< Single Integer.
    CONSTANT_FLOAT,     //!< Single float.
    CONSTANT_VECTOR2,   //!< 2D float vector.
    CONSTANT_VECTOR3,   //!< 3D float vector.
    CONSTANT_VECTOR4,   //!< 4D float vector.
    CONSTANT_MATRIX2,   //!< 2x2 float matrix.
    CONSTANT_MATRIX3,   //!< 3x3 float matrix.
    CONSTANT_MATRIX4,   //!< 4x4 float matrix.
};

enum EShaderVersions
{
    DUMMYSHADER_VERSION,
    
    GLSL_VERSION_1_20,
    GLSL_VERSION_1_30,
    GLSL_VERSION_1_40,
    GLSL_VERSION_1_50,
    GLSL_VERSION_3_30_6,
    GLSL_VERSION_4_00_8,
    
    HLSL_VERTEX_1_0,
    HLSL_VERTEX_2_0,
    HLSL_VERTEX_2_a,
    HLSL_VERTEX_3_0,
    HLSL_VERTEX_4_0,
    HLSL_VERTEX_4_1,
    HLSL_VERTEX_5_0,
    
    HLSL_PIXEL_1_0,
    HLSL_PIXEL_1_1,
    HLSL_PIXEL_1_2,
    HLSL_PIXEL_1_3,
    HLSL_PIXEL_1_4,
    HLSL_PIXEL_2_0,
    HLSL_PIXEL_2_a,
    HLSL_PIXEL_2_b,
    HLSL_PIXEL_3_0,
    HLSL_PIXEL_4_0,
    HLSL_PIXEL_4_1,
    HLSL_PIXEL_5_0,
    
    HLSL_GEOMETRY_4_0,
    HLSL_GEOMETRY_4_1,
    HLSL_GEOMETRY_5_0,
    
    HLSL_COMPUTE_4_0,
    HLSL_COMPUTE_4_1,
    HLSL_COMPUTE_5_0,
    
    HLSL_HULL_5_0,
    
    HLSL_DOMAIN_5_0,
    
    CG_VERSION_2_0,
};


class Texture;
class Shader;
class ShaderTable;

struct SMaterialStates;
struct SMeshSurfaceTexture;

/**
Construction of the shader object callback function. A shader callback can be used to update the
shader constants (or rather variables) before the shader is set and the object rendered.
When a ShaderTable is bounded to a Mesh or a Billboard both parameters are always none zero
and you do not need to check if they are valid pointers.
\param Table: Pointer to a ShaderTable object which is currently used.
\param Object: Pointer to a MaterialNode object which is currently used.
*/
typedef boost::function<void (ShaderTable* Table, const scene::MaterialNode* Object)> ShaderObjectCallback;

/**
Construction of the shader surface callback. This is similar to "PFNSHADEROBJECTCALLBACKPROC" but in this
case the callback will be called for each surface. You can update your shader settings for the individual
textures.
*/
typedef boost::function<void (ShaderTable* Table, const std::vector<SMeshSurfaceTexture>* TextureList)> ShaderSurfaceCallback;


} // /namespace video

} // /namespace sp


#endif



// ================================================================================
