/*
 * Render context file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "RenderSystem/spRenderContext.hpp"


namespace sp
{
namespace video
{


RenderContext::RenderContext() : ParentWindow_(0)
{
    resetConfig();
}
RenderContext::~RenderContext()
{
}

void RenderContext::resetConfig()
{
    /* Reset all configuration */
    ParentWindow_   = 0;
    ColorDepth_     = 0;
    isFullscreen_   = false;
    Resolution_     = dim::size2di(0);
    Flags_          = SDeviceFlags();
}


} // /namespace video

} // /namespace sp



// ================================================================================
