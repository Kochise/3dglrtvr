/*
 * Render context header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_RENDERCONTEXT_H__
#define __SP_RENDERCONTEXT_H__


#include "Base/spStandard.hpp"
#include "Base/spDimension.hpp"
#include "Base/spInputOutputString.hpp"
#include "Platform/spSoftPixelDeviceFlags.hpp"


namespace sp
{
namespace video
{


//! Render context class which holds the graphics context objects and the window objects.
class SP_EXPORT RenderContext
{
    
    public:
        
        virtual ~RenderContext();
        
        /* Functions */
        
        virtual bool openGraphicsScreen(
            void* ParentWindow, const dim::size2di &Resolution, const io::stringc &Title,
            s32 ColorDepth, bool isFullscreen, const SDeviceFlags &Flags
        ) = 0;
        virtual void closeGraphicsScreen() = 0;
        
        /**
        Flips the buffers to see the final image onto the monitor.
        If vsync (vertical synchronisation) is enabled the program waits at this point until the graphics
        card releases the synchronisation.
        */
        virtual void flipBuffers() = 0;
        
        //! Activates the render context by making it to the current.
        virtual bool activate() = 0;
        
        //! Returns the window object. For Windows it's a HWND* and on Linux it's a Window*.
        virtual void* getWindowObject() = 0;
        
    protected:
        
        /* Functions */
        
        RenderContext();
        
        void resetConfig();
        
        /* Members */
        
        void* ParentWindow_;
        
        dim::size2di Resolution_;
        s32 ColorDepth_;
        bool isFullscreen_;
        SDeviceFlags Flags_;
        
};


} // /namespace video

} // /namespace sp


#endif



// ================================================================================
