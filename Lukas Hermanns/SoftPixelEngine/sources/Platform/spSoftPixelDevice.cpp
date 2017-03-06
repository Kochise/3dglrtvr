/*
 * SoftPixelDevice file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Platform/spSoftPixelDeviceOS.hpp"
#include "GUI/spGUIManager.hpp"
#include "RenderSystem/spRenderSystem.hpp"
#include "RenderSystem/OpenGL/spOpenGLRenderSystem.hpp"
#include "RenderSystem/OpenGL/spOpenGLRenderContext.hpp"
#include "RenderSystem/OpenGLES/spOpenGLES1RenderSystem.hpp"
#include "RenderSystem/OpenGLES/spOpenGLES2RenderSystem.hpp"
#include "RenderSystem/OpenGLES/spOpenGLESRenderContext.hpp"
#include "RenderSystem/Direct3D9/spDirect3D9RenderSystem.hpp"
#include "RenderSystem/Direct3D9/spDirect3D9RenderContext.hpp"
#include "RenderSystem/Direct3D11/spDirect3D11RenderSystem.hpp"
#include "RenderSystem/Direct3D11/spDirect3D11RenderContext.hpp"
#include "RenderSystem/spDummyRenderSystem.hpp"
#include "RenderSystem/spDesktopRenderContext.hpp"
#include "Framework/Sound/spWinMMSoundDevice.hpp"
#include "Framework/Sound/spXAudio2SoundDevice.hpp"
#include "Framework/Sound/spOpenSLESSoundDevice.hpp"
#include "Framework/Sound/spDummySoundDevice.hpp"


namespace sp
{


/*
 * Internal members
 */

SoftPixelDevice*            __spDevice              = 0;
audio::SoundDevice*         __spSoundDevice         = 0;
video::RenderSystem*        __spVideoDriver         = 0;
video::RenderContext*       __spRenderContext       = 0;
scene::SceneGraph*          __spSceneManager        = 0;
scene::CollisionDetector*   __spCollisionDetector   = 0;
io::InputControl*           __spInputControl        = 0;
io::FileSystem*             __spFileSystem          = 0;
io::OSInformator*           __spOSInformator        = 0;
gui::GUIManager*            __spGUIManager          = 0;


/*
 * Standard and OS independet SoftPixelDevice functions
 */

SoftPixelDevice::SoftPixelDevice(
    const video::ERenderSystems RendererType, const dim::size2di &Resolution,
    s32 ColorDepth, const io::stringc &Title, bool isFullscreen, const SDeviceFlags &Flags)
    : WindowObject_(0), RendererType_(RendererType), Resolution_(Resolution),
    ColorDepth_(ColorDepth), Title_(Title), isFullscreen_(isFullscreen), Flags_(Flags),
    FrameRate_(0), DefaultSceneManager_(0)
{
}
SoftPixelDevice::~SoftPixelDevice()
{
}

video::RenderSystem* SoftPixelDevice::getRenderSystem() const
{
    return __spVideoDriver;
}
video::RenderContext* SoftPixelDevice::getRenderContext() const
{
    return __spRenderContext;
}

io::InputControl* SoftPixelDevice::getInputControl() const
{
    return __spInputControl ? __spInputControl : __spInputControl = new io::InputControl();
}
io::FileSystem* SoftPixelDevice::getFileSystem() const
{
    return __spFileSystem ? __spFileSystem : __spFileSystem = new io::FileSystem();
}
io::OSInformator* SoftPixelDevice::getOSInformator() const
{
    return __spOSInformator ? __spOSInformator : __spOSInformator = new io::OSInformator();
}

gui::GUIManager* SoftPixelDevice::getGUIManager() const
{
    #ifdef SP_COMPILE_WITH_GUI
    return __spGUIManager ? __spGUIManager : __spGUIManager = new gui::GUIManager();
    #else
    io::Log::error("This engine was not compiled with the GUI");
    return 0;
    #endif
}

audio::SoundDevice* SoftPixelDevice::getSoundDevice(const audio::ESoundDevices Type) const
{
    #ifdef SP_COMPILE_WITH_SOUNDSYSTEM
    return __spSoundDevice ? __spSoundDevice : __spSoundDevice = createSoundDevice(Type);
    #else
    io::Log::error("This engine was not compiled with the sound system");
    return 0;
    #endif
}

scene::CollisionDetector* SoftPixelDevice::getCollisionDetector() const
{
    return __spCollisionDetector ? __spCollisionDetector : __spCollisionDetector = new scene::CollisionDetector();
}
scene::SceneGraph* SoftPixelDevice::getSceneGraph(const scene::ESceneGraphs Type) const
{
    if (!__spSceneManager)
    {
        switch (Type)
        {
            #ifdef SP_COMPILE_WITH_SCENEGRAPH_SIMPLE
            case scene::SCENEGRAPH_SIMPLE:
                __spSceneManager = new scene::SceneGraphSimple(); break;
            #endif
            #ifdef SP_COMPILE_WITH_SCENEGRAPH_SIMPLE_CHILDTREE
            case scene::SCENEGRAPH_SIMPLE_CHILDTREE:
                __spSceneManager = new scene::SceneGraphSimpleChildTree(); break;
            #endif
            default:
                io::Log::error("Specified scene graph is not supported");
                return 0;
        }
    }
    
    return __spSceneManager;
}

bool SoftPixelDevice::updateDeviceSettings(
    const dim::size2di &Resolution, s32 ColorDepth, bool isFullscreen, const SDeviceFlags &Flags, void* ParentWindow)
{
    return true;
}

void SoftPixelDevice::setActiveSceneGraph(scene::SceneGraph* ActiveSceneGraph)
{
    if (ActiveSceneGraph)
    {
        if (!DefaultSceneManager_)
            DefaultSceneManager_ = __spSceneManager;
        __spSceneManager = ActiveSceneGraph;
    }
    else
        __spSceneManager = DefaultSceneManager_;
}
scene::SceneGraph* SoftPixelDevice::getActiveSceneGraph() const
{
    return __spSceneManager;
}

void SoftPixelDevice::setFrameRate(u32 FrameRate)
{
    if (FrameRate > 0)
        FrameRate_ = static_cast<u32>(1000.0f / static_cast<f32>(FrameRate + 0.5f));
    else
        FrameRate_ = 0;
}

io::stringc SoftPixelDevice::getVersion() const
{
    return
        io::stringc("SoftPixel Engine")
        + " - v."   + io::stringc(SOFTPIXEL_VERSION_MAJOR)
        + "."       + io::stringc(SOFTPIXEL_VERSION_MINOR)
    #if SOFTPIXEL_VERSION_REVISION
        + "."       + io::stringc(SOFTPIXEL_VERSION_REVISION)
    #endif
    #ifdef SOFTPIXEL_VERSION_STATUS
        + " "       + io::stringc(SOFTPIXEL_VERSION_STATUS)
    #endif
    ;
}

void SoftPixelDevice::sleep(u32 Milliseconds)
{
    // do nothing
}
void SoftPixelDevice::beep(u32 Milliseconds, u32 Frequency)
{
    // do nothing
}
io::stringc SoftPixelDevice::getUserCharList() const
{
    return "";
}

void SoftPixelDevice::manipulateScreenSize(const dim::size2di &ScreenSize)
{
    /* Manipulate internal screen size values */
    Resolution_     = ScreenSize;
    __Screen_Width  = Resolution_.Width;
    __Screen_Height = Resolution_.Height;
}

void SoftPixelDevice::setWindowTitle(const io::stringc &Title)
{
}
io::stringc SoftPixelDevice::getWindowTitle() const
{
    return "";
}
void SoftPixelDevice::setWindowPosition(const dim::point2di &Position)
{
}
dim::point2di SoftPixelDevice::getWindowPosition() const
{
    return 0;
}
dim::size2di SoftPixelDevice::getWindowSize() const
{
    return 0;
}
bool SoftPixelDevice::isWindowActive() const
{
    return true;
}


/*
 * ======= Statics: =======
 */

u32 SoftPixelDevice::getFPS()
{
    static u64 LastTime = getMilliseconds();
    static u32 Frames;
    static u32 fps = 62;
    
    ++Frames;
    
    u64 CurTime = getMilliseconds();
    
    if (CurTime > LastTime + 1000)
    {
        LastTime    = CurTime;
        fps         = Frames;
        Frames      = 0;
    }
    
    return fps;
}

u64 SoftPixelDevice::getMilliseconds()
{
    #if defined(SP_PLATFORM_LINUX)
    return static_cast<u64>(clock()) / 1000;
    #else
    return static_cast<u64>(clock());
    #endif
}

u16 SoftPixelDevice::getTime(const ETimeTypes Type)
{
    time_t RawTime = time(0);
    tm* Time = localtime(&RawTime);
    
    s32 TimeVal = 0;
    
    switch (Type)
    {
        case TIME_SECOND:
            TimeVal = Time->tm_sec; break;
        case TIME_MINUTE:
            TimeVal = Time->tm_min; break;
        case TIME_HOUR:
            TimeVal = Time->tm_hour; break;
        case TIME_DAY:
            TimeVal = Time->tm_mday; break;
        case TIME_MONTH:
            TimeVal = Time->tm_mon + 1; break;
        case TIME_YEAR:
            TimeVal = Time->tm_year + 1900; break;
    }
    
    return static_cast<u16>(TimeVal);
}

io::stringc SoftPixelDevice::getTime()
{
    io::stringc Str[6] = {
        io::stringc((s32)getTime(TIME_YEAR  )),
        io::stringc((s32)getTime(TIME_MONTH )),
        io::stringc((s32)getTime(TIME_DAY   )),
        io::stringc((s32)getTime(TIME_HOUR  )),
        io::stringc((s32)getTime(TIME_MINUTE)),
        io::stringc((s32)getTime(TIME_SECOND))
    };
    
    for (s32 i = 1; i < 6; ++i)
    {
        if (Str[i].size() == 1)
            Str[i] = "0" + Str[i];
    }
    
    return Str[2] + "/" + Str[1] + "/" + Str[0] + " " + Str[3] + ":" + Str[4] + ":" + Str[5];
}


/*
 * ======= Protected: =======
 */

void SoftPixelDevice::autoDetectRenderSystem()
{
    if (RendererType_ == video::RENDERER_AUTODETECT)
    {
        #if defined(SP_PLATFORM_WINDOWS)
        RendererType_ = video::RENDERER_DIRECT3D9;
        #elif defined(SP_PLATFORM_ANDROID) || defined(SP_PLATFORM_IOS)
        RendererType_ = video::RENDERER_OPENGLES1;
        #else
        RendererType_ = video::RENDERER_OPENGL;
        #endif
    }
    
    io::stringc Name;
    
    switch (RendererType_)
    {
        case video::RENDERER_OPENGL:
            Name = "OpenGL"; break;
        case video::RENDERER_OPENGLES1:
            Name = "OpenGL|ES 1"; break;
        case video::RENDERER_OPENGLES2:
            Name = "OpenGL|ES 2"; break;
        case video::RENDERER_DIRECT3D9:
            Name = "Direct3D9"; break;
        case video::RENDERER_DIRECT3D11:
            Name = "Direct3D11"; break;
    }
    
    if (!checkRenderSystem(RendererType_))
    {
        if (RendererType_ != video::RENDERER_DIRECT3D11 && checkRenderSystem(video::RENDERER_DIRECT3D11))
        {
            RendererType_ = video::RENDERER_DIRECT3D11;
            io::Log::error(Name + " is not supported; using Direct3D11");
        }
        else if (RendererType_ != video::RENDERER_DIRECT3D9 && checkRenderSystem(video::RENDERER_DIRECT3D9))
        {
            RendererType_ = video::RENDERER_DIRECT3D9;
            io::Log::error(Name + " is not supported; using Direct3D9");
        }
        else if (RendererType_ != video::RENDERER_OPENGL && checkRenderSystem(video::RENDERER_OPENGL))
        {
            RendererType_ = video::RENDERER_OPENGL;
            io::Log::error(Name + " is not supported; using OpenGL");
        }
        else if (RendererType_ != video::RENDERER_OPENGLES2 && checkRenderSystem(video::RENDERER_OPENGLES2))
        {
            RendererType_ = video::RENDERER_OPENGLES2;
            io::Log::error(Name + " is not supported; using OpenGL|ES 2");
        }
        else if (RendererType_ != video::RENDERER_OPENGLES1 && checkRenderSystem(video::RENDERER_OPENGLES1))
        {
            RendererType_ = video::RENDERER_OPENGLES1;
            io::Log::error(Name + " is not supported; using OpenGL|ES 1");
        }
        else
        {
            RendererType_ = video::RENDERER_DUMMY;
            io::Log::error(Name + " is not supported; using Dummy");
        }
    }
}

bool SoftPixelDevice::checkRenderSystem(const video::ERenderSystems Type) const
{
    switch (Type)
    {
        #ifdef SP_COMPILE_WITH_OPENGL
        case video::RENDERER_OPENGL:
            return true;
        #endif
        
        #ifdef SP_COMPILE_WITH_OPENGLES1
        case video::RENDERER_OPENGLES1:
            return true;
        #endif
        
        #ifdef SP_COMPILE_WITH_OPENGLES2
        case video::RENDERER_OPENGLES2:
            return true;
        #endif
        
        #ifdef SP_COMPILE_WITH_DIRECT3D9
        case video::RENDERER_DIRECT3D9:
        {
            c8 SysDir[256];
            GetSystemDirectory(SysDir, 256);
            
            if (io::FileSystem().findFile(io::stringc(SysDir) + "\\d3d9.dll"))
                return true;
        }
        break;
        #endif
        
        #ifdef SP_COMPILE_WITH_DIRECT3D11
        case video::RENDERER_DIRECT3D11:
        {
            c8 SysDir[256];
            GetSystemDirectory(SysDir, 256);
            
            if (io::FileSystem().findFile(io::stringc(SysDir) + "\\d3d11.dll"))
                return true;
        }
        break;
        #endif
        
        case video::RENDERER_DUMMY:
            return true;
        
        default:
            break;
    }
    
    return false;
}

void SoftPixelDevice::createRenderSystem()
{
    autoDetectRenderSystem();
    
    switch (RendererType_)
    {
        #ifdef SP_COMPILE_WITH_OPENGL
        case video::RENDERER_OPENGL:
            __spVideoDriver     = new video::OpenGLRenderSystem();
            __spRenderContext   = new video::OpenGLRenderContext();
            break;
        #endif
        #ifdef SP_COMPILE_WITH_OPENGLES1
        case video::RENDERER_OPENGLES1:
            __spVideoDriver     = new video::OpenGLES1RenderSystem();
            __spRenderContext   = new video::OpenGLESRenderContext(false);
            break;
        #endif
        #ifdef SP_COMPILE_WITH_OPENGLES2
        case video::RENDERER_OPENGLES2:
            __spVideoDriver     = new video::OpenGLES2RenderSystem();
            __spRenderContext   = new video::OpenGLESRenderContext(true);
            break;
        #endif
        #ifdef SP_COMPILE_WITH_DIRECT3D9
        case video::RENDERER_DIRECT3D9:
            __spVideoDriver     = new video::Direct3D9RenderSystem();
            __spRenderContext   = new video::Direct3D9RenderContext();
            break;
        #endif
        #ifdef SP_COMPILE_WITH_DIRECT3D11
        case video::RENDERER_DIRECT3D11:
            __spVideoDriver     = new video::Direct3D11RenderSystem();
            __spRenderContext   = new video::Direct3D11RenderContext();
            break;
        #endif
        case video::RENDERER_DUMMY:
        default:
            __spVideoDriver     = new video::DummyRenderSystem();
            __spRenderContext   = new video::DesktopRenderContext();
            break;
    }
}

void SoftPixelDevice::deleteResourceDevices()
{
    MemoryManager::deleteMemory(__spCollisionDetector);
    MemoryManager::deleteMemory(__spInputControl);
    MemoryManager::deleteMemory(__spFileSystem);
    MemoryManager::deleteMemory(__spOSInformator);
    MemoryManager::deleteMemory(__spGUIManager);
    MemoryManager::deleteMemory(__spSceneManager);
    MemoryManager::deleteMemory(__spVideoDriver);
    MemoryManager::deleteMemory(__spRenderContext);
    MemoryManager::deleteMemory(__spSoundDevice);
}

void SoftPixelDevice::printConsoleHeader()
{
    io::Log::message(getVersion(), 0);                                                          // Engine version
    io::Log::message("Copyright (c) 2008 - Lukas Hermanns", 0);                                 // Copyright
    io::Log::message(io::OSInformator().getOSVersion(), 0);                                     // OS version
    io::Log::message("Compiled with: " + io::OSInformator().getCompilerVersion(), 0);           // Compiler information
    io::Log::message("Using renderer: " + __spVideoDriver->getVersion(), 0);                    // Renderer version
    
    if (__spVideoDriver->queryVideoSupport(video::QUERY_SHADER))
        io::Log::message("Shader version: " + __spVideoDriver->getShaderVersion(), 0);          // Shader version
    
    io::Log::message(__spVideoDriver->getRenderer() + ": " + __spVideoDriver->getVendor(), 0);  // Renderer information
    
    __spVideoDriver->printWarning();
    
    io::Log::message("", 0);
}

void SoftPixelDevice::resetCursorSpeedLock()
{
    if (__spInputControl)
        __spInputControl->isCursorSpeedBlocked_ = false;
}

#ifdef SP_COMPILE_WITH_SOUNDSYSTEM

audio::SoundDevice* SoftPixelDevice::createSoundDevice(audio::ESoundDevices DeviceType) const
{
    if (DeviceType == audio::SOUNDDEVICE_AUTODETECT)
    {
        #if defined(SP_COMPILE_WITH_XAUDIO2)
        DeviceType = audio::SOUNDDEVICE_WINMM;//SOUNDDEVICE_XAUDIO2;
        #elif defined(SP_COMPILE_WITH_WINMM)
        DeviceType = audio::SOUNDDEVICE_WINMM;
        #elif defined(SP_COMPILE_WITH_OPENSLES)
        DeviceType = audio::SOUNDDEVICE_OPENSLES;
        #else
        DeviceType = audio::SOUNDDEVICE_DUMMY;
        #endif
    }
    else
    {
        #ifndef SP_COMPILE_WITH_WINMM
        if (DeviceType == audio::SOUNDDEVICE_WINMM)
        {
            io::Log::warning("WinMM is not supported; using Dummy");
            DeviceType = audio::SOUNDDEVICE_DUMMY;
        }
        #endif
        #ifndef SP_COMPILE_WITH_XAUDIO2
        if (DeviceType == audio::SOUNDDEVICE_XAUDIO2)
        {
            io::Log::warning("XAudio2 is not supported; using Dummy");
            DeviceType = audio::SOUNDDEVICE_DUMMY;
        }
        #endif
        #ifndef SP_COMPILE_WITH_OPENSLES
        if (DeviceType == audio::SOUNDDEVICE_OPENSLES)
        {
            io::Log::warning("OpenSL|ES is not supported; using Dummy");
            DeviceType = audio::SOUNDDEVICE_DUMMY;
        }
        #endif
    }
    
    switch (DeviceType)
    {
        #ifdef SP_COMPILE_WITH_XAUDIO2
        case audio::SOUNDDEVICE_XAUDIO2:
            return new audio::XAudio2SoundDevice();
        #endif
        
        #ifdef SP_COMPILE_WITH_OPENSLES
        case audio::SOUNDDEVICE_OPENSLES:
            return new audio::OpenSLESSoundDevice();
        #endif
        
        #ifdef SP_COMPILE_WITH_WINMM
        case audio::SOUNDDEVICE_WINMM:
            return new audio::WinMMSoundDevice();
        #endif
        
        default:
            break;
    }
    
    return new audio::DummySoundDevice();
}

#endif


/*
 * Global functions
 */

#if defined(SP_PLATFORM_ANDROID)

SP_EXPORT SoftPixelDevice* createGraphicsDevice(
    android_app* App, const video::ERenderSystems RendererType, io::stringc Title, const bool isFullscreen)
{
    return __spDevice = new SoftPixelDeviceAndroid(App, RendererType, Title, isFullscreen);
}

#elif defined(SP_PLATFORM_IOS)

SP_EXPORT SoftPixelDevice* createGraphicsDevice(
    const video::ERenderSystems RendererType, io::stringc Title, const bool isFullscreen)
{
    return __spDevice = new SoftPixelDeviceIOS(RendererType, Title, isFullscreen);
}

#else

SP_EXPORT SoftPixelDevice* createGraphicsDevice(
    const video::ERenderSystems RendererType, const dim::size2di &Resolution, const s32 ColorDepth,
    io::stringc Title, const bool isFullscreen, const SDeviceFlags &Flags, void* ParentWindow)
{
    #if defined(SP_PLATFORM_WINDOWS)
    return __spDevice = new SoftPixelDeviceWin32(
        RendererType, Resolution, ColorDepth, Title, isFullscreen, Flags, ParentWindow
    );
    #elif defined(SP_PLATFORM_MACOSX)
    return __spDevice = new SoftPixelDeviceMacOSX(
        RendererType, Resolution, ColorDepth, Title, isFullscreen, Flags
    );
    #elif defined(SP_PLATFORM_LINUX)
    return __spDevice = new SoftPixelDeviceLinux(
        RendererType, Resolution, ColorDepth, Title, isFullscreen, Flags
    );
    #endif
}

#endif

SP_EXPORT void deleteDevice()
{
    MemoryManager::deleteMemory(__spDevice);
}


} // /namespace sp



// ================================================================================
