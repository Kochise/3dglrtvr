/*
 * OpenGL|ES EGL render context file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "RenderSystem/OpenGLES/spOpenGLESRenderContext.hpp"

#if defined(SP_COMPILE_WITH_OPENGLES1) || defined(SP_COMPILE_WITH_OPENGLES2)


#include "RenderSystem/OpenGLES/spOpenGLESFunctionsARB.hpp"
#include "Base/spInternalDeclarations.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{
namespace video
{


OpenGLESRenderContext::OpenGLESRenderContext(bool isGLES2)
    : RenderContext(), isGLES2_(isGLES2)
    #ifdef SP_USE_GLES_EGL
    ,Display_(EGL_NO_DISPLAY)
    ,Context_(EGL_NO_CONTEXT)
    ,Surface_(EGL_NO_SURFACE)
    #endif
{
}
OpenGLESRenderContext::~OpenGLESRenderContext()
{
}

bool OpenGLESRenderContext::openGraphicsScreen(
    void* ParentWindow, const dim::size2di &Resolution, const io::stringc &Title,
    s32 ColorDepth, bool isFullscreen, const SDeviceFlags &Flags)
{
    return createRenderContext(ParentWindow, isGLES2_);
}
void OpenGLESRenderContext::closeGraphicsScreen()
{
    
}

void OpenGLESRenderContext::flipBuffers()
{
    #ifdef SP_USE_GLES_EGL
    if (Display_ != EGL_NO_DISPLAY && Surface_ != EGL_NO_SURFACE)
        eglSwapBuffers(Display_, Surface_);
    #endif
}

bool OpenGLESRenderContext::activate()
{
    // do nothing -> for EGL there are is no support for multiple render contexts
}

void* OpenGLESRenderContext::getWindowObject()
{
    return 0;
}


/*
 * ======= Private: =======
 */

bool OpenGLESRenderContext::createRenderContext(void* Window, bool isGLES2)
{
    #ifdef SP_USE_GLES_EGL
    
    android_app* App = static_cast<android_app*>(Window);
    
    /* Setup context attributes */
    const EGLint AttribsGLES1[] = {
        EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
        EGL_BLUE_SIZE,      8,
        EGL_GREEN_SIZE,     8,
        EGL_RED_SIZE,       8,
        EGL_DEPTH_SIZE,     24,
        EGL_NONE
    };
    
    const EGLint AttribsGLES2[] = {
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
        EGL_BLUE_SIZE,          8,
        EGL_GREEN_SIZE,         8,
        EGL_RED_SIZE,           8,
        EGL_DEPTH_SIZE,         24,
        EGL_NONE
    };
    
    EGLint Format;
    EGLint NumConfigs;
    EGLConfig Config;
    
    /* Initialize display and choose context configuration */
    Display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    
    eglInitialize(Display_, 0, 0);
    eglChooseConfig(Display_, isGLES2 ? AttribsGLES2 : AttribsGLES1, &Config, 1, &NumConfigs);
    eglGetConfigAttrib(Display_, Config, EGL_NATIVE_VISUAL_ID, &Format);
    
    ANativeWindow_setBuffersGeometry(App->window, 0, 0, Format);
    
    /* Create window surface */
    Surface_ = eglCreateWindowSurface(Display_, Config, App->window, 0);
    
    /* Create render context */
    if (isGLES2)
    {
        EGLint ContextAttrib[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
        Context_ = eglCreateContext(Display_, Config, EGL_NO_CONTEXT, ContextAttrib);
    }
    else
        Context_ = eglCreateContext(Display_, Config, EGL_NO_CONTEXT, 0);
    
    /* Make current OpenGL context */
    if (eglMakeCurrent(Display_, Surface_, Surface_, Context_) == EGL_FALSE)
    {
        io::Log::error("Could not activate OpenGL|ES render context");
        return false;
    }
    
    /* Query screen resolution */
    eglQuerySurface(Display_, Surface_, EGL_WIDTH, &__Screen_Width);
    eglQuerySurface(Display_, Surface_, EGL_HEIGHT, &__Screen_Height);
    
    #endif
    
    return true;
}

void OpenGLESRenderContext::releaseRenderContext(void* Window)
{
    #ifdef SP_USE_GLES_EGL
    
    if (Display_ != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(Display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        
        if (Context_ != EGL_NO_CONTEXT)
            eglDestroyContext(Display_, Context_);
        
        if (Surface_ != EGL_NO_SURFACE)
            eglDestroySurface(Display_, Surface_);
        
        eglTerminate(Display_);
    }
    
    Display_ = EGL_NO_DISPLAY;
    Context_ = EGL_NO_CONTEXT;
    Surface_ = EGL_NO_SURFACE;
    
    #ifdef __DEBUG_EGL__ // !!!
    io::Log::message("DEB: releaseRenderContext successful");
    #endif
    
    #endif
}


} // /namespace video

} // /namespace sp


#endif



// ================================================================================
