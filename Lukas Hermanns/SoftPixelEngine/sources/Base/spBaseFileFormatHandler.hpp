/*
 * Base file format handler header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_BASEFILEFORMATHANDLER_H__
#define __SP_BASEFILEFORMATHANDLER_H__


#include "Base/spStandard.hpp"
#include "Base/spInputOutput.hpp"
#include "Base/spMemoryManagement.hpp"


namespace sp
{
namespace io
{


//! Base loader class. Base class for scene::MeshLoader, video::ImageLoader and sound::SoundLoader.
class SP_EXPORT BaseFileFormatHandler
{
    
    public:
        
        virtual ~BaseFileFormatHandler()
        {
            MemoryManager::deleteMemory(FileSys_);
        }
        
        /* Inline functions */
        
        inline io::stringc getFilename() const
        {
            return Filename_;
        }
        
    protected:
        
        /* Functions */
        
        BaseFileFormatHandler(const io::stringc &Filename = "") :
            FileSys_(0), File_(0), Filename_(Filename)
        {
            FileSys_ = MemoryManager::createMemory<io::FileSystem>("io::FileSystem (BaseFileFormatHandler)");
        }
        
        /* Members */
        
        io::FileSystem* FileSys_;
        io::File* File_;
        
        io::stringc Filename_;
        
};


} // /namespace io

} // /namespace sp


#endif



// ================================================================================
