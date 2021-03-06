/*
 * Material configuration types header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_MATERIAL_CONFIG_TYPES_H__
#define __SP_MATERIAL_CONFIG_TYPES_H__


#include "Base/spStandard.hpp"
#include "Base/spDimension.hpp"
#include "RenderSystem/spTextureBase.hpp"

#include <limits.h>
#include <boost/function.hpp>


namespace sp
{

namespace scene { class Mesh; }

namespace video
{


class MeshBuffer;

/*
 * Typedefinitions & macros
 */

static const u8 TEXTURE_IGNORE  = UCHAR_MAX;
static const c8* TEXPATH_IGNORE = "?";


typedef boost::function<void (scene::Mesh* Obj, bool isBegin)> UserMaterialCallback;
typedef boost::function<void (scene::Mesh* Obj, std::vector<MeshBuffer*>* LODSurfaceList, u32 LODIndex)> UserRenderCallback;


/*
 * Enumerations
 */

//! Supported render systems.
enum ERenderSystems
{
    RENDERER_AUTODETECT,    //!< This is not a driver but the engine will choose the best suitable video driver for the operating-system.
    RENDERER_OPENGL,        //!< OpenGL 1.1 - 4.1 renderer.
    RENDERER_OPENGLES1,     //!< OpenGL|ES 1.1 renderer (only for Android and iOS).
    RENDERER_OPENGLES2,     //!< OpenGL|ES 2.0 renderer (only for Android and iOS).
    RENDERER_DIRECT3D9,     //!< Direct3D 9.0c renderer (only for MS/Windows).
    RENDERER_DIRECT3D11,    //!< Direct3D 11.0 renderer (only for MS/Windows Vista or later). This renderer is backwards compatible to Direct3D 9 and only supported for VisualC++.
    RENDERER_DUMMY,         //!< "Dummy" renderer. Just for debugging or other none graphical perposes.
};

//! Mesh wireframe types for Entities, Sprites and Terrains.
enum EWireframeTypes
{
    WIREFRAME_POINTS = 0,   //!< Vertices' positions are rendered in the form of points.
    WIREFRAME_LINES,        //!< Triangle's edges are rendered in the form of lines.
    WIREFRAME_SOLID,        //!< Triangles are rendered filled (or solid).
};

//! Triangle face types used for rendering and collision-detection.
enum EFaceTypes
{
    FACE_FRONT, //!< Front face culling (default): triangle's front face are used.
    FACE_BACK,  //!< Back face culling: triangle's back face are used.
    FACE_BOTH   //!< No face culling: both front and back sides of a triangle are used.
};

//! Shading types for 3D models. Decide about how the normals are computed.
enum EShadingTypes
{
    SHADING_FLAT,       //!< Flat shading: Normals are computed for each vertex.
    SHADING_GOURAUD,    //!< Gouraud shading (by default): Normals areERendererDataTypes computed for each vertex and smoothed for adjacency triangles.
    SHADING_PHONG,      //!< Phong shading: Same like gouraud shading but normals will be normalized for each pixel (only supported with shader).
    SHADING_PERPIXEL,   //!< Per pixel shading: Lighting computations are processed for each pixel (only supported with shader).
};

//! Size comparisions types for depth- or alpha tests.
enum ESizeComparisionTypes
{
    CMPSIZE_NEVER = 0,      //!< Never passes.
    CMPSIZE_EQUAL,          //!< Passes if the incoming value is equal to the stored value.
    CMPSIZE_NOTEQUAL,       //!< Passes if the incoming value is not equal to the stored value.
    CMPSIZE_LESS,           //!< Passes if the incoming value is less than the stored value.
    CMPSIZE_LESSEQUAL,      //!< Passes if the incoming value is less than or equal to the stored value.
    CMPSIZE_GREATER,        //!< Passes if the incoming value is greater than the stored value.
    CMPSIZE_GREATEREQUAL,   //!< Passes if the incoming value is greater than or equal to the stored value.
    CMPSIZE_ALWAYS,         //!< Always passes.
};

//! Blending modes for source and destination (or target): Blending = Source * SourceFactor + Dest * DestFactor.
enum EBlendingTypes
{
    BLEND_ZERO = 0,     //!< Source & Dest(0, 0, 0, 0).
    BLEND_ONE,          //!< Source & Dest(1, 1, 1, 1).
    BLEND_SRCCOLOR,     //!< Dest(SourceR, SourceG, SourceB, SourceA).
    BLEND_INVSRCCOLOR,  //!< Dest(1 - SourceR, 1 - SourceG, 1 - SourceB, 1 - SourceA).
    BLEND_SRCALPHA,     //!< Source & Dest(SourceA, SourceA, SourceA, SourceA).
    BLEND_INVSRCALPHA,  //!< Source & Dest(1 - SourceA, 1 - SourceA, 1 - SourceA, 1 - SourceA).
    BLEND_DESTCOLOR,    //!< Source(DestR, DestG, DestB, DestA).
    BLEND_INVDESTCOLOR, //!< Source(1 - DestR, 1 - DestG, 1 - DestB, 1 - DestA).
    BLEND_DESTALPHA,    //!< Source & Dest(DestA, DestA, DestA, DestA).
    BLEND_INVDESTALPHA, //!< Source & Dest(1 - DestA, 1 - DestA, 1 - DestA, 1 - DestA).
};

//! Typical blending combinations
enum EDefaultBlendingTypes
{
    BLEND_NORMAL,   //!< Normal blending.
    BLEND_BRIGHT,   //!< Brightness blending (The brighter the color the more visible).
    BLEND_DARK,     //!< Darkness blending (The darker the color the more visible).
};

//! Texture environment types which tell the renderer how to produce incoming texels (texture pixels) for multi-texturing.
enum ETextureEnvTypes
{
    TEXENV_MODULATE = 0,    //!< Incoming texels are multiplied by the previous texels (by default).
    TEXENV_REPLACE,         //!< Incoming texels replace the previous texels.
    TEXENV_ADD,             //!< Incoming texels are added to the previous texels.
    TEXENV_ADDSIGNED,       //!< Incoming texels are added minus 0.5 to the previous texels.
    TEXENV_SUBTRACT,        //!< Incoming texels are subtracted from the previous texels.
    TEXENV_INTERPOLATE,     //!< The texels are calculated by the following equation: [ Tex0 x Tex2 + Tex1 x (1 - Tex2) ], where 'Tex' are the texels from the respective textures.
    TEXENV_DOT3,            //!< The texels are calculated for dot3 bump mapping.
};

//! Texture coordinate generation types which tell the renderer how to manipulate the vertices' texture coordinates.
enum EMappingGenTypes
{
    MAPGEN_DISABLE = 0,     //!< Texture coordinates will not be manipulated and the user set vertices' texture coordinates are used (by default).
    MAPGEN_OBJECT_LINEAR,   //!< Texture coordinates are computed by the local space coordinates.
    MAPGEN_EYE_LINEAR,      //!< Texture coordinates are computed by the global space coordinates.
    MAPGEN_SPHERE_MAP,      //!< Texture coordinates are computed for sphere mapping.
    MAPGEN_NORMAL_MAP,      //!< Texture coordinates are computed for normal mapping.
    MAPGEN_REFLECTION_MAP,  //!< Texture coordinates are computed for reflection-/ cube mapping.
};

//! Texture mapping generation coordinates
enum EMappingGenCoords
{
    MAPGEN_NONE = 0x0000,   //!< No texture coordinate (disabled).
    MAPGEN_S    = 0x0001,   //!< S texture coordinate (or rather X).
    MAPGEN_T    = 0x0002,   //!< T texture coordinate (or rather Y).
    MAPGEN_R    = 0x0004,   //!< R texture coordinate (or rather Z).
    MAPGEN_Q    = 0x0008,   //!< Q texture coordinate (or rather W).
};

//! Render states to enabled, disable or set video driver configurations in more detail (used for advanced rendering!).
enum ERenderStates
{
    RENDER_ALPHATEST,       //!< Alpha test: if enabled an incomming pixel will only be rendered if the alpha channel passes the alpha test.
    RENDER_BLEND,           //!< Blending: if enabled alpha blending will be proceeded.
    RENDER_COLORMATERIAL,   //!< Color material: if enabled the vertices are colored by their vertex color, otherwise the object material color will be used for each vertex.
    RENDER_CULLFACE,        //!< Face culling: if enabled a triangle will only be rendered if the front face is visible.
    RENDER_DEPTH,           //!< Depth test: if enabled an incomming pixel will only be rendered if the depth value passes the depth test.
    RENDER_DITHER,          //!< Dithering: if enabled a color change over will be rendered with dither effect (only with a 8 or 16 bit color format).
    RENDER_FOG,             //!< Fog effect: if enabled fog effect is rendered.
    RENDER_LIGHTING,        //!< Lighting: if enabled lighting computations are proceeded.
    RENDER_LINESMOOTH,      //!< Lines smooth: if enabled 2D and 3D lines are rendered with anti aliasing (or rather smooth).
    RENDER_MULTISAMPLE,     //!< Multi sample: if enabled anti-aliasing (or rather multi-sampling) will be proceeded.
    RENDER_NORMALIZE,       //!< Normalize: if enabled each vertex's normal will be normalized by the hardware (this is used when objects are scaled).
    RENDER_POINTSMOOTH,     //!< Points smooth: if enabled 2D and 3D points are rendered with anti aliasing (or rather smooth).
    RENDER_RESCALENORMAL,   //!< Rescale normals: similiar to normalize normals with other calculation equations.
    RENDER_SCISSOR,         //!< Scissor: if enabled a scissor rectangle is masked over the screen whereby each primitive (triangles, lines and points) are clipped inside.
    RENDER_STENCIL,         //!< Stencil: if enabled an incomming pixel will only be rendered if it passes the stencil test (amongst others used for real-time shadow).
    RENDER_TEXTURE,         //!< Texturing: if enabled texturing is activated, otherwise no subsequent 3D object will have any mapped texture.
    RENDER_TEXTURECUBEMAP,  //!< Texture cube map: if enabled cube mapping is activated.
};

//! Video feature support queries
enum EVideoFeatureQueries
{
    QUERY_ANTIALIASING,                 //!< Query if anti-aliasing is supported.
    QUERY_MULTI_TEXTURE,                //!< Query if multi-texturing is supported.
    QUERY_MULTISAMPLE_TEXTURE,          //!< Query if multisample textures are supported.
    QUERY_HARDWARE_MESHBUFFER,          //!< Query if mesh buffers are supported (including vertex- and index buffers).
    QUERY_HARDWARE_INSTANCING,          //!< Query if hardware instancing is supported.
    QUERY_STENCIL_BUFFER,               //!< Query if a stencil buffer is supported.
    QUERY_RENDERTARGET,                 //!< Query if render targets (frame-buffer-objects for OpenGL) are supported.
    QUERY_MULTISAMPLE_RENDERTARGET,     //!< Query if render targets (frame-buffer-objects for OpenGL) are supported.
    QUERY_VOLUMETRIC_FOG,               //!< Query if volumetric fog coordinates are supported.
    
    QUERY_BILINEAR_FILTER,              //!< Query if the bilinear texture filter is supported (low quality).
    QUERY_TRILINEAR_FILTER,             //!< Query if the trilinear texture filter is supported (medium quality, by default).
    QUERY_ANISOTROPY_FILTER,            //!< Query if the anisotropic texture filter is supported (high quality).
    QUERY_MIPMAPS,                      //!< Query if texture MIP-mapping is supported (even with the SoftwareRenderer).
    QUERY_VOLUMETRIC_TEXTURE,           //!< Query if volumetric (or rather 3D-) textures are supported.
    
    QUERY_SHADER,                       //!< Query if shader are generally supported (shader programs, GLSL or HLSL).
    QUERY_GEOMETRY_SHADER,              //!< Query if geometry shader are supported.
    QUERY_TESSELLATION_SHADER,          //!< Query if tessellation shader are supported.
    QUERY_VETEX_PROGRAM,                //!< Query if vertex programs are supported.
    QUERY_FRAGMENT_PROGRAM,             //!< Query if pixel programs are supported.
    QUERY_GLSL,                         //!< Query if GLSL is supported (only with OpenGL).
    QUERY_HLSL,                         //!< Query if HLSL is supported (only with DirectX).
    QUERY_VERTEX_SHADER_1_1,            //!< Query if HLSL vertex shader version is at least 1.1.
    QUERY_VERTEX_SHADER_2_0,            //!< HLSL vertex shader 2.0+.
    QUERY_VERTEX_SHADER_3_0,            //!< HLSL vertex shader 3.0+.
    QUERY_VERTEX_SHADER_4_0,            //!< HLSL vertex shader 4.0+.
    QUERY_VERTEX_SHADER_4_1,            //!< HLSL vertex shader 4.1+.
    QUERY_VERTEX_SHADER_5_0,            //!< HLSL vertex shader 5.0+.
    QUERY_PIXEL_SHADER_1_1,             //!< Query if HLSL pixel shader version is at least 1.1.
    QUERY_PIXEL_SHADER_1_2,             //!< HLSL pixel shader 1.2+.
    QUERY_PIXEL_SHADER_1_3,             //!< HLSL pixel shader 1.3+.
    QUERY_PIXEL_SHADER_1_4,             //!< HLSL pixel shader 1.4+.
    QUERY_PIXEL_SHADER_2_0,             //!< HLSL pixel shader 2.0+.
    QUERY_PIXEL_SHADER_3_0,             //!< HLSL pixel shader 3.0+.
    QUERY_PIXEL_SHADER_4_0,             //!< HLSL pixel shader 4.0+.
    QUERY_PIXEL_SHADER_4_1,             //!< HLSL pixel shader 4.1+.
    QUERY_PIXEL_SHADER_5_0,             //!< HLSL pixel shader 5.0+.
};

//! Renderable primitives with their list order.
enum ERenderPrimitives
{
    PRIMITIVE_POINTS = 0,               //!< Point list (by default only one pixel).
    PRIMITIVE_LINES,                    //!< Line list (by default with one pixel width).
    PRIMITIVE_LINE_STRIP,               //!< Line strip where each primitive after the first one begins with the previous vertex.
    PRIMITIVE_LINE_LOOP,                //!< Line loop which is similiar to line strip but the last primtive ends with the first vertex. (Only supported for OpenGL).
    PRIMITIVE_TRIANGLES,                //!< Triangle list.
    PRIMITIVE_TRIANGLE_STRIP,           //!< Triangle strip where each primitive after the first one begins with the previous vertex.
    PRIMITIVE_TRIANGLE_FAN,             //!< Triangle fan where each primitive use the first vertex, the previous vertex and a new vertex.
    PRIMITIVE_QUADS,                    //!< Quad list. (Only supported for OpenGL).
    PRIMITIVE_QUAD_STRIP,               //!< Quad strip where each primitive after the first one begins with the previous vertex. (Only supported for OpenGL).
    PRIMITIVE_POLYGON,                  //!< Polygon. This is relatively slow. (Only supported for OpenGL).
    PRIMITIVE_LINE_STRIP_ADJACENCY,     //!< Adjacency line strips.
    PRIMITIVE_LINES_ADJACENCY,          //!< Adjacency line list.
    PRIMITIVE_TRIANGLE_STRIP_ADJACENCY, //!< Adjacency triangle strips.
    PRIMITIVE_TRIANGLES_ADJACENCY,      //!< Adjacency triangle list.
    PRIMITIVE_PATCHES,                  //!< Patches (main type for tessellation).
};

//! Mesh buffer usage types.
enum EMeshBufferUsage
{
    MESHBUFFER_STATIC = 0,  //!< Static usage. Buffer is not modified often.
    MESHBUFFER_DYNAMIC,     //!< Dynamic usage. Buffer is often modified.
};


/**
SMeshSurfaceTexture represents the structure for each texture object stored on a mesh buffer.
It contains a video::Texture object and some other configuration such as the matrix transformation.
*/
struct SMeshSurfaceTexture
{
    SMeshSurfaceTexture(Texture* DefTexture = 0)
        : TextureObject(DefTexture)
    {
        TextureLayer        = 0;
        TexMappingCoords    = MAPGEN_NONE;
        TexMappingGen       = MAPGEN_DISABLE;
        TexEnvType          = TEXENV_MODULATE;
    }
    ~SMeshSurfaceTexture()
    {
    }
    
    /* Operators */
    
    bool operator == (const SMeshSurfaceTexture &other) const
    {
        return
            TextureLayer        == other.TextureLayer &&
            TextureObject       == other.TextureObject &&
            TexMappingCoords    == other.TexMappingCoords &&
            TexMappingGen       == other.TexMappingGen &&
            TexEnvType          == other.TexEnvType;
            //Matrix              == other.Matrix;
    }
    
    bool operator != (const SMeshSurfaceTexture &other) const
    {
        return !(*this == other);
    }
    
    /* Members */
    
    u8 TextureLayer;
    
    Texture* TextureObject;
    
    s32 TexMappingCoords;
    EMappingGenTypes TexMappingGen;
    ETextureEnvTypes TexEnvType;
    
    dim::matrix4f Matrix;
    
};


} // /namespace video

} // /namespace sp


#endif



// ================================================================================
