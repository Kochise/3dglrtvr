/*
 * Core header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_STANDARD_H__
#define __SP_STANDARD_H__


/* === Core configurations === */

#define __SOFTPIXELENGINE__         // SoftPixel Engine identity

#define __SP_EXPORT_TO_LIBRARY__    // Exports the SoftPixel Engine to a library

#define SOFTPIXEL_VERSION_MAJOR     3
#define SOFTPIXEL_VERSION_MINOR     1
#define SOFTPIXEL_VERSION_REVISION  0
//#define SOFTPIXEL_VERSION_STATUS    "alpha" // "beta"/ "alpha" or undef

#if defined(_ANDROID)
#   define SP_PLATFORM_ANDROID
#elif defined(__WIN32__) || defined(_MSC_VER)
#   define SP_PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__unix__)
#   define SP_PLATFORM_LINUX
#elif defined(__APPLE__)
#   include "TargetConditionals.h"
#   if defined(__MACOSX__)//TARGET_OS_MAC)
#       define SP_PLATFORM_MACOSX
#   elif defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
#       define SP_PLATFORM_IOS
#   endif
#elif defined(__FreeBSD__)
#   define SP_PLATFORM_FREEBSD // unsupported yet
#   error The "SoftPixel Engine" is not supported under "FreeBSD" yet!
#elif defined(__sparc__) || defined(__sun__)
#   define SP_PLATFORM_SOLARIS // unsupported yet
#   error The "SoftPixel Engine" is not supported under "Sun Solaris" yet!
#elif defined(_XBOX)
#   define SP_PLATFORM_XBOX // unsupported yet
#   error The "SoftPixel Engine" is not supported under "XBox" yet!
#else
#   error Unknown platform, no support!
#endif

#ifdef __SP_EXPORT_TO_LIBRARY__
#   if defined(SP_PLATFORM_WINDOWS)
#       define SP_EXPORT __declspec(dllexport)
#   else
#       define SP_EXPORT
#   endif
#else
#   define SP_EXPORT
#endif

#if defined(__GNUC__)
#   define SP_COMPILER_GCC  // GNU Compiler Collection
#elif defined(_MSC_VER)
#   define SP_COMPILER_VC   // VisualC++
#endif

#if defined(SP_COMPILER_VC)
#   define _USE_MATH_DEFINES
#endif

#ifndef SP_DONT_DEFINE_BOOST_MACROS
#   define foreach BOOST_FOREACH
#endif

/* === Compilation configurations === */

#define SP_COMPILE_WITH_NETWORKSYSTEM       // Network system
#define SP_COMPILE_WITH_SOUNDSYSTEM         // Sound system (effects each sound device)
#define SP_COMPILE_WITH_TOOLS               // Tools (effects each tool)
#define SP_COMPILE_WITH_PRIMITIVES          // Standard primitive objects (Cube, Sphere etc.)
#define SP_COMPILE_WITH_GUI                 // Graphics User Interface (GUI)
#define SP_COMPILE_WITH_NEWTONPHYSICS       // NewtonGameDynamics (physics engine and only external library)
#define SP_COMPILE_WITH_CG                  // Cg Toolkit by NVIDIA

#define SP_COMPILE_WITH_OPENGL              // OpenGL 1.1 - 4.1
#define SP_COMPILE_WITH_OPENGLES1           // OpenGL|ES 1.0 - 1.1
#define SP_COMPILE_WITH_OPENGLES2           // OpenGL|ES 2.0
#define SP_COMPILE_WITH_DIRECT3D9           // Direct3D 9
#define SP_COMPILE_WITH_DIRECT3D11          // Direct3D 11

#define SP_COMPILE_WITH_SCENEGRAPH_SIMPLE                   // Simple scene manager
#define SP_COMPILE_WITH_SCENEGRAPH_SIMPLE_CHILDTREE         // Simple scene manager with child tree hierarchy
//#define SP_COMPILE_WITH_SCENEGRAPH_EXPANSIVE                // Expansive scene manager (not supported yet)
//#define SP_COMPILE_WITH_SCENEGRAPH_EXPANSIVE_CHILDTREE      // Expansive scene manager with child tree hierarchy (not supported yet)

#ifdef SP_COMPILE_WITH_SOUNDSYSTEM
#   define SP_COMPILE_WITH_WINMM        // Windows Multi Media system
#   define SP_COMPILE_WITH_OPENSLES     // OpenSL|ES 1.0
#   define SP_COMPILE_WITH_XAUDIO2      // DirectX XAudio2 sound device
#endif

#ifdef SP_COMPILE_WITH_TOOLS
#   define SP_COMPILE_WITH_LIGHTMAPGENERATOR    // Lightmap generator
#   define SP_COMPILE_WITH_MODELCOMBINER        // Model combiner
#   define SP_COMPILE_WITH_SCRIPTLOADER         // Script loader
#   define SP_COMPILE_WITH_PARTICLEANIMATOR     // Particle animator
#   define SP_COMPILE_WITH_TEXTUREMANIPULATOR   // Texture manipulator
#   define SP_COMPILE_WITH_XMLPARSER            // XML parser
#   define SP_COMPILE_WITH_PATHFINDER           // Path finder
#endif

#ifdef SP_COMPILE_WITH_PRIMITIVES
#   define SP_COMPILE_WITH_PRIMITIVE_CUBE           // Cube
#   define SP_COMPILE_WITH_PRIMITIVE_CONE           // Cone
#   define SP_COMPILE_WITH_PRIMITIVE_CYLINDER       // Cylinder
#   define SP_COMPILE_WITH_PRIMITIVE_SPHERE         // Sphere
#   define SP_COMPILE_WITH_PRIMITIVE_ICOSPHERE      // IcoSphere
#   define SP_COMPILE_WITH_PRIMITIVE_TORUS          // Torus
#   define SP_COMPILE_WITH_PRIMITIVE_TORUSKNOT      // TorusKnot
#   define SP_COMPILE_WITH_PRIMITIVE_SPIRAL         // Spiral
#   define SP_COMPILE_WITH_PRIMITIVE_PIPE           // Pipe
#   define SP_COMPILE_WITH_PRIMITIVE_PLANE          // Plane
#   define SP_COMPILE_WITH_PRIMITIVE_DISK           // Disk
#   define SP_COMPILE_WITH_PRIMITIVE_CUBOCTAHEDRON  // Cuboctahedron
#   define SP_COMPILE_WITH_PRIMITIVE_TETRAHEDRON    // Tetrahedron
#   define SP_COMPILE_WITH_PRIMITIVE_OCTAHEDRON     // Octahedron
#   define SP_COMPILE_WITH_PRIMITIVE_DODECAHEDRON   // Dodecahedron
#   define SP_COMPILE_WITH_PRIMITIVE_ICOSAHEDRON    // Icosahedron
#   define SP_COMPILE_WITH_PRIMITIVE_TEAPOT         // Utah Teapot
#   ifdef SP_COMPILE_WITH_PRIMITIVE_TEAPOT
//#       define SP_COMPILE_WITH_PRIMITIVE_TEAPOT_DYNAMIC // Create the teapot using Bezier patches
#   endif
#   define SP_COMPILE_WITH_PRIMITIVE_SUPERSHAPE     // Super shape
#endif

#define SP_COMPILE_WITH_TEXLOADER_BMP       // Texture loader BMP (Windows Bitmaps)
#define SP_COMPILE_WITH_TEXLOADER_JPG       // Texture loader JPG (Joint Photographics Experts)
#define SP_COMPILE_WITH_TEXLOADER_PCX       // Texture loader PCX (Pixture Exchange)
#define SP_COMPILE_WITH_TEXLOADER_PNG       // Texture loader PNG (Portable Network Graphics)
#define SP_COMPILE_WITH_TEXLOADER_TGA       // Texture loader TGA (Targa True Image)
#define SP_COMPILE_WITH_TEXLOADER_DDS       // Texture loader DDS (Direct Draw Surface)
#define SP_COMPILE_WITH_TEXLOADER_WAD       // Texture loader WAD (Where is All the Data)
#define SP_COMPILE_WITH_TEXSAVER_BMP        // Texture saver BMP
#define SP_COMPILE_WITH_MESHLOADER_3DS      // Mesh loader 3DS (3D Studio)
#define SP_COMPILE_WITH_MESHLOADER_B3D      // Mesh loader B3D (Blitz3D)
#define SP_COMPILE_WITH_MESHLOADER_X        // Mesh loader X (DirectX)
#define SP_COMPILE_WITH_MESHLOADER_MD2      // Mesh loader MD2 (Quake 2)
#define SP_COMPILE_WITH_MESHLOADER_MD3      // Mesh loader MD3 (Quake III)
#define SP_COMPILE_WITH_MESHLOADER_MS3D     // Mesh loader MS3D (Milkshape 3D)
#define SP_COMPILE_WITH_MESHLOADER_OBJ      // Mesh loader OBJ (Wavefront Object)
#define SP_COMPILE_WITH_MESHLOADER_SPM      // Mesh loader SPM (SoftPixel Mesh)
#define SP_COMPILE_WITH_MESHSAVER_B3D       // Mesh saver B3D
#define SP_COMPILE_WITH_MESHSAVER_OBJ       // Mesh saver OBJ
#define SP_COMPILE_WITH_MESHSAVER_SPM       // Mesh saver SPM
#define SP_COMPILE_WITH_SCENELOADER_BSP1    // Quake 1 BSP loader
#define SP_COMPILE_WITH_SCENELOADER_BSP3    // Quake 3 BSP loader
#define SP_COMPILE_WITH_SCENELOADER_SPSB    // Scene loader SPS (SoftPixel Scene)

/* === OS dependent configuration (do not edit!) === */

#if !defined(SP_PLATFORM_WINDOWS) && !defined(SP_PLATFORM_XBOX)
#   undef SP_COMPILE_WITH_DIRECT3D9
#   undef SP_COMPILE_WITH_XAUDIO2
#endif

#if !defined(SP_PLATFORM_WINDOWS)
#   undef SP_COMPILE_WITH_DIRECT3D11
#   undef SP_COMPILE_WITH_WINMM
#   undef SP_COMPILE_WITH_NEWTONPHYSICS
#endif

#if defined(SP_PLATFORM_ANDROID) || defined(SP_PLATFORM_IOS)
#	undef SP_COMPILE_WITH_OPENGL
#   undef SP_COMPILE_WITH_NETWORKSYSTEM
#   undef SP_COMPILE_WITH_SOUNDSYSTEM
#   undef SP_COMPILE_WITH_GUI
#   undef SP_COMPILE_WITH_NEWTONPHYSICS
#   undef SP_COMPILE_WITH_CG
#else
#   undef SP_COMPILE_WITH_OPENGLES1
#   undef SP_COMPILE_WITH_OPENGLES2
#   undef SP_COMPILE_WITH_OPENSLES
#endif

#if defined(SP_PLATFORM_IOS)
#   undef SP_COMPILE_WITH_OPENSLES
#endif

#if defined(SP_PLATFORM_ANDROID)
#   define SP_USE_GLES_EGL
#endif

#if defined(SP_PLATFORM_MACOSX) || defined(SP_PLATFORM_LINUX)
#   undef SP_COMPILE_WITH_CG
#endif

/* === Compiler dependent configuration (do not edit!) === */

#if defined(SP_COMPILER_VC)
#   define _USE_MATH_DEFINES
#else
#   undef SP_COMPILE_WITH_DIRECT3D11
#   undef SP_COMPILE_WITH_XAUDIO2
#endif

/* === Doxygen configuration === */

#ifdef __DOXYGEN__
#   define SP_COMPILE_WITH_OPENGL
#   define SP_COMPILE_WITH_OPENGLES1
#   define SP_COMPILE_WITH_OPENGLES2
#   define SP_COMPILE_WITH_DIRECT3D9
#   define SP_COMPILE_WITH_DIRECT3D11
#   define SP_COMPILE_WITH_WINMM
#   define SP_COMPILE_WITH_XAUDIO2
#   define SP_COMPILE_WITH_OPENSLES
#   define SP_COMPILE_WITH_NEWTONPHYSICS
#   define SP_COMPILE_WITH_CG
#   define SP_PLATFORM_WINDOWS
#   define SP_PLATFORM_MACOSX
#   define SP_PLATFORM_LINUX
#   define SP_PLATFORM_ANDROID
#   define SP_PLATFORM_IOS
#endif


namespace sp
{


/* === Standard types === */

typedef char                c8;
typedef wchar_t             c16;

typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long int     s64;

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long int   u64;

typedef float               f32;
typedef double              f64;
typedef long double         f128;


/* Macros */

static const s32 MAX_COUNT_OF_TEXTURES  = 8;
static const s32 MAX_COUNT_OF_LIGHTS    = 8;

extern s32 __Screen_Width, __Screen_Height;


} // /namespace sp


#endif



// ================================================================================
