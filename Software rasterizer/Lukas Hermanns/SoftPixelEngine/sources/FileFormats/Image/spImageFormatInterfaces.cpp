/*
 * Image format interfaces file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "FileFormats/Image/spImageFormatInterfaces.hpp"


namespace sp
{
namespace video
{


/*
 * ImageLoader class
 */

ImageLoader::ImageLoader(const io::stringc &Filename)
    : io::BaseFileFormatHandler(Filename), isFileError_(false)
{
    if ( !( File_ = FileSys_->readResourceFile(Filename) ) )
        isFileError_ = true;
}
ImageLoader::~ImageLoader()
{
}


/*
 * ImageSaver class
 */

ImageSaver::ImageSaver(const io::stringc &Filename)
    : io::BaseFileFormatHandler(Filename), isFileError_(false)
{
    if ( !( File_ = FileSys_->openFile(Filename, io::FILE_WRITE) ) )
        isFileError_ = true;
}
ImageSaver::~ImageSaver()
{
}


} // /namespace video

} // /namespace sp



// ================================================================================
