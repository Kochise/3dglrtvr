/*
 * Image buffer header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_VIDEO_IMAGEBUFFER_H__
#define __SP_VIDEO_IMAGEBUFFER_H__


#include "Base/spStandard.hpp"
#include "Base/spDimension.hpp"
#include "Base/spImageManagement.hpp"
#include "Base/spMath.hpp"
#include "Base/spMaterialColor.hpp"
#include "FileFormats/Image/spImageFormatInterfaces.hpp"


namespace sp
{
namespace video
{


//! Data types for the renderer (vertex- and index buffer).
enum ERendererDataTypes
{
    DATATYPE_FLOAT,             //!< 32bit floating-point.
    DATATYPE_DOUBLE,            //!< 64bit fliating-point. This data type is not supported for OpenGL|ES.
    DATATYPE_BYTE,              //!< 8bit integer.
    DATATYPE_SHORT,             //!< 16bit integer.
    DATATYPE_INT,               //!< 32bit integer.
    DATATYPE_UNSIGNED_BYTE,     //!< 8bit unsigned integer.
    DATATYPE_UNSIGNED_SHORT,    //!< 16bit unsigned integer.
    DATATYPE_UNSIGNED_INT,      //!< 32bit unsigned integer. This data type is not supported for OpenGL|ES.
};


class SP_EXPORT ImageBuffer
{
    
    public:
        
        ImageBuffer();
        ImageBuffer(ImageBuffer* Buffer);
        ImageBuffer(
            const EPixelFormats Format, const dim::size2di &Size, u32 Depth = 1,
            const ERendererDataTypes DataType = DATATYPE_UNSIGNED_BYTE
        );
        ImageBuffer(
            const u8* Buffer, const EPixelFormats Format, const dim::size2di &Size, u32 Depth = 1
        );
        ImageBuffer(
            const f32* Buffer, const EPixelFormats Format, const dim::size2di &Size, u32 Depth = 1
        );
        ~ImageBuffer();
        
        /* === Functions === */
        
        void setFormat(const EPixelFormats Format);
        void setDataType(const ERendererDataTypes DataType);
        
        u32 getDataTypeSize() const;
        
        void setPixelColor(const dim::point2di &Pos, const video::color &Color);
        video::color getPixelColor(const dim::point2di &Pos) const;
        
        /* === Static functions === */
        
        static u32 getFormatSize(const EPixelFormats Format);
        
        /* === Inline functions === */
        
        inline const void* getImageBuffer() const
        {
            return ImageBuffer_;
        };
        inline void* getImageBuffer()
        {
            return ImageBuffer_;
        };
        
        inline u32 getPixelCount() const
        {
            return Size_.getArea() * Depth_;
        }
        inline u32 getPixelSize() const
        {
            return getFormatSize() * getDataTypeSize();
        }
        inline u32 getBufferSize() const
        {
            return getPixelCount() * getPixelSize();
        }
        
        inline dim::size2di getSize() const
        {
            return Size_;
        }
        inline u32 getDepth() const
        {
            return Depth_;
        }
        
        inline EPixelFormats getFormat() const
        {
            return Format_;
        }
        inline u32 getFormatSize() const
        {
            return FormatSize_;
        }
        
        inline ERendererDataTypes getDataType() const
        {
            return DataType_;
        }
        
    private:
        
        /* === Functions === */
        
        bool allocRawBuffer(void* &Buffer, const u32 PixelCount) const;
        void freeRawBuffer(void* &Buffer);
        
        void createBuffer();
        void clearBuffer();
        void fillBuffer(const video::color &Color);
        void copyBuffer(const void* Buffer);
        
        /* === Members === */
        
        void* ImageBuffer_;
        
        dim::size2di Size_;
        u32 Depth_;
        
        ERendererDataTypes DataType_;
        EPixelFormats Format_;
        u32 FormatSize_;
        
};


} // /namespace video

} // /namespace sp


#endif



// ================================================================================
