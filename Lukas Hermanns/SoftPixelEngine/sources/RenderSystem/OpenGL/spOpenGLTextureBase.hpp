/*
 * OpenGL texture base header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_OPENGL_TEXTUREBASE_H__
#define __SP_OPENGL_TEXTUREBASE_H__


#include "Base/spStandard.hpp"

#if defined(SP_COMPILE_WITH_OPENGL) || defined(SP_COMPILE_WITH_OPENGLES1) || defined(SP_COMPILE_WITH_OPENGLES2)


#include "RenderSystem/spTextureBase.hpp"
#include "RenderSystem/OpenGL/spOpenGLCoreHeader.hpp"


namespace sp
{
namespace video
{


class SP_EXPORT GLTextureBase : public Texture
{
    
    public:
        
        virtual ~GLTextureBase();
        
        /* Functions */
        
        bool valid() const;
        
        void setFilter(const ETextureFilters Filter);
        void setFilter(const ETextureFilters MagFilter, const ETextureFilters MinFilter);
        void setMagFilter(const ETextureFilters Filter);
        void setMinFilter(const ETextureFilters Filter);
        
        void setMipMapFilter(const ETextureMipMapFilters MipMapFilter);
        
        void setWrapMode(const ETextureWrapModes Wrap);
        void setWrapMode(
            const ETextureWrapModes WrapU, const ETextureWrapModes WrapV, const ETextureWrapModes WrapW = TEXWRAP_REPEAT
        );
        
        void generateMipMap();
        
        void bind(s32 Level = 0) const;
        void unbind(s32 Level = 0) const;
        
    protected:
        
        friend class GLProgrammableFunctionPipeline;
        friend class GLFramebufferObject;
        
        /* Functions */
        
        GLTextureBase();
        GLTextureBase(const STextureCreationFlags &CreationFlags);
        
        void createHWTexture();
        void deleteHWTexture();
        
        virtual void setupTextureAttributes();
        virtual void setupTextureFormats(
            const EPixelFormats Format, const EHWTextureFormats HWFormat, GLenum &GLFormat, GLenum &GLInternalFormat
        ) = 0;
        
        virtual void updateTextureImage();
        virtual void updateTextureImageNormal(
            dim::vector3di Size, s32 FormatSize, GLenum GLInternalFormat, GLenum GLFormat,
            const u8* ImageBuffer, s32 Level
        ) = 0;
        virtual void updateTextureImageMipmaps(
            dim::vector3di Size, s32 FormatSize, GLenum GLInternalFormat, GLenum GLFormat
        );
        
        /* Inline functions */
        
        inline GLuint getTexID() const
        {
            return *static_cast<GLuint*>(OrigID_);
        }
        inline GLuint* getTexPtrID() const
        {
            return static_cast<GLuint*>(OrigID_);
        }
        
        /* Members */
        
        GLenum GLFormat_, GLInternalFormat_, GLDimension_;
        
};


} // /namespace video

} // /namespace sp


#endif

#endif



// ================================================================================
