/*
 * OpenGL texture header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "RenderSystem/OpenGL/spOpenGLTexture.hpp"

#if defined(SP_COMPILE_WITH_OPENGL)


#include "Base/spImageManagement.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"
#include "RenderSystem/OpenGL/spOpenGLFunctionsARB.hpp"
#include "RenderSystem/OpenGL/spOpenGLRenderSystem.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;

namespace video
{


/*
 * Internal members
 */

const GLenum GLTexFormatList[] = {
    #if 1
    GL_COLOR_INDEX, GL_LUMINANCE, GL_INTENSITY, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA,
    GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_BGR, GL_DEPTH_COMPONENT, GL_RGBA, GL_BGRA
    #else
    GL_ALPHA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_DEPTH_COMPONENT
    #endif
};

const GLenum GLTexInternalFormatListFloat16[] = {
    #if 1
    1, GL_LUMINANCE16F_ARB, GL_INTENSITY8, 1, 1, 1, GL_ALPHA8, GL_LUMINANCE16F_ARB, GL_LUMINANCE8_ALPHA8,
    GL_RGB16F_ARB, GL_RGB16F_ARB, GL_DEPTH_COMPONENT24, GL_RGBA16F_ARB, GL_RGBA16F_ARB
    #else
    GL_R16, GL_R16, GL_RG16, GL_RGB16F_ARB, GL_RGB16F_ARB, GL_RGBA16F_ARB, GL_RGBA16F_ARB, GL_DEPTH_COMPONENT24
    #endif
};

const GLenum GLTexInternalFormatListFloat32[] = {
    #if 1
    1, GL_LUMINANCE32F_ARB, GL_INTENSITY8, 1, 1, 1, GL_ALPHA8, GL_LUMINANCE32F_ARB, GL_LUMINANCE8_ALPHA8,
    GL_RGB32F_ARB, GL_RGB32F_ARB, GL_DEPTH_COMPONENT32F, GL_RGBA32F_ARB, GL_RGBA32F_ARB
    #else
    GL_R32F, GL_R32F, GL_RG32F, GL_RGB32F_ARB, GL_RGB32F_ARB, GL_RGBA32F_ARB, GL_RGBA32F_ARB, GL_DEPTH_COMPONENT32F
    #endif
};

extern GLenum GLTexInternalFormatListUByte8[];


/*
 * OpenGLTexture class
 */

OpenGLTexture::OpenGLTexture()
    : GLTextureBase(), GLFramebufferObject()
{
}
OpenGLTexture::OpenGLTexture(const STextureCreationFlags &CreationFlags)
    : GLTextureBase(CreationFlags), GLFramebufferObject()
{
    updateFormat();
    
    if (CreationFlags.ImageBuffer)
        Texture::updateImageBuffer(CreationFlags.ImageBuffer);
}
OpenGLTexture::~OpenGLTexture()
{
    deleteFramebuffer();
}

void OpenGLTexture::setColorIntensity(f32 Red, f32 Green, f32 Blue)
{
    /* Check if the image is not empty */
    if (!ImageBuffer_)
        return;
    
    /* Use this texture */
    glBindTexture(GLDimension_, getTexID());
    
    /* Settings */
    glPixelTransferf(GL_RED_SCALE, Red);
    glPixelTransferf(GL_GREEN_SCALE, Green);
    glPixelTransferf(GL_BLUE_SCALE, Blue);
    
    /* Recreate the image */
    updateImageBuffer();
    
    /* Back setting */
    glPixelTransferf(GL_RED_SCALE, 1.0f);
    glPixelTransferf(GL_GREEN_SCALE, 1.0f);
    glPixelTransferf(GL_BLUE_SCALE, 1.0f);
}

void OpenGLTexture::setCubeMapFace(const ECubeMapDirections Face)
{
    CubeMapFace_ = Face;
    
    if (isRenderTarget_ && DimensionType_ == TEXTURE_CUBEMAP)
        setupCubeMapFace(getTexID(), Format_, CubeMapFace_);
}


/* === Image buffer === */

void OpenGLTexture::shareImageBuffer()
{
    /* Bind this texture */
    glBindTexture(GLDimension_, getTexID());
    
    /* Get the dimensions */
    GLint GLInternalFormat;
    s32 Width, Height, FormatSize;
    
    glGetTexLevelParameteriv(GLDimension_, 0, GL_TEXTURE_WIDTH, &Width);
    glGetTexLevelParameteriv(GLDimension_, 0, GL_TEXTURE_HEIGHT, &Height);
    glGetTexLevelParameteriv(GLDimension_, 0, GL_TEXTURE_INTERNAL_FORMAT, &GLInternalFormat);
    
    /* Determine texture format */
    switch (GLInternalFormat)
    {
        case GL_LUMINANCE8:
            GLFormat_   = GL_LUMINANCE;
            Format_     = PIXELFORMAT_GRAY;
            FormatSize  = 1;
            break;
        case GL_LUMINANCE8_ALPHA8:
            GLFormat_   = GL_LUMINANCE_ALPHA;
            Format_     = PIXELFORMAT_GRAYALPHA;
            FormatSize  = 2;
            break;
        case GL_RGB8:
        case GL_RGB16F_ARB:
        case GL_RGB32F_ARB:
            GLFormat_   = GL_RGB;
            Format_     = PIXELFORMAT_RGB;
            FormatSize  = 3;
            break;
        case GL_RGBA8:
        case GL_RGBA16F_ARB:
        case GL_RGBA32F_ARB:
            GLFormat_   = GL_RGBA;
            Format_     = PIXELFORMAT_RGBA;
            FormatSize  = 4;
            break;
        default:
            io::Log::error("Unexpected internal texture format"); return;
    }
    
    /* Check if the dimensions has changed */
    if (Width != Size_.Width || Height != Size_.Height || FormatSize != FormatSize_)
    {
        /* Update the dimensions */
        Size_.Width     = Width;
        Size_.Height    = Height;
        FormatSize_     = FormatSize;
        
        /* Delete the old image buffer */
        MemoryManager::deleteBuffer(ImageBuffer_);
        
        /* Allocate new memory */
        ImageBuffer_ = new u8[ Size_.Width * Size_.Height * FormatSize_ ];
    }
    
    /* Update renderer texture format */
    updateFormat();
    
    /* Get the image data */
    if (DimensionType_ == TEXTURE_CUBEMAP)
    {
        const s32 OffsetSize = Size_.Width*(Size_.Height/Depth_)*FormatSize_;
        glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + CubeMapFace_, 0, GLFormat_, GL_UNSIGNED_BYTE, ImageBuffer_ + OffsetSize * (s32)CubeMapFace_);
    }
    else
        glGetTexImage(GLDimension_, 0, GLFormat_, GL_UNSIGNED_BYTE, ImageBuffer_);
}

void OpenGLTexture::updateImageBuffer()
{
    /* Update dimension and format */
    const bool ReCreateTexture = (GLDimension_ != GLBasePipeline::getGlTexDimension(DimensionType_));
    
    updateFormat();
    
    if (ReCreateTexture)
        createHWTexture();
    
    /* Clear the image data */
    glBindTexture(GLDimension_, getTexID());
    
    /* Update format and texture image */
    setupTextureAttributes();
    updateTextureImage();
    
    /* Update render target (create/delete) */
    if (__spVideoDriver->RenderQuery_[RenderSystem::RENDERQUERY_RENDERTARGET])
    {
        deleteFramebuffer();
        
        if (isRenderTarget_)
        {
            #ifdef SP_COMPILE_WITH_OPENGL
            if (MultiSamples_ > 0 && __spVideoDriver->RenderQuery_[RenderSystem::RENDERQUERY_MULTISAMPLE_RENDERTARGET])
            {
                createFramebufferMultisample(
                    getTexID(), dim::size2di(Size_.Width, Size_.Height / Depth_),
                    GLDimension_, GLInternalFormat_, MultiSamples_,
                    MultiRenderTargetList_, Format_, DimensionType_, CubeMapFace_
                );
            }
            else
            #endif
            {
                createFramebuffer(
                    getTexID(), dim::size2di(Size_.Width, Size_.Height / Depth_),
                    GLDimension_, Format_, DimensionType_, CubeMapFace_
                );
            }
        }
    }
}


/*
 * ======= Private: =======
 */

void OpenGLTexture::updateFormat()
{
    /* Update OpenGL texture formats */
    setupTextureFormats(Format_, HWFormat_, GLFormat_, GLInternalFormat_);
    
    /* Update format size */
    updateFormatSize();
    
    /* Determine which dimension is used */
    GLDimension_ = GLBasePipeline::getGlTexDimension(DimensionType_);
}

void OpenGLTexture::updateMultiRenderTargets()
{
    updateImageBuffer();
    updateMultiFramebuffer(Size_, Format_, MultiRenderTargetList_);
}
void OpenGLTexture::updateFramebufferMultisample()
{
    blitFramebufferMultisample(Size_, MultiRenderTargetList_.size());
}

void OpenGLTexture::setupTextureFormats(
    const EPixelFormats Format, const EHWTextureFormats HWFormat, GLenum &GLFormat, GLenum &GLInternalFormat)
{
    if (Format >= PIXELFORMAT_INDEX && Format <= PIXELFORMAT_BGRA)
    {
        GLFormat = GLTexFormatList[Format];
        
        switch (HWFormat)
        {
            case HWTEXFORMAT_UBYTE8:
                GLInternalFormat = GLTexInternalFormatListUByte8[Format]; break;
            case HWTEXFORMAT_FLOAT16:
                GLInternalFormat = GLTexInternalFormatListFloat16[Format]; break;
            case HWTEXFORMAT_FLOAT32:
                GLInternalFormat = GLTexInternalFormatListFloat32[Format]; break;
        }
    }
}

void OpenGLTexture::updateTextureImageNormal(
    dim::vector3di Size, s32 FormatSize, GLenum GLInternalFormat, GLenum GLFormat, const u8* ImageBuffer, s32 Level)
{
    switch (DimensionType_)
    {
        case TEXTURE_1D:
        {
            glTexImage1D(
                GL_TEXTURE_1D, Level, GLInternalFormat, Size.X,
                0, GLFormat, GL_UNSIGNED_BYTE, ImageBuffer
            );
        }
        break;
        
        case TEXTURE_2D:
        {
            glTexImage2D(
                GL_TEXTURE_2D, Level, GLInternalFormat, Size.X, Size.Y,
                0, GLFormat, GL_UNSIGNED_BYTE, ImageBuffer
            );
        }
        break;
        
        case TEXTURE_3D:
        {
            glTexImage3DEXT(
                GL_TEXTURE_3D, Level, GLInternalFormat, Size.X, Size.Y, Size.Z,
                0, GLFormat, GL_UNSIGNED_BYTE, ImageBuffer
            );
        }
        break;
        
        case TEXTURE_CUBEMAP:
        {
            const s32 OffsetSize = Size.X*Size.Y*FormatSize;
            
            for (s32 i = 0; i < 6; ++i)
            {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, Level, GLInternalFormat, Size.X, Size.Y,
                    0, GLFormat, GL_UNSIGNED_BYTE, (ImageBuffer ? (const u8*)(ImageBuffer + i*OffsetSize) : 0)
                );
            }
        }
        break;
    }
}


} // /namespace video

} // /namespace sp


#endif



// ================================================================================
