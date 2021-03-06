/*
 * OpenGL base renderer header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_OPENGL_BASEPIPELINE_H__
#define __SP_OPENGL_BASEPIPELINE_H__


#include "Base/spStandard.hpp"

#if defined(SP_COMPILE_WITH_OPENGL) || defined(SP_COMPILE_WITH_OPENGLES1) || defined(SP_COMPILE_WITH_OPENGLES2)

#include "RenderSystem/spRenderSystem.hpp"
#include "RenderSystem/OpenGL/spOpenGLCoreHeader.hpp"


namespace sp
{
namespace video
{


//! OpenGL base render system. This is the parent class of all OpenGL and OpenGL|ES renderers.
class SP_EXPORT GLBasePipeline : virtual public RenderSystem
{
    
    public:
        
        virtual ~GLBasePipeline();
        
        /* === Render system information === */
        
        virtual io::stringc getRenderer()       const;
        virtual io::stringc getVersion()        const;
        virtual io::stringc getVendor()         const;
        virtual io::stringc getShaderVersion()  const;
        
        virtual s32 getMaxAnisotropicFilter()   const;
        
        virtual bool queryExtensionSupport(const io::stringc &TargetExtension) const;
        
        virtual dim::EMatrixProjectionTypes getProjectionMatrixType() const;
        
        /* === Video buffer control functions === */
        
        virtual void clearBuffers(const s32 ClearFlags = BUFFER_COLOR | BUFFER_DEPTH);
        
        /* === Configuration functions === */
        
        virtual void setFrontFace(bool isFrontFace);
        
        virtual void setClearColor(const color &Color);
        virtual void setColorMask(bool isRed, bool isGreen, bool isBlue, bool isAlpha = true);
        virtual void setDepthMask(bool isDepth);
        
        /* === Hardware mesh buffers === */
        
        virtual void createVertexBuffer(void* &BufferID);
        virtual void createIndexBuffer(void* &BufferID);
        
        virtual void deleteVertexBuffer(void* &BufferID);
        virtual void deleteIndexBuffer(void* &BufferID);
        
        virtual void updateVertexBuffer(
            void* BufferID, const dim::UniversalBuffer &BufferData, const VertexFormat* Format, const EMeshBufferUsage Usage
        );
        virtual void updateIndexBuffer(
            void* BufferID, const dim::UniversalBuffer &BufferData, const IndexFormat* Format, const EMeshBufferUsage Usage
        );
        
        virtual void updateVertexBufferElement(void* BufferID, const dim::UniversalBuffer &BufferData, u32 Index);
        virtual void updateIndexBufferElement(void* BufferID, const dim::UniversalBuffer &BufferData, u32 Index);
        
        /* === Render states === */
        
        virtual void setDefaultAlphaBlending();
        virtual void enableBlending();
        virtual void disableBlending();
        
        /* === Stack operation functions === */
        
        virtual void pushViewport();
        virtual void popViewport();
        
        /* === Simple drawing functions === */
        
        virtual void setBlending(const EBlendingTypes SourceBlend, const EBlendingTypes DestBlend);
        virtual void setClipping(bool Enable, const dim::point2di &Position, const dim::size2di &Size);
        virtual void setViewport(const dim::point2di &Position, const dim::size2di &Size);
        
        virtual void setLineSize(s32 Size = 1);
        
        /* === Texture loading and creating === */
        
        virtual Texture* loadTexture(ImageLoader* Loader);
        virtual Texture* copyTexture(const Texture* Tex);
        virtual Texture* createTexture(
            const dim::size2di &Dimension, const EPixelFormats Format, const u8* ImageBuffer, const io::stringc &Filename = ""
        );
        
        /* === Matrix controll === */
        
        virtual void setWorldMatrix(const dim::matrix4f &Matrix);
        
        virtual dim::matrix4f getViewMatrix() const;
        
    protected:
        
        friend class GLTextureBase;
        friend class GLFramebufferObject;
        friend class OpenGLES2Texture;
        
        /* === Functions === */
        
        GLBasePipeline();
        
        static GLenum getGlTexDimension(const ETextureDimensions DimType);
        
        virtual void setInvertScreen(bool Enable);
        
        virtual Font* createTextureFont(
            video::Texture* FontTexture, const std::vector<dim::rect2di> &ClipList, s32 FontHeight
        );
        
        /* === Inline functions === */
        
        static inline void setGlRenderState(GLenum Mode, bool Enable)
        {
            if (Enable)
                glEnable(Mode);
            else
                glDisable(Mode);
        }
        static inline bool getGlRenderState(GLenum Mode)
        {
            GLboolean Enable;
            glGetBooleanv(Mode, &Enable);
            return Enable;
        }
        
        inline bool queryGLVersion(s32 Major, s32 Minor) const
        {
            return GLMajorVersion_ > Major || GLMinorVersion_ >= Minor;
        }
        
        /* === Members === */
        
        s32 GLMajorVersion_, GLMinorVersion_;
        s32 MultiTextureCount_;
        
        bool isInvertScreen_;
        
};


} // /namespace video

} // /namespace sp


#endif

#endif



// ================================================================================
