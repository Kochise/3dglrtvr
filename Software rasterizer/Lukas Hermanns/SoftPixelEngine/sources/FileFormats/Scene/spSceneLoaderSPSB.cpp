/*
 * SoftPixel Sandbox Scene loader file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "FileFormats/Scene/spSceneLoaderSPSB.hpp"
#include "Framework/Tools/spToolScriptLoader.hpp"

#ifdef SP_COMPILE_WITH_SCENELOADER_SPSB


#include "Platform/spSoftPixelDeviceOS.hpp"

#include <boost/foreach.hpp>


namespace sp
{

extern video::RenderSystem* __spVideoDriver;
extern scene::SceneGraph* __spSceneManager;

namespace scene
{


const s32 SceneLoaderSPSB::MAGIC_NUMBER         = *(s32*)"SpSb";
const s32 SceneLoaderSPSB::VERSION_MIN_SUPPORT  = SceneLoaderSPSB::SPSB_VERSION_1_00;
const s32 SceneLoaderSPSB::VERSION_MAX_SUPPORT  = SceneLoaderSPSB::SPSB_VERSION_1_03;

SceneLoaderSPSB::SceneLoaderSPSB() : SkyBox_(0), FormatVersion_(0)
{
}
SceneLoaderSPSB::~SceneLoaderSPSB()
{
}

Mesh* SceneLoaderSPSB::loadScene(const io::stringc &Filename, const io::stringc &TexturePath, const s32 Flags)
{
    if (!Flags)
    {
        io::Log::warning("Loading an SPSB file with no flags has no effect");
        return 0;
    }
    
    Filename_       = Filename;
    TexturePath_    = TexturePath;
    Flags_          = Flags;
    
    /* Open file for reading */
    File_ = FileSys_->readResourceFile(Filename);
    
    if (!File_)
        return 0;
    
    /* Read header */
    if (!readHeader())
        return 0;
    
    /* Read scene configuration, textures, shaders and lightmaps */
    if (Flags & SCENEFLAG_CONFIG)
        readSceneConfig();
    
    if (Flags & SCENEFLAG_TEXTURES)
    {
        readTextures();
        readTextureClasses();
    }
    
    if (FormatVersion_ >= SPSB_VERSION_1_02 && (Flags & SCENEFLAG_SHADERS))
        readShaders();
    
    if (Flags & SCENEFLAG_LIGHTMAPS)
    {
        readLightmaps();
        readLightmapScene();
    }
    
    /* Read scene objects */
    if (Flags & SCENEFLAG_MESHES)
        readMeshes();
    if (Flags & SCENEFLAG_LIGHTS)
        readLights();
    if (Flags & SCENEFLAG_CAMERAS)
        readCameras();
    if (Flags & SCENEFLAG_WAYPOINTS)
        readWayPoints();
    
    /* Apply queues */
    applyQueues();
    
    FileSys_->closeFile(File_);
    
    return 0;
}


/*
 * ======= Protected: =======
 */

bool SceneLoaderSPSB::readHeader()
{
    /* Read header buffer */
    readBuffer(&Header_, sizeof(SHeader));
    
    /* Check magic and version number */
    if (Header_.Magic != SceneLoaderSPSB::MAGIC_NUMBER)
    {
        io::Log::error("Scene file has invalid magic number");
        return false;
    }
    
    if (Header_.Version < SceneLoaderSPSB::VERSION_MIN_SUPPORT)
    {
        io::Log::error("Scene file version is too old to be loaded");
        return false;
    }
    
    if (Header_.Version > SceneLoaderSPSB::VERSION_MAX_SUPPORT)
    {
        io::Log::error("Scene loader is too old to load this scene file");
        return false;
    }
    
    FormatVersion_ = Header_.Version;
    
    /* Print format version */
    const s32 Major = FormatVersion_ / 100;
    const s32 Minor = FormatVersion_ % 100;
    
    io::Log::message(
        "Format version " + io::stringc(Major) + "." +
        (Minor < 10 ? io::stringc("0") + io::stringc(Minor) : io::stringc(Minor))
    );
    
    return true;
}

bool SceneLoaderSPSB::seekLump(const SLump &Lump)
{
    if (Lump.Offset > 0 && Lump.Size > 0)
    {
        File_->setSeek(Lump.Offset);
        return true;
    }
    return false;
}


/*
 * ======= Reading functions for main lumps: =======
 */

void SceneLoaderSPSB::readSceneConfig()
{
    if (!seekLump(Header_.SceneConfig))
        return;
    
    /* Read resource path */
    if (FormatVersion_ >= SPSB_VERSION_1_03)
        ResourcePath_ = readString();
    
    /* Read skybox texture filenames */
    video::Texture* SkyboxTextures[6] = { 0 };
    bool hasSkyBox = true;
    
    __spVideoDriver->setTextureGenFlags(video::TEXGEN_WRAP, video::TEXWRAP_CLAMP);
    
    for (s32 i = 0; i < 6; ++i)
    {
        const io::stringc Filename = readString();
        
        if (Filename.size())
            SkyboxTextures[i] = __spVideoDriver->loadTexture(getAbsolutePath(Filename));
        else
            hasSkyBox = false;
    }
    
    __spVideoDriver->setTextureGenFlags(video::TEXGEN_WRAP, video::TEXWRAP_REPEAT);
    
    /* Create skybox */
    if (hasSkyBox)
        SkyBox_ = __spSceneManager->createSkyBox(SkyboxTextures);
}

void SceneLoaderSPSB::readMeshes()
{
    if (!seekLump(Header_.Meshes))
        return;
    
    for (u32 i = 0; i < Header_.Meshes.Count; ++i)
    {
        /* Read base object data */
        SBaseObject BaseObject;
        readBaseObject(BaseObject);
        
        scene::Mesh* Object = 0;
        
        const EMeshBuildTypes BuildType = static_cast<EMeshBuildTypes>(readValue<s8>());
        
        switch (BuildType)
        {
            case MESHBUILD_BASICMESH:
            {
                SBasicMesh BasicMesh;
                readBuffer(&BasicMesh, sizeof(SBasicMesh));
                
                Object = __spSceneManager->createMesh(
                    static_cast<scene::EBasicMeshes>(BasicMesh.BasicMeshType),
                    scene::SMeshConstruct(
                        BasicMesh.SegsVert,
                        BasicMesh.SegsHorz,
                        BasicMesh.InnerRadius,
                        BasicMesh.OuterRadius,
                        BasicMesh.RotationDeg,
                        BasicMesh.RotationDist,
                        BasicMesh.HasCap,
                        static_cast<video::EShadingTypes>(BasicMesh.Shading)
                    )
                );
            }
            break;
            
            case MESHBUILD_PROCMESH:
            {
                const EProceduralMeshes ProcMeshType = static_cast<EProceduralMeshes>(readValue<s8>());
                
                switch (ProcMeshType)
                {
                    case PROCMESH_STAIRCASE:
                    {
                        SProcMeshStaircase ProcMeshStaircase;
                        readBuffer(&ProcMeshStaircase, sizeof(SProcMeshStaircase));
                    }
                    break;
                    
                    default:
                        break;
                }
                
                Object = __spSceneManager->createMesh();
            }
            break;
            
            case MESHBUILD_MESHFILE:
            {
                const io::stringc Filename = readString();
                
                Object = __spSceneManager->loadMesh(getAbsolutePath(Filename));
            }
            break;
            
            default:
                break;
        }
        
        if (!Object)
        {
            io::Log::error("Unexpected failure while loading meshes");
            return;
        }
        
        /* Setup object name and location */
        applyBaseObject(Object, BaseObject);
        
        /* Read and apply shader class */
        if (FormatVersion_ >= SPSB_VERSION_1_02)
        {
            u32 ShaderClassId = readValue<u32>();
            
            //...
        }
        
        /* Read and apply material */
        SMaterial Material;
        readMaterial(Material, Object->getMaterial());
        
        if (Material.GetShadow)
            Object->setVisible(false);
        
        Object->updateNormals();
        
        /* Read and setup surfaces */
        const u32 SurfaceCount = readValue<u32>();
        
        for (u32 s = 0; s < SurfaceCount; ++s)
        {
            video::MeshBuffer* Surface = Object->getMeshBuffer(s);
            
            if (!Surface)
            {
                io::Log::error("Unexpected failure while loading surfaces");
                return;
            }
            
            Surface->setName(readString());
            
            if (FormatVersion_ >= SPSB_VERSION_1_02)
            {
                /* Read hardware buffer information */
                SHardwareBuffer HardwareBuffer;
                readBuffer(&HardwareBuffer, sizeof(SHardwareBuffer));
                
                switch (HardwareBuffer.VertexFormat)
                {
                    case 0: Surface->setVertexFormat(__spVideoDriver->getVertexFormatDefault()); break;
                    case 1: Surface->setVertexFormat(__spVideoDriver->getVertexFormatReduced()); break;
                    case 2: Surface->setVertexFormat(__spVideoDriver->getVertexFormatExtended()); break;
                    case 3: Surface->setVertexFormat(__spVideoDriver->getVertexFormatFull()); break;
                }
                
                Surface->setIndexFormat(
                    static_cast<video::ERendererDataTypes>(HardwareBuffer.IndexFormat)
                );
                
                Surface->setHardwareInstancing(HardwareBuffer.InstanceCount);
            }
            
            /* Read texture information */
            const u32 TexClassId = readValue<u32>();
            const s8 LayerCount = readValue<s8>();
            
            /* Read and get texture class */
            const STextureClass* TexClass = 0;
            
            if (TexClassId > 0)
            {
                std::map<u32, STextureClass>::iterator itTexClass = TextureClasses_.find(TexClassId);
                
                if (itTexClass != TextureClasses_.end())
                    TexClass = &(itTexClass->second);
                else
                    io::Log::error("Wrong ID number for texture class");
            }
            
            /* Read texture layers */
            for (s8 l = 0; l < LayerCount; ++l)
            {
                SSurfaceLayer Layer;
                readBuffer(&Layer, sizeof(SSurfaceLayer));
                
                /* Setup texture coordinate generation */
                if (Layer.AutoMapMode != AUTOMAP_DISABLE)
                    Object->textureAutoMap(l, Layer.Density, s, Layer.AutoMapMode == AUTOMAP_GLOBAL);
                
                if (Layer.TexId > 0)
                {
                    /* Apply texture */
                    video::Texture* Tex = Textures_[Layer.TexId];
                    
                    if (Tex)
                    {
                        if (Surface->getTexture(l))
                            Surface->setTexture(l, Tex);
                        else
                            Surface->addTexture(Tex);
                    }
                    else
                        io::Log::error("Wrong ID number for mesh texture");
                }
                
                if (TexClass && l < TexClass->Layers.size())
                {
                    /* Setup texture configuration from texture class */
                    const STextureClassLayer* ClassLayer = &(TexClass->Layers[l]);
                    
                    Surface->setTextureEnv(l, ClassLayer->Environment);
                    Surface->setMappingGen(l, ClassLayer->Mapping);
                    
                    if (!Layer.TexId && ClassLayer->DefaultTexture)
                    {
                        /* Apply default texture */
                        if (Surface->getTexture(l))
                            Surface->setTexture(l, ClassLayer->DefaultTexture);
                        else
                            Surface->addTexture(ClassLayer->DefaultTexture);
                    }
                }
            }
        }
        
        if (FormatVersion_ >= SPSB_VERSION_1_01)
        {
            /* Read collision information */
            const s8 CollisionModel = readValue<s8>();
            const scene::EPickingTypes PickingModel = static_cast<scene::EPickingTypes>(readValue<s8>());
            
            //...
        }
    }
}

void SceneLoaderSPSB::readLights()
{
    if (!seekLump(Header_.Lights))
        return;
    
    for (u32 i = 0; i < Header_.Lights.Count; ++i)
    {
        /* Read base object data */
        SBaseObject BaseObject;
        readBaseObject(BaseObject);
        
        /* Read light settings */
        const s8 LightModel         = readValue<s8>();
        const s8 Volumetric         = readValue<s8>();
        const f32 VolumetricRadius  = readValue<f32>();
        
        /* Create light source object */
        scene::Light* Object = __spSceneManager->createLight(
            static_cast<scene::ELightModels>(LightModel)
        );
        
        Object->setVolumetric(Volumetric != 0);
        Object->setVolumetricRadius(VolumetricRadius);
        
        /* Setup object name and location */
        applyBaseObject(Object, BaseObject);
        
        if (FormatVersion_ >= SPSB_VERSION_1_02)
        {
            /* Read shader render target object */
            SShaderRTObject ShaderRTObject;
            readShaderRTObject(ShaderRTObject);
            
            //...
        }
    }
}

void SceneLoaderSPSB::readCameras()
{
    if (!seekLump(Header_.Cameras))
        return;
    
    for (u32 i = 0; i < Header_.Cameras.Count; ++i)
    {
        /* Read base object data */
        SBaseObject BaseObject;
        readBaseObject(BaseObject);
        
        /* Read camera settings */
        const f32 FieldOfView   = readValue<f32>();
        const f32 NearPlane     = readValue<f32>();
        const f32 FarPlane      = readValue<f32>();
        
        /* Create camera object */
        scene::Camera* Object = __spSceneManager->createCamera();
        
        Object->setRange(NearPlane, FarPlane);
        Object->setFOV(FieldOfView);
        
        /* Setup object name and location */
        applyBaseObject(Object, BaseObject);
        
        #if 0
        if (FormatVersion_ >= SPSB_VERSION_1_03)
        {
            /* Read render target texture */
            const u32 TexId = readValue<u32>();
            
            if (TexId > 0)
            {
                video::Texture* Tex = Textures_[TexId];
                
                if (Tex)
                    applyCameraRenderTarget(Object, Tex);
                else
                    io::Log::error("Wrong ID number for camera's render target texture");
            }
        }
        #endif
    }
}

void SceneLoaderSPSB::readWayPoints()
{
    if (!seekLump(Header_.WayPoints))
        return;
    
    for (u32 i = 0; i < Header_.WayPoints.Count; ++i)
    {
        SWayPoint WayPoint;
        
        /* Read base object data */
        readBaseObject(WayPoint.BaseObject);
        
        /* Read neighbor ID list */
        const u32 NeighborCount = readValue<u32>();
        
        WayPoint.NeighborIdList.resize(NeighborCount);
        
        for (u32 j = 0; j < NeighborCount; ++j)
            WayPoint.NeighborIdList[j] = readValue<u32>();
        
        WayPoints_.push_back(WayPoint);
    }
}

void SceneLoaderSPSB::readTextures()
{
    if (!seekLump(Header_.Textures))
        return;
    
    for (u32 i = 0; i < Header_.Textures.Count; ++i)
    {
        /* Create new texture */
        const u32 Id = readValue<u32>();
        const io::stringc Filename = readString();
        
        video::Texture* Tex = 0;
        
        if (Filename.size())
            Tex = __spVideoDriver->loadTexture(getAbsolutePath(Filename));
        else
        {
            s32 Width = 1, Height = 1;
            
            if (FormatVersion_ >= SPSB_VERSION_1_02)
            {
                Width = readValue<s32>();
                Height = readValue<s32>();
            }
            
            Tex = __spVideoDriver->createTexture(dim::size2di(Width, Height));
        }
        
        /* Read texture information */
        STextureConfig TexConfig;
        readBuffer(&TexConfig, sizeof(STextureConfig));
        
        Tex->setFormat(static_cast<video::EPixelFormats>(TexConfig.Format));
        Tex->setHardwareFormat(static_cast<video::EHWTextureFormats>(TexConfig.HWFormat));
        Tex->setFilter(static_cast<video::ETextureFilters>(TexConfig.Filter));
        Tex->setMipMapFilter(static_cast<video::ETextureMipMapFilters>(TexConfig.MipMapFilter));
        Tex->setAnisotropicSamples(TexConfig.Anisotropy);
        Tex->setMipMapping(TexConfig.HasMipMaps != 0);
        Tex->setWrapMode(
            static_cast<video::ETextureWrapModes>(TexConfig.WrapModeX),
            static_cast<video::ETextureWrapModes>(TexConfig.WrapModeY),
            static_cast<video::ETextureWrapModes>(TexConfig.WrapModeZ)
        );
        Tex->setColorKey(TexConfig.ColorKey);
        Tex->setDimension(static_cast<video::ETextureDimensions>(TexConfig.Dimension));
        Tex->setRenderTarget(TexConfig.RenderTarget != 0);
        
        /* Add texture to the map */
        Textures_[Id] = Tex;
    }
}

void SceneLoaderSPSB::readTextureClasses()
{
    if (!seekLump(Header_.TextureClasses))
        return;
    
    for (u32 i = 0; i < Header_.TextureClasses.Count; ++i)
    {
        /* Create new texture class */
        const u32 Id = readValue<u32>();
        
        STextureClass* TexClass = &(TextureClasses_[Id]);
        
        /* Read unused class name */
        readString();
        
        /* Read layers */
        const u8 LayerCount = readValue<u8>();
        
        for (u8 l = 0; l < LayerCount; ++l)
        {
            STextureClassLayer Layer;
            
            /* Read unused layer name */
            readString();
            
            /* Read layer information */
            Layer.Type          = static_cast<ETextureClassLayerTypes>(readValue<s8>());
            Layer.Environment   = static_cast<video::ETextureEnvTypes>(readValue<s8>());
            Layer.Mapping       = static_cast<video::EMappingGenTypes>(readValue<s8>());
            
            /* Read and get default texture */
            const u32 TexId = readValue<u32>();
            
            if (TexId > 0)
            {
                video::Texture* Tex = Textures_[TexId];
                
                if (Tex)
                    Layer.DefaultTexture = Tex;
                else
                    io::Log::error("Wrong ID number for texture class layer");
            }
            
            TexClass->Layers.push_back(Layer);
        }
    }
}

void SceneLoaderSPSB::readLightmaps()
{
    if (!seekLump(Header_.Lightmaps))
        return;
    
    /* Temporary memory */
    video::color Texel;
    video::Texture* Tex = 0;
    
    for (u32 i = 0; i < Header_.Lightmaps.Count; ++i)
    {
        /* Read size and create texture */
        const s32 Size = readValue<s32>();
        
        Tex = __spVideoDriver->createTexture(Size);
        
        dim::point2di Pos;
        
        /* Read lightmap texel blocks */
        for (s32 j = 0, c = Size * Size / 8; j < c; ++j)
        {
            u8 BitSet = readValue<u8>();
            
            for (s32 k = 0; k < 8; ++k)
            {
                /* Read texel color */
                if (math::getBit(BitSet, k))
                {
                    Texel.Red   = readValue<u8>();
                    Texel.Green = readValue<u8>();
                    Texel.Blue  = readValue<u8>();
                }
                else
                    Texel = video::color(readValue<u8>());
                
                /* Set texel color */
                Tex->setPixelColor(Pos, Texel);
                
                /* Boost texel coordinate */
                if (++Pos.X >= Size)
                {
                    Pos.X = 0;
                    ++Pos.Y;
                }
            }
        }
        
        /* Update image buffer and add texture to the list */
        Tex->updateImageBuffer();
        
        LightmapTextures_.push_back(Tex);
    }
}

void SceneLoaderSPSB::readLightmapScene()
{
    if (!seekLump(Header_.LightmapScene))
        return;
    
    for (u32 i = 0; i < Header_.LightmapScene.Count; ++i)
    {
        /* Create mesh and set first mesh as main lightmap scene object */
        scene::Mesh* Mesh = __spSceneManager->createMesh();
        
        Mesh->setName("LM:" + readString());
        const u32 SurfaceCount = readValue<u32>();
        
        Mesh->getMaterial()->setLighting(false);
        
        /* Read all surfaces */
        for (u32 s = 0; s < SurfaceCount; ++s)
        {
            const io::stringc Name(readString());
            
            const s8 VertexFormat       = readValue<s8>(); //!!! todo
            const s8 IndexFormat        = readValue<s8>();
            const u8 LayerCount         = readValue<u8>();
            const u32 LightmapTexIndex  = readValue<u32>();
            
            /* Create new surface */
            video::MeshBuffer* Surface = Mesh->createMeshBuffer(
                __spVideoDriver->getVertexFormatDefault(), static_cast<video::ERendererDataTypes>(IndexFormat)
            );
            
            Surface->setName(Name);
            
            /* Read surface layers */
            for (u8 l = 0; l < LayerCount; ++l)
            {
                const u32 TexId         = readValue<u32>();
                const s8 Environment    = readValue<s8>();
                const s8 MappingGen     = readValue<s8>();
                
                if (TexId > 0)
                {
                    video::Texture* Tex = Textures_[TexId];
                    
                    if (Tex)
                    {
                        Surface->addTexture(Tex);
                        Surface->setTextureEnv(l, static_cast<video::ETextureEnvTypes>(Environment));
                        Surface->setMappingGen(l, static_cast<video::EMappingGenTypes>(MappingGen));
                    }
                    else
                        io::Log::error("Wrong ID number for lightmap scene's texture");
                }
            }
            
            /* Apply lightmap texture */
            if (LightmapTexIndex < LightmapTextures_.size())
                Surface->addTexture(LightmapTextures_[LightmapTexIndex]);
            
            /* Read all vertices */
            const u32 VertexCount = readValue<u32>();
            
            for (u32 j = 0; j < VertexCount; ++j)
            {
                const dim::vector3df Coord(readValue<dim::vector3df>());
                const dim::vector3df Normal(readValue<dim::vector3df>());
                
                Surface->addVertex(Coord, Normal, readValue<dim::point2df>());
                
                for (u8 l = 0; l < LayerCount; ++l)
                    Surface->setVertexTexCoord(j, readValue<dim::point2df>(), l);
                
                /* Create new triangle */
                if ((j + 1) % 3 == 0)
                {
                    Surface->addTriangle(0, 1, 2);
                    Surface->addIndexOffset(3);
                }
            }
        }
        
        Mesh->updateMeshBuffer();
        
        #if 1
        break; //!!! -> currently only read the main lightmap scene object for debugging
        #endif
    }
}

void SceneLoaderSPSB::readShaders()
{
    if (!seekLump(Header_.Shaders))
        return;
    
    
}


/*
 * ======= Base reading functions: =======
 */

void SceneLoaderSPSB::readBaseObject(SBaseObject &Object)
{
    Object.Type     = readValue<s8>();
    Object.Id       = readValue<u32>();
    Object.ParentId = readValue<u32>();
    Object.Name     = readString();
    
    if (FormatVersion_ >= SPSB_VERSION_1_01)
        Object.Flags = readString();
    
    Object.Position = readMatrix();
    Object.Rotation = readMatrix();
    Object.Scaling  = readMatrix();
}

void SceneLoaderSPSB::readMaterial(SMaterial &Material, video::MaterialStates* MatStates)
{
    /* Read material buffer */
    readBuffer(&Material, sizeof(SMaterial));
    
    /* Apply material settings */
    MatStates->setDiffuseColor(Material.DiffuseColor);
    MatStates->setAmbientColor(Material.AmbientColor);
    MatStates->setSpecularColor(Material.SpecularColor);
    MatStates->setEmissionColor(Material.EmissionColor);
    
    MatStates->setPolygonOffset(
        Material.PolygonOffset != 0, Material.PolygonOffsetFactor, Material.PolygonOffsetUnits
    );
    MatStates->setWireframe(
        static_cast<video::EWireframeTypes>(Material.WireframeFront),
        static_cast<video::EWireframeTypes>(Material.WireframeBack)
    );
    
    MatStates->setShininess(Material.Shininess);
    MatStates->setAlphaReference(Material.AlphaReference);
    MatStates->setDepthMethod(static_cast<video::ESizeComparisionTypes>(Material.DepthMethod));
    MatStates->setAlphaMethod(static_cast<video::ESizeComparisionTypes>(Material.AlphaMethod));
    MatStates->setBlendSource(static_cast<video::EBlendingTypes>(Material.BlendSource));
    MatStates->setBlendTarget(static_cast<video::EBlendingTypes>(Material.BlendTarget));
    MatStates->setRenderFace(static_cast<video::EFaceTypes>(Material.RenderFace));
    
    MatStates->setColorMaterial(Material.ColorMaterial != 0);
    MatStates->setLighting(Material.Lighting != 0);
    MatStates->setBlending(Material.Blending != 0);
    MatStates->setDepthBuffer(Material.DepthBuffer != 0);
    MatStates->setFog(Material.Fog != 0);
    MatStates->setShading(static_cast<video::EShadingTypes>(Material.Shading));
}

void SceneLoaderSPSB::readShaderRTObject(SShaderRTObject &Object)
{
    readBuffer(&Object, sizeof(SShaderRTObject));
}


/*
 * ======= Other functions: =======
 */

void SceneLoaderSPSB::applyQueues()
{
    /* Apply parent queue */
    foreach (const SParentQueue &Queue, QueueParents_)
    {
        if (Queue.Object)
        {
            scene::SceneNode* Parent = ObjectIdMap_[Queue.ParentId];
            
            if (Parent)
                Queue.Object->setParent(Parent, true);
        }
    }
}

void SceneLoaderSPSB::applyBaseObject(scene::SceneNode* Node, const SBaseObject &Object)
{
    /* Set name and transformation */
    Node->setName(Object.Name);
    Node->setPositionMatrix(Object.Position);
    Node->setRotationMatrix(Object.Rotation);
    Node->setScaleMatrix(Object.Scaling);
    
    /* Map object ID */
    ObjectIdMap_[Object.Id] = Node;
    
    /* Add parent queue */
    if (Object.ParentId)
    {
        SParentQueue Queue;
        {
            Queue.Object    = Node;
            Queue.ParentId  = Object.ParentId;
        }
        QueueParents_.push_back(Queue);
    }
}

void SceneLoaderSPSB::applyCameraRenderTarget(scene::Camera* Cam, video::Texture* Tex)
{
    // do nothing
}

io::stringc SceneLoaderSPSB::getAbsolutePath(const io::stringc &Path) const
{
    if (!Path.size() || !ResourcePath_.size() || !(Flags_ & SCENEFLAG_ABSOLUTEPATH))
        return Path;
    
    const io::stringc AbsolutePath(ResourcePath_ + Path);
    
    if (!io::FileSystem().findFile(AbsolutePath))
        return Path;
    
    return AbsolutePath;
}


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
