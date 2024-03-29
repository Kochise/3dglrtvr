/*
 * Scene graph file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "SceneGraph/spSceneGraph.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"
#include "Base/spInternalDeclarations.hpp"

#include <boost/foreach.hpp>


namespace sp
{

extern io::InputControl* __spInputControl;
extern video::RenderSystem* __spVideoDriver;
extern scene::SceneGraph* __spSceneManager;

namespace scene
{


/*
 * Internal members
 */

StencilManager* __spStencilManager = 0;


/*
 * Internal functions
 */

bool cmpObjectLights(Light* &obj1, Light* &obj2)
{
    /* Compare visiblity */
    if (obj1->getVisible() != obj2->getVisible())
        return obj1->getVisible();
    
    /* Compare light model */
    /*if (obj1->getLightingType() == LIGHT_DIRECTIONAL && obj2->getLightingType() != LIGHT_DIRECTIONAL)
        return true;
    if (obj1->getLightingType() != LIGHT_DIRECTIONAL && obj2->getLightingType() == LIGHT_DIRECTIONAL)
        return false;*/
    
    /* Compare distance to camera */
    const dim::vector3df CamPos(__spSceneManager->getActiveCamera()->getPosition(true));
    
    return
        math::getDistanceSq(obj1->getPosition(true), CamPos) <
        math::getDistanceSq(obj2->getPosition(true), CamPos);
}

bool cmpObjectSceneNodes(SceneNode* &obj1, SceneNode* &obj2)
{
    /* Compare visiblity */
    if (obj1->getVisible() != obj2->getVisible())
        return obj1->getVisible();
    
    /* Compare material nodes */
    if (obj1->getType() >= NODE_MESH && obj2->getType() >= NODE_MESH)
        return static_cast<MaterialNode*>(obj1)->compare(static_cast<MaterialNode*>(obj2));
    
    /* Compare types */
    return obj1->getType() > obj2->getType();
}

bool cmpObjectRenderNodes(RenderNode* &obj1, RenderNode* &obj2)
{
    /* Compare visiblity */
    if (obj1->getVisible() != obj2->getVisible())
        return obj1->getVisible();
    
    /* Compare material nodes */
    if (obj1->getType() >= NODE_MESH && obj2->getType() >= NODE_MESH)
        return static_cast<MaterialNode*>(obj1)->compare(static_cast<MaterialNode*>(obj2));
    
    /* Compare types */
    return obj1->getType() > obj2->getType();
}


/*
 * SceneGraph class
 */

video::VertexFormat* SceneGraph::DefaultVertexFormat_       = 0;
video::ERendererDataTypes SceneGraph::DefaultIndexFormat_   = video::DATATYPE_UNSIGNED_SHORT;

bool SceneGraph::TextureLoadingState_ = true;;

SceneGraph::SceneGraph(const ESceneGraphs Type)
    : RenderNode(NODE_SCENEGRAPH), GraphType_(Type), hasChildTree_(false),
    ActiveCamera_(0), ActiveMesh_(0), isStencilEffects_(false),
    WireframeFront_(video::WIREFRAME_SOLID), WireframeBack_(video::WIREFRAME_SOLID)
{
    /* Initialization */
    memset(__spLightIDList, 0, sizeof(__spLightIDList));
}
SceneGraph::~SceneGraph()
{
    /* Delete all scene nodes */
    MemoryManager::deleteList(NodeList_);
    MemoryManager::deleteList(CameraList_);
    MemoryManager::deleteList(LightList_);
    MemoryManager::deleteList(RenderList_);
    
    /* Delete all pointers */
    MemoryManager::deleteMemory(__spStencilManager);
}

StencilManager* SceneGraph::getStencilManager()
{
    return __spStencilManager ?
        __spStencilManager :
        __spStencilManager = MemoryManager::createMemory<scene::StencilManager>("scene::StencilManager");
}

/* Object lists */

void SceneGraph::addSceneNode(SceneNode* Object)
{
    if (Object) NodeList_.push_back(Object);
}
void SceneGraph::removeSceneNode(SceneNode* Object)
{
    if (Object) removeObjectFromList<SceneNode, SceneNode>(Object, NodeList_);
}

void SceneGraph::addSceneNode(Camera* Object)
{
    if (Object) CameraList_.push_back(Object);
}
void SceneGraph::removeSceneNode(Camera* Object)
{
    if (Object) removeObjectFromList<Camera, Camera>(Object, CameraList_);
}

void SceneGraph::addSceneNode(Light* Object)
{
    if (Object) LightList_.push_back(Object);
}
void SceneGraph::removeSceneNode(Light* Object)
{
    if (Object) removeObjectFromList<Light, Light>(Object, LightList_);
}

void SceneGraph::addSceneNode(RenderNode* Object)
{
    if (Object) RenderList_.push_back(Object);
}
void SceneGraph::removeSceneNode(RenderNode* Object)
{
    if (Object) removeObjectFromList<RenderNode, RenderNode>(Object, RenderList_);
}

void SceneGraph::addRootNode(SceneNode* Object)
{
    // do nothing
}
void SceneGraph::removeRootNode(SceneNode* Object)
{
    // do nothing
}

/* Create a simple scene node */

SceneNode* SceneGraph::createNode()
{
    SceneNode* NewSceneNode = new SceneNode(NODE_BASICNODE);
    addSceneNode(NewSceneNode);
    return NewSceneNode;
}

/* === Meshes (empty, primitives, models, supershapes, skyboxes, highmaps) === */

Mesh* SceneGraph::createMesh()
{
    Mesh* NewMesh = MemoryManager::createMemory<Mesh>("scene::Mesh (Empty)");
    NewMesh->getMaterial()->setWireframe(WireframeFront_, WireframeBack_);
    addSceneNode(NewMesh);
    return NewMesh;
}

Mesh* SceneGraph::createMesh(const EBasicMeshes Model, const SMeshConstruct &BuildConstruct)
{
    Mesh* NewMesh = MemoryManager::createMemory<Mesh>("scene::Mesh (Basic)");
    BasicMeshGenerator().createMesh(NewMesh, Model, BuildConstruct);
    NewMesh->getMaterial()->setWireframe(WireframeFront_, WireframeBack_);
    addSceneNode(NewMesh);
    return NewMesh;
}

Mesh* SceneGraph::createSuperShape(const f32 ValueList[12], s32 Detail)
{
    Mesh* NewMesh = MemoryManager::createMemory<Mesh>("scene::Mesh (SuperShape)");
    BasicMeshGenerator().createSuperShape(NewMesh, ValueList, Detail);
    NewMesh->getMaterial()->setWireframe(WireframeFront_, WireframeBack_);
    addSceneNode(NewMesh);
    return NewMesh;
}

Mesh* SceneGraph::createSkyBox(video::Texture* TextureList[6], f32 Radius)
{
    Mesh* NewMesh = BasicMeshGenerator().createSkyBox(TextureList, Radius);
    NewMesh->getMaterial()->setWireframe(WireframeFront_, WireframeBack_);
    addSceneNode(NewMesh);
    return NewMesh;
}

Mesh* SceneGraph::createHeightField(const video::Texture* TexHeightMap, const s32 Segments)
{
    Mesh* NewMesh = BasicMeshGenerator().createHeightField(TexHeightMap, Segments);
    NewMesh->getMaterial()->setWireframe(WireframeFront_, WireframeBack_);
    addSceneNode(NewMesh);
    return NewMesh;
}

Mesh* SceneGraph::createMeshList(std::list<Mesh*> MergeList, bool isOldDelete)
{
    /* Allocate a new mesh */
    Mesh* NewMesh = MemoryManager::createMemory<Mesh>("scene::Mesh (List)");
    NewMesh->getMaterial()->setWireframe(WireframeFront_, WireframeBack_);
    
    /* Temporary variables */
    u32 Indices[3];
    u32 s = 0, i = 0, k;
    dim::matrix4f NormalMatrix;
    
    std::list<video::Texture*> TexList;
    
    video::MeshBuffer* Surface = 0, * NewSurface = 0;
    
    // Loop all meshes in the list
    foreach (Mesh* Obj, MergeList)
    {
        /* Matrix transformation */
        spWorldMatrix.reset();
        
        Obj->updateModelviewMatrix();
        
        NormalMatrix = spWorldMatrix;
        NormalMatrix.setPosition(0);
        
        /* Loop the surfaces of the current mesh */
        for (s = 0; s < Obj->getMeshBufferCount(); ++s)
        {
            Surface = Obj->getMeshBuffer(s);
            
            /* Create a new surface */
            NewSurface = NewMesh->createMeshBuffer(
                Surface->getVertexFormat(), Surface->getIndexFormat()->getDataType()
            );
            
            NewSurface->setName(Surface->getName().size() ? Surface->getName() : Obj->getName());
            
            /* Get the texture list */
            TexList = Surface->getTextureList();
            
            /* Add all textures */
            for (std::list<video::Texture*>::iterator itTex = TexList.begin(); itTex != TexList.end(); ++itTex)
                NewSurface->addTexture(*itTex);
            
            /* Add all vertices */
            for (i = 0; i < Surface->getVertexCount(); ++i)
            {
                k = NewSurface->addVertex(
                    spWorldMatrix       * Surface->getVertexCoord(i),
                    (NormalMatrix       * Surface->getVertexNormal(i)).normalize(),
                    spTextureMatrix[0]  * Surface->getVertexTexCoord(i),
                    Surface->getVertexColor(i),
                    Surface->getVertexFog(i)
                );
                
                /* Set texture coordinates */
                for (u8 j = 1; j < 8; ++j)
                {
                    NewSurface->setVertexTexCoord(
                        k, spTextureMatrix[j] * Surface->getVertexTexCoord(i, j), j
                    );
                }
            }
            
            /* Add all triangles */
            for (i = 0; i < Surface->getTriangleCount(); ++i)
            {
                Surface->getTriangleIndices(i, Indices);
                NewSurface->addTriangle(Indices[0], Indices[1], Indices[2]);
            }
        }
    }
    
    /* Delete the original meshs */
    if (isOldDelete)
    {
        foreach (Mesh* Obj, MergeList)
            deleteNode(Obj);
    }
    
    /* Update mesh buffer and add the mesh to the list */
    NewMesh->updateMeshBuffer();
    
    addSceneNode(NewMesh);
    
    return NewMesh;
}

Mesh* SceneGraph::createMeshSurface(Mesh* Model, u32 Surface)
{
    /* Check if surface is not out of range */
    if (Surface >= Model->getMeshBufferCount())
        return 0;
    
    /* Allocate a new mesh */
    Mesh* NewMesh = MemoryManager::createMemory<Mesh>("scene::Mesh (Surface)");
    NewMesh->getMaterial()->setWireframe(WireframeFront_, WireframeBack_);
    
    /* Temporary variables */
    s32 i;
    u32 v[3];
    video::Texture* Tex = 0;
    
    /* Create a new surface */
    video::MeshBuffer* NewSurface = NewMesh->createMeshBuffer();
    
    /* Get current surface */
    video::MeshBuffer* OldSurface = Model->getMeshBuffer(Surface);
    
    /* Loop for all vertices of the specified surface */
    for (i = 0; i < OldSurface->getVertexCount(); ++i)
    {
        NewSurface->addVertex(
            OldSurface->getVertexCoord(i),
            OldSurface->getVertexNormal(i),
            OldSurface->getVertexTexCoord(i),
            OldSurface->getVertexColor(i),
            OldSurface->getVertexFog(i)
        );
    }
    
    /* Loop for all triangles of the specified surface */
    for (i = 0; i < OldSurface->getTriangleCount(); ++i)
    {
        OldSurface->getTriangleIndices(i, v);
        NewSurface->addTriangle(v[0], v[1], v[2]);
    }
    
    /* Add all textures of the specified surface */
    for (i = 0; i < MAX_COUNT_OF_TEXTURES; ++i)
    {
        if (Tex = OldSurface->getTexture(i))
            NewSurface->addTexture(Tex);
    }
    
    /* Update mesh buffer and add the mesh to the list */
    NewMesh->updateMeshBuffer();
    
    addSceneNode(NewMesh);
    
    return NewMesh;
}

Mesh* SceneGraph::loadMesh(io::stringc Filename, io::stringc TexturePath, const EMeshFileFormats Format)
{
    /* Information message */
    Filename.adjustPath();
    io::Log::message("Load mesh: \"" + Filename + "\"");
    io::Log::upperTab();
    
    /* Temporary variables/ constants */
    if (TexturePath == video::TEXPATH_IGNORE)
        TexturePath = Filename.getPathPart();
    
    MeshLoader* Loader = 0;
    
    /* === Check if the file does exist === */
    
    if (!io::FileSystem().findFile(Filename))
    {
        /* Error message */
        io::Log::error("Could not found mesh file");
        io::Log::lowerTab();
        
        /* Return an empty model */
        return createMesh();
    }
    
    /* === Select format === */
    
    const EMeshFileFormats FileFormat = getMeshFileFormat(Filename, Format);
    
    switch (FileFormat)
    {
        #ifdef SP_COMPILE_WITH_MESHLOADER_SPM
        case MESHFORMAT_SPM:
            Loader = MemoryManager::createMemory<MeshLoaderSPM>("scene::MeshLoaderSPM"); break;
        #endif
        #ifdef SP_COMPILE_WITH_MESHLOADER_3DS
        case MESHFORMAT_3DS:
            Loader = MemoryManager::createMemory<MeshLoader3DS>("scene::MeshLoader3DS"); break;
        #endif
        #ifdef SP_COMPILE_WITH_MESHLOADER_MS3D
        case MESHFORMAT_MS3D:
            Loader = MemoryManager::createMemory<MeshLoaderMS3D>("scene::MeshLoaderMS3D"); break;
        #endif
        #ifdef SP_COMPILE_WITH_MESHLOADER_X
        case MESHFORMAT_X:
            Loader = MemoryManager::createMemory<MeshLoaderX>("scene::MeshLoaderX"); break;
        #endif
        #ifdef SP_COMPILE_WITH_MESHLOADER_B3D
        case MESHFORMAT_B3D:
            Loader = MemoryManager::createMemory<MeshLoaderB3D>("scene::MeshLoaderB3D"); break;
        #endif
        #ifdef SP_COMPILE_WITH_MESHLOADER_MD2
        case MESHFORMAT_MD2:
            Loader = MemoryManager::createMemory<MeshLoaderMD2>("scene::MeshLoaderMD2"); break;
        #endif
        #ifdef SP_COMPILE_WITH_MESHLOADER_MD3
        case MESHFORMAT_MD3:
            Loader = MemoryManager::createMemory<MeshLoaderMD3>("scene::MeshLoaderMD3"); break;
        #endif
        #ifdef SP_COMPILE_WITH_MESHLOADER_OBJ
        case MESHFORMAT_OBJ:
            Loader = MemoryManager::createMemory<MeshLoaderOBJ>("scene::MeshLoaderOBJ"); break;
        #endif
        
        default:
            /* Print an error message */
            io::Log::error("Mesh has unsupported file format");
            io::Log::lowerTab();
            
            /* Return an empty model */
            return createMesh();
    }
    
    /* Load the model with the determined model loader */
    Mesh* NewMesh = Loader->loadMesh(Filename, TexturePath);
    
    /* Add the mesh to the list */
    addSceneNode(NewMesh);
    
    /* Delete the temporary mesh loader */
    MemoryManager::deleteMemory(Loader);
    
    io::Log::lowerTab();
    
    NewMesh->getMaterial()->setWireframe(WireframeFront_, WireframeBack_);
    
    /* Return the new mesh */
    return NewMesh;
}

bool SceneGraph::saveMesh(Mesh* Model, io::stringc Filename, const EMeshFileFormats Format)
{
    /* Information message */
    io::Log::error("Save model: \"" + Filename + "\"");
    io::Log::upperTab();
    
    MeshSaver* Saver = 0;
    
    /* === Select format === */
    
    const EMeshFileFormats FileFormat = getMeshFileFormat(Filename, Format);
    
    switch (FileFormat)
    {
        #ifdef SP_COMPILE_WITH_MESHSAVER_SPM
        case MESHFORMAT_SPM:
            Saver = MemoryManager::createMemory<MeshSaverSPM>("scene::MeshSaverSPM"); break;
        #endif
        #ifdef SP_COMPILE_WITH_MESHSAVER_B3D
        case MESHFORMAT_B3D:
            Saver = MemoryManager::createMemory<MeshSaverB3D>("scene::MeshSaverB3D"); break;
        #endif
        #ifdef SP_COMPILE_WITH_MESHSAVER_OBJ
        case MESHFORMAT_OBJ:
            Saver = MemoryManager::createMemory<MeshSaverOBJ>("scene::MeshSaverOBJ"); break;
        #endif
        
        default:
            /* Print an error message */
            io::Log::error("Mesh has unsupported file format");
            io::Log::lowerTab();
            
            /* Exit with a failure */
            return false;
    }
    
    if (!Saver)
    {
        io::Log::lowerTab();
        return false;
    }
    
    /* Save the model with the determined model saver */
    bool Result = Saver->saveMesh(Model, Filename);
    
    /* Delete the temporary mesh saver */
    MemoryManager::deleteMemory(Saver);
    
    io::Log::lowerTab();
    
    return Result;
}

Mesh* SceneGraph::loadScene(io::stringc Filename, io::stringc TexturePath, const ESceneFileFormats Format, const s32 Flags)
{
    /* Information message */
    Filename.adjustPath();
    io::Log::message("Load scene: \"" + Filename + "\"");
    io::Log::upperTab();
    
    /* Temporary variables */
    Mesh* NewMesh = 0;
    
    if (TexturePath == video::TEXPATH_IGNORE)
        TexturePath = Filename.getPathPart();
    
    SceneLoader* Loader = 0;
    
    /* === Select format === */
    
    const ESceneFileFormats FileFormat = getSceneFileFormat(Filename, Format);
    
    switch (FileFormat)
    {
        #ifdef SP_COMPILE_WITH_SCENELOADER_SPSB
        case SCENEFORMAT_SPSB:
            Loader = MemoryManager::createMemory<SceneLoaderSPSB>("scene::SceneLoaderSPSB"); break;
        #endif
        #ifdef SP_COMPILE_WITH_SCENELOADER_BSP1
        case SCENEFORMAT_BSP1:
            Loader = MemoryManager::createMemory<SceneLoaderBSP1>("scene::SceneLoaderBSP1"); break;
        #endif
        #ifdef SP_COMPILE_WITH_SCENELOADER_BSP3
        case SCENEFORMAT_BSP3:
            Loader = MemoryManager::createMemory<SceneLoaderBSP3>("scene::SceneLoaderBSP3"); break;
        #endif
        
        default:
        {
            /* Print an error message */
            io::Log::error("Scene has unsupported file format");
            io::Log::lowerTab();
            
            /* Allocate a new & empty mesh */
            NewMesh = MemoryManager::createMemory<Mesh>("scene::Mesh (Empty)");
            
            /* Add the mesh to the list */
            addSceneNode(NewMesh);
            
            /* Return the new mesh */
            return NewMesh;
        }
    }
    
    /* Load the scene with the determined scene loader */
    NewMesh = Loader->loadScene(Filename, TexturePath, Flags);
    
    /* Add the mesh to the list */
    addSceneNode(NewMesh);
    
    /* Delete the temporary scene loader */
    MemoryManager::deleteMemory(Loader);
    
    io::Log::lowerTab();
    
    if (NewMesh)
        NewMesh->getMaterial()->setWireframe(WireframeFront_, WireframeBack_);
    
    /* Return the new mesh */
    return NewMesh;
}

void SceneGraph::createFurMesh(
    Mesh* Model, video::Texture* FurTexture, s32 LayerCount, f32 HairLength, s32 HairCloseness)
{
    if (!Model || !Model->getMeshBufferCount() || !FurTexture || LayerCount < 2)
        return;
    
    /* === Create the hair texture (for layers) === */
    
    /* Copy the fur texture */
    video::Texture* HairTexture = __spVideoDriver->copyTexture(FurTexture);
    
    /* Change the type for the alpha-channel */
    HairTexture->setFormat(video::PIXELFORMAT_RGBA);
    
    /* Loop for the image data */
    for (s32 y = 0, x; y < HairTexture->getSize().Height; ++y)
    {
        for (x = 0; x < HairTexture->getSize().Width; ++x)
        {
            if (math::Randomizer::randInt(HairCloseness) != 1)
            {
                /* Change the pixel transparency */
                video::color Color( HairTexture->getPixelColor(dim::point2di(x, y)) );
                Color.Alpha = 0;
                HairTexture->setPixelColor(dim::point2di(x, y), Color);
            }
        }
    }
    
    /* Update the image data */
    HairTexture->updateImageBuffer();
    
    /* === Create the hair layers === */
    
    /* Allocate a layer list */
    std::vector<Mesh*> Layer;
    Layer.resize(LayerCount);
    video::MeshBuffer* Surface = Model->getMeshBuffer(0), * LayerSurface = 0;
    
    /* Temporary variables */
    dim::vector3df Coord, Normal;
    
    /* Loop for all layers */
    for (s32 i = 0, j; i < LayerCount; ++i)
    {
        Layer[i]        = copyNode(Model);
        LayerSurface    = Layer[i]->getMeshBuffer(0);
        
        Layer[i]->setParent(Model);
        Layer[i]->setPosition(0);
        Layer[i]->setScale(1);
        
        LayerSurface->setTexture(0, HairTexture);
        
        Layer[i]->getMaterial()->setColorMaterial(false);
        Layer[i]->getMaterial()->getDiffuseColor().Alpha = 255 - 255 * (i+1) / LayerCount;
        
        for (j = 0; j < LayerSurface->getVertexCount(); ++j)
        {
            Coord   = Surface->getVertexCoord(j);
            Normal  = Surface->getVertexNormal(j);
            
            Coord += Normal * (HairLength * (f32)(i+1) / LayerCount);
            
            LayerSurface->setVertexCoord(j, Coord);
        }
        
        Layer[i]->updateMeshBuffer();
    }
}

/* === Lights, billboard, cameras, terrains === */

Light* SceneGraph::createLight(const ELightModels Type)
{
    Light* hLight = new Light(Type);
    addSceneNode(hLight);
    return hLight;
}
Billboard* SceneGraph::createBillboard(video::Texture* BaseTexture)
{
    Billboard* NewBillboard = new Billboard(BaseTexture);
    NewBillboard->getMaterial()->setWireframe(WireframeFront_);
    addSceneNode(NewBillboard);
    return NewBillboard;
}
Camera* SceneGraph::createCamera()
{
    return createCamera<Camera>();
}
Terrain* SceneGraph::createTerrain(
    const video::SHeightMapTexture &TextureHeightMap, const dim::size2di &Resolution, s32 GeoMIPLevels)
{
    Terrain* NewTerrain = new Terrain(TextureHeightMap, Resolution, GeoMIPLevels);
    NewTerrain->getMaterial()->setWireframe(WireframeFront_, WireframeBack_);
    addSceneNode(NewTerrain);
    return NewTerrain;
}

/*
 * Renders the whole scene for each camera
 */

void SceneGraph::renderScene()
{
    for (std::list<Camera*>::iterator itCam = CameraList_.begin(); itCam != CameraList_.end(); ++itCam)
    {
        if ((*itCam)->getVisible())
            renderScene(*itCam);
    }
}

void SceneGraph::renderScene(Camera* ActiveCamera)
{
    /* Configure view to the current active camera */
    setActiveCamera(ActiveCamera);
    
    if (ActiveCamera)
    {
        ActiveCamera->updateViewmode();
        ActiveCamera->updateModelviewMatrix();
    }
    
    spWorldMatrix.reset();
    
    /* Render the scene graph */
    render();
    
    /* Disable all 3d geometry rendering settings */
    __spVideoDriver->disableTriangleListStates();
    __spVideoDriver->disableTexturing();
    __spVideoDriver->setDefaultAlphaBlending();
    
    /* Special effects (stencil shadows) */
    if (isStencilEffects_)
        __spStencilManager->renderStencilShadows(ActiveCamera);
    
    /* Disable all 3d rendering states */
    __spVideoDriver->disable3DRenderStates();
}

/*
 * Renders the whole scene as a stereo image
 * You can use '3d glaces' because this scene will renderd two times in a red and a green color mask
 */

void SceneGraph::renderSceneStereoImage(Camera* ActiveCamera, f32 CamDegree, f32 CamDist)
{
    if (!ActiveCamera)
        return;
    
    /* Temporary variables and static members */
    
    static video::Texture* StereoImageA, * StereoImageB;
    
    const dim::size2di ScrSize(__Screen_Width, __Screen_Height);
    
    video::Texture* CurRenderTarget = __spVideoDriver->getRenderTarget();
    
    if (!StereoImageA)
    {
        StereoImageA = __spVideoDriver->createTexture(ScrSize);
        StereoImageA->setMipMapping(false);
        StereoImageA->setFilter(video::FILTER_LINEAR);
        StereoImageA->setRenderTarget(true);
        
        StereoImageB = __spVideoDriver->createTexture(ScrSize);
        StereoImageB->setMipMapping(false);
        StereoImageB->setFilter(video::FILTER_LINEAR);
        StereoImageB->setRenderTarget(true);
    }
    
    /* Red color scene */
    
    __spVideoDriver->setRenderTarget(StereoImageA);
    
    __spVideoDriver->setColorMask(true, true, true, true);
    __spVideoDriver->clearBuffers();
    __spVideoDriver->setColorMask(false, true, false, true);
    {
        ActiveCamera->move(dim::vector3df(CamDist, 0, 0));
        ActiveCamera->turn(dim::vector3df(0, CamDegree, 0));
    }
    renderScene(ActiveCamera);
    
    /* Green color scene */
    
    __spVideoDriver->setRenderTarget(StereoImageB);
    
    __spVideoDriver->setColorMask(true, true, true, true);
    __spVideoDriver->clearBuffers();
    __spVideoDriver->setColorMask(true, false, false, true);
    {
        ActiveCamera->turn(dim::vector3df(0, -CamDegree, 0));
        ActiveCamera->move(dim::vector3df(-CamDist*2, 0, 0));
        ActiveCamera->turn(dim::vector3df(0, -CamDegree, 0));
    }
    renderScene(ActiveCamera);
    
    __spVideoDriver->setRenderTarget(CurRenderTarget);
    
    __spVideoDriver->setColorMask(true, true, true, true);
    __spVideoDriver->clearBuffers();
    {
        ActiveCamera->turn(dim::vector3df(0, CamDegree, 0));
        ActiveCamera->move(dim::vector3df(CamDist, 0, 0));
    }
    
    /* Drawing */
    
    __spVideoDriver->beginDrawing2D();
    {
        const dim::rect2df Clipping(0, 0, 1, 1);
        
        __spVideoDriver->draw2DImage(
            StereoImageA, dim::rect2di(0, 0, ScrSize.Width, ScrSize.Height), Clipping
        );
        __spVideoDriver->draw2DImage(
            StereoImageB, dim::rect2di(0, 0, ScrSize.Width, ScrSize.Height), Clipping, video::color(255, 255, 255, 128)
        );
    }
    __spVideoDriver->endDrawing2D();
}

void SceneGraph::updateAnimations()
{
    foreach (Animation* Anim, AnimationList_)
        Anim->updateAnimation();
}

void SceneGraph::clearScene(
    bool isDeleteNodes, bool isDeleteMeshes, bool isDeleteCameras,
    bool isDeleteLights, bool isDeleteBillboards, bool isDeleteTerrains)
{
    if (isDeleteNodes)
        MemoryManager::deleteList(NodeList_);
    if (isDeleteCameras)
        MemoryManager::deleteList(CameraList_);
    if (isDeleteLights)
        MemoryManager::deleteList(LightList_);
    
    if (isDeleteMeshes && isDeleteBillboards && isDeleteTerrains)
        MemoryManager::deleteList(RenderList_);
    else
    {
        if (isDeleteMeshes)
            clearRenderObjectList<Mesh>(NODE_MESH, RenderList_);
        if (isDeleteBillboards)
            clearRenderObjectList<Billboard>(NODE_BILLBOARD, RenderList_);
        if (isDeleteTerrains)
            clearRenderObjectList<Terrain>(NODE_TERRAIN, RenderList_);
    }
}

SceneNode* SceneGraph::copyNode(const SceneNode* Object)
{
    SceneNode* NewObject = Object->copy();
    addSceneNode(NewObject);
    return NewObject;
}
Mesh* SceneGraph::copyNode(const Mesh* Object)
{
    Mesh* NewObject = Object->copy();
    addSceneNode(NewObject);
    return NewObject;
}
Light* SceneGraph::copyNode(const Light* Object)
{
    Light* NewObject = Object->copy();
    addSceneNode(NewObject);
    return NewObject;
}
Billboard* SceneGraph::copyNode(const Billboard* Object)
{
    Billboard* NewObject = Object->copy();
    addSceneNode(NewObject);
    return NewObject;
}
Camera* SceneGraph::copyNode(const Camera* Object)
{
    Camera* NewObject = Object->copy();
    addSceneNode(NewObject);
    return NewObject;
}
Terrain* SceneGraph::copyNode(const Terrain* Object)
{
    /*Terrain* NewObject = Object->copy();
    addSceneNode(NewObject);
    return NewObject;*/
    return 0;
}

bool SceneGraph::deleteNode(SceneNode* Object)
{
    if (!Object)
        return false;
    
    switch (Object->getType())
    {
        case NODE_BASICNODE:
            return removeObjectFromList<SceneNode, SceneNode>  (Object, NodeList_,      true);
        case NODE_CAMERA:
            return removeObjectFromList<Camera, Camera>        (Object, CameraList_,    true);
        case NODE_LIGHT:
            return removeObjectFromList<Light, Light>          (Object, LightList_,     true);
        case NODE_MESH:
            return removeObjectFromList<Mesh, RenderNode>      (Object, RenderList_,    true);
        case NODE_BILLBOARD:
            return removeObjectFromList<Billboard, RenderNode> (Object, RenderList_,    true);
        case NODE_TERRAIN:
            return removeObjectFromList<Terrain, RenderNode>   (Object, RenderList_,    true);
    }
    
    return false;
}

std::list<SceneNode*> SceneGraph::findNodes(const io::stringc &Name) const
{
    std::list<SceneNode*> NodeList;
    
    addNodeToList<SceneNode ,SceneNode> (Name, NodeList, NodeList_  );
    addNodeToList<Camera, Camera>       (Name, NodeList, CameraList_);
    addNodeToList<Light, Light>         (Name, NodeList, LightList_ );
    addNodeToList<Mesh, RenderNode>     (Name, NodeList, RenderList_);
    addNodeToList<Billboard, RenderNode>(Name, NodeList, RenderList_);
    addNodeToList<Terrain, RenderNode>  (Name, NodeList, RenderList_);
    
    return NodeList;
}

SceneNode* SceneGraph::findNode(const io::stringc &Name) const
{
    SceneNode* Object = 0;
    
    if (Object = findNodeInList<SceneNode>  (Name, NodeList_    )) return Object;
    if (Object = findNodeInList<Camera>     (Name, CameraList_  )) return Object;
    if (Object = findNodeInList<Light>      (Name, LightList_   )) return Object;
    if (Object = findNodeInList<RenderNode> (Name, RenderList_  )) return Object;
    if (Object = findNodeInList<RenderNode> (Name, RenderList_  )) return Object;
    if (Object = findNodeInList<RenderNode> (Name, RenderList_  )) return Object;
    
    return 0;
}

std::list<SceneNode*> SceneGraph::findChildren(const SceneNode* ParentNode) const
{
    std::list<SceneNode*> NodeList;
    
    addChildToList<SceneNode>   (ParentNode, NodeList, NodeList_    );
    addChildToList<Camera>      (ParentNode, NodeList, CameraList_  );
    addChildToList<Light>       (ParentNode, NodeList, LightList_   );
    addChildToList<RenderNode>  (ParentNode, NodeList, RenderList_  );
    
    return NodeList;
}

SceneNode* SceneGraph::findChild(const SceneNode* ParentNode, const io::stringc &Name) const
{
    SceneNode* Child = 0;
    
    if (Child = findChildInList<SceneNode>  (ParentNode, NodeList_,     Name)) return Child;
    if (Child = findChildInList<Camera>     (ParentNode, CameraList_,   Name)) return Child;
    if (Child = findChildInList<Light>      (ParentNode, LightList_,    Name)) return Child;
    if (Child = findChildInList<RenderNode> (ParentNode, RenderList_,   Name)) return Child;
    if (Child = findChildInList<RenderNode> (ParentNode, RenderList_,   Name)) return Child;
    if (Child = findChildInList<RenderNode> (ParentNode, RenderList_,   Name)) return Child;
    
    return 0;
}


/*
 * ========== Extra functions: ==========
 */

std::list<Mesh*> SceneGraph::getMeshList() const
{
    return filterRenderNodeList<Mesh>(scene::NODE_MESH);
}
std::list<Billboard*> SceneGraph::getBillboardList() const
{
    return filterRenderNodeList<Billboard>(scene::NODE_BILLBOARD);
}
std::list<Terrain*> SceneGraph::getTerrainList() const
{
    return filterRenderNodeList<Terrain>(scene::NODE_TERRAIN);
}

void SceneGraph::setWireframe(const video::EWireframeTypes Type)
{
    WireframeFront_ = WireframeBack_ = Type;
    
    foreach (RenderNode* Obj, RenderList_)
    {
        if (Obj->getType() == NODE_MESH || Obj->getType() == NODE_BILLBOARD || Obj->getType() == NODE_TERRAIN)
            static_cast<MaterialNode*>(Obj)->getMaterial()->setWireframe(Type);
    }
}

void SceneGraph::setWireframe(const video::EWireframeTypes TypeFront, const video::EWireframeTypes TypeBack)
{
    WireframeFront_ = TypeFront;
    WireframeBack_  = TypeBack;
    
    foreach (Mesh* Obj, getMeshList())
        Obj->getMaterial()->setWireframe(WireframeFront_, WireframeBack_);
}

void SceneGraph::setRenderFace(const video::EFaceTypes Face)
{
    foreach (Mesh* Obj, getMeshList())
        Obj->getMaterial()->setRenderFace(Face);
}

void SceneGraph::removeTexture(const video::Texture* Tex)
{
    if (!Tex)
        return;
    
    s32 i, j;
    video::MeshBuffer* Surface = 0;
    
    /* Search in each mesh */
    foreach (Mesh* Obj, getMeshList())
    {
        for (i = 0; i < Obj->getMeshBufferCount(); ++i)
        {
            Surface = Obj->getMeshBuffer(i);
            
            for (j = MAX_COUNT_OF_TEXTURES - 1; j >= 0; --j)
            {
                if (Surface->getTexture(j) == Tex)
                    Surface->removeTexture(j);
            }
        }
    }
    
    /* Search in each billboard */
    foreach (Billboard* Obj, getBillboardList())
    {
        if (Obj->getTexture() && Obj->getTexture() == Tex)
            Obj->setTexture(0);
    }
}

void SceneGraph::setLighting(bool isLighting)
{
    __isLighting = isLighting;
}
bool SceneGraph::getLighting() const
{
    return __isLighting;
}

void SceneGraph::setDefaultVertexFormat(video::VertexFormat* Format)
{
    if (Format)
        DefaultVertexFormat_ = Format;
    else
        DefaultVertexFormat_ = __spVideoDriver->getVertexFormatDefault();
}
video::VertexFormat* SceneGraph::getDefaultVertexFormat()
{
    return DefaultVertexFormat_;
}

void SceneGraph::setDefaultIndexFormat(const video::ERendererDataTypes Format)
{
    if ( ( Format == video::DATATYPE_UNSIGNED_BYTE && __spVideoDriver->getRendererType() == video::RENDERER_OPENGL ) ||
         Format == video::DATATYPE_UNSIGNED_SHORT || Format == video::DATATYPE_UNSIGNED_INT )
    {
        DefaultIndexFormat_ = Format;
    }
}
video::ERendererDataTypes SceneGraph::getDefaultIndexFormat()
{
    return DefaultIndexFormat_;
}

void SceneGraph::setTextureLoadingState(bool AllowTextureLoading)
{
    TextureLoadingState_ = AllowTextureLoading;
}
bool SceneGraph::getTextureLoadingState()
{
    return TextureLoadingState_;
}

u32 SceneGraph::getSceneVertexCount() const
{
    u32 VerticesCount = 0;
    
    foreach (Mesh* Obj, getMeshList())
        VerticesCount += Obj->getVertexCount();
    
    return VerticesCount;
}

u32 SceneGraph::getSceneTriangleCount() const
{
    u32 TriangleCount = 0;
    
    foreach (Mesh* Obj, getMeshList())
        TriangleCount += Obj->getTriangleCount();
    
    return TriangleCount;
}

u32 SceneGraph::getSceneObjectsCount() const
{
    return getNodeList().size();
}


/*
 * ======= Protected: =======
 */

void SceneGraph::sortRenderList(std::list<RenderNode*> &ObjectList, const dim::matrix4f &BaseMatrix)
{
    ActiveCamera_->updateModelviewMatrix();
    
    foreach (RenderNode* Obj, ObjectList)
    {
        if (Obj->getVisible())
        {
            spWorldMatrix = BaseMatrix;
            Obj->updateTransformation();
        }
    }
    
    ObjectList.sort(cmpObjectRenderNodes);
}

void SceneGraph::sortLightList(std::list<Light*> &ObjectList)
{
    const s32 MaxLightCount = __spVideoDriver->getMaxLightCount();
    
    if (ObjectList.size() <= MaxLightCount)
        return;
    
    /* Sort light list */
    ObjectList.sort(cmpObjectLights);
    
    /* Update renderer lights for the first [MaxLightCount] objects */
    u32 LightID = 0;
    video::color Diffuse, Ambient, Specular;
    
    foreach (Light* Obj, ObjectList)
    {
        Obj->LightID_ = LightID;
        
        Obj->getLightingColor(Diffuse, Ambient, Specular);
        __spVideoDriver->setLightColor(LightID, Diffuse, Ambient, Specular);
        
        if (++LightID >= MaxLightCount)
            break;
    }
}

EMeshFileFormats SceneGraph::getMeshFileFormat(
    const io::stringc &Filename, const EMeshFileFormats DefaultFormat) const
{
    const io::stringc FileExtension = Filename.getExtensionPart().upper();
    
    if ( ( DefaultFormat == MESHFORMAT_UNKNOWN && FileExtension == "SPM" ) || DefaultFormat == MESHFORMAT_SPM )
        return MESHFORMAT_SPM;
    else if ( ( DefaultFormat == MESHFORMAT_UNKNOWN && FileExtension == "3DS" ) || DefaultFormat == MESHFORMAT_3DS )
        return MESHFORMAT_3DS;
    else if ( ( DefaultFormat == MESHFORMAT_UNKNOWN && FileExtension == "MS3D" ) || DefaultFormat == MESHFORMAT_MS3D )
        return MESHFORMAT_MS3D;
    else if ( ( DefaultFormat == MESHFORMAT_UNKNOWN && FileExtension == "X" ) || DefaultFormat == MESHFORMAT_X )
        return MESHFORMAT_X;
    else if ( ( DefaultFormat == MESHFORMAT_UNKNOWN && FileExtension == "B3D" ) || DefaultFormat == MESHFORMAT_B3D )
        return MESHFORMAT_B3D;
    else if ( ( DefaultFormat == MESHFORMAT_UNKNOWN && FileExtension == "MD2" ) || DefaultFormat == MESHFORMAT_MD2 )
        return MESHFORMAT_MD2;
    else if ( ( DefaultFormat == MESHFORMAT_UNKNOWN && FileExtension == "MD3" ) || DefaultFormat == MESHFORMAT_MD3 )
        return MESHFORMAT_MD3;
    else if ( ( DefaultFormat == MESHFORMAT_UNKNOWN && FileExtension == "OBJ" ) || DefaultFormat == MESHFORMAT_OBJ )
        return MESHFORMAT_OBJ;
    
    return MESHFORMAT_UNKNOWN;
}

ESceneFileFormats SceneGraph::getSceneFileFormat(
    const io::stringc &Filename, const ESceneFileFormats DefaultFormat) const
{
    const io::stringc FileExtension = Filename.getExtensionPart().upper();
    
    if ( ( DefaultFormat == SCENEFORMAT_UNKNOWN && FileExtension == "SPSB" ) || DefaultFormat == SCENEFORMAT_SPSB )
        return SCENEFORMAT_SPSB;
    else if (DefaultFormat == SCENEFORMAT_UNKNOWN && FileExtension == "BSP")
    {
        // Get the magic number of the BSP file, because there are more than one types of this file format
        io::FileSystem TempLoader;
        io::File* TempFile = TempLoader.openFile(Filename);
        s32 MagicNumber = TempFile->readValue<s32>();
        TempLoader.closeFile(TempFile);
        
        // Check which BSP file version the file has
        if (MagicNumber == 0x17 || MagicNumber == 0x1E)
            return SCENEFORMAT_BSP1;
        else
            return SCENEFORMAT_BSP3;
    }
    else if (DefaultFormat == SCENEFORMAT_BSP1 || DefaultFormat == SCENEFORMAT_BSP3)
        return DefaultFormat;
    
    return SCENEFORMAT_UNKNOWN;
}


} // /namespace scene

} // /namespace sp



// ================================================================================
