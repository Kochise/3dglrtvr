/*
 * OpenGL framebuffer object header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_OPENGL_FRAMEBUFFEROBJECT_H__
#define __SP_OPENGL_FRAMEBUFFEROBJECT_H__


#include "Base/spStandard.hpp"

#if defined(SP_COMPILE_WITH_OPENGL) || defined(SP_COMPILE_WITH_OPENGLES2)


#include "RenderSystem/spTextureBase.hpp"
#include "RenderSystem/OpenGL/spOpenGLCoreHeader.hpp"


namespace sp
{
namespace video
{


class SP_EXPORT GLFramebufferObject
{
    
    public:
        
        virtual ~GLFramebufferObject();
        
    protected:
        
        /* === Functions === */
        
        GLFramebufferObject();
        
        void createFramebuffer(
            GLuint TexID, const dim::size2di &Size, GLenum GLDimension,
            const EPixelFormats Format, const ETextureDimensions DimensionType, const ECubeMapDirections CubeMapFace
        );
        
        #ifdef SP_COMPILE_WITH_OPENGL
        
        void createFramebufferMultisample(
            GLuint TexID, const dim::size2di &Size, GLenum GLDimension, GLenum GLInternalFormat,
            s32 Samples, const std::vector<Texture*> &MultiRenderTargets, const EPixelFormats Format,
            const ETextureDimensions DimensionType, const ECubeMapDirections CubeMapFace
        );
        
        void blitFramebufferMultisample(const dim::size2di &Size, u32 RTCount);
        
        void updateMultiFramebuffer(
            const dim::size2di &Size, const EPixelFormats Format, const std::vector<Texture*> &MultiRenderTargets
        );
        
        #endif
        
        void setupCubeMapFace(
            GLuint TexID, const EPixelFormats Format, const ECubeMapDirections CubeMapFace
        );
        
        void deleteFramebuffer();
        
    private:
        
        friend class GLProgrammableFunctionPipeline;
        
        /* === Functions === */
        
        void releaseFramebuffer(GLuint &BufferID);
        void releaseRenderbuffer(GLuint &BufferID);
        
        void attachFramebufferTexture(
            GLuint TexID, GLenum GLDimension, const EPixelFormats Format,
            const ETextureDimensions DimensionType, const ECubeMapDirections CubeMapFace
        );
        
        bool checkFrameBufferErrors();
        
        /* === Inline functions === */
        
        inline bool isMultiSampled() const
        {
            return MSFrameBufferID_ != 0;
        }
        
        /* === Members === */
        
        GLuint FrameBufferID_, DepthBufferID_;
        
        GLuint MSFrameBufferID_;
        std::vector<GLuint> MSColorBufferID_;
        
};


} // /namespace video

} // /namespace sp


#endif

#endif



// ================================================================================
