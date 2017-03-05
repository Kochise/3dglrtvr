/*
 * Image buffer management file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Base/spImageManagement.hpp"
#include "RenderSystem/spTextureBase.hpp"


namespace sp
{
namespace video
{


/*
 * SUniversalImageData structure
 */

SHeightMapTexture::SHeightMapTexture() : ImageBuffer(0)
{
}
SHeightMapTexture::SHeightMapTexture(const Texture* hTexture) : ImageBuffer(0)
{
    createBuffer(hTexture);
}
SHeightMapTexture::~SHeightMapTexture()
{
}

void SHeightMapTexture::createBuffer(const Texture* hTexture)
{
    clearBuffer();
    
    Size = hTexture->getSize();
    
    const s32 ImageBufferSize = Size.Width * Size.Height;
    
    ImageBuffer = new f32[ImageBufferSize];
    memset(ImageBuffer, 0, ImageBufferSize*sizeof(f32));
    
    for (s32 y = 0, x, i = 0, j; y < Size.Height; ++y)
    {
        for (x = 0; x < Size.Width; ++x, ++i)
            ImageBuffer[i] = hTexture->getPixelColor( dim::point2di(x, y) ).getBrightness<f32>() / 255;
    }
}

void SHeightMapTexture::createBuffer(const dim::size2di &NewSize, s32 NewFormat, const f32* NewImageBuffer)
{
    clearBuffer();
    
    Size = NewSize;
    
    const s32 ImageBufferSize = Size.Width * Size.Height;
    
    ImageBuffer = new f32[ImageBufferSize];
    memcpy(ImageBuffer, NewImageBuffer, ImageBufferSize);
}

void SHeightMapTexture::clearBuffer()
{
    Size = 0;
    MemoryManager::deleteBuffer(ImageBuffer);
}

f32 SHeightMapTexture::getHeightValue(const dim::point2df &Pos) const
{
    if (!ImageBuffer)
        return 0.0f;
    
    /* Get the first coordinate */
    s32 Pos1X = (s32)( (Pos.X - (s32)Pos.X) * Size.Width );
    s32 Pos1Y = (s32)( (Pos.Y - (s32)Pos.Y) * Size.Height );
    
    math::Clamp(Pos1X, 0, Size.Width - 1);
    math::Clamp(Pos1Y, 0, Size.Height - 1);
    
    /* Get the second coordinate */
    s32 Pos2X = Pos1X + 1;
    s32 Pos2Y = Pos1Y + 1;
    
    math::Clamp(Pos2X, 0, Size.Width - 1);
    math::Clamp(Pos2Y, 0, Size.Height - 1);
    
    /* Process the interpolation */
    f32 RatioX = Pos.X * Size.Width   - Pos1X;
    f32 RatioY = Pos.Y * Size.Height  - Pos1Y;
    
    math::Clamp(RatioX, 0.0f, 1.0f);
    math::Clamp(RatioY, 0.0f, 1.0f);
    
    const f32 RatioInvX = 1.0f - RatioX;
    const f32 RatioInvY = 1.0f - RatioY;
    
    /* Compute the final height value */
    const f32 val1 = ImageBuffer[ Pos1Y * Size.Width + Pos1X ];
    const f32 val2 = ImageBuffer[ Pos1Y * Size.Width + Pos2X ];
    const f32 val3 = ImageBuffer[ Pos2Y * Size.Width + Pos2X ];
    const f32 val4 = ImageBuffer[ Pos2Y * Size.Width + Pos1X ];
    
    return
        ( val1 * RatioInvX + val2 * RatioX ) * RatioInvY +
        ( val4 * RatioInvX + val3 * RatioX ) * RatioY;
}

dim::vector3df SHeightMapTexture::getNormal(const dim::point2df &Pos, const dim::point2df &Adjustment) const
{
    dim::vector3df a(Pos.X - Adjustment.X, 0, Pos.Y);
    dim::vector3df b(Pos.X + Adjustment.X, 0, Pos.Y);
    dim::vector3df c(Pos.X, 0, Pos.Y - Adjustment.Y);
    dim::vector3df d(Pos.X, 0, Pos.Y + Adjustment.Y);
    
    a.Y = getHeightValue(dim::point2df(a.X, a.Z));
    b.Y = getHeightValue(dim::point2df(b.X, b.Z));
    c.Y = getHeightValue(dim::point2df(c.X, c.Z));
    d.Y = getHeightValue(dim::point2df(d.X, d.Z));
    
    return (c - d).cross(a - b).normalize();
}


namespace ImageConverter
{

SP_EXPORT void flipImageColors(u8* ImageBuffer, s32 Width, s32 Height, s32 FormatSize)
{
    if ( ImageBuffer && Width > 0 && Height > 0 && ( FormatSize == 3 || FormatSize == 4 ) )
    {
        const u32 ImageBufferSize = Width * Height * FormatSize;
        
        for (u32 i = 0; i < ImageBufferSize; i += FormatSize)
            math::Swap(ImageBuffer[i], ImageBuffer[i + 2]);
    }
}

SP_EXPORT void flipImageHorz(u8* ImageBuffer, s32 Width, s32 Height, s32 FormatSize)
{
    /* Check for valid parameter values */
    if (!ImageBuffer || Width <= 0 || Height <= 0 || FormatSize < 1 || FormatSize > 4)
        return;
    
    /* Temporary variables */
    const s32 Pitch     = Width * FormatSize;
    const s32 HalfPitch = Pitch/2;
    
    /* Loop the array */
    for (s32 y = 0, x; y < Height; ++y)
    {
        for (x = 0; x < HalfPitch; x += 3)
        {
            math::Swap(ImageBuffer[ y*Pitch + ( Pitch - x - 1 ) - 0 ], ImageBuffer[ y*Pitch + x + 2 ]);
            math::Swap(ImageBuffer[ y*Pitch + ( Pitch - x - 1 ) - 1 ], ImageBuffer[ y*Pitch + x + 1 ]);
            math::Swap(ImageBuffer[ y*Pitch + ( Pitch - x - 1 ) - 2 ], ImageBuffer[ y*Pitch + x + 0 ]);
        }
    }
}

SP_EXPORT void flipImageVert(u8* ImageBuffer, s32 Width, s32 Height, s32 FormatSize)
{
    /* Check for valid parameter values */
    if (!ImageBuffer || Width <= 0 || Height <= 0 || FormatSize < 1 || FormatSize > 4)
        return;
    
    /* Temporary variables */
    const s32 Pitch         = Width*FormatSize;
    const s32 HalfHeight    = Height/2;
    
    /* Loop the array */
    for (s32 y = 0, x; y < HalfHeight; ++y)
    {
        for (x = 0; x < Pitch; ++x)
            math::Swap(ImageBuffer[ (Height - y - 1)*Pitch + x ], ImageBuffer[ y*Pitch + x ]);
    }
}

SP_EXPORT void scaleImage(u8* &ImageBuffer, s32 Width, s32 Height, s32 NewWidth, s32 NewHeight, s32 FormatSize)
{
    /* Check if the memory is not empty */
    if ( !ImageBuffer || Width <= 0 || Height <= 0 || NewWidth <= 0 || NewHeight <= 0 ||
         FormatSize < 1 || FormatSize > 4 || ( Width == NewWidth && Height == NewHeight ) )
    {
        return;
    }
    
    /* Temporary variables */
    s32 x, y, i, j, k;
    
    /* Allocate new memory */
    const u32 ImageBufferSize = NewWidth * NewHeight * FormatSize;
    
    u8* NewImageBuffer = new u8[ImageBufferSize];
    
    /* Loop for the new image size */
    for (y = 0; y < NewHeight; ++y)
    {
        for (x = 0; x < NewWidth; ++x)
        {
            for (i = 0; i < FormatSize; ++i)
            {
                /* Get the image data indices */
                j = y * NewWidth + x;
                k = ( y * Height / NewHeight ) * Width + ( x * Width / NewWidth );
                
                /* Fill the new image data */
                NewImageBuffer[ j * FormatSize + i ] = ImageBuffer[ k * FormatSize + i ];
            }
        }
    }
    
    /* Delete the old memory */
    delete [] ImageBuffer;
    
    /* Use the new memory */
    ImageBuffer = NewImageBuffer;
}

SP_EXPORT void halveImage(u8* &ImageBuffer, s32 Width, s32 Height, s32 FormatSize)
{
    /* Check for valid parameter values */
    if (!ImageBuffer || Width <= 0 || Height <= 0 || FormatSize < 1 || FormatSize > 4)
        return;
    
    /* Temporary variables */
    s32 x, y, i, x2, y2;
    
    /* New image data size */
    s32 NewWidth    = Width;
    s32 NewHeight   = Height;
    
    /* Change the new size */
    if (NewWidth > 1)
        NewWidth >>= 1;
    if (NewHeight > 1)
        NewHeight >>= 1;
    
    /* Save the old image data */
    u8* OldImageBuffer = ImageBuffer;
    
    /* Allocate new image data */
    ImageBuffer = new u8[ NewWidth * NewHeight * FormatSize ];
    
    if (Width > 1 && Height > 1)
    {
        
        /* Loop for the half image size */
        for (y = y2 = 0; y < NewHeight; ++y, y2 = y << 1)
        {
            for (x = x2 = 0; x < NewWidth; ++x, x2 = x << 1)
            {
                for (i = 0; i < FormatSize; ++i)
                {
                    /* Fill the current new data elements */
                    ImageBuffer[ ( y * NewWidth + x ) * FormatSize + i ] = (
                        OldImageBuffer[ ( (y2    ) * Width + (x2    ) ) * FormatSize + i ] +
                        OldImageBuffer[ ( (y2    ) * Width + (x2 + 1) ) * FormatSize + i ] +
                        OldImageBuffer[ ( (y2 + 1) * Width + (x2    ) ) * FormatSize + i ] +
                        OldImageBuffer[ ( (y2 + 1) * Width + (x2 + 1) ) * FormatSize + i ]
                    ) / 4;
                } // next color component
            } // next pixel
        } // next line
        
    }
    else if (Width == 1 || Height == 1)
    {
        
        s32 MaxSize = math::Max(NewWidth, NewHeight);
        
        /* Loop for the half image size */
        for (x = 0, x2 = 0; x < MaxSize; ++x, x2 = x << 1)
        {
            for (i = 0; i < FormatSize; ++i)
            {
                /* Fill the current new data elements */
                ImageBuffer[ x * FormatSize + i ] = (
                    OldImageBuffer[ ( x2     ) * FormatSize + i ] +
                    OldImageBuffer[ ( x2 + 1 ) * FormatSize + i ]
                ) / 2;
            } // next color component
        } // next pixel
        
    }
    
    /* Delete the old image data */
    delete [] OldImageBuffer;
}

/* ======= Conversion macors ======= */

#define __SP_CONVERT_1_TO_2(src, dest, i, j)            \
    dest[j+0] = src[i+0];                               \
    dest[j+1] = 255;
#define __SP_CONVERT_1_TO_3(src, dest, i, j)            \
    dest[j+0] = src[i+0];                               \
    dest[j+1] = src[i+0];                               \
    dest[j+2] = src[i+0];
#define __SP_CONVERT_1_TO_4(src, dest, i, j)            \
    dest[j+0] = src[i+0];                               \
    dest[j+1] = src[i+0];                               \
    dest[j+2] = src[i+0];                               \
    dest[j+3] = 255;

#define __SP_CONVERT_2_TO_1(src, dest, i, j)            \
    dest[j+0] = src[i+0];
#define __SP_CONVERT_2_TO_3(src, dest, i, j)            \
    dest[j+0] = src[i+0];                               \
    dest[j+1] = src[i+0];                               \
    dest[j+2] = src[i+0];
#define __SP_CONVERT_2_TO_4(src, dest, i, j)            \
    dest[j+0] = src[i+0];                               \
    dest[j+1] = src[i+0];                               \
    dest[j+2] = src[i+0];                               \
    dest[j+3] = src[i+1];

#define __SP_CONVERT_3_TO_1(src, dest, i, j)            \
    dest[j+0] = ( src[i+0] + src[i+1] + src[i+2] ) / 3;
#define __SP_CONVERT_3_TO_2(src, dest, i, j)            \
    dest[j+0] = ( src[i+0] + src[i+1] + src[i+2] ) / 3; \
    dest[j+1] = 255;
#define __SP_CONVERT_3_TO_4(src, dest, i, j)            \
    dest[j+0] = src[i+0];                               \
    dest[j+1] = src[i+1];                               \
    dest[j+2] = src[i+2];                               \
    dest[j+3] = 255;

#define __SP_CONVERT_4_TO_1(src, dest, i, j)            \
    dest[j+0] = ( src[i+0] + src[i+1] + src[i+2] ) / 3;
#define __SP_CONVERT_4_TO_2(src, dest, i, j)            \
    dest[j+0] = ( src[i+0] + src[i+1] + src[i+2] ) / 3; \
    dest[j+1] = src[i+3];
#define __SP_CONVERT_4_TO_3(src, dest, i, j)            \
    dest[j+0] = src[i+0];                               \
    dest[j+1] = src[i+1];                               \
    dest[j+2] = src[i+2];


SP_EXPORT void convertImageFormat(
    u8* &ImageBuffer, s32 Width, s32 Height, s32 OldFormatSize, s32 NewFormatSize)
{
    /* Check if the memory is not empty */
    if (!ImageBuffer || Width <= 0 || Height <= 0 || OldFormatSize < 1 || OldFormatSize > 4 ||
        NewFormatSize < 1 || NewFormatSize > 4 || OldFormatSize == NewFormatSize)
    {
        return;
    }
    
    /* Temporary variables */
    s32 x, y, i = 0, j = 0;
    
    /* Allocate new memory */
    u8* NewImageBuffer = new u8[ Width * Height * NewFormatSize ];
    
    /* Loop for the image size */
    for (y = 0; y < Height; ++y)
    {
        for (x = 0; x < Width; ++x, i += OldFormatSize, j += NewFormatSize)
        {
            
            /* Select the source's internal format */
            switch (OldFormatSize)
            {
                case 1:
                    switch (NewFormatSize)
                    {
                        case 2: __SP_CONVERT_1_TO_2(ImageBuffer, NewImageBuffer, i, j); break;
                        case 3: __SP_CONVERT_1_TO_3(ImageBuffer, NewImageBuffer, i, j); break;
                        case 4: __SP_CONVERT_1_TO_4(ImageBuffer, NewImageBuffer, i, j); break;
                    }
                    break;
                    
                case 2:
                    switch (NewFormatSize)
                    {
                        case 1: __SP_CONVERT_2_TO_1(ImageBuffer, NewImageBuffer, i, j); break;
                        case 3: __SP_CONVERT_2_TO_3(ImageBuffer, NewImageBuffer, i, j); break;
                        case 4: __SP_CONVERT_2_TO_4(ImageBuffer, NewImageBuffer, i, j); break;
                    }
                    break;
                    
                case 3:
                    switch (NewFormatSize)
                    {
                        case 1: __SP_CONVERT_3_TO_1(ImageBuffer, NewImageBuffer, i, j); break;
                        case 2: __SP_CONVERT_3_TO_2(ImageBuffer, NewImageBuffer, i, j); break;
                        case 4: __SP_CONVERT_3_TO_4(ImageBuffer, NewImageBuffer, i, j); break;
                    }
                    break;
                    
                case 4:
                    switch (NewFormatSize)
                    {
                        case 1: __SP_CONVERT_4_TO_1(ImageBuffer, NewImageBuffer, i, j); break;
                        case 2: __SP_CONVERT_4_TO_2(ImageBuffer, NewImageBuffer, i, j); break;
                        case 3: __SP_CONVERT_4_TO_3(ImageBuffer, NewImageBuffer, i, j); break;
                    }
                    break;
            } // /switch
            
        } // next pixel
    } // next line
    
    /* Delete the old memory */
    delete [] ImageBuffer;
    
    /* Use the new memory */
    ImageBuffer = NewImageBuffer;
}

SP_EXPORT void blurImage(u8* &ImageBuffer, s32 Width, s32 Height, s32 FormatSize)
{
    /* Check if the iamge data is not empty */
    if (!ImageBuffer || Width <= 0 || Height <= 0 || FormatSize < 1 || FormatSize > 4)
        return;
    
    /* Temporary variables */
    s32 y, x, i;
    
    /* Allocate new memory */
    u8* NewImageBuffer = new u8[ Width * Height * FormatSize ];
    
    /* Loop the image data (main body) */
    for (y = 0; y < Height - 1; ++y)
    {
        for (x = 0; x < Width - 1; ++x)
        {
            for (i = 0; i < FormatSize; ++i)
            {
                /* Fill the new image data */
                NewImageBuffer[ ( y * Width + x ) * FormatSize + i ] = (
                    ImageBuffer[ ( y * Width + x       ) * FormatSize + i ] + 
                    ImageBuffer[ ( y * Width + x+1     ) * FormatSize + i ] + 
                    ImageBuffer[ ( (y+1) * Width + x+1 ) * FormatSize + i ] + 
                    ImageBuffer[ ( (y+1) * Width + x   ) * FormatSize + i ]
                ) / 4;
            }
        } // next pixel
    } // next line
    
    /* Loop the image data (edge - x axis) */
    for (x = 0; x < Width - 1; ++x)
    {
        for (i = 0; i < FormatSize; ++i)
        {
            /* Fill the new image data */
            NewImageBuffer[ ( (Height-1) * Width + x ) * FormatSize + i ] = (
                ImageBuffer[ ( (Height-1) * Width + x   ) * FormatSize + i ] + 
                ImageBuffer[ ( (Height-1) * Width + x+1 ) * FormatSize + i ] + 
                ImageBuffer[ ( x+1                      ) * FormatSize + i ] + 
                ImageBuffer[ ( x                        ) * FormatSize + i ]
            ) / 4;
        }
    } // next pixel
    
    /* Loop the image data (edge - y axis) */
    for (y = 0; y < Height - 1; ++y)
    {
        for (i = 0; i < FormatSize; ++i)
        {
            /* Fill the new image data */
            NewImageBuffer[ ( y * Width + (Width-1) ) * FormatSize + i ] = (
                ImageBuffer[ ( y * Width + (Width-1)     ) * FormatSize + i ] + 
                ImageBuffer[ ( y * Width                 ) * FormatSize + i ] + 
                ImageBuffer[ ( (y+1) * Width + (Width-1) ) * FormatSize + i ] + 
                ImageBuffer[ ( (y+1) * Width             ) * FormatSize + i ]
            ) / 4;
        }
    } // next pixel
    
    /* Loop the image data (edge right bottom) */
    for (i = 0; i < FormatSize; ++i)
    {
        /* Fill the new image data */
        NewImageBuffer[ ( (Height-1) * Width + (Width-1) ) * FormatSize + i ] = (
            ImageBuffer[ ( (Height-1) * Width + (Width-1) ) * FormatSize + i ] + 
            ImageBuffer[ ( (Height-1) * Width             ) * FormatSize + i ] + 
            ImageBuffer[                                                       i ] + 
            ImageBuffer[ ( Width-1                        ) * FormatSize + i ]
        ) / 4;
    }
    
    /* Delete the old memory */
    delete [] ImageBuffer;
    
    /* Use the new memory */
    ImageBuffer = NewImageBuffer;
}

SP_EXPORT void changeImageData(
    u8* &ImageBuffer, s32 Width, s32 Height, s32 OldFormatSize,
    s32 NewWidth, s32 NewHeight, s32 NewFormatSize)
{
    /* Scale the iamge */
    scaleImage(ImageBuffer, Width, Height, NewWidth, NewHeight, OldFormatSize);
    
    /* Convert the image */
    convertImageFormat(ImageBuffer, Width, Height, OldFormatSize, NewFormatSize);
}

SP_EXPORT void turnImage(u8* ImageBuffer, s32 Width, s32 Height, s32 FormatSize, const EImageTurnDegrees Degree)
{
    /* Temporary variables */
    s32 x, y, i, j = 0, k;
    
    /* Get image buffer size */
    const u32 ImageBufferSize = Width * Height * FormatSize;
    
    /* Allocate temporary memory */
    u8* TmpImageBuffer = new u8[ImageBufferSize];
    
    /* Copy the old image buffer temporary */
    memcpy(TmpImageBuffer, ImageBuffer, ImageBufferSize);
    
    /* Select the degree type */
    switch (Degree)
    {
        case TURNDEGREE_90:
        {
            for (y = 0; y < Height; ++y)
            {
                for (x = 0, i = Width*(Height - 1) + y; x < Width; ++x, i -= Width, ++j)
                {
                    for (k = 0; k < FormatSize; ++k)
                        ImageBuffer[i*FormatSize + k] = TmpImageBuffer[j*FormatSize + k];
                }
            }
        }
        break;
        
        case TURNDEGREE_180:
        {
            for (j = 0, i = Width*Height - 1; j < Width*Height; --i, ++j)
            {
                for (k = 0; k < FormatSize; ++k)
                    ImageBuffer[i*FormatSize + k] = TmpImageBuffer[j*FormatSize + k];
            }
        }
        break;
        
        case TURNDEGREE_270:
        {
            for (y = 0; y < Height; ++y)
            {
                for (x = 0, i = Width - 1 - y; x < Width; ++x, i += Width, ++j)
                {
                    for (k = 0; k < FormatSize; ++k)
                        ImageBuffer[i*FormatSize + k] = TmpImageBuffer[j*FormatSize + k];
                }
            }
        }
        break;
    }
    
    /* Delete the temporary memory */
    delete [] TmpImageBuffer;
}

SP_EXPORT bool checkImageSize(dim::size2di &InputSize)
{
    dim::size2di OutputSize;
    
    /* Get the correct size */
    OutputSize.Width    = math::RoundPow2(InputSize.Width);
    OutputSize.Height   = math::RoundPow2(InputSize.Height);
    
    /* Check if the size has changed */
    if (OutputSize != InputSize)
    {
        InputSize = OutputSize;
        return true;
    }
    
    return false;
}

SP_EXPORT s32 getMipmapLevelsCount(s32 Width, s32 Height)
{
    return math::Round( log(math::Max<f32>((f32)Width, (f32)Height)) / log(2.0f) );
}

} // /namespace ImageConverter


} // /namespace video

} // /namespace sp



// ================================================================================
