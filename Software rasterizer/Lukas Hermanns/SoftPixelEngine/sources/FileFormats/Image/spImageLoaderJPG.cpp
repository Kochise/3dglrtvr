/*
 * Image loader JPG file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "FileFormats/Image/spImageLoaderJPG.hpp"

#ifdef SP_COMPILE_WITH_TEXLOADER_JPG


#include "Plugins/jpeglib/jpeglib.h"

//#include <jpeglib.h>
#include <setjmp.h>


namespace sp
{
namespace video
{


ImageLoaderJPG::ImageLoaderJPG(const io::stringc &Filename) : ImageLoader(Filename)
{
}
ImageLoaderJPG::~ImageLoaderJPG()
{
}

SImageData* ImageLoaderJPG::loadImageData()
{
    /* Check if the file is not corrupt */
    if (checkForError())
        return 0;
    
    /* Allocate new texture RAW data & header buffer */
    SImageData* texture = new SImageData();
    
    jpeg_decompress_struct cInfo;
    
    jpeg_error_mgr jerr;
	
    cInfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cInfo);
    
    /* Read complete file into a buffer */
    const u32 FileSize = File_->getSize();
    u8* FileBuffer = new u8[FileSize];
    File_->readBuffer(FileBuffer, FileSize);
    
    jpeg_mem_src(&cInfo, FileBuffer, FileSize);
	
	/* Decompress the JPG image data */
    decompressJPG(&cInfo, texture);
    
    jpeg_destroy_decompress(&cInfo);
    
    delete [] FileBuffer;
	
	return texture;
}


/*
 * ======= Private: =======
 */

bool ImageLoaderJPG::checkForError()
{
    /* Check if the file has opened correct */
    if (isFileError())
        return true;
    
    s32 JFIF = 0;
    
    File_->setSeek(6, io::FILEPOS_BEGIN);
    JFIF = File_->readValue<s32>();
    File_->setSeek(0, io::FILEPOS_BEGIN);
    
    /* Check if the file is corrupt */
    if (!(JFIF == 0x4A464946 || JFIF == 0x4649464A))
    {
        /* Print a detailed error message */
        io::Log::error("JPEG file is corrupt and has incorrect JFIF header ( 0x" + io::getHex(JFIF) + " )");
        
        /* An error has been detected, the file is corrupt */
        return true;
    }
    
    /* No error has been detected */
    return false;
}

void ImageLoaderJPG::decompressJPG(jpeg_decompress_struct* cInfo, SImageData* TextureInfo)
{
    /* Start the decompression */
    jpeg_read_header(cInfo, true);
    jpeg_start_decompress(cInfo);
    
    /* Settings */
    s32 RowSpan                 = cInfo->image_width * cInfo->num_components;
    TextureInfo->Width          = cInfo->image_width;
    TextureInfo->Height         = cInfo->image_height;
    TextureInfo->ImageBuffer    = new u8[RowSpan * TextureInfo->Height];
    
    /* Loop the array y-axis */
    u8** rowPtr = new u8*[TextureInfo->Height];
    for (u32 i = 0; i < TextureInfo->Height; ++i)
        rowPtr[i] = &(TextureInfo->ImageBuffer[i*RowSpan]);
    
    u32 rowsRead = 0;
    while (cInfo->output_scanline < cInfo->output_height)
        rowsRead += jpeg_read_scanlines(cInfo, &rowPtr[rowsRead], cInfo->output_height - rowsRead);
    
    /* Delete the array pointer */
    MemoryManager::deleteBuffer(rowPtr);
    
    /* Finish the decompression */
    jpeg_finish_decompress(cInfo);
}


} // /namespace video

} // /namespace sp


#endif



// ================================================================================
