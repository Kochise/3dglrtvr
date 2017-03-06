/*
 * OpenGL texture base file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "RenderSystem/OpenGL/spOpenGLTextureBase.hpp"

#if defined(SP_COMPILE_WITH_OPENGL) || defined(SP_COMPILE_WITH_OPENGLES1) || defined(SP_COMPILE_WITH_OPENGLES2)


#include "Platform/spSoftPixelDeviceOS.hpp"
#include "RenderSystem/OpenGL/spOpenGLPipelineBase.hpp"
#include "RenderSystem/OpenGL/spOpenGLFunctionsARB.hpp"
#include "RenderSystem/OpenGLES/spOpenGLESFunctionsARB.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;

namespace video
{


/*
 * Internal members
 */

extern s32 STDPixelFormatDataSize[];

GLenum GLTexInternalFormatListUByte8[] = {
    #if 1
    GL_ALPHA, GL_ALPHA, GL_ALPHA, GL_ALPHA, GL_ALPHA, GL_ALPHA, GL_ALPHA,
    GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGB, GL_ALPHA, GL_RGBA, GL_RGBA
    #else
    GL_ALPHA8, GL_LUMINANCE8, GL_LUMINANCE8_ALPHA8, GL_RGB8, GL_RGB8, GL_DEPTH_COMPONENT16, GL_RGBA8, GL_RGBA8
    #endif
};

#if defined(SP_COMPILE_WITH_OPENGL)

GLenum GLTextureWrapModes[] = {
    GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE
};

#elif defined(SP_COMPILE_WITH_OPENGLES1) || defined(SP_COMPILE_WITH_OPENGLES2)

GLenum GLTextureWrapModes[] = {
    GL_REPEAT, GL_REPEAT, GL_CLAMP_TO_EDGE
};

#endif


/*
 * GLTextureBase class
 */

GLTextureBase::GLTextureBase()
    : Texture(), GLFormat_(0), GLInternalFormat_(0), GLDimension_(0)
{
    createHWTexture();
}
GLTextureBase::GLTextureBase(const STextureCreationFlags &CreationFlags)
    : Texture(CreationFlags), GLFormat_(0), GLInternalFormat_(0), GLDimension_(0)
{
    createHWTexture();
}
GLTextureBase::~GLTextureBase()
{
    deleteHWTexture();
}

bool GLTextureBase::valid() const
{
    return OrigID_ && glIsTexture(getTexID());
}

void GLTextureBase::setFilter(const ETextureFilters Filter)
{
    if (MagFilter_ != Filter || MinFilter_ != Filter)
    {
        MagFilter_ = MinFilter_ = Filter;
        updateImageBuffer();
    }
}
void GLTextureBase::setFilter(const ETextureFilters MagFilter, const ETextureFilters MinFilter)
{
    if (MagFilter_ != MagFilter || MinFilter_ != MinFilter)
    {
        MagFilter_ = MagFilter;
        MinFilter_ = MinFilter;
        updateImageBuffer();
    }
}
void GLTextureBase::setMagFilter(const ETextureFilters Filter)
{
    if (MagFilter_ != Filter)
    {
        MagFilter_ = Filter;
        updateImageBuffer();
    }
}
void GLTextureBase::setMinFilter(const ETextureFilters Filter)
{
    if (MinFilter_ != Filter)
    {
        MinFilter_ = Filter;
        updateImageBuffer();
    }
}

void GLTextureBase::setMipMapFilter(const ETextureMipMapFilters MipMapFilter)
{
    if (MipMapFilter_ != MipMapFilter)
    {
        MipMapFilter_ = MipMapFilter;
        updateImageBuffer();
    }
}

void GLTextureBase::setWrapMode(const ETextureWrapModes Wrap)
{
    if (WrapMode_.X != Wrap || WrapMode_.Y != Wrap || WrapMode_.Z != Wrap)
    {
        WrapMode_.X = WrapMode_.Y = WrapMode_.Z = Wrap;
        updateImageBuffer();
    }
}
void GLTextureBase::setWrapMode(
    const ETextureWrapModes WrapU, const ETextureWrapModes WrapV, const ETextureWrapModes WrapW)
{
    if (WrapMode_.X != WrapU || WrapMode_.Y != WrapV || WrapMode_.Z != WrapW)
    {
        WrapMode_.X = WrapU;
        WrapMode_.Y = WrapV;
        WrapMode_.Z = WrapW;
        updateImageBuffer();
    }
}

void GLTextureBase::generateMipMap()
{
    if (MipMaps_)
    {
        glBindTexture(GLDimension_, getTexID());
        glGenerateMipmapEXT(GLDimension_);
    }
}

void GLTextureBase::bind(s32 Level) const
{
    /* Setup texture layers */
    if (__spVideoDriver->RenderQuery_[RenderSystem::RENDERQUERY_MULTI_TEXTURE])
        glActiveTextureARB(GL_TEXTURE0 + Level);
    
    /* Bind and enable the texture */
    glBindTexture(GLDimension_, *static_cast<GLuint*>(ID_));
    
    if (__spVideoDriver->getRendererType() != RENDERER_OPENGLES2)
        glEnable(GLDimension_);
}

void GLTextureBase::unbind(s32 Level) const
{
    /* Setup texture layers */
    if (__spVideoDriver->RenderQuery_[RenderSystem::RENDERQUERY_MULTI_TEXTURE])
        glActiveTextureARB(GL_TEXTURE0 + Level);
    
    /* Unbind and disable the texture */
    glBindTexture(GLDimension_, 0);
    
    if (__spVideoDriver->getRendererType() != RENDERER_OPENGLES2)
        glDisable(GLDimension_);
}


/*
 * ======= Private: =======
 */

void GLTextureBase::createHWTexture()
{
    /* Delete old OpenGL hardware texture */
    deleteHWTexture();
    
    /* Allocate memory for the OpenGL texture name */
    void* PrevID = OrigID_;
    
    /* Update current ID */
    OrigID_ = new GLuint;
    
    if (ID_ == PrevID)
        ID_ = OrigID_;
    
    /* Create new OpenGL hardware texture */
    glGenTextures(1, getTexPtrID());
}
void GLTextureBase::deleteHWTexture()
{
    if (OrigID_)
    {
        /* Delete OpenGL hardware texture */
        if (glIsTexture(getTexID()))
            glDeleteTextures(1, getTexPtrID());
        
        /* Delete memory of the OpenGL texture name */
        delete static_cast<GLuint*>(OrigID_);
        
        if (ID_ == OrigID_)
            ID_ = 0;
        OrigID_ = 0;
    }
}

void GLTextureBase::setupTextureAttributes()
{
    /* Wrap modes (reapeat, mirror, clamp) */
    glTexParameteri(GLDimension_, GL_TEXTURE_WRAP_S, GLTextureWrapModes[WrapMode_.X]);
    glTexParameteri(GLDimension_, GL_TEXTURE_WRAP_T, GLTextureWrapModes[WrapMode_.Y]);
    
    #if defined(SP_COMPILE_WITH_OPENGL)
    glTexParameteri(GLDimension_, GL_TEXTURE_WRAP_R, GLTextureWrapModes[WrapMode_.Z]);
    #endif
    
    /* MIP-mapping */
    #if defined(SP_COMPILE_WITH_OPENGL) || defined(SP_COMPILE_WITH_OPENGLES1)
    #   if defined(SP_COMPILE_WITH_OPENGLES1)
    if (__spVideoDriver->getRendererType() == RENDERER_OPENGLES1)
    #   endif
        glTexParameteri(GLDimension_, GL_GENERATE_MIPMAP, MipMaps_ ? GL_TRUE : GL_FALSE);
    #endif
    
    /* Anisotropy */
    if (MipMapFilter_ == FILTER_ANISOTROPIC)
        glTexParameteri(GLDimension_, GL_TEXTURE_MAX_ANISOTROPY_EXT, AnisotropicSamples_);
    
    /* Magnification filter */
    glTexParameteri(GLDimension_, GL_TEXTURE_MAG_FILTER, MagFilter_ == FILTER_SMOOTH ? GL_LINEAR : GL_NEAREST);
    
    /* Minification filter */
    if (MinFilter_ == FILTER_SMOOTH)
    {
        if (MipMaps_)
        {
            glTexParameteri(
                GLDimension_, GL_TEXTURE_MIN_FILTER,
                MipMapFilter_ == FILTER_BILINEAR ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR
            );
        }
        else
            glTexParameteri(GLDimension_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else if (MinFilter_ == FILTER_LINEAR)
    {
        if (MipMaps_)
        {
            glTexParameteri(
                GLDimension_, GL_TEXTURE_MIN_FILTER,
                MipMapFilter_ == FILTER_BILINEAR ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_LINEAR
            );
        }
        else
            glTexParameteri(GLDimension_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
}

void GLTextureBase::updateTextureImage()
{
    GLenum GLFormat, GLInternalFormat;
    setupTextureFormats(Format_, HWFormat_, GLFormat, GLInternalFormat);
    
    dim::vector3di Size(Size_.Width, Size_.Height, Depth_);
    
    if (Size.Z > 1)
        Size.Y /= Size.Z;
    
    #if 0
    if (MipMaps_)
        updateTextureImageMipmaps(Size, STDPixelFormatDataSize[Format_], GLInternalFormat, GLFormat, ImageBuffer_);
    else
    #endif
        updateTextureImageNormal(Size, STDPixelFormatDataSize[Format_], GLInternalFormat, GLFormat, ImageBuffer_, 0);
}

void GLTextureBase::updateTextureImageMipmaps(
    dim::vector3di Size, s32 FormatSize, GLenum GLInternalFormat, GLenum GLFormat)
{
    s32 Width   = Size.X;
    s32 Height  = Size.Y;
    
    /* Temporary memories */
    s32 i                       = 0;
    u8* pData                   = 0;
    const s32 ImageBufferSize   = Width*Height*4;//FormatSize;
    
    /* Allocate new image data memory */
    pData = new u8[ImageBufferSize];
    
    /* Copy the image data */
    #if 1
    
    switch (FormatSize)
    {
        case 1:
        {
            for (s32 i = 0; i < Width*Height; ++i)
            {
                pData[i*4+0] = ImageBuffer_[i];
                pData[i*4+1] = ImageBuffer_[i];
                pData[i*4+2] = ImageBuffer_[i];
                pData[i*4+3] = 255;
            }
        }
        break;
        
        case 2:
        {
            for (s32 i = 0; i < Width*Height; ++i)
            {
                pData[i*4+0] = ImageBuffer_[(i << 1)+0];
                pData[i*4+1] = ImageBuffer_[(i << 1)+0];
                pData[i*4+2] = ImageBuffer_[(i << 1)+0];
                pData[i*4+3] = ImageBuffer_[(i << 1)+1];
            }
        }
        break;
        
        case 3:
        {
            for (s32 i = 0; i < Width*Height; ++i)
            {
                pData[i*4+0] = ImageBuffer_[i*3+0];
                pData[i*4+1] = ImageBuffer_[i*3+1];
                pData[i*4+2] = ImageBuffer_[i*3+2];
                pData[i*4+3] = 255;
            }
        }
        break;
        
        case 4:
        {
            for (s32 i = 0; i < ImageBufferSize; ++i)
                pData[i] = ImageBuffer_[i];
        }
        break;
    }
    
    FormatSize  = 4;
    GLFormat    = GL_RGBA;
    
    #else
    
    memcpy(pData, ImageBuffer_, ImageBufferSize);
    
    #endif
    
    /* Get the optimized size & scale if the input size is not correct */
    dim::size2di Size2D(Size.X, Size.Y);
    ImageConverter::checkImageSize(Size2D);
    Size.X = Size2D.Width, Size.Y = Size2D.Height;
    
    /* Get the maximal size */
    GLint MaxSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxSize);
    
    /* Check the size of maxima */
    if (Size.X > MaxSize) Size.X = MaxSize;
    if (Size.Y > MaxSize) Size.Y = MaxSize;
    
    /* Clamp the image data size, must be power of to (2^n) */
    if (Size.X != Width || Size.Y != Height)
        ImageConverter::scaleImage(pData, Width, Height, Size.X, Size.Y, FormatSize);
    
    Size.Y /= Size.Z;
    
    /* Loop for all mipmap levels */
    while (1)
    {
        /* Build a new mipmap level */
        updateTextureImageNormal(Size, FormatSize, GLInternalFormat, GLFormat, pData, i++);
        
        /* Check if the last mipmap level has been achieved */
        if (Size.X == 1 && Size.Y == 1 && Size.Z == 1)
            break;
        
        /* Scale the image data to the half size */
        ImageConverter::halveImage(pData, Size.X, Size.Y * Size.Z, FormatSize);
        
        /* Half the size */
        if (Size.X > 1) Size.X >>= 1;
        if (Size.Y > 1) Size.Y >>= 1;
        if (Size.Z > 1) Size.Z >>= 1;
    }
    
    /* Delete temporary memory */
    MemoryManager::deleteBuffer(pData);
}


} // /namespace video

} // /namespace sp


#endif



// ================================================================================
