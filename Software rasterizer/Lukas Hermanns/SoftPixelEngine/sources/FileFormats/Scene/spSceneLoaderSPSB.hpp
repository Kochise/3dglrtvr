/*
 * SoftPixel Sandbox Scene loader header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_SCENELOADER_SPSB_H__
#define __SP_SCENELOADER_SPSB_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_SCENELOADER_SPSB


#include "Base/spInputOutputLog.hpp"
#include "Base/spInputOutputFileSystem.hpp"
#include "Base/spDimension.hpp"
#include "SceneGraph/spSceneMesh.hpp"
#include "RenderSystem/spTextureBase.hpp"
#include "FileFormats/Mesh/spMeshLoader.hpp"

#include <list>


namespace sp
{
namespace scene
{


enum ESceneLoaderFlags
{
    SCENEFLAG_ABSOLUTEPATH  = 0x00000001, //!< Load all resources with the absolute path. By default relative paths are used.
    
    SCENEFLAG_CONFIG        = 0x00000002, //!< Load scene configuration. By default used.
    
    SCENEFLAG_MESHES        = 0x00000004, //!< Load meshes. By default used.
    SCENEFLAG_LIGHTS        = 0x00000008, //!< Load light sources. By default used.
    SCENEFLAG_CAMERAS       = 0x00000010, //!< Load cameras. By default used.
    SCENEFLAG_WAYPOINTS     = 0x00000020, //!< Load way points. By default used.
    SCENEFLAG_SCRIPTS       = 0x00000040, //!< Load scripts. By default used.
    
    SCENEFLAG_TEXTURES      = 0x00000080, //!< Load textures and texture classes. By default used.
    SCENEFLAG_LIGHTMAPS     = 0x00000100, //!< Load lightmaps and lightmap scene. By default used.
    SCENEFLAG_SHADERS       = 0x00000200, //!< Load shaders and shader classes. By default used.
    
    SCENEFLAG_OBJECTS       = SCENEFLAG_MESHES | SCENEFLAG_LIGHTS | SCENEFLAG_CAMERAS | SCENEFLAG_WAYPOINTS,
    SCENEFLAG_ALL           = SCENEFLAG_CONFIG | SCENEFLAG_OBJECTS | SCENEFLAG_TEXTURES | SCENEFLAG_LIGHTMAPS | SCENEFLAG_SHADERS,
};


class SP_EXPORT SceneLoaderSPSB : public SceneLoader
{
    
    public:
        
        SceneLoaderSPSB();
        virtual ~SceneLoaderSPSB();
        
        /* === Functions === */
        
        virtual Mesh* loadScene(const io::stringc &Filename, const io::stringc &TexturePath, const s32 Flags);
        
    protected:
        
        /* === Macros === */
        
        static const s32 MAGIC_NUMBER;
        static const s32 VERSION_MIN_SUPPORT;
        static const s32 VERSION_MAX_SUPPORT;
        
        #if defined(_MSC_VER)
        #   pragma pack(push, packing)
        #   pragma pack(1)
        #   define SP_PACK_STRUCT
        #elif defined(__GNUC__)
        #   define SP_PACK_STRUCT __attribute__((packed))
        #else
        #   define SP_PACK_STRUCT
        #endif
        
        /* === Enumerations === */
        
        enum EFileVersions
        {
            SPSB_VERSION_1_00 = 100,
            SPSB_VERSION_1_01 = 101,
            SPSB_VERSION_1_02 = 102,
            SPSB_VERSION_1_03 = 103,
        };
        
        enum EMeshBuildTypes
        {
            MESHBUILD_MODIFIED,
            MESHBUILD_BASICMESH,
            MESHBUILD_PROCMESH,
            MESHBUILD_MESHFILE,
        };
        
        enum EProceduralMeshes
        {
            PROCMESH_UNKNOWN,
            PROCMESH_STAIRCASE,
        };
        
        enum EAutoMapModes
        {
            AUTOMAP_DISABLE = 0,
            AUTOMAP_LOCAL,
            AUTOMAP_GLOBAL,
        };
        
        enum ETextureClassLayerTypes
        {
            TEXCLASSLAYER_CUSTOM = 0,
            TEXCLASSLAYER_LIGHTMAP,
            TEXCLASSLAYER_SHADER,
        };
        
        /* === Structures === */
        
        struct SLump
        {
            u32 Offset; //!< Start offset (in bytes)
            u32 Size;   //!< Size for the whole lump directory (in bytes)
            u32 Count;  //!< Count of elements
        };
        
        struct SHeader
        {
            s32 Magic;      //!< Magic number "SbSb". You can use (*(int*)"SbSb").
            s32 Version;    //!< Format version: 100, 101 and 102.
            
            SLump SceneConfig;
            
            SLump Meshes;
            SLump Lights;
            SLump Cameras;
            SLump WayPoints;
            
            SLump Textures;
            SLump TextureClasses;
            
            SLump Lightmaps;
            SLump LightmapScene;
            
            SLump Shaders;
        }
        SP_PACK_STRUCT;
        
        struct STextureClassLayer
        {
            STextureClassLayer()
                : Type(TEXCLASSLAYER_CUSTOM), Environment(video::TEXENV_MODULATE),
                Mapping(video::MAPGEN_DISABLE), DefaultTexture(0)
            {
            }
            ~STextureClassLayer()
            {
            }
            
            /* Members */
            ETextureClassLayerTypes Type;
            video::ETextureEnvTypes Environment;
            video::EMappingGenTypes Mapping;
            video::Texture* DefaultTexture;
        };
        
        struct STextureClass
        {
            std::vector<STextureClassLayer> Layers;
        };
        
        struct SBaseObject
        {
            s8 Type;
            u32 Id;
            u32 ParentId;
            io::stringc Name;
            io::stringc Flags;
            dim::matrix4f Position;
            dim::matrix4f Rotation;
            dim::matrix4f Scaling;
        };
        
        struct SWayPoint
        {
            SBaseObject BaseObject;
            std::vector<u32> NeighborIdList;
        };
        
        struct SMaterial
        {
            video::color DiffuseColor;
            video::color AmbientColor;
            video::color SpecularColor;
            video::color EmissionColor;
            
            f32 PolygonOffsetFactor;
            f32 PolygonOffsetUnits;
            
            s8 WireframeFront;
            s8 WireframeBack;
            s8 Shading;
            
            f32 Shininess;
            f32 AlphaReference;
            s8 DepthMethod;
            s8 AlphaMethod;
            s8 BlendSource;
            s8 BlendTarget;
            s8 RenderFace;
            
            s8 ColorMaterial;
            s8 Lighting;
            s8 Blending;
            s8 DepthBuffer;
            s8 Fog;
            s8 PolygonOffset;
            
            s8 CastShadow;
            s8 GetShadow;
        };
        //SP_PACK_STRUCT;
        
        struct SBasicMesh
        {
            s8 BasicMeshType;
            s32 SegsVert;
            s32 SegsHorz;
            f32 InnerRadius;
            f32 OuterRadius;
            s8 HasCap;
            s8 Shading;
            f32 RotationDeg;
            f32 RotationDist;
        }
        SP_PACK_STRUCT;
        
        struct SProcMeshStaircase
        {
            s8 StaricaseType;
            s8 Sides;
            s8 LeftSide;
            s8 RightSide;
            s32 StairsCount;
            f32 Width;
            f32 Height;
            f32 StraightDepth;
            f32 SpiralAngle;
            f32 SpiralRadius;
        }
        SP_PACK_STRUCT;
        
        struct SHardwareBuffer
        {
            s8 VertexFormat;
            s8 IndexFormat;
            s8 TangentVectorLayer;
            s8 BinormalVectorLayer;
            s32 InstanceCount;
        }
        SP_PACK_STRUCT;
        
        struct SSurfaceLayer
        {
            s8 AutoMapMode;
            f32 Density;
            s8 IsCustomTex;
            u32 TexId;
        }
        SP_PACK_STRUCT;
        
        struct STextureConfig
        {
            s8 Format;
            s8 HWFormat;
            s8 Filter;
            s8 MipMapFilter;
            s8 Anisotropy;
            s8 HasMipMaps;
            s8 WrapModeX;
            s8 WrapModeY;
            s8 WrapModeZ;
            video::color ColorKey;
            s8 Dimension;
            s8 RenderTarget;
        };
        //SP_PACK_STRUCT;
        
        struct SShaderRTObject
        {
            u32 ShaderClassId;
            u32 TexId;
        }
        SP_PACK_STRUCT;
        
        struct SParentQueue
        {
            scene::SceneNode* Object;
            u32 ParentId;
        };
        
        #ifdef _MSC_VER
        #   pragma pack(pop, packing)
        #endif
        
        #undef SP_PACK_STRUCT
        
        /* === Functions === */
        
        bool readHeader();
        bool seekLump(const SLump &Lump);
        
        virtual void readSceneConfig();
        virtual void readMeshes();
        virtual void readLights();
        virtual void readCameras();
        virtual void readWayPoints();
        virtual void readTextures();
        virtual void readTextureClasses();
        virtual void readLightmaps();
        virtual void readLightmapScene();
        virtual void readShaders();
        
        virtual void readBaseObject(SBaseObject &Object);
        virtual void readMaterial(SMaterial &Material, video::MaterialStates* MatStates);
        virtual void readShaderRTObject(SShaderRTObject &Object);
        
        virtual void applyQueues();
        
        virtual void applyBaseObject(scene::SceneNode* Node, const SBaseObject &Object);
        virtual void applyCameraRenderTarget(scene::Camera* Cam, video::Texture* Tex);
        
        virtual io::stringc getAbsolutePath(const io::stringc &Path) const;
        
        /* === Inline functions === */
        
        inline io::stringc readString() const
        {
            return File_->readStringData();
        }
        inline void readBuffer(void* Buffer, u32 Size, u32 Count = 1) const
        {
            File_->readBuffer(Buffer, Size, Count);
        }
        inline dim::matrix4f readMatrix() const
        {
            return File_->readMatrix<f32>();
        }
        
        /* === Templates === */
        
        template <typename T> inline T readValue() const
        {
            return File_->readValue<T>();
        }
        
        /* === Members === */
        
        s32 FormatVersion_;
        SHeader Header_;
        
        std::map<u32, video::Texture*> Textures_;
        std::map<u32, STextureClass> TextureClasses_;
        std::map<u32, scene::SceneNode*> ObjectIdMap_;
        
        std::vector<video::Texture*> LightmapTextures_;
        
        std::list<SWayPoint> WayPoints_;
        std::list<SParentQueue> QueueParents_;
        
        io::stringc ResourcePath_;
        scene::Mesh* SkyBox_;
        
};


} // /namespace scene

} // /namespace sp


#endif

#endif



// ================================================================================
