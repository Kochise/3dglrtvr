/*
 * Image format interfaces header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_IMAGEFORMATINTERFACES_H__
#define __SP_IMAGEFORMATINTERFACES_H__


#include "Base/spStandard.hpp"
#include "Base/spInputOutput.hpp"
#include "Base/spBaseFileFormatHandler.hpp"
#include "Base/spMath.hpp"
#include "Base/spMaterialColor.hpp"

#include <cstdio>


namespace sp
{
namespace video
{


/*
 * Enumerations
 */

#if 1 //todo -> don't use the old EPixelFormats enum further !!!

//! Texture pixel formats
enum EPixelFormats
{
    /* 1 component */
    PIXELFORMAT_INDEX = 0,  //!< Color index.
    PIXELFORMAT_STENCIL,    //!< Stencil.
    PIXELFORMAT_INTENSITY,  //!< Intensity.
    PIXELFORMAT_RED,        //!< Only red component.
    PIXELFORMAT_GREEN,      //!< Only green component.
    PIXELFORMAT_BLUE,       //!< Only blue component.
    PIXELFORMAT_ALPHA,      //!< Only alpha channel.
    PIXELFORMAT_GRAY,       //!< Gray scaled (or rather luminance).
    
    /* 2 components */
    PIXELFORMAT_GRAYALPHA,  //!< Gray scaled with alpha channel.
    
    /* 3 components */
    PIXELFORMAT_RGB,        //!< Red/ Green/ Blue components in the normal order RGB.
    PIXELFORMAT_BGR,        //!< Red/ Green/ Blue components in the inverse order BGR.
    PIXELFORMAT_DEPTH,      //!< Depth component (floating point).
    
    /* 4 components */
    PIXELFORMAT_RGBA,       //!< Red/ Green/ Blue components with alpha channel in the normal order RGBA.
    PIXELFORMAT_BGRA,       //!< Red/ Green/ Blue components with alpha channel in the inverse order BGRA.
};

#else //todo -> use new EPixelFormats enum !!!

//! Texture pixel formats
enum EPixelFormats
{
    PIXELFORMAT_ALPHA,      //!< Alpha channel but no color components.
    PIXELFORMAT_GRAY,       //!< Gray scaled.
    PIXELFORMAT_GRAYALPHA,  //!< Gray scaled with alpha channel.
    PIXELFORMAT_RGB,        //!< Red/ Green/ Blue components in the normal order RGB.
    PIXELFORMAT_BGR,        //!< Red/ Green/ Blue components in the inverse order BGR.
    PIXELFORMAT_RGBA,       //!< Red/ Green/ Blue components with alpha channel in the normal order RGBA.
    PIXELFORMAT_BGRA,       //!< Red/ Green/ Blue components with alpha channel in the inverse order BGRA.
    PIXELFORMAT_DEPTH,      //!< Depth component (floating point).
};

#endif

//! Image file formats
enum EImageFileFormats
{
    IMAGEFORMAT_UNKNOWN,    //!< Unknown file format.
    
    /* Supported formats */
    IMAGEFORMAT_BMP,        //!< Windows Bitmap (bmp, dib).
    IMAGEFORMAT_JPG,        //!< Joint Photographic Experts (jpg, jpeg, jpe, jfif).
    IMAGEFORMAT_PNG,        //!< Portable Network Graphics (png).
    IMAGEFORMAT_TGA,        //!< Targa True Image (tga, tpci).
    IMAGEFORMAT_PCX,        //!< Picture Exchange (pcx).
    IMAGEFORMAT_DDS,        //!< Direct Draw Surface (dds).
    IMAGEFORMAT_WAD,        //!< Where is All the Data (wad).
    
    /* Unsupported formats (just for identification) */
    IMAGEFORMAT_GIF,        //!< GraphicsInterchangeFormat (gif) (unsupported, only for detection).
};


/*
 * Structures
 */

//! Image data for loading or saving.
struct SImageData
{
    SImageData()
        : bpp(24), Format(PIXELFORMAT_RGB), FormatSize(3), ImageBuffer(0)
    {
    }
    ~SImageData()
    {
        MemoryManager::deleteBuffer(ImageBuffer);
    }
    
    s32 Width;              // Dimension: width
    s32 Height;             // Dimension: height
    
    s32 bpp;                // bits per pixel
    EPixelFormats Format;   // TEXTURE_RGB, TEXTURE_RGBA ...
    s32 FormatSize;         // 1/ 2/ 3/ 4
    
    u32 ID;                 // Identification number
    
    u8* ImageBuffer;        // Image RAW data
};


//! ImageLoader class used as image loading interface.
class SP_EXPORT ImageLoader : public io::BaseFileFormatHandler
{
    
    public:
        
        virtual ~ImageLoader();
        
        /* Functions */
        
        virtual SImageData* loadImageData() = 0;
        
    protected:
        
        ImageLoader(const io::stringc &Filename);
        
        inline bool isFileError() const
        {
            return isFileError_;
        }
        
        /* Members */
        
        bool isFileError_;
        
};

//! ImageSaver class used as image saving interface.
class SP_EXPORT ImageSaver : public io::BaseFileFormatHandler
{
    
    public:
        
        virtual bool saveImageData(SImageData* ImageData) = 0;
        
        virtual ~ImageSaver();
        
    protected:
        
        ImageSaver(const io::stringc &Filename);
        
        inline bool isFileError() const
        {
            return isFileError_;
        }
        
        /* Members */
        
        bool isFileError_;
        
};


} // /namespace video

} // /namespace sp


#endif



// ================================================================================
