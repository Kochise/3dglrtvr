/*
 * Image buffer file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Base/spImageBuffer.hpp"


namespace sp
{
namespace video
{


ImageBuffer::ImageBuffer()
    : ImageBuffer_(0), Depth_(0), DataType_(DATATYPE_UNSIGNED_BYTE),
    Format_(PIXELFORMAT_RGB), FormatSize_(ImageBuffer::getFormatSize(Format_))
{
}
ImageBuffer::ImageBuffer(ImageBuffer* Buffer)
    : ImageBuffer_(0), Depth_(0), DataType_(DATATYPE_UNSIGNED_BYTE),
    Format_(PIXELFORMAT_RGB), FormatSize_(ImageBuffer::getFormatSize(Format_))
{
    if (Buffer)
    {
        /* Copy configuration */
        Size_       = Buffer->Size_;
        Depth_      = Buffer->Depth_;
        DataType_   = Buffer->DataType_;
        Format_     = Buffer->Format_;
        FormatSize_ = Buffer->FormatSize_;
        
        copyBuffer(Buffer);
    }
}
ImageBuffer::ImageBuffer(
    const EPixelFormats Format, const dim::size2di &Size, u32 Depth, const ERendererDataTypes DataType)
    : ImageBuffer_(0), Size_(Size), Depth_(Depth), DataType_(DataType),
    Format_(Format), FormatSize_(ImageBuffer::getFormatSize(Format_))
{
    createBuffer();
    clearBuffer();
}
ImageBuffer::ImageBuffer(
    const u8* Buffer, const EPixelFormats Format, const dim::size2di &Size, u32 Depth)
    : ImageBuffer_(0), Size_(Size), Depth_(Depth), DataType_(DATATYPE_UNSIGNED_BYTE),
    Format_(Format), FormatSize_(ImageBuffer::getFormatSize(Format_))
{
    copyBuffer(Buffer);
}
ImageBuffer::ImageBuffer(
    const f32* Buffer, const EPixelFormats Format, const dim::size2di &Size, u32 Depth)
    : ImageBuffer_(0), Size_(Size), Depth_(Depth), DataType_(DATATYPE_FLOAT),
    Format_(Format), FormatSize_(ImageBuffer::getFormatSize(Format_))
{
    copyBuffer(Buffer);
}
ImageBuffer::~ImageBuffer()
{
    freeRawBuffer(ImageBuffer_);
}

void ImageBuffer::setFormat(const EPixelFormats Format)
{
    //todo
    
    Format_     = Format;
    FormatSize_ = ImageBuffer::getFormatSize(Format_);
}

void ImageBuffer::setDataType(const ERendererDataTypes DataType)
{
    //todo
    
    DataType_ = DataType;
}

u32 ImageBuffer::getDataTypeSize() const
{
    return DataType_ == DATATYPE_FLOAT ? 4 : 1;
}

void ImageBuffer::setPixelColor(const dim::point2di &Pos, const video::color &Color)
{
    /* Check the correct image data range */
    if (!ImageBuffer_ || Pos.X < 0 || Pos.Y < 0 || Pos.X >= Size_.Width || Pos.Y >= Size_.Height)
        return;
    
    const s32 Offset = (Pos.Y * Size_.Width + Pos.X) * FormatSize_;
    
    switch (DataType_)
    {
        case DATATYPE_UNSIGNED_BYTE:
        {
            u8* Buffer = static_cast<u8*>(ImageBuffer_);
            
            switch (FormatSize_)
            {
                case 1:
                {
                    Buffer[Offset    ] = Color.getBrightness<u8>();
                }
                break;
                
                case 2:
                {
                    Buffer[Offset    ] = Color.getBrightness<u8>();
                    Buffer[Offset + 1] = Color.Alpha;
                }
                break;
                
                case 3:
                {
                    Buffer[Offset    ] = Color.Red;
                    Buffer[Offset + 1] = Color.Green;
                    Buffer[Offset + 2] = Color.Blue;
                }
                break;
                
                case 4:
                {
                    Buffer[Offset    ] = Color.Red;
                    Buffer[Offset + 1] = Color.Green;
                    Buffer[Offset + 2] = Color.Blue;
                    Buffer[Offset + 3] = Color.Alpha;
                }
                break;
                
            }
        }
        break;
        
        case DATATYPE_FLOAT:
        {
            f32* Buffer = static_cast<f32*>(ImageBuffer_);
            
            switch (FormatSize_)
            {
                case 1:
                {
                    Buffer[Offset    ] = Color.getBrightness<f32>() / 255;
                }
                break;
                
                case 2:
                {
                    Buffer[Offset    ] = Color.getBrightness<f32>() / 255;
                    Buffer[Offset + 1] = static_cast<f32>(Color.Alpha) / 255;
                }
                break;
                
                case 3:
                {
                    f32 FltColor[4];
                    Color.getFloatArray(FltColor);
                    
                    Buffer[Offset    ] = FltColor[0];
                    Buffer[Offset + 1] = FltColor[1];
                    Buffer[Offset + 2] = FltColor[2];
                }
                break;
                
                case 4:
                {
                    f32 FltColor[4];
                    Color.getFloatArray(FltColor);
                    
                    Buffer[Offset    ] = FltColor[0];
                    Buffer[Offset + 1] = FltColor[1];
                    Buffer[Offset + 2] = FltColor[2];
                    Buffer[Offset + 3] = FltColor[3];
                }
                break;
            }
        }
        break;
    }
}

video::color ImageBuffer::getPixelColor(const dim::point2di &Pos) const
{
    /* Check the correct image data range */
    if (!ImageBuffer_ || Pos.X < 0 || Pos.Y < 0 || Pos.X >= Size_.Width || Pos.Y >= Size_.Height)
        return video::emptycolor;
    
    const s32 Offset = (Pos.Y * Size_.Width + Pos.X) * FormatSize_;
    
    switch (DataType_)
    {
        case DATATYPE_UNSIGNED_BYTE:
        {
            u8* Buffer = static_cast<u8*>(ImageBuffer_);
            
            switch (FormatSize_)
            {
                case 1:
                    return video::color(Buffer[Offset], Buffer[Offset], Buffer[Offset]);
                case 2:
                    return video::color(Buffer[Offset], Buffer[Offset], Buffer[Offset], Buffer[Offset + 1]);
                case 3:
                    return video::color(Buffer[Offset], Buffer[Offset + 1], Buffer[Offset + 2]);
                case 4:
                    return video::color(Buffer[Offset], Buffer[Offset + 1], Buffer[Offset + 2], Buffer[Offset + 3]);
            }
        }
        break;
        
        case DATATYPE_FLOAT:
        {
            f32* Buffer = static_cast<f32*>(ImageBuffer_);
            
            switch (FormatSize_)
            {
                case 1:
                    return dim::vector3df(Buffer[Offset], Buffer[Offset], Buffer[Offset]);
                case 2:
                    return dim::vector4df(Buffer[Offset], Buffer[Offset], Buffer[Offset], Buffer[Offset + 1]);
                case 3:
                    return dim::vector3df(Buffer[Offset], Buffer[Offset + 1], Buffer[Offset + 2]);
                case 4:
                    return dim::vector4df(Buffer[Offset], Buffer[Offset + 1], Buffer[Offset + 2], Buffer[Offset + 3]);
            }
        }
        break;
    }
    
    return video::emptycolor;
}

u32 ImageBuffer::getFormatSize(const EPixelFormats Format)
{
    switch (Format)
    {
        case PIXELFORMAT_DEPTH:
        case PIXELFORMAT_GRAY:
            return 1;
        case PIXELFORMAT_GRAYALPHA:
            return 2;
        case PIXELFORMAT_RGB:
        case PIXELFORMAT_BGR:
            return 3;
        case PIXELFORMAT_RGBA:
        case PIXELFORMAT_BGRA:
            return 4;
    }
    return 0;
}


/*
 * ======= Private: =======
 */

bool ImageBuffer::allocRawBuffer(void* &Buffer, const u32 PixelCount) const
{
    switch (DataType_)
    {
        case DATATYPE_UNSIGNED_BYTE:
            Buffer = new u8[PixelCount * getFormatSize()];
            return true;
        case DATATYPE_FLOAT:
            Buffer = new f32[PixelCount * getFormatSize()];
            return true;
    }
    return false;
}

void ImageBuffer::freeRawBuffer(void* &Buffer)
{
    if (Buffer)
    {
        delete [] static_cast<u8*>(Buffer);
        Buffer = 0;
    }
}

void ImageBuffer::createBuffer()
{
    freeRawBuffer(ImageBuffer_);
    allocRawBuffer(ImageBuffer_, getPixelCount());
}
void ImageBuffer::clearBuffer()
{
    if (ImageBuffer_)
        memset(ImageBuffer_, 0, getBufferSize());
}
void ImageBuffer::fillBuffer(const video::color &Color)
{
    if (ImageBuffer_)
    {
        void* SrcBuffer = 0;
        if (allocRawBuffer(SrcBuffer, 1))
        {
            //todo -> set SrcBuffer color
            
            const u32 Size = getBufferSize();
            const u32 Step = getPixelSize();
            
            for (u32 Offset = 0; Offset < Size; Offset += Step)
                memcpy(static_cast<u8*>(ImageBuffer_) + Offset, SrcBuffer, Step);
            
            freeRawBuffer(SrcBuffer);
        }
    }
}

void ImageBuffer::copyBuffer(const void* Buffer)
{
    if (Buffer)
    {
        createBuffer();
        memcpy(ImageBuffer_, Buffer, getBufferSize());
    }
}


} // /namespace video

} // /namespace sp



// ================================================================================
