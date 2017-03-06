/*
 * OpenGL fixed function renderer file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "RenderSystem/OpenGL/spOpenGLPipelineFixed.hpp"

#if defined(SP_COMPILE_WITH_OPENGL) || defined(SP_COMPILE_WITH_OPENGLES1)


#include "SceneGraph/spSceneCamera.hpp"
#include "Base/spInternalDeclarations.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"

#if defined(SP_COMPILE_WITH_OPENGL)
#   include "RenderSystem/OpenGL/spOpenGLFunctionsARB.hpp"
#   include "RenderSystem/OpenGL/spOpenGLTexture.hpp"
#elif defined(SP_COMPILE_WITH_OPENGLES1)
#   include "RenderSystem/OpenGLES/spOpenGLESFunctionsARB.hpp"
#   include "RenderSystem/OpenGLES/spOpenGLES1Texture.hpp"
#endif


namespace sp
{

extern io::InputControl* __spInputControl;
extern scene::SceneGraph* __spSceneManager;

namespace video
{


/*
 * ======= Internal members =======
 */

s32 GLTextureEnvList[] = {
    GL_MODULATE, GL_REPLACE, GL_ADD, GL_ADD_SIGNED, GL_SUBTRACT, GL_INTERPOLATE, GL_DOT3_RGB,
};

#if defined(SP_COMPILE_WITH_OPENGL)

GLenum GLBasicDataTypes[] = {
    GL_FLOAT, GL_DOUBLE, GL_BYTE, GL_SHORT, GL_INT, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT
};

s32 GLMappingGenList[] = {
    GL_OBJECT_LINEAR, GL_OBJECT_LINEAR, GL_EYE_LINEAR, GL_SPHERE_MAP, GL_NORMAL_MAP, GL_REFLECTION_MAP,
};

#endif


extern s32 STDPixelFormatDataSize[];


/*
 * ======= GLFixedFunctionPipeline class =======
 */

dim::matrix4f GLFixedFunctionPipeline::ExtTmpMat_;

GLFixedFunctionPipeline::GLFixedFunctionPipeline()
    : RenderSystem(RENDERER_DUMMY), GLBasePipeline()
{
    /* General settings */
    isCullFace_ = GL_FALSE;
    
    setFogRange(0.1f);
    
    MaxClippingPlanes_ = GL_MAX_CLIP_PLANES;
    ClippingPlanes_.resize(MaxClippingPlanes_);
    
    /* Create default vertex formats */
    createDefaultVertexFormats();
}
GLFixedFunctionPipeline::~GLFixedFunctionPipeline()
{
}


/*
 * ======= Render system information =======
 */

s32 GLFixedFunctionPipeline::getMultitexCount() const
{
    s32 MultiTextureCount;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &MultiTextureCount);
    return MultiTextureCount;
}
s32 GLFixedFunctionPipeline::getMaxLightCount() const
{
    s32 MultiLightCount;
    glGetIntegerv(GL_MAX_LIGHTS, &MultiLightCount);
    return MultiLightCount;
}


/*
 * ======= Configuration functions =======
 */

void GLFixedFunctionPipeline::setShadeMode(const EShadeModeTypes ShadeMode)
{
    switch (ShadeMode)
    {
        case SHADEMODE_SMOOTH:
            glShadeModel(GL_SMOOTH); break;
        case SHADEMODE_FLAT:
            glShadeModel(GL_FLAT); break;
    }
}


/*
 * ======= Rendering functions =======
 */

void GLFixedFunctionPipeline::updateLight(
    u32 LightID, const scene::ELightModels LightType, bool isVolumetric,
    const dim::vector3df &Direction, f32 SpotInnerConeAngle, f32 SpotOuterConeAngle,
    f32 AttenuationConstant, f32 AttenuationLinear, f32 AttenuationQuadratic)
{
    /* Use an OpenGL light */
    u32 ID = LightID + GL_LIGHT0;
    
    /* Lighting location */
    if (LightType == scene::LIGHT_DIRECTIONAL)
    {
        f32 LightPos[4] = { -Direction.X, -Direction.Y, -Direction.Z, 0.0 };
        glLightfv(ID, GL_POSITION, LightPos);
    }
    else
    {
        f32 OriginVector[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glLightfv(ID, GL_POSITION, OriginVector);
    }
    
    // !TODO! -> replace all following light functions out of render loop !!!
    
    /* Spot light attributes */
    if (LightType == scene::LIGHT_SPOT)
    {
        glLightfv(ID, GL_SPOT_DIRECTION, &Direction.X);
        glLightf(ID, GL_SPOT_CUTOFF, SpotOuterConeAngle);
        glLightf(ID, GL_SPOT_EXPONENT, SpotInnerConeAngle);
    }
    else
    {
        glLightf(ID, GL_SPOT_CUTOFF, 180);
        glLighti(ID, GL_SPOT_EXPONENT, 0);
    }
    
    /* Volumetric light attenuations */
    if (isVolumetric)
    {
        glLightf(ID, GL_CONSTANT_ATTENUATION, AttenuationConstant);
        glLightf(ID, GL_LINEAR_ATTENUATION, AttenuationLinear);
        glLightf(ID, GL_QUADRATIC_ATTENUATION, AttenuationQuadratic);
    }
    else
    {
        glLighti(ID, GL_CONSTANT_ATTENUATION, 1);
        glLighti(ID, GL_LINEAR_ATTENUATION, 0);
        glLighti(ID, GL_QUADRATIC_ATTENUATION, 0);
    }
}


/*
 * ======= Render states =======
 */

void GLFixedFunctionPipeline::setRenderState(const video::ERenderStates Type, s32 State)
{
    switch (Type)
    {
        case RENDER_ALPHATEST:
            setGlRenderState(GL_ALPHA_TEST, State); break;
        case RENDER_BLEND:
            setGlRenderState(GL_BLEND, State); break;
        case RENDER_COLORMATERIAL:
            setGlRenderState(GL_COLOR_MATERIAL, State); break;
        case RENDER_CULLFACE:
            setGlRenderState(GL_CULL_FACE, State); break;
        case RENDER_DEPTH:
            setGlRenderState(GL_DEPTH_TEST, State); break;
        case RENDER_DITHER:
            setGlRenderState(GL_DITHER, State); break;
        case RENDER_FOG:
            setGlRenderState(GL_FOG, State); break;
        case RENDER_LIGHTING:
            setGlRenderState(GL_LIGHTING, State); break;
        case RENDER_LINESMOOTH:
            setGlRenderState(GL_LINE_SMOOTH, State); break;
        case RENDER_MULTISAMPLE:
            setGlRenderState(GL_MULTISAMPLE, State); break;
        case RENDER_NORMALIZE:
            setGlRenderState(GL_NORMALIZE, State); break;
        case RENDER_POINTSMOOTH:
            setGlRenderState(GL_POINT_SMOOTH, State); break;
        case RENDER_RESCALENORMAL:
            setGlRenderState(GL_RESCALE_NORMAL, State); break;
        case RENDER_SCISSOR:
            setGlRenderState(GL_SCISSOR_TEST, State); break;
        case RENDER_STENCIL:
            setGlRenderState(GL_STENCIL_TEST, State); break;
        #if defined(GL_TEXTURE_CUBE_MAP)
        case RENDER_TEXTURECUBEMAP:
            setGlRenderState(GL_TEXTURE_CUBE_MAP, State); break;
        #endif
        case RENDER_TEXTURE:
            __isTexturing = (State != 0); break;
    }
}

s32 GLFixedFunctionPipeline::getRenderState(const video::ERenderStates Type) const
{
    switch (Type)
    {
        case RENDER_ALPHATEST:
            return (s32)getGlRenderState(GL_ALPHA_TEST);
        case RENDER_BLEND:
            return (s32)getGlRenderState(GL_BLEND);
        case RENDER_COLORMATERIAL:
            return (s32)getGlRenderState(GL_COLOR_MATERIAL);
        case RENDER_CULLFACE:
            return (s32)getGlRenderState(GL_CULL_FACE);
        case RENDER_DEPTH:
            return (s32)getGlRenderState(GL_DEPTH_TEST);
        case RENDER_DITHER:
            return (s32)getGlRenderState(GL_DITHER);
        case RENDER_FOG:
            return (s32)getGlRenderState(GL_FOG);
        case RENDER_LIGHTING:
            return (s32)getGlRenderState(GL_LIGHTING);
        case RENDER_LINESMOOTH:
            return (s32)getGlRenderState(GL_LINE_SMOOTH);
        case RENDER_MULTISAMPLE:
            return (s32)getGlRenderState(GL_MULTISAMPLE);
        case RENDER_NORMALIZE:
            return (s32)getGlRenderState(GL_NORMALIZE);
        case RENDER_POINTSMOOTH:
            return (s32)getGlRenderState(GL_POINT_SMOOTH);
        case RENDER_RESCALENORMAL:
            return (s32)getGlRenderState(GL_RESCALE_NORMAL);
        case RENDER_SCISSOR:
            return (s32)getGlRenderState(GL_SCISSOR_TEST);
        case RENDER_STENCIL:
            return (s32)getGlRenderState(GL_STENCIL_TEST);
        #if defined(GL_TEXTURE_CUBE_MAP)
        case RENDER_TEXTURECUBEMAP:
            return (s32)getGlRenderState(GL_TEXTURE_CUBE_MAP);
        #endif
        case RENDER_TEXTURE:
            return (s32)__isTexturing;
    }
    
    return 0;
}

void GLFixedFunctionPipeline::disableTriangleListStates()
{
    /* Back settings - texture */
    if (RenderQuery_[RENDERQUERY_MULTI_TEXTURE])
        glActiveTextureARB(GL_TEXTURE0);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    /* Back settings - mesh buffer */
    if (RenderQuery_[RENDERQUERY_HARDWARE_MESHBUFFER])
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
    }
    
    /* Back settings - polygons */
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    #if defined(SP_COMPILE_WITH_OPENGL)
    /* Disable mapping projection */
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
    #endif
    
    /* Default render functions */
    glAlphaFunc(GL_ALWAYS, 0.0);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    
    LastMaterial_ = 0;
}

void GLFixedFunctionPipeline::disable3DRenderStates()
{
    /* Disable all used states */
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
}

void GLFixedFunctionPipeline::disableTexturing()
{
    glDisable(GL_TEXTURE_2D);
}


/*
 * ======= Lighting =======
 */

void GLFixedFunctionPipeline::addDynamicLightSource(
    u32 LightID, scene::ELightModels Type,
    video::color &Diffuse, video::color &Ambient, video::color &Specular,
    f32 AttenuationConstant, f32 AttenuationLinear, f32 AttenuationQuadratic)
{
    /* Enable the light */
    glEnable(GL_LIGHT0 + LightID);
    
    /* Set the light color */
    setLightColor(LightID, Diffuse, Ambient, Specular);
}

void GLFixedFunctionPipeline::setLightStatus(u32 LightID, bool isEnable)
{
    setGlRenderState(GL_LIGHT0 + LightID, isEnable);
}

void GLFixedFunctionPipeline::setLightColor(
    u32 LightID, const video::color &Diffuse, const video::color &Ambient, const video::color &Specular)
{
    /* Use an OpenGL light */
    LightID += GL_LIGHT0;
    
    /* Temporary memories */
    f32 pDiffuse[4], pAmbient[4], pSpecular[4];
    
    /* Color: Diffuse */
    Diffuse.getFloatArray(pDiffuse);
    glLightfv(LightID, GL_DIFFUSE, pDiffuse);
    
    /* Color: Ambient */
    Ambient.getFloatArray(pAmbient);
    glLightfv(LightID, GL_AMBIENT, pAmbient);
    
    /* Color: Specular */
    Specular.getFloatArray(pSpecular);
    glLightfv(LightID, GL_SPECULAR, pSpecular);
}


/*
 * ======= Fog effect =======
 */

void GLFixedFunctionPipeline::setFog(const EFogTypes Type)
{
    /* Select the fog mode */
    switch (FogType_ = Type)
    {
        case FOG_NONE:
        {
            __isFog = false;
        }
        break;
        
        case FOG_STATIC:
        {
            __isFog = true;
            
            /* Set fog type */
            switch (FogMode_)
            {
                case FOG_PALE:
                    glFogi(GL_FOG_MODE, GL_EXP); break;
                case FOG_THICK:
                    glFogi(GL_FOG_MODE, GL_EXP2); break;
            }
            
            /* Range settings */
            glFogf(GL_FOG_DENSITY, FogRange_);
            glFogf(GL_FOG_START, FogNear_);
            glFogf(GL_FOG_END, FogFar_);
        }
        break;
        
        case FOG_VOLUMETRIC:
        {
            __isFog = true;
            
            /* Renderer settings */
            glFogi(GL_FOG_MODE, GL_LINEAR);
            glFogf(GL_FOG_DENSITY, FogRange_);
            glFogf(GL_FOG_START, 0.0f);
            glFogf(GL_FOG_END, 1.0f);
            
            #if defined(GL_FOG_COORDINATE_SOURCE_EXT) && defined(GL_FOG_COORDINATE_EXT)
            glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT);
            #endif
        }
        break;
    }
}

void GLFixedFunctionPipeline::setFogColor(const video::color &Color)
{
    f32 ColorBuf[4];
    Color.getFloatArray(ColorBuf);
    glFogfv(GL_FOG_COLOR, ColorBuf);
    FogColor_ = Color;
}

void GLFixedFunctionPipeline::setFogRange(f32 Range, f32 NearPlane, f32 FarPlane, const EFogModes Mode)
{
    FogRange_   = Range;
    FogNear_    = NearPlane;
    FogFar_     = FarPlane;
    FogMode_    = Mode;
    
    if (FogType_ != FOG_VOLUMETRIC)
    {
        /* Set fog type */
        switch (FogMode_)
        {
            case FOG_PALE:
                glFogi(GL_FOG_MODE, GL_EXP); break;
            case FOG_THICK:
                glFogi(GL_FOG_MODE, GL_EXP2); break;
        }
        
        /* Set range */
        glFogf(GL_FOG_DENSITY, FogRange_);
        glFogf(GL_FOG_START, FogNear_);
        glFogf(GL_FOG_END, FogFar_);
    }
}


/*
 * ======= Clipping planes =======
 */

void GLFixedFunctionPipeline::setClipPlane(u32 Index, const dim::plane3df &Plane, bool Enable)
{
    if (Index >= MaxClippingPlanes_)
        return;
    
    setGlRenderState(GL_CLIP_PLANE0 + Index, Enable);
    
    ClippingPlanes_[Index] = SClipPlane(Index, Plane, Enable);
}


/*
 * ======= Simple drawing functions =======
 */

void GLFixedFunctionPipeline::beginDrawing2D()
{
    LastMaterial_ = 0;
    
    /* Set render states */
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
	glEnable(GL_BLEND);
	
    #ifdef SP_COMPILE_WITH_OPENGL
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    #endif
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    if (RenderQuery_[RENDERQUERY_MULTI_TEXTURE])
    {
        /* Use first texture layer */
        glActiveTextureARB(GL_TEXTURE0);
        glClientActiveTextureARB(GL_TEXTURE0);
    }
    
    if (RenderQuery_[RENDERQUERY_HARDWARE_MESHBUFFER])
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
    }
    
    /* Texture matrix */
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    
	/* Modelview matrix */
    glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	glLoadIdentity();
	
    /* Projection matrix */
    glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
    Matrix2D_.make2Dimensional(__Screen_Width, !isInvertScreen_ ? -__Screen_Height : __Screen_Height);
    
    /* Other modes & options */
    glViewport(0, 0, __Screen_Width, __Screen_Height);
	
    if (isSolidMode_)
    {
        glGetBooleanv(GL_CULL_FACE, &isCullFace_);
        glDisable(GL_CULL_FACE);
    }
    else
        glCullFace(GL_FRONT);
    
    #ifdef __DRAW2DARRAYS__
    /* Set vertex pointers for array-drawing */
    glVertexPointer(4, GL_FLOAT, sizeof(scene::SPrimitiveVertex2D), (c8*)Vertices2D_ + ARY_OFFSET_VERTEX);
    glTexCoordPointer(2, GL_FLOAT, sizeof(scene::SPrimitiveVertex2D), (c8*)Vertices2D_ + ARY_OFFSET_TEXCOORD);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(scene::SPrimitiveVertex2D), (c8*)Vertices2D_ + ARY_OFFSET_COLOR);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    #endif
}

void GLFixedFunctionPipeline::endDrawing2D()
{
	/* Reset render states */
    glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	#if 0
    /* Projection matrix */
    glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	/* Modelview matrix */
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	#endif
	
    /* Other settings */
    if (isSolidMode_ && isCullFace_)
        glEnable(GL_CULL_FACE);
    
    #ifdef __DRAW2DARRAYS__
    /* Reset vertex pointers for array-drawing */
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    #endif
    
    glColor4ub(255, 255, 255, 255);
}

void GLFixedFunctionPipeline::beginDrawing3D()
{
    LastMaterial_ = 0;
    
    /* Set render states */
	glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
	glEnable(GL_BLEND);
    
    /* Update camera view */
    if (__spSceneManager && __spSceneManager->getActiveCamera())
    {
        __spSceneManager->getActiveCamera()->updateViewmode();
        __spSceneManager->getActiveCamera()->updateModelviewMatrix();
    }
    
    /* Matrix reset */
	glLoadIdentity();
	
    if (RenderQuery_[RENDERQUERY_MULTI_TEXTURE])
    {
        /* Use first texture layer */
        glActiveTextureARB(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
    }
    
    /* Normal polygon mode */
    #ifdef SP_COMPILE_WITH_OPENGL
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    #endif
}

void GLFixedFunctionPipeline::endDrawing3D()
{
    /* Matrix reset */
    glLoadIdentity();
    
    /* Disable lighting & fog */
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    
    glColor4ub(255, 255, 255, 255);
}

void GLFixedFunctionPipeline::setPointSize(s32 Size)
{
    glPointSize(Size);
}


/*
 * ======= Primitive drawing =======
 */

color GLFixedFunctionPipeline::getPixelColor(const dim::point2di &Position) const
{
    video::color Color;
    glReadPixels(Position.X, __Screen_Height - Position.Y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &Color.Red);
    return Color;
}

void GLFixedFunctionPipeline::draw2DPolygonImage(
    const ERenderPrimitives Type, Texture* Tex, const scene::SPrimitiveVertex2D* VerticesList, u32 Count)
{
    Tex->bind();
    draw2DPolygon(Type, VerticesList, Count);
    Tex->unbind();
}


/*
 * ======= Texture creation =======
 */

Texture* GLFixedFunctionPipeline::createScreenShot(const dim::point2di &Position, dim::size2di Size)
{
    /* Use the standard dimension */
    if (Size == 0)
        Size = dim::size2di(__Screen_Width, __Screen_Height);
    
    /* Setup texture creation flags */
    STextureCreationFlags CreationFlags;
    
    CreationFlags.Filename      = "Screenshot";
    CreationFlags.Size          = Size;
    CreationFlags.MagFilter     = TexGenMagFilter_;
    CreationFlags.MinFilter     = TexGenMinFilter_;
    CreationFlags.MipMapFilter  = TexGenMipMapFilter_;
    CreationFlags.Format        = PIXELFORMAT_RGB;
    CreationFlags.MipMaps       = false;
    CreationFlags.WrapMode      = TexGenWrapMode_;
    
    /* Allocate the new texture */
    #if defined(SP_COMPILE_WITH_OPENGL)
    Texture* NewTexture = new OpenGLTexture(CreationFlags);
    #elif defined(SP_COMPILE_WITH_OPENGLES1)
    Texture* NewTexture = new OpenGLES1Texture(CreationFlags);
    #endif
    
    /* Setup render states */
    pushViewport();
    setViewport(Position, Size);
    
    glBindTexture(GL_TEXTURE_2D, *static_cast<GLuint*>(NewTexture->getOrigID()));
    
    /* Copy screen image into the OpenGL texture */
    glCopyTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB,
        Position.X, __Screen_Height - Size.Height - Position.Y,
        Size.Width, Size.Height, 0
    );
    
    /* Reset states */
    glBindTexture(GL_TEXTURE_2D, 0);
    popViewport();
    
    /* Add the texture to the texture list */
    TextureList_.push_back(NewTexture);
    
    return NewTexture;
}

void GLFixedFunctionPipeline::createScreenShot(Texture* Tex, const dim::point2di &Position)
{
    if (!Tex || Tex->getDimension() != TEXTURE_2D)
        return;
    
    /* Bind the texture */
    Tex->bind();
    
    /* Copy the screen buffer into the texture buffer */
    glCopyTexSubImage2D(
        GL_TEXTURE_2D, 0, 0, 0,
        Position.X, __Screen_Height - Tex->getSize().Height - Position.Y,
        Tex->getSize().Width, Tex->getSize().Height
    );
    
    /* Undbing texturing */
    Tex->unbind();
}


/*
 * ======= Matrix controll =======
 */

void GLFixedFunctionPipeline::updateModelviewMatrix()
{
    const dim::matrix4f ModelviewMatrix(getViewMatrix() * getWorldMatrix());
    
    if (__spSceneManager && __spSceneManager->getActiveCamera() && __spSceneManager->getActiveCamera()->getOrtho()) // !!!
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(ModelviewMatrix.getArray());
    }
    else
    {
        ExtTmpMat_.reset();
        ExtTmpMat_[10] = -1;
        ExtTmpMat_ *= ModelviewMatrix;
        
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(ExtTmpMat_.getArray());
    }
}

void GLFixedFunctionPipeline::setProjectionMatrix(const dim::matrix4f &Matrix)
{
    scene::spProjectionMatrix = Matrix;
    
    if (!isInvertScreen_)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(Matrix.getArray());
    }
    else
    {
        ExtTmpMat_.reset();
        ExtTmpMat_[5] = -1;
        ExtTmpMat_ *= Matrix;
        
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(ExtTmpMat_.getArray());
    }
}

void GLFixedFunctionPipeline::setViewMatrix(const dim::matrix4f &Matrix)
{
    RenderSystem::setViewMatrix(Matrix);
    
    #if 0
    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(getViewMatrix().getArray());
    
    #else
    
    ExtTmpMat_.reset();
    ExtTmpMat_[10] = -1;
    ExtTmpMat_ *= Matrix;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(ExtTmpMat_.getArray());
    
    #endif
    
    /* Update each clipping plane */
    for (std::vector<SClipPlane>::iterator it = ClippingPlanes_.begin(); it != ClippingPlanes_.end(); ++it)
    {
        if (it->Enable)
            glClipPlane(GL_CLIP_PLANE0 + it->Index, it->PlaneEquation);
    }
}

void GLFixedFunctionPipeline::setTextureMatrix(const dim::matrix4f &Matrix, u8 TextureLayer)
{
    scene::spTextureMatrix[TextureLayer] = Matrix;
    
    if (RenderQuery_[RENDERQUERY_MULTI_TEXTURE])
    {
        glActiveTextureARB(GL_TEXTURE0 + TextureLayer);
        glClientActiveTextureARB(GL_TEXTURE0 + TextureLayer);
    }
    
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(Matrix.getArray());
}


/*
 * ======= Private: =======
 */

/* === Drawing 2D - private functions === */

void GLFixedFunctionPipeline::setDrawingMatrix2D()
{
    glLoadMatrixf(Matrix2D_.getArray());
}

void GLFixedFunctionPipeline::setupTextureLayer(const SMeshSurfaceTexture &Tex)
{
    /* Load texture matrix */
    if (Tex.TexMappingGen == MAPGEN_REFLECTION_MAP)
    {
        /* Flip texture matrix on Z-axis to emulate left handed coordinate system */
        GLFixedFunctionPipeline::ExtTmpMat_        = Tex.Matrix;
        GLFixedFunctionPipeline::ExtTmpMat_[ 8]    = -GLFixedFunctionPipeline::ExtTmpMat_[ 8];
        GLFixedFunctionPipeline::ExtTmpMat_[ 9]    = -GLFixedFunctionPipeline::ExtTmpMat_[ 9];
        GLFixedFunctionPipeline::ExtTmpMat_[10]    = -GLFixedFunctionPipeline::ExtTmpMat_[10];
        
        glLoadMatrixf(GLFixedFunctionPipeline::ExtTmpMat_.getArray());
    }
    else
        glLoadMatrixf(Tex.Matrix.getArray());
    
    #if defined(SP_COMPILE_WITH_OPENGL)
    /* Setup texture coordinate generation */
    setupTextureLayerCoordinate(GL_TEXTURE_GEN_S, GL_S, Tex.TexMappingCoords & MAPGEN_S, GLMappingGenList[Tex.TexMappingGen]);
    setupTextureLayerCoordinate(GL_TEXTURE_GEN_T, GL_T, Tex.TexMappingCoords & MAPGEN_T, GLMappingGenList[Tex.TexMappingGen]);
    setupTextureLayerCoordinate(GL_TEXTURE_GEN_R, GL_R, Tex.TexMappingCoords & MAPGEN_R, GLMappingGenList[Tex.TexMappingGen]);
    setupTextureLayerCoordinate(GL_TEXTURE_GEN_Q, GL_Q, Tex.TexMappingCoords & MAPGEN_Q, GLMappingGenList[Tex.TexMappingGen]);
    #endif
    
    /* Textrue environment */
    if (Tex.TexEnvType != TEXENV_MODULATE)
    {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GLTextureEnvList[Tex.TexEnvType]);
    }
    else
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void GLFixedFunctionPipeline::bindTextureList(const std::vector<SMeshSurfaceTexture> &TextureList)
{
    /* Use texture matrix */
    glMatrixMode(GL_TEXTURE);
    
    if (RenderQuery_[RENDERQUERY_MULTI_TEXTURE])
    {
        s32 TextureLayer = 0;
        
        /* Bind each texture */
        for (std::vector<SMeshSurfaceTexture>::const_iterator itTex = TextureList.begin(); itTex != TextureList.end(); ++itTex)
        {
            if (itTex->TextureObject)
            {
                /* Bind current texture and setup texture layer */
                itTex->TextureObject->bind(TextureLayer);
                setupTextureLayer(*itTex);
            }
            ++TextureLayer;
        }
    }
    else if (!TextureList.empty())
    {
        if (TextureList.begin()->TextureObject)
        {
            /* Bind only the first texture and setup texture layer */
            TextureList.begin()->TextureObject->bind(0);
            setupTextureLayer(*TextureList.begin());
        }
    }
}

void GLFixedFunctionPipeline::unbindTextureList(const std::vector<SMeshSurfaceTexture> &TextureList)
{
    if (RenderQuery_[RENDERQUERY_MULTI_TEXTURE])
    {
        s32 TextureLayer = 0;
        
        /* Unbind each texture */
        for (std::vector<SMeshSurfaceTexture>::const_iterator itTex = TextureList.begin(); itTex != TextureList.end(); ++itTex)
        {
            if (itTex->TextureObject)
                itTex->TextureObject->unbind(TextureLayer);
            ++TextureLayer;
        }
    }
    else if (!TextureList.empty() && TextureList.begin()->TextureObject)
        TextureList.begin()->TextureObject->unbind(0);
}

void GLFixedFunctionPipeline::drawTextureFont(
    Font* FontObject, const dim::point2di &Position, const io::stringc &Text, const color &Color)
{
    /* Bind texture */
    FontObject->getTexture()->bind(0);
    
    if (FontObject->getTexture()->getFormatSize() < 4)
        setBlending(BLEND_SRCALPHA, BLEND_ONE);
    
    /* Enable vertex buffer */
    glEnableClientState(GL_VERTEX_ARRAY);
    glClientActiveTextureARB(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    /* Draw each character in the text string */
    std::vector<u32*>* VertexBufferList = (std::vector<u32*>*)FontObject->getID();
    dim::point2df Pos(Position.cast<f32>());
    
    /* Initial transformation */
    glMatrixMode(GL_PROJECTION);
    setDrawingMatrix2D();
    
    glMatrixMode(GL_MODELVIEW);
    
    glColor4ub(Color.Red, Color.Green, Color.Blue, Color.Alpha);
    
    /* Draw each character */
    for (u32 i = 0; i < Text.size(); ++i)
    {
        const u32 CurChar = static_cast<u32>(static_cast<u8>(Text[i]) - ' ');
        
        if (CurChar >= FontObject->CharWidthList_.size())
            continue;
        
        u32* BufferID = (*VertexBufferList)[CurChar];
        
        if (!BufferID)
            continue;
        
        /* Setup transformation */
        glLoadIdentity();
        glTranslatef(Pos.X, Pos.Y, 0);
        
        Pos.X += (1.0f + static_cast<f32>(FontObject->CharWidthList_[CurChar]));
        
        /* Bind vertex buffer */
        if (RenderQuery_[RENDERQUERY_HARDWARE_MESHBUFFER])
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, *BufferID);
        
        const c8* vboPointerOffset = 0;//(RenderQuery_[RENDERQUERY_MESH_BUFFERS] ? 0 : (const c8*)MeshBuffer->getVertexBuffer().getArray());
        
        glVertexPointer(2, GL_INT, 16, vboPointerOffset);
        glTexCoordPointer(2, GL_FLOAT, 16, vboPointerOffset + 8);
        
        /* Draw current character */
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    
    /* Disable vertex buffer */
    glDisableClientState(GL_VERTEX_ARRAY);
    glClientActiveTextureARB(GL_TEXTURE0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    /* Unbind texture and vertex buffer */
    FontObject->getTexture()->unbind(0);
    
    if (FontObject->getTexture()->getFormatSize() < 4)
        setDefaultAlphaBlending();
    
    if (RenderQuery_[RENDERQUERY_HARDWARE_MESHBUFFER])
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}


} // /namespace video

} // /namespace sp


#endif



// ================================================================================
