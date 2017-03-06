/*
 * Image loader JPG header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_IMAGELOADER_JPG_H__
#define __SP_IMAGELOADER_JPG_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_TEXLOADER_JPG


#include "FileFormats/Image/spImageFormatInterfaces.hpp"

struct jpeg_decompress_struct;


namespace sp
{
namespace video
{


class SP_EXPORT ImageLoaderJPG : public ImageLoader
{
    
    public:
        
        ImageLoaderJPG(const io::stringc &Filename);
        ~ImageLoaderJPG();
        
        SImageData* loadImageData();
        
    private:
        
        /* Functions */
        
        bool checkForError();
        
        void decompressJPG(jpeg_decompress_struct* cInfo, SImageData* TextureInfo);
        
};


} // /namespace video

} // /namespace sp


#endif

#endif



// ================================================================================
