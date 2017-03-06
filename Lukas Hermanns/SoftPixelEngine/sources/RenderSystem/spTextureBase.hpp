/*
 * Textrue base header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_RENDERSYSTEM_TEXTURE_H__
#define __SP_RENDERSYSTEM_TEXTURE_H__


#include "Base/spStandard.hpp"
#include "Base/spDimension.hpp"
#include "Base/spInputOutputString.hpp"
#include "Base/spImageManagement.hpp"
#include "Base/spImageBuffer.hpp"
#include "Base/spBasicObject.hpp"
#include "Base/spMath.hpp"
#include "Base/spMaterialColor.hpp"


namespace sp
{
namespace video
{


/*
 * Enumerations
 */

enum ETextureFilters
{
    FILTER_LINEAR,  //!< Linear ('Retro style') texture mapping.
    FILTER_SMOOTH,  //!< Smooth texture mapping.
};

//! Texture mip-map filters.
enum ETextureMipMapFilters
{
    FILTER_BILINEAR,    //!< Bilinear filtering.
    FILTER_TRILINEAR,   //!< Trilinear filtering (default).
    FILTER_ANISOTROPIC, //!< Anisotropic filtering.
};

//! Texture coordinate wrap modes.
enum ETextureWrapModes
{
    TEXWRAP_REPEAT, //!< Texture coordinates are not wraped (default).
    TEXWRAP_MIRROR, //!< Texture coordinates are mirrored.
    TEXWRAP_CLAMP,  //!< Texture coordinates are clamped in the range [0.0 .. 1.0] (e.g. used for sky-box and cube-map textures).
};

enum ETextureMixTypes
{
    TEXTUREMIX_ADD = 0,
    TEXTUREMIX_SET,
    TEXTUREMIX_MULTIPLY,
};

enum EAlphaBlendingTypes
{
    BLENDING_BRIGHTNESS = 0,
    BLENDING_DARKNESS
};

enum ETextureDimensions
{
    TEXTURE_1D = 1,         //!< 1 dimensional textures (e.g. for Cel-/ Toon Shading).
    TEXTURE_2D,             //!< 2 dimensional textures (default dimension).
    TEXTURE_3D,             //!< 3 dimensional textures (with depth information).
    TEXTURE_CUBEMAP,        //!< Cube maps (6 sides: +X, -X, +Y, -Y, +Z, -Z).
};

enum ETextureFrameDirection
{
    TEXTURE_FRAME_HORIZONTAL,
    TEXTURE_FRAME_VERTICAL,
};

//! Internal renderer image buffer format for textures.
enum EHWTextureFormats
{
    HWTEXFORMAT_UBYTE8,     //!< 8-bit unsigned byte color components (for PIXELFORMAT_DEPTH 16 bit).
    HWTEXFORMAT_FLOAT16,    //!< 16-bit floating point color components (for PIXELFORMAT_DEPTH 24 bit).
    HWTEXFORMAT_FLOAT32,    //!< 32-bit floating point color components (for PIXELFORMAT_DEPTH 32 bit).
};

//! Cubemap directions
enum ECubeMapDirections
{
    CUBEMAP_POSITIVE_X = 0, //!< Position X direction (+X).
    CUBEMAP_NEGATIVE_X,     //!< Negative X direction (-X).
    CUBEMAP_POSITIVE_Y,     //!< Position Y direction (+Y).
    CUBEMAP_NEGATIVE_Y,     //!< Negative Y direction (-Y).
    CUBEMAP_POSITIVE_Z,     //!< Position Z direction (+Z).
    CUBEMAP_NEGATIVE_Z      //!< Negative Z direction (-Z).
};


/*
 * Structures
 */

struct STextureCreationFlags
{
    STextureCreationFlags()
        : ImageBuffer(0), MagFilter(FILTER_SMOOTH), MinFilter(FILTER_SMOOTH), MipMapFilter(FILTER_TRILINEAR),
        Format(PIXELFORMAT_RGB), Anisotropy(1), MipMaps(true), WrapMode(TEXWRAP_REPEAT)
    {
    }
    ~STextureCreationFlags()
    {
    }
    
    /* Members */
    io::stringc Filename;
    dim::size2di Size;
    const u8* ImageBuffer;
    ETextureFilters MagFilter, MinFilter;
    ETextureMipMapFilters MipMapFilter;
    EPixelFormats Format;
    s32 Anisotropy;
    bool MipMaps;
    dim::vector3d<ETextureWrapModes> WrapMode;
};


//! Texture base class.
class SP_EXPORT Texture : public BasicObject
{
    
    public:
        
        Texture();
        Texture(const STextureCreationFlags &CreationFlags);
        virtual ~Texture();
        
        //! \return Texture's filename or user texture's name.
        inline io::stringc getFilename() const
        {
            return Filename_;
        }
        
        //! \return True if the texture is correctly created by the used renderer.
        virtual bool valid() const;
        
        /* Transfilter (color key) */
        
        /**
        Sets the color key.
        \param Color: Color which is to be blended out. The alpha channel specifies the transparency of this color.
        e.g. when you have a mesh-wire-fence the color between the mesh-wire (normally black {0|0|0} for green {0|255|0})
        shall be blended out the color key has to be (0, 0, 0, 0) for black or (0, 255, 0, 0) or green.
        */
        virtual void setColorKey(const video::color &Color); // Normal color key
        virtual void setColorKey(const video::color &Color, u8 Variation); // Using variation
        virtual void setColorKey(const dim::point2di &Pos, u8 Alpha = 0, u8 Variation = 0); // Using the color of specified coordinate
        virtual void setColorKeyAlpha(const EAlphaBlendingTypes Mode = BLENDING_BRIGHTNESS); // Like the alpha blending effect
        virtual void setColorKeyMask(Texture* MaskTexture, const EAlphaBlendingTypes Mode = BLENDING_BRIGHTNESS);
        
        inline video::color getColorKey() const
        {
            return ColorKey_;
        }
        
        /* Image size */
        
        //! \return True if the texture width and height are "Power Of Two" values (2^n: 1, 2, 4, 8, 16 etc.).
        virtual bool isSizePOT() const;
        
        //! \return Texture size for a "Power Of Two" dimension (e.g. 120 to 128, 260 to 256 etc.).
        virtual dim::size2di getSizePOT() const;
        
        //! Resizes the texture to a "Power Of Two" dimension if it is currently not.
        virtual void setSizePOT();
        
        /**
        Changes the texture's size. Do not use this function non-stop because the image-buffer-resize
        procedure is not very fast and don't forgett the MIP-map auto generation!
        */
        virtual void setSize(const dim::size2di &Size);
        
        //! \return Texture's size.
        inline dim::size2di getSize() const
        {
            return Size_;
        }
        inline dim::size2di getBackupSize() const
        {
            return SizeBackup_;
        }
        
        /* Pixel color manipulation */
        
        /**
        Sets a pixel's color.
        \param Pos: 2D position in screen space.
        \param Color: New color for the pixel.
        */
        void setPixelColor(const dim::point2di &Pos, const video::color &Color);
        video::color getPixelColor(const dim::point2di &Pos) const;
        
        /**
        Sets a pixel's color.
        \param Pos: 2D position in texture space ( [1.0 | 1.0] is equivalent to the texture width and height).
        \param Color: New color for the pixel.
        */
        void setPixelColor(const dim::point2df &Pos, const video::color &Color);
        video::color getPixelColor(const dim::point2df &Pos) const;
        
        /* Extra option functions */
        
        //! Makes a negative image.
        virtual void flipColors();
        
        virtual void mixTexture(Texture* hTexture, const ETextureMixTypes Type = TEXTUREMIX_ADD); // Mixing two textures
        virtual void addTextureFrame(Texture* NewFrame, const ETextureFrameDirection FrameDirection = TEXTURE_FRAME_HORIZONTAL);
        
        virtual void setColorIntensity(f32 Red, f32 Green, f32 Blue);
        virtual void setAlphaIntensity(u8 Alpha, const ETextureMixTypes Type = TEXTUREMIX_SET);
        
        //! Changes the pixel format. Typicallformats are PIXELFORMAT_RGB, PIXELFORMAT_RGBA
        virtual void setFormat(const EPixelFormats Format);
        
        //! Save backup from the current image buffer. This can be useful before modifying the image buffer.
        virtual void saveBackup();
        //! Load backup to the current image buffer.
        virtual void loadBackup();
        //! Clear the image buffer backup.
        virtual void clearBackup();
        
        /* Dimension */
        
        /**
        Enables or disables render target mode for the texture. If enabled the texture's image buffer can not be changed
        by the CPU only by the GPU via rendering into the texture.
        */
        virtual void setRenderTarget(bool Enable);
        inline bool getRenderTarget() const
        {
            return isRenderTarget_;
        }
        
        //! Sets depth if this texture is volumetric (Dimension: 3D).
        virtual void setDepth(s32 Depth);
        
        //! Return depth if this texture is volumetric (Dimension: 3D). Otherwise 1.
        inline s32 getDepth() const
        {
            return Depth_;
        }
        
        /**
        Sets the texture's dimension. The dimension can be: 1D, 2D, 3D or CubeMap. If the dimension is set to a CubeMap
        the texture has 6 faces (+X, -X, +Y, -Y, +Z, -Z). If the dimension is set to a 3D texture "setDepth" or "getDepth"
        functions can be used to configure texture's depth.
        \param Type: Dimension type. There are 1D, 2D, 3D or CubeMap.
        \param Depth: Depth for a 3D texture. By default 0 (means ignore).
        */
        virtual void setDimension(const ETextureDimensions Type, s32 Depth = 0);
        inline ETextureDimensions getDimension() const
        {
            return DimensionType_;
        }
        
        /**
        Sets the current CubeMap render-target face. Use this function to switch between
        the six faces (+X, -X, +Y, -Y, +Z, -Z) when rendering the CubeMap.
        */
        virtual void setCubeMapFace(const ECubeMapDirections Face);
        inline ECubeMapDirections getCubeMapFace() const
        {
            return CubeMapFace_;
        }
        
        /* Multiple render targets */
        
        /**
        Adds a new multi-render-target texture. To use several render targets at once you have to add several
        textures to a render-target-texture. e.g. convert a texture to a render target and add several textures.
        */
        virtual void addMultiRenderTarget(Texture* Tex);
        
        //! Removes the specified multi-render-target.
        virtual void removeMultiRenderTarget(Texture* Tex);
        
        //! Clears all multi-render-targets.
        virtual void clearMultiRenderTarget();
        
        //! Returns the multi-render-targets list.
        inline std::vector<Texture*> getMultiRenderTargets() const
        {
            return MultiRenderTargetList_;
        }
        
        /* Hardware texture data type */
        
        virtual void setHardwareFormat(const EHWTextureFormats HardwareFormat);
        inline EHWTextureFormats getHardwareFormat() const
        {
            return HWFormat_;
        }
        
        /* Image data updating and recreating */
        
        //! Flips the image on X-axis
        virtual void flipImageHorizontal();
        
        //! Flips the image on Y-axis
        virtual void flipImageVertical();
        
        //! Turns the image in 90, 180 or 270 degrees
        virtual void turnImage(const EImageTurnDegrees Degree);
        
        /**
        Enables or disables MIP-mapping. By default MIP-mapping is enabled and in 3D graphics normally
        it shoudl be always enabled. If MIP-mapping is disabled the texture can look very ugly when the count
        of rendered pixels are considerably less then the count of texels stored in the texture.
        Moreover using MIP-mapping makes the render process faster! This is due to the fact that less memory
        must be transmited for rendering operations. Particularly conspicuous in software rendering.
        However when using render-targets which are updated non-stop a texture without MIP-maps makes sense
        because auto.-MIP-map generation is not very fast with non-stop changes.
        */
        virtual void setMipMapping(bool MipMaps);
        virtual bool getMipMapping() const
        {
            return MipMaps_;
        }
        
        //! Copies the hardware image buffer into the texture image buffer.
        virtual void shareImageBuffer();
        
        //! Copies the texture image buffer into the hardware image buffer (recreating the renderer texture).
        virtual void updateImageBuffer();
        
        //! Copies the new image buffer. The buffer size must be [Width * Height * ForamtSize]!
        virtual void updateImageBuffer(const u8* ImageBuffer);
        
        //! Recreates the texture with the new size and image buffer. The buffer size must be [NewWidth * NewHeight * FormatSize]!
        virtual void updateImageBuffer(const dim::size2di &Size, const u8* ImageBuffer);
        
        //! Copies the specified area from the image buffer.
        virtual void updateImageBuffer(const dim::point2di &Position, const dim::size2di &Size, const u8* ImageBuffer);
        
        //! Generates the mipmaps if enabled.
        virtual void generateMipMap();
        
        /* Binding & unbinding */
        
        virtual void bind(s32 Level = 0) const;
        virtual void unbind(s32 Level = 0) const;
        
        /* Animation */
        
        //! Adds the specified texture to the animation frame list.
        virtual void addAnimFrame(video::Texture* AnimFrame);
        virtual void removeAnimFrame(video::Texture* AnimFrame);
        
        //! Returns a reference to the animation frame list.
        inline std::vector<video::Texture*>& getAnimFrameList()
        {
            return AnimFrameList_;
        }
        
        //! Enables or disables texture frame animation.
        virtual void setAnimation(bool Enable);
        inline bool getAnimation() const
        {
            return isAnim_;
        }
        
        //! Sets the new animation frame.
        virtual void setAnimFrame(u32 Frame);
        
        //! Returns the count of animation frames.
        inline s32 getAnimFrameCount() const
        {
            return AnimFrameList_.size();
        }
        
        /* Extra inline functions */
        
        //! Returns Texture's current ID. This is the current frame's texture number.
        inline void* getID() const
        {
            return ID_;
        }
        
        //! Returns Texture's original ID. Normally only used internally for the renderer.
        inline void* getOrigID() const
        {
            return OrigID_;
        }
        
        /**
        \return Pointer to the current image buffer. If the texture is an animation texture
        this image buffer points the the current animation's frame. The size of the image buffer
        can be calculated with "Width * Height * Format" (getSize().Width * getSize().Height * getFormat()).
        The internal format (or just "format") can be individual. Available are multiple formats but only four
        differnt component counts are available (1, 2, 3, 4). e.g. video::TEXTURE_ALPHA only has
        one component. video::TEXTURE_RGB has three components and video::TEXTURE_RGBA or video::TEXTURE_BGRA
        have four components.
        */
        inline const u8* getImageBuffer() const
        {
            return ImageBuffer_;
        }
        inline const u8* getImageBufferBackup() const
        {
            return ImageBufferBackup_;
        }
        
        /**
        Texture's filter mode. By default video::FILTER_SMOOTH.
        The renderers normally calls this type "linear". But in this engine a "linear" texture
        has none-smoothed texels. This is the second value: video::FILTER_LINEAR.
        Mag (magnification) is the filter when the texels are bigger then one pixel on the screen.
        Min (minification) is the filter when the texels are smaller then one pixel on the screen (normally using MipMaps).
        */
        virtual void setFilter(const ETextureFilters Filter);
        virtual void setFilter(const ETextureFilters MagFilter, const ETextureFilters MinFilter);
        virtual void setMagFilter(const ETextureFilters Filter);
        virtual void setMinFilter(const ETextureFilters Filter);
        
        inline ETextureFilters getMagFilter() const
        {
            return MagFilter_;
        }
        inline ETextureFilters getMinFilter() const
        {
            return MinFilter_;
        }
        
        /**
        Texture's mipmap filter. By default video::FILTER_TRILINEAR. Three filter types are supported: FILTER_BILINEAR,
        FILTER_TRILINEAR and FILTER_ANISOTROPIC. If the filter is FILTER_ANISOTROPIC the anisotropy factor
        can be set by "setAnisotropicSamples". The texture quality grows with these parameters.
        i.e. FILTER_BILINEAR looks worst and FILTER_ANISOTROPIC looks best. This is the method on how
        MIP-mapping is computed. If the texture has no MIP-maps the filter is unnecessary.
        */
        virtual void setMipMapFilter(const ETextureMipMapFilters MipMapFilter);
        inline ETextureMipMapFilters getMipMapFilter() const
        {
            return MipMapFilter_;
        }
        
        /**
        Texture coordinate wrap mode. This type says the renderer how to handle the texture coordinate greater then 1.0
        or smaller then 0.0. Three types are supported: TEXWRAP_REPEAT (default value), TEXWRAP_MIRROR, TEXWRAP_CLAMP
        (normall used for SkyBoxes).
        */
        virtual void setWrapMode(const ETextureWrapModes Wrap);
        virtual void setWrapMode(
            const ETextureWrapModes WrapU, const ETextureWrapModes WrapV, const ETextureWrapModes WrapW = TEXWRAP_REPEAT
        );
        
        inline dim::vector3d<ETextureWrapModes> getWrapMode() const
        {
            return WrapMode_;
        }
        
        /**
        \return Texture's format. In OpenGL it is called "InternalFormat". Only values are 1, 2, 3 or 4.
        * 1: TEXTURE_INDEX, TEXTURE_STENCIL, TEXTURE_DEPTH, TEXTURE_RED, TEXTURE_GREEN, TEXTURE_BLUE, TEXTURE_ALPHA, TEXTURE_GRAY.
        * 2: TEXTURE_GRAYALPHA
        * 3: TEXTURE_RGB, TEXTURE_BGR (bgr not supported yet).
        * 4: TEXTURE_RGBA, TEXTURE_BGRA (bgra not supported yet).
        */
        inline s32 getFormatSize() const
        {
            return FormatSize_;
        }
        
        //! \return Texture's image buffer format. These are one of the enumerated values of video::EPixelFormats.
        inline EPixelFormats getFormat() const
        {
            return Format_;
        }
        
        //! Sets the anisotropic filter samples. Modern high performance graphics-cards are able to use 16 samples.
        inline void setAnisotropicSamples(s32 Samples)
        {
            AnisotropicSamples_ = Samples;
        }
        inline s32 getAnisotropicSamples() const
        {
            return AnisotropicSamples_;
        }
        
        //! Sets multi samples (for anti-aliased textures).
        virtual void setMultiSamples(s32 Samples);
        //! Returns count of multi samples. By default 0.
        inline s32 getMultiSamples() const
        {
            return MultiSamples_;
        }
        
    protected:
        
        /* === Functions === */
        
        virtual void init();
        
        virtual void replaceImageBuffer(u8* NewImageBuffer);
        
        virtual void updateFormatSize();
        virtual s32 getFormatSize(const EPixelFormats Format) const;
        
        virtual void updateMultiRenderTargets();
        
        virtual void updateHeightDepthRatio();
        
        /* === Members === */
        
        /* ID */
        void* OrigID_;                                              // OpenGL (GLuint*), Direct3D9 (SD3D9HWTexture*), Direct3D11 (SD3D11HWTexture*)
        void* ID_;                                                  // Current ID (OrigID_ or AnimTex->OrigID_)
        
        /* Creation flags */
        EPixelFormats                       Format_, FormatBackup_; // RGB, RGBA ...
        s32                                 FormatSize_;            // 3 (RGB), 4 (RGBA) ...
        EHWTextureFormats                   HWFormat_;
        io::stringc                         Filename_;
        dim::size2di                        Size_, SizeBackup_;
        ETextureDimensions                  DimensionType_;
        s32                                 Depth_;
        ETextureFilters                     MagFilter_, MinFilter_;
        ETextureMipMapFilters               MipMapFilter_;
        dim::vector3d<ETextureWrapModes>    WrapMode_;
        bool                                MipMaps_;
        
        /* Extra options */
        s32 AnisotropicSamples_;
        s32 MultiSamples_;
        
        /* Options */
        ECubeMapDirections CubeMapFace_;
        video::color ColorKey_;
        
        /* Render target */
        bool isRenderTarget_;
        std::vector<Texture*> MultiRenderTargetList_;
        
        /* Animation */
        bool isAnim_;
        std::vector<Texture*> AnimFrameList_;
        
        /**
         * Used image buffer (pixel buffer)
         * This image buffer represents the current texture pixel buffer
         * (normal, resized or with manipulated surface)
         */
        u8* ImageBuffer_;
        
        /**
         * Backup of the current image buffer (by default 0)
         * This image buffer can be saved and loaded when changing format, size etc.
         * Its format is always RGBA.
         */
        u8* ImageBufferBackup_;
        
};


} // /namespace video

} // /namespace sp


#endif



// ================================================================================
