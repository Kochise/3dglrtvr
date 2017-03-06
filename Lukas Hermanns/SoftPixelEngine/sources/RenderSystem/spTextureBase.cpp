/*
 * Texture base file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "RenderSystem/spTextureBase.hpp"
#include "Base/spImageManagement.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;

namespace video
{


/*
 * Internal members
 */

s32 STDPixelFormatDataSize[] = {
    #if 1
    /* 1 component */
    1, // Index color
    1, // Stencil
    1, // Intensity
    1, // Red
    1, // Green
    1, // Blue
    1, // Alpha
    1, // Gray
    
    /* 2 components */
    2, // Gray alpha
    
    /* 3 components */
    3, // RGB
    3, // BGR
    3, // Depth
    
    /* 4 components */
    4, // RGBA
    4 // BGRA
    #else
    1, 1, 2, 3, 3, 4, 4, 1
    #endif
};


/*
 * Texture class
 */

Texture::Texture()
    : BasicObject(), ID_(0), OrigID_(0), ImageBuffer_(0), ImageBufferBackup_(0), DimensionType_(TEXTURE_2D), WrapMode_(TEXWRAP_REPEAT)
{
    init();
}
Texture::Texture(const STextureCreationFlags &CreationFlags)
    : BasicObject(), ID_(0), OrigID_(0), ImageBuffer_(0), ImageBufferBackup_(0), DimensionType_(TEXTURE_2D), WrapMode_(TEXWRAP_REPEAT)
{
    /* Initialization */
    init();
    
    /* General settings */
    Filename_       = CreationFlags.Filename;
    Size_           = CreationFlags.Size;
    Format_         = CreationFlags.Format;
    MagFilter_      = CreationFlags.MagFilter;
    MinFilter_      = CreationFlags.MinFilter;
    MipMapFilter_   = CreationFlags.MipMapFilter;
    MipMaps_        = CreationFlags.MipMaps;
    WrapMode_       = CreationFlags.WrapMode;
    
    updateFormatSize();
}
Texture::~Texture()
{
    /* Delete the backup */
    clearBackup();
    
    /* Delete the used image data */
    MemoryManager::deleteBuffer(ImageBuffer_);
}

bool Texture::valid() const
{
    return false;
}

void Texture::setColorKey(const video::color &Color)
{
    /* Check if the image is not empty */
    if (!ImageBuffer_ || ColorKey_ == Color)
        return;
    
    /* Check if the current type is bad */
    if (FormatSize_ < 3)
        setFormat(PIXELFORMAT_RGB);
    
    /* Set the new transparency color */
    ColorKey_ = Color;
    
    /* Allocate new image data */
    u8* NewImageBuffer = new u8[ Size_.Width * Size_.Height * (Color.Alpha == 255 ? 3 : 4) ];
    u8 r, g, b;
    
    /* The alpha channel to determine if the transparency shall created or removed */
    if (Color.Alpha == 255)
    {
        /* Loop for the new image data */
        for (s32 y = 0, x; y < Size_.Height; ++y)
        {
            for (x = 0; x < Size_.Width; ++x)
            {
                /* Fill the new image data */
                r = NewImageBuffer[ (y * Size_.Width + x)*3 + 0 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 0 ];
                g = NewImageBuffer[ (y * Size_.Width + x)*3 + 1 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 1 ];
                b = NewImageBuffer[ (y * Size_.Width + x)*3 + 2 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 2 ];
            }
        }
        
        /* Set the internal format & type */
        Format_     = PIXELFORMAT_RGB;
        FormatSize_ = 3;
    }
    else
    {
        /* Loop for the new image data */
        for (s32 y = 0, x; y < Size_.Height; ++y)
        {
            for (x = 0; x < Size_.Width; ++x)
            {
                /* Fill the new image data */
                r = NewImageBuffer[ (y * Size_.Width + x)*4 + 0 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 0 ];
                g = NewImageBuffer[ (y * Size_.Width + x)*4 + 1 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 1 ];
                b = NewImageBuffer[ (y * Size_.Width + x)*4 + 2 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 2 ];
                NewImageBuffer[ (y*Size_.Width+x)*4+3 ] =
                    r == Color.Red && g == Color.Green && b == Color.Blue ? Color.Alpha : 255;
            }
        }
        
        /* Set the internal format & type */
        Format_     = PIXELFORMAT_RGBA;
        FormatSize_ = 4;
    }
    
    /* Update the new image data */
    replaceImageBuffer(NewImageBuffer);
}

void Texture::setColorKey(const video::color &Color, u8 Variation)
{
    /* Check if the image is not empty */
    if (!ImageBuffer_)
        return;
    
    /* Check if the current type is bad */
    if (FormatSize_ < 3)
        setFormat(PIXELFORMAT_RGB);
    
    /* Set the new transparency color */
    ColorKey_ = Color;
    
    /* Allocate new image data */
    u8* NewImageBuffer = new u8[ Size_.Width * Size_.Height * 4 ];
    u8 r, g, b;
    
    /* Loop for the new image data */
    for (s32 y = 0, x; y < Size_.Height; ++y)
    {
        for (x = 0; x < Size_.Width; ++x)
        {
            /* Fill the new image data */
            r = NewImageBuffer[ (y * Size_.Width + x)*4 + 0 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 0 ];
            g = NewImageBuffer[ (y * Size_.Width + x)*4 + 1 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 1 ];
            b = NewImageBuffer[ (y * Size_.Width + x)*4 + 2 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 2 ];
            
            /* Check the variation degree */
            if (math::Abs<s16>(s16(r - Color.Red    )) <= Variation &&
                math::Abs<s16>(s16(g - Color.Green  )) <= Variation &&
                math::Abs<s16>(s16(b - Color.Blue   )) <= Variation)
            {
                NewImageBuffer[ (y * Size_.Width + x)*4 + 3 ] = Color.Alpha;
            }
            else
                NewImageBuffer[ (y * Size_.Width + x)*4 + 3 ] = 255;
        }
    }
    
    /* Set the internal format & type */
    Format_ = PIXELFORMAT_RGBA;
    updateFormatSize();
    
    /* Update the new image data */
    replaceImageBuffer(NewImageBuffer);
}

void Texture::setColorKey(const dim::point2di &Pos, u8 Alpha, u8 Variation)
{
    const video::color Color(getPixelColor(Pos), Alpha);
    
    if (Variation == 0)
        setColorKey(Color);
    else
        setColorKey(Color, Variation);
}

void Texture::setColorKeyAlpha(const EAlphaBlendingTypes Mode)
{
    /* Check if the image is not empty */
    if (!ImageBuffer_)
        return;
    
    /* Check if the current type is bad */
    if (FormatSize_ < 3)
        setFormat(PIXELFORMAT_RGB);
    
    /* Set the new transparency color */
    ColorKey_ = video::color(Mode == BLENDING_BRIGHTNESS ? 0 : 255);
    
    /* Allocate new image data */
    u8* NewImageBuffer = new u8[ Size_.Width * Size_.Height * 4 ];
    u8 r, g, b;
    
    if (Mode == BLENDING_BRIGHTNESS)
    {
        /* Loop for the new image data */
        for (s32 y = 0, x; y < Size_.Height; ++y)
        {
            for (x = 0; x < Size_.Width; ++x)
            {
                /* Fill the new image data */
                r = NewImageBuffer[ (y * Size_.Width + x)*4 + 0 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 0 ];
                g = NewImageBuffer[ (y * Size_.Width + x)*4 + 1 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 1 ];
                b = NewImageBuffer[ (y * Size_.Width + x)*4 + 2 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 2 ];
                NewImageBuffer[ (y * Size_.Width + x)*4 + 3 ] = static_cast<u8>( video::color(r, g, b).getBrightness<s32>() );
            }
        }
    }
    else if (Mode == BLENDING_DARKNESS)
    {
        /* Loop for the new image data */
        for (s32 y = 0, x; y < Size_.Height; ++y)
        {
            for (x = 0; x < Size_.Width; ++x)
            {
                /* Fill the new image data */
                r = NewImageBuffer[ (y * Size_.Width + x)*4 + 0 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 0 ];
                g = NewImageBuffer[ (y * Size_.Width + x)*4 + 1 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 1 ];
                b = NewImageBuffer[ (y * Size_.Width + x)*4 + 2 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 2 ];
                NewImageBuffer[ (y * Size_.Width + x)*4 + 3 ] = static_cast<u8>( video::color(255 - r, 255 - g, 255 - b).getBrightness<s32>() );
            }
        }
    }
    
    /* Set the internal format & type */
    Format_ = PIXELFORMAT_RGBA;
    updateFormatSize();
    
    /* Update the new image data */
    replaceImageBuffer(NewImageBuffer);
}

void Texture::setColorKeyMask(Texture* MaskTexture, const EAlphaBlendingTypes Mode)
{
    /* Check if the image is not empty */
    if (!ImageBuffer_ || !MaskTexture || !MaskTexture->ImageBuffer_)
        return;
    
    /* Check if the current type is bad */
    if (FormatSize_ < 3)
        setFormat(PIXELFORMAT_RGB);
    
    /* Set the new transparency color */
    ColorKey_ = video::color(Mode == BLENDING_BRIGHTNESS ? 0 : 255);
    
    /* Get the mask image buffer */
    const u8* MaskImageBuffer = MaskTexture->getImageBuffer();
    
    /* Get the mask image size */
    dim::size2di MaskSize(MaskTexture->getSize());
    
    /* Get the internal format of the mask texture */
    s32 MaskFormatSize = MaskTexture->getFormatSize();
    
    /* Check if the mask texture type is bad */
    if (MaskFormatSize < 3)
        MaskTexture->setFormat(PIXELFORMAT_RGB);
    
    /* Allocate new image data */
    u8* NewImageBuffer = new u8[ Size_.Width * Size_.Height * 4 ];
    u8 r, g, b, a;
    
    /* Loop for the new image data */
    for (s32 y = 0, x, i; y < Size_.Height; ++y)
    {
        for (x = 0; x < Size_.Width; ++x)
        {
            /* Fill the new image data */
            NewImageBuffer[ ( (y * Size_.Width + x) << 2 ) + 0 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 0 ];
            NewImageBuffer[ ( (y * Size_.Width + x) << 2 ) + 1 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 1 ];
            NewImageBuffer[ ( (y * Size_.Width + x) << 2 ) + 2 ] = ImageBuffer_[ (y * Size_.Width + x)*FormatSize_ + 2 ];
            
            /* Get the mask image data */
            if (MaskSize == Size_)
                i = (y * Size_.Width + x) * MaskFormatSize;
            else
                i = ( (y*MaskSize.Height/Size_.Height)*MaskSize.Width + (x*MaskSize.Width/Size_.Width) ) * MaskFormatSize;
            
            r = MaskImageBuffer[i+0];
            g = MaskImageBuffer[i+1];
            b = MaskImageBuffer[i+2];
            
            /* Get the alpha channel */
            if (Mode == BLENDING_BRIGHTNESS)
                a = ( r + g + b ) / 3;
            else if (Mode == BLENDING_DARKNESS)
                a = 255 - ( r + g + b ) / 3;
            
            /* Fill the new alpha channel */
            NewImageBuffer[ ( (y*Size_.Width+x) << 2 ) + 3 ] = a;
        }
    }
    
    /* Set the internal format & type */
    Format_ = PIXELFORMAT_RGBA;
    updateFormatSize();
    
    /* Update the new image data */
    replaceImageBuffer(NewImageBuffer);
}

bool Texture::isSizePOT() const
{
    return math::RoundPow2(Size_.Width) == Size_.Width && math::RoundPow2(Size_.Height) == Size_.Height;
}
dim::size2di Texture::getSizePOT() const
{
    return dim::size2di(math::RoundPow2(Size_.Width), math::RoundPow2(Size_.Height));
}
void Texture::setSizePOT()
{
    if (!isSizePOT())
        setSize(getSizePOT());
}

void Texture::setSize(const dim::size2di &Size)
{
    /* Check if the image is not empty */
    if (!ImageBuffer_ || Size_ == Size || Size.Width <= 0 || Size.Height <= 0)
        return;
    
    /* Scale the image data */
    ImageConverter::scaleImage(
        ImageBuffer_, Size_.Width, Size_.Height, Size.Width, Size.Height, FormatSize_
    );
    
    /* Set the new size */
    Size_ = Size;
    
    /* Recreate the image */
    updateImageBuffer();
}

video::color Texture::getPixelColor(const dim::point2di &Pos) const
{
    /* Check the correct image data range */
    if (ImageBuffer_ && Pos.X >= 0 && Pos.Y >= 0 && Pos.X < Size_.Width && Pos.Y < Size_.Height)
    {
        const s32 i = (Pos.Y * Size_.Width + Pos.X) * FormatSize_;
        
        switch (FormatSize_)
        {
            case 1:
                return video::color(ImageBuffer_[i], ImageBuffer_[i], ImageBuffer_[i]);
            case 2:
                return video::color(ImageBuffer_[i], ImageBuffer_[i], ImageBuffer_[i], ImageBuffer_[i+1]);
            case 3:
                return video::color(ImageBuffer_[i], ImageBuffer_[i+1], ImageBuffer_[i+2]);
            case 4:
                return video::color(ImageBuffer_[i], ImageBuffer_[i+1], ImageBuffer_[i+2], ImageBuffer_[i+3]);
        }
    }
    
    return video::emptycolor;
}

void Texture::setPixelColor(const dim::point2di &Pos, const video::color &Color)
{
    /* Check the correct image data range */
    if (ImageBuffer_ && Pos.X >= 0 && Pos.Y >= 0 && Pos.X < Size_.Width && Pos.Y < Size_.Height)
    {
        s32 i = (Pos.Y * Size_.Width + Pos.X) * FormatSize_;
        
        switch (FormatSize_)
        {
            case 1:
                ImageBuffer_[i] = (Color.Red + Color.Green + Color.Blue) / 3;
                break;
                
            case 2:
                ImageBuffer_[i  ] = (Color.Red + Color.Green + Color.Blue) / 3;
                ImageBuffer_[i+1] = Color.Alpha;
                break;
                
            case 3:
                ImageBuffer_[i  ] = Color.Red;
                ImageBuffer_[i+1] = Color.Green;
                ImageBuffer_[i+2] = Color.Blue;
                break;
                
            case 4:
                ImageBuffer_[i  ] = Color.Red;
                ImageBuffer_[i+1] = Color.Green;
                ImageBuffer_[i+2] = Color.Blue;
                ImageBuffer_[i+3] = Color.Alpha;
                break;
        }
    }
}

video::color Texture::getPixelColor(const dim::point2df &Pos) const
{
    return getPixelColor(
        dim::point2di(
            (s32)( ( Pos.X - static_cast<s32>(Pos.X) ) * Size_.Width ),
            (s32)( ( Pos.Y - static_cast<s32>(Pos.Y) ) * Size_.Height )
        )
    );
}

void Texture::setPixelColor(const dim::point2df &Pos, const video::color &Color)
{
    setPixelColor(
        dim::point2di(
            (s32)( ( Pos.X - static_cast<s32>(Pos.X) ) * Size_.Width ),
            (s32)( ( Pos.Y - static_cast<s32>(Pos.Y) ) * Size_.Height )
        ),
        Color
    );
}

void Texture::flipColors()
{
    /* Check if the image is not empty */
    if (!ImageBuffer_)
        return;
    
    /* Determine the image data size */
    const s32 ImageBufferSize = Size_.Width * Size_.Height * FormatSize_;
    
    /* Flip the image data */
    for (s32 i = 0; i < ImageBufferSize; ++i)
        ImageBuffer_[i] = 255 - ImageBuffer_[i];
    
    /* Recreate the image */
    updateImageBuffer();
}

// !!! MUST BE UPDATED (using the TextureManager) !!!
void Texture::mixTexture(Texture* Tex, const ETextureMixTypes Type)
{
    /* Check if the image is not empty */
    if (!ImageBuffer_)
        return;
    
    /* Get the other texture information */
    const u8* OtherImageBuffer    = Tex->getImageBuffer();
    dim::size2di OtherSize  = Tex->getSize();
    
    /* Temporary vairables */
    register s32 x, y, i, j, k;
    
    /* Determine the type of texture mixing */
    if (Type == TEXTUREMIX_ADD)
    {
        for (y = 0; y < Size_.Height; ++y)
        {
            for (x = 0; x < Size_.Width; ++x)
            {
                for (i = 0; i < 3; ++i)
                {
                    /* Determine the indices */
                    j = FormatSize_ * ( y * Size_.Width + x ) + i;
                    k = FormatSize_ * ( y * OtherSize.Height/Size_.Height*OtherSize.Width + x*OtherSize.Width/Size_.Width ) + i;
                    
                    /* Fill the image data */
                    ImageBuffer_[j] = ( ImageBuffer_[j] + OtherImageBuffer[k] ) / 2;
                }
            }
        }
    }
    else if (Type == TEXTUREMIX_MULTIPLY)
    {
        for (y = 0; y < Size_.Height; ++y)
        {
            for (x = 0; x < Size_.Width; ++x)
            {
                for (i = 0; i < 3; ++i)
                {
                    /* Determine the indices */
                    j = FormatSize_ * ( y * Size_.Width + x ) + i;
                    k = FormatSize_ * ( y * OtherSize.Height/Size_.Height*OtherSize.Width + x*OtherSize.Width/Size_.Width ) + i;
                    
                    /* Fill the image data */
                    ImageBuffer_[j] = ( ImageBuffer_[j] * OtherImageBuffer[k] ) / 255;
                }
            }
        }
    }
    
    /* Recreate the image */
    updateImageBuffer();
}

void Texture::setColorIntensity(f32 Red, f32 Green, f32 Blue) { }

void Texture::setAlphaIntensity(u8 Alpha, const ETextureMixTypes Type)
{
    /* Check the conditions */
    if (!ImageBuffer_)
        return;
    
    /* Clamp the type */
    if (FormatSize_ == 1)
        setFormat(PIXELFORMAT_GRAYALPHA);
    else if (FormatSize_ == 3)
        setFormat(PIXELFORMAT_RGBA);
    
    /* Temporary variables */
    s32 x, y, i = FormatSize_ - 1;
    
    /* Select the mixing type */
    switch (Type)
    {
        case TEXTUREMIX_ADD:
        {
            /* Modify the alpha channel */
            for (y = 0; y < Size_.Height; ++y)
                for (x = 0; x < Size_.Width; ++x, i += FormatSize_)
                    ImageBuffer_[i] += Alpha;
        }
        break;
        
        case TEXTUREMIX_SET:
        {
            /* Modify the alpha channel */
            for (y = 0; y < Size_.Height; ++y)
                for (x = 0; x < Size_.Width; ++x, i += FormatSize_)
                    ImageBuffer_[i] = Alpha;
        }
        break;
        
        case TEXTUREMIX_MULTIPLY:
        {
            /* Modify the alpha channel */
            for (y = 0; y < Size_.Height; ++y)
                for (x = 0; x < Size_.Width; ++x, i += FormatSize_)
                    ImageBuffer_[i] = u8( (s32)ImageBuffer_[i] * Alpha / 255 );
        }
        break;
    }
    
    /* Recreate the image */
    updateImageBuffer();
}

void Texture::addMultiRenderTarget(Texture* Tex)
{
    if (Tex)
    {
        MultiRenderTargetList_.push_back(Tex);
        updateMultiRenderTargets();
    }
}
void Texture::removeMultiRenderTarget(Texture* Tex)
{
    if (Tex)
    {
        MemoryManager::removeElement(MultiRenderTargetList_, Tex);
        updateMultiRenderTargets();
    }
}
void Texture::clearMultiRenderTarget()
{
    if (MultiRenderTargetList_.empty())
    {
        MultiRenderTargetList_.clear();
        updateMultiRenderTargets();
    }
}

void Texture::setHardwareFormat(const EHWTextureFormats HardwareFormat)
{
    HWFormat_ = HardwareFormat;
    updateImageBuffer();
}

void Texture::flipImageHorizontal()
{
    /* Check if the image is not empty */
    if (!ImageBuffer_)
        return;
    
    /* Flip image data on the x-axis */
    ImageConverter::flipImageHorz(ImageBuffer_, Size_.Width, Size_.Height, FormatSize_);
    
    /* Recreate the image */
    updateImageBuffer();
}

void Texture::flipImageVertical()
{
    /* Check if the image is not empty */
    if (!ImageBuffer_)
        return;
    
    /* Flip image data on the y-axis */
    ImageConverter::flipImageVert(ImageBuffer_, Size_.Width, Size_.Height, FormatSize_);
    
    /* Recreate the image */
    updateImageBuffer();
}

void Texture::turnImage(const EImageTurnDegrees Degree)
{
    /* Check if the image is not empty */
    if (!ImageBuffer_)
        return;
    
    /* Turn image data */
    ImageConverter::turnImage(ImageBuffer_, Size_.Width, Size_.Height, FormatSize_, Degree);
    
    /* Recreate the image */
    updateImageBuffer();
}

void Texture::setMipMapping(bool MipMaps)
{
    if (MipMaps_ != MipMaps)
    {
        MipMaps_ = MipMaps;
        updateImageBuffer();
    }
}

void Texture::setFormat(const EPixelFormats Format)
{
    /* Check if the image is not empty and the type is new */
    if (!ImageBuffer_ || Format_ == Format)
        return;
    
    const s32 NewFormatSize = getFormatSize(Format);
    
    if (NewFormatSize != FormatSize_)
    {
        /* Convert the image data */
        ImageConverter::convertImageFormat(ImageBuffer_, Size_.Width, Size_.Height, FormatSize_, NewFormatSize);
        
        /* Update format */
        Format_     = Format;
        FormatSize_ = NewFormatSize;
        
        /* Recreate image */
        updateImageBuffer();
    }
}

void Texture::saveBackup()
{
    /* Clear the old backup if exist */
    clearBackup();
    
    /* Save current dimensions */
    FormatBackup_   = Format_;
    SizeBackup_     = Size_;
    
    const u32 ImageBufferSize = SizeBackup_.Width * SizeBackup_.Height * getFormatSize(FormatBackup_);
    
    /* Allocate new memory for image buffer */
    ImageBufferBackup_ = new u8[ImageBufferSize];
    
    /* Copy the image buffer */
    memcpy(ImageBufferBackup_, ImageBuffer_, ImageBufferSize);
}

void Texture::loadBackup()
{
    if (!ImageBufferBackup_)
        return;
    
    /* Get the image buffer size */
    const u32 ImageBufferSize = SizeBackup_.Width * SizeBackup_.Height * getFormatSize(FormatBackup_);
    
    /* Check if the image buffer needs to be reallocated */
    if (FormatBackup_ != Format_ || SizeBackup_ != Size_)
    {
        /* Delete the old image buffer */
        MemoryManager::deleteBuffer(ImageBuffer_);
        
        /* Allocate new memory for the image buffer */
        ImageBuffer_ = new u8[ImageBufferSize];
        
        /* Reset the dimensions */
        Size_   = SizeBackup_;
        Format_ = FormatBackup_;
        updateFormatSize();
    }
    
    /* Copy the image buffer */
    memcpy(ImageBuffer_, ImageBufferBackup_, ImageBufferSize);
    
    /* Recreate the image */
    updateImageBuffer();
}

void Texture::clearBackup()
{
    MemoryManager::deleteBuffer(ImageBufferBackup_);
}

void Texture::addTextureFrame(Texture* NewFrame, const ETextureFrameDirection FrameDirection)
{
    /* Check if the image is not empty and the same for the new texture frame */
    if (!ImageBuffer_ || !NewFrame || !NewFrame->getImageBuffer())
        return;
    
    /* Temporary variables */
    register s32 x, y, i, j, k;
    
    /* Get the temporary image data size */
    u32 ImageBufferSize = NewFrame->Size_.Width * NewFrame->Size_.Height * NewFrame->FormatSize_;
    
    /* Allocate temporary image data */
    u8* TmpImageBuffer = new u8[ImageBufferSize];
    
    /* Copy the image data */
    memcpy(TmpImageBuffer, NewFrame->getImageBuffer(), ImageBufferSize);
    
    /* Clamp the texture frame size to this */
    if (FrameDirection == TEXTURE_FRAME_HORIZONTAL && NewFrame->Size_.Height != Size_.Height)
    {
        /* Scale the image data of the texture frame */
        ImageConverter::scaleImage(
            TmpImageBuffer, NewFrame->Size_.Width, NewFrame->Size_.Height,
            NewFrame->Size_.Width, Size_.Height, NewFrame->FormatSize_
        );
    }
    else if (FrameDirection == TEXTURE_FRAME_VERTICAL && NewFrame->Size_.Width != Size_.Width)
    {
        /* Scale the image data of the texture frame */
        ImageConverter::scaleImage(
            TmpImageBuffer, NewFrame->Size_.Width, NewFrame->Size_.Height,
            Size_.Width, NewFrame->Size_.Height, NewFrame->FormatSize_
        );
    }
    
    /* Clamp the texture frame format to this */
    if (NewFrame->FormatSize_ != FormatSize_)
    {
        /* Convert the image data */
        ImageConverter::convertImageFormat(TmpImageBuffer, Size_.Width, Size_.Height, NewFrame->Format_, Format_);
    }
    
    /* Get the new texture size */
    dim::size2di NewSize;
    dim::point2di Translation;
    
    if (FrameDirection == TEXTURE_FRAME_HORIZONTAL)
    {
        NewSize     = dim::size2di(Size_.Width + NewFrame->Size_.Width, Size_.Height);
        Translation = dim::point2di(Size_.Width, 0);
    }
    else if (FrameDirection == TEXTURE_FRAME_VERTICAL)
    {
        NewSize     = dim::size2di(Size_.Width, Size_.Height + NewFrame->Size_.Height);
        Translation = dim::point2di(0, Size_.Height);
    }
    
    /* Get the new image data size */
    ImageBufferSize = NewSize.Width * NewSize.Height * FormatSize_;
    
    /* Allocate new image data */
    u8* NewImageBuffer = new u8[ImageBufferSize];
    
    /* Copy the image data of this texture */
    for (y = 0; y < Size_.Height; ++y)
    {
        for (x = 0; x < Size_.Width; ++x)
        {
            for (i = 0; i < FormatSize_; ++i)
            {
                j = ( y * NewSize.Width + x ) * FormatSize_ + i;
                k = ( y * Size_.Width + x ) * FormatSize_ + i;
                
                NewImageBuffer[j] = ImageBuffer_[k];
            }
        }
    }
    
    /* Copy the image data of the new texture frame */
    for (y = 0; y < NewFrame->Size_.Height; ++y)
    {
        for (x = 0; x < NewFrame->Size_.Width; ++x)
        {
            for (i = 0; i < FormatSize_; ++i)
            {
                j = ( ( y + Translation.Y ) * NewSize.Width + Translation.X + x ) * FormatSize_ + i;
                k = ( y * NewFrame->Size_.Width + x ) * FormatSize_ + i;
                
                NewImageBuffer[j] = TmpImageBuffer[k];
            }
        }
    }
    
    /* Set the new texture size */
    Size_ = NewSize;
    
    /* Use the new image buffer */
    replaceImageBuffer(NewImageBuffer);
    
    /* Delete the temporary image buffers */
    MemoryManager::deleteBuffer(TmpImageBuffer);
}


/* === Dimension === */

void Texture::setDimension(const ETextureDimensions Type, s32 Depth)
{
    if (DimensionType_ != Type || (Type == TEXTURE_3D && Depth > 0 && Depth != Depth_))
    {
        DimensionType_ = Type;
        
        if (DimensionType_ == TEXTURE_CUBEMAP)
            Depth_ = 6;
        else if (DimensionType_ == TEXTURE_3D)
            Depth_ = Depth;
        else
            Depth_ = 1;
        
        updateHeightDepthRatio();
        updateImageBuffer();
    }
}

void Texture::setCubeMapFace(const ECubeMapDirections Face)
{
    CubeMapFace_ = Face;
}

void Texture::setRenderTarget(bool Enable)
{
    if (isRenderTarget_ != Enable)
    {
        isRenderTarget_ = Enable;
        if (__spVideoDriver->RenderQuery_[RenderSystem::RENDERQUERY_RENDERTARGET])
            updateImageBuffer();
    }
}

void Texture::setDepth(s32 Depth)
{
    if (DimensionType_ == TEXTURE_3D && Depth > 0)
    {
        Depth_ = Depth;
        updateHeightDepthRatio();
        updateImageBuffer();
    }
}

void Texture::setMultiSamples(s32 Samples)
{
    if (Samples != MultiSamples_)
    {
        MultiSamples_ = math::Max(0, Samples);
        if (isRenderTarget_ && __spVideoDriver->RenderQuery_[RenderSystem::RENDERQUERY_MULTISAMPLE_RENDERTARGET])
            updateImageBuffer();
    }
}


/* === Animation === */

void Texture::addAnimFrame(video::Texture* AnimFrame)
{
    if (AnimFrame)
        AnimFrameList_.push_back(AnimFrame);
}
void Texture::removeAnimFrame(video::Texture* AnimFrame)
{
    for (std::vector<video::Texture*>::iterator it = AnimFrameList_.begin(); it != AnimFrameList_.end(); ++it)
    {
        if (*it == AnimFrame)
        {
            AnimFrameList_.erase(it);
            break;
        }
    }
}

void Texture::setAnimation(bool Enable)
{
    if (isAnim_ = Enable && !AnimFrameList_.empty())
        ID_ = AnimFrameList_[0]->OrigID_;
    else
        ID_ = OrigID_;
}

void Texture::setAnimFrame(u32 Frame)
{
    if (isAnim_ && Frame < AnimFrameList_.size())
        ID_ = AnimFrameList_[Frame]->OrigID_;
}


/* === Filter, MipMapping, Texture coordinate wraps === */

void Texture::setFilter(const ETextureFilters Filter)
{
    MagFilter_ = MinFilter_ = Filter;
}
void Texture::setFilter(const ETextureFilters MagFilter, const ETextureFilters MinFilter)
{
    MagFilter_ = MagFilter;
    MinFilter_ = MinFilter;
}
void Texture::setMagFilter(const ETextureFilters Filter)
{
    MagFilter_ = Filter;
}
void Texture::setMinFilter(const ETextureFilters Filter)
{
    MinFilter_ = Filter;
}

void Texture::setMipMapFilter(const ETextureMipMapFilters MipMapFilter)
{
    MipMapFilter_ = MipMapFilter;
}

void Texture::setWrapMode(const ETextureWrapModes Wrap)
{
    WrapMode_.X = WrapMode_.Y = WrapMode_.Z = Wrap;
}
void Texture::setWrapMode(
    const ETextureWrapModes WrapU, const ETextureWrapModes WrapV, const ETextureWrapModes WrapW)
{
    WrapMode_.X = WrapU;
    WrapMode_.Y = WrapV;
    WrapMode_.Z = WrapW;
}

void Texture::shareImageBuffer() { }
void Texture::updateImageBuffer() { }

void Texture::updateImageBuffer(const u8* ImageBuffer)
{
    /* Check if the new image buffer is valid */
    if (!ImageBuffer)
        return;
    
    /* Delete the old image buffer */
    MemoryManager::deleteBuffer(ImageBuffer_);
    
    /* Get the image buffer size */
    const s32 ImageBufferSize = Size_.Width * Size_.Height * FormatSize_;
    
    /* Allocate new memory for the image buffer */
    ImageBuffer_ = new u8[ImageBufferSize];
    
    /* Copy the image buffer */
    memcpy(ImageBuffer_, ImageBuffer, ImageBufferSize);
    
    /* Recreate the image */
    updateImageBuffer();
}

void Texture::updateImageBuffer(const dim::size2di &Size, const u8* ImageBuffer)
{
    /* Set the new size and update the image buffer */
    Size_ = Size;
    updateImageBuffer(ImageBuffer);
}

void Texture::updateImageBuffer(const dim::point2di &Position, const dim::size2di &Size, const u8* ImageBuffer)
{
    /* Check if the image buffers and location are valid */
    if (!ImageBuffer_ || !ImageBuffer ||
        Position.X < 0 || Position.X + Size.Width >= Size_.Width ||
        Position.Y < 0 || Position.Y + Size.Height >= Size_.Height)
    {
        return;
    }
    
    /* Temporary variables */
    s32 x, y, i, j, k = 0;
    
    const dim::rect2di Rect(
        Position.X, Position.Y, Position.X + Size.Width, Position.Y + Size.Height
    );
    
    /* Loop for each pixel which is to be replaced */
    for (y = Rect.Top; y < Rect.Bottom; ++y)
    {
        j = ( y * Size_.Width + Rect.Left ) * FormatSize_;
        
        for (x = Rect.Left; x < Rect.Right; ++x)
        {
            for (i = 0; i < FormatSize_; ++i, ++j, ++k)
                ImageBuffer_[j] = ImageBuffer[k];
        }
    }
    
    /* Recreate the image */
    updateImageBuffer();
}

void Texture::generateMipMap() { }

void Texture::bind(s32 Level) const { }
void Texture::unbind(s32 Level) const { }


/*
 * ======= Protected: =======
 */

void Texture::init()
{
    /* Identification */
    Filename_           = "";
    Depth_              = 1;
    
    /* Pixel format */
    Format_             = PIXELFORMAT_RGB;
    FormatBackup_       = Format_;
    updateFormatSize();
    HWFormat_           = HWTEXFORMAT_UBYTE8;
    
    /* Filter mode */
    MagFilter_          = FILTER_SMOOTH;
    MinFilter_          = FILTER_SMOOTH;
    MipMapFilter_       = FILTER_TRILINEAR;
    AnisotropicSamples_ = 1;
    MultiSamples_       = 0;
    MipMaps_            = true;
    
    /* Further settings */
    ColorKey_           = video::color(0, 0, 0, 255);
    isRenderTarget_     = false;
    isAnim_             = false;
    CubeMapFace_        = CUBEMAP_POSITIVE_X;
}

void Texture::replaceImageBuffer(u8* NewImageBuffer)
{
    MemoryManager::deleteBuffer(ImageBuffer_);
    
    ImageBuffer_ = NewImageBuffer;
    
    updateImageBuffer();
}

void Texture::updateFormatSize()
{
    FormatSize_ = STDPixelFormatDataSize[Format_];
}
s32 Texture::getFormatSize(const EPixelFormats Format) const
{
    return STDPixelFormatDataSize[Format];
}

void Texture::updateMultiRenderTargets()
{
}

void Texture::updateHeightDepthRatio()
{
    if (Depth_ > 0)
    {
        const s32 Remainder = Size_.Height % Depth_;
        
        if (Remainder)
        {
            const s32 HeightLow     = Size_.Height - Remainder;
            const s32 HeightHigh    = Size_.Height - Remainder + Depth_;
            
            if (Size_.Height - HeightLow <= HeightHigh - Size_.Height)
                setSize(dim::size2di(Size_.Width, HeightLow));
            else
                setSize(dim::size2di(Size_.Width, HeightHigh));
        }
    }
}


} // /namespace video

} // /namespace sp



// ================================================================================
