/*
 * Image buffer management header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_IMAGEMANAGEMENT_H__
#define __SP_IMAGEMANAGEMENT_H__


#include "Base/spStandard.hpp"
#include "Base/spInputOutput.hpp"
#include "Base/spMath.hpp"
#include "Base/spMaterialColor.hpp"
#include "FileFormats/Image/spImageFormatInterfaces.hpp"

#include <cstdio>


namespace sp
{
namespace video
{


/*
 * Enumerations
 */

//! Methods of image data rotation
enum EImageTurnDegrees
{
    TURNDEGREE_90,  //!< Turn 90 degrees.
    TURNDEGREE_180, //!< Turn 180 degrees.
    TURNDEGREE_270, //!< Turn 270 degrees.
};


/*
 * Structures
 */

class Texture;

//! Heightmap texture structure for more precision.
struct SP_EXPORT SHeightMapTexture
{
    SHeightMapTexture();
    SHeightMapTexture(const Texture* hTexture);
    ~SHeightMapTexture();
    
    /* Functions */
    
    //! Creates the buffer out of the texture's image buffer.
    void createBuffer(const Texture* hTexture);
    
    //! Creates the buffer by copying the input buffer. Don't forget to delete the input buffer!
    void createBuffer(const dim::size2di &NewSize, s32 NewFormat, const f32* NewImageBuffer);
    
    //! Clears (or rather deletes) the buffer.
    void clearBuffer();
    
    /**
    Determines the height value at a specified position. The height value will be interpolated to
    have a smooth result with small heightmaps, too.
    \param Pos: Position where the height value shall be sampled (in a range or [0.0, 1.0]).
    \return Height value which is to be determined.
    */
    f32 getHeightValue(const dim::point2df &Pos) const;
    
    /**
    Calculates the normal vector at a specified position and adjustment.
    \param Pos: Position where the normal vector shall be calculated.
    \param Adjustment: Adjustment (or rather offset) to get the area of the face which normal shall be calculated.
    \return Calculated normal vector.
    */
    dim::vector3df getNormal(const dim::point2df &Pos, const dim::point2df &Adjustment) const;
    
    /* Members */
    
    dim::size2di Size;  //!< Heightmap texture size.
    f32* ImageBuffer;   //!< Floating-point array for more precision in height-mapping (array size = Size.Width * Size.Height).
};


//! ImageConverter namespace used for image buffer manipulation. This is particular used in the Texture class (e.g. for "setFormat", "setSize" etc.).
namespace ImageConverter
{

//! Flips the color values (BGR -> RGB)
SP_EXPORT void flipImageColors(u8* ImageBuffer, s32 Width, s32 Height, s32 FormatSize = 3);

//! Flips the image data on the x-axis
SP_EXPORT void flipImageHorz(u8* ImageBuffer, s32 Width, s32 Height, s32 FormatSize = 3);

//! Flips the image data on the y-axis
SP_EXPORT void flipImageVert(u8* ImageBuffer, s32 Width, s32 Height, s32 FormatSize = 3);

//! Scales the image to a new size
SP_EXPORT void scaleImage(u8* &ImageBuffer, s32 Width, s32 Height, s32 NewWidth, s32 NewHeight, s32 FormatSize = 3);

//! Scales the image to a half size (smooth)
SP_EXPORT void halveImage(u8* &ImageBuffer, s32 Width, s32 Height, s32 FormatSize = 3);

//! Converts the image data formats (e.g. RGB -> RGBA)
SP_EXPORT void convertImageFormat(u8* &ImageBuffer, s32 Width, s32 Height, s32 OldFormatSize, s32 NewFormatSize);

//! Blurs the image data (for mipmap levels)
SP_EXPORT void blurImage(u8* &ImageBuffer, s32 Width, s32 Height, s32 FormatSize = 3);

//! Universal function (calls the other needed functions)
SP_EXPORT void changeImageData(
    u8* &ImageBuffer, s32 Width, s32 Height, s32 FormatSize,
    s32 NewWidth, s32 NewHeight, s32 NewFormatSize
);

//! Turns the image data in 90°, 180° or 270°
SP_EXPORT void turnImage(u8* ImageBuffer, s32 Width, s32 Height, s32 FormatSize, const EImageTurnDegrees Degree);

//! Get the correct image size (2^x)
SP_EXPORT bool checkImageSize(dim::size2di &InputSize);

//! Get the count of mipmap levels
SP_EXPORT s32 getMipmapLevelsCount(s32 Width, s32 Height);

} // /namespace ImageConverter


} // /namespace video

} // /namespace sp


#endif



// ================================================================================
