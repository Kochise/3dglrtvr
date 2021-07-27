/*
 * OpenGL texture header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_VIDEODRIVER_OPENGLTEXTURE_H__
#define __SP_VIDEODRIVER_OPENGLTEXTURE_H__


#include "Base/spStandard.hpp"

#if defined(SP_COMPILE_WITH_OPENGL)


#include "RenderSystem/OpenGL/spOpenGLTextureBase.hpp"
#include "RenderSystem/OpenGL/spOpenGLFramebufferObject.hpp"


namespace sp
{
namespace video
{


class OpenGLDriver;

class SP_EXPORT OpenGLTexture : public GLTextureBase, public GLFramebufferObject
{
    
    public:
        
        OpenGLTexture();
        OpenGLTexture(const STextureCreationFlags &CreationFlags);
        ~OpenGLTexture();
        
        /* Extra option functions */
        
        void setColorIntensity(f32 Red, f32 Green, f32 Blue);
        
        /* Dimension */
        
        void setCubeMapFace(const ECubeMapDirections Face);
        
        /* Functions for updating image buffer */
        
        void shareImageBuffer();
        void updateImageBuffer();
        
    private:
        
        friend class GLProgrammableFunctionPipeline;
        friend class GLFramebufferObject;
        
        /* Private functions */
        
        void updateFormat();
        void updateMultiRenderTargets();
        void updateFramebufferMultisample();
        
        void setupTextureFormats(
            const EPixelFormats Format, const EHWTextureFormats HWFormat, GLenum &GLFormat, GLenum &GLInternalFormat
        );
        
        void updateTextureImageNormal(
            dim::vector3di Size, s32 FormatSize, GLenum GLInternalFormat, GLenum GLFormat,
            const u8* ImageBuffer, s32 Level
        );
        
};


} // /namespace video

} // /namespace sp


#endif

#endif



// ================================================================================
