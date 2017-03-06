/*
 * Mesh loader SPM file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "FileFormats/Mesh/spMeshLoaderSPM.hpp"

#ifdef SP_COMPILE_WITH_MESHLOADER_SPM


#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;
extern scene::SceneGraph* __spSceneManager;

namespace scene
{


MeshLoaderSPM::MeshLoaderSPM()
    : MeshLoader(), AnimNode_(0), AnimMorphTarget_(0), AnimSkeletal_(0)
{
    has32BitIndices_    = false;
    hasVertexColors_    = false;
    hasVertexFogCoords_ = false;
    hasVertexNormals_   = false;
}
MeshLoaderSPM::~MeshLoaderSPM()
{
}

Mesh* MeshLoaderSPM::loadMesh(const io::stringc &Filename, const io::stringc &TexturePath)
{
    if (!openLoadFile(Filename, TexturePath))
        return Mesh_;
    
    if (!readHeader())
    {
        io::Log::error("Loading SPM mesh failed");
        return Mesh_;
    }
    
    readChunkObject();
    
    FileSys_->closeFile(File_);
    
    return Mesh_;
}


/*
 * ======= Private: =======
 */

bool MeshLoaderSPM::readHeader()
{
    // Read header information: magic number ("SPMD"), format version
    const s32 MagicNumber = File_->readValue<s32>();
    
    if (MagicNumber != SPM_MAGIC_NUMBER)
    {
        io::Log::error("SPM file has invalid magic number");
        return false;
    }
    
    const s16 Version = File_->readValue<u16>();
    
    if (Version < SPM_VERSION_MIN_NR)
    {
        io::Log::error("SPM file version is too low so it may load incorrect");
        return false;
    }
    else if (Version > SPM_VERSION_NUMBER)
    {
        io::Log::error("SPM file version is too high, unknown format");
        return false;
    }
    
    return true;
}

void MeshLoaderSPM::readChunkObject()
{
    // Read main mesh and each sub mesh
    u32 SubMeshCount = File_->readValue<u32>();
    
    readChunkSubMesh(Mesh_);
    
    for (u32 i = 1; i < SubMeshCount; ++i)
    {
        readChunkSubMesh(0);
        Mesh_->addLODSubMesh(CurMesh_);
    }
}

void MeshLoaderSPM::readChunkSubMesh(Mesh* SubMesh)
{
    if (SubMesh)
        CurMesh_ = SubMesh;
    else
        CurMesh_ = __spSceneManager->createMesh();
    
    // Read object information: name, flags
    CurMesh_->setName(File_->readStringData());
    
    // Get the flags
    s16 MeshFlags = File_->readValue<s16>();
    
    if (MeshFlags & MDLSPM_CHUNK_GOURAUDSHADING)
        CurMesh_->setShading(video::SHADING_GOURAUD);
    else
        CurMesh_->setShading(video::SHADING_FLAT);
    
    // Reserved data (leap data, just for editoring)
    u32 ReservedDataSize = File_->readValue<u32>();
    File_->setSeek(ReservedDataSize, io::FILEPOS_CURRENT);
    
    // Read each surface
    u32 SurfaceCount = File_->readValue<u32>();
    
    for (u32 s = 0; s < SurfaceCount; ++s)
        readChunkSurface();
    
    // Read animaions
    if (MeshFlags & MDLSPM_CHUNK_NODE_ANIM)
        readChunkAnimationNode();
    if (MeshFlags & MDLSPM_CHUNK_MORPHTARGET_ANIM)
        readChunkAnimationMorphTarget();
    if (MeshFlags & MDLSPM_CHUNK_SKELETAL_ANIM)
        readChunkAnimationSkeletal();
    
    // Update mesh buffers
    CurMesh_->updateMeshBuffer();
    
    if (!hasVertexNormals_)
        CurMesh_->updateNormals();
}

void MeshLoaderSPM::readChunkSurface()
{
    Surface_ = CurMesh_->createMeshBuffer(SceneGraph::getDefaultVertexFormat(), SceneGraph::getDefaultIndexFormat());
    
    // Read surface information: name, flags
    Surface_->setName(File_->readStringData());
    
    // Set the flags
    s16 SurfaceFlags = File_->readValue<s16>();
    
    has32BitIndices_    = (SurfaceFlags & MDLSPM_CHUNK_INDEX32BIT);
    hasVertexColors_    = (SurfaceFlags & MDLSPM_CHUNK_VERTEXCOLOR);
    hasVertexFogCoords_ = (SurfaceFlags & MDLSPM_CHUNK_VERTEXFOG);
    hasVertexNormals_   = (SurfaceFlags & MDLSPM_CHUNK_VERTEXNORMAL);
    
    // Read texture coordinates dimensions
    for (u8 i = 0; i < MAX_COUNT_OF_TEXTURES; ++i)
        TexCoordsDimensions_[i] = File_->readValue<u8>();
    
    // Write each texture
    TexLayerCount_ = File_->readValue<u8>();
    
    for (u8 i = 0; i < TexLayerCount_; ++i)
        readChunkTexture();
    
    // Write each vertex
    const u32 VertexCount = File_->readValue<u32>();
    
    if (VertexCount)
    {
        // Write vertex default values: color, fog coord
        if (!hasVertexColors_)
            DefaultVertexColor_ = File_->readColor();
        if (!hasVertexFogCoords_)
            DefaultVertexFogCoord_ = File_->readValue<f32>();
        
        // Write the vertex data
        for (u32 i = 0; i < VertexCount; ++i)
            readChunkVertex();
    }
    
    // Write each triangle
    const u32 TriangleCount = File_->readValue<u32>();
    
    if (TriangleCount)
    {
        // Write the triangle data
        for (u32 i = 0; i < TriangleCount; ++i)
            readChunkTriangle();
    }
}

void MeshLoaderSPM::readChunkVertex()
{
    // Temporary variables;
    dim::vector3df Position, Normal;
    dim::vector3df TexCoords[MAX_COUNT_OF_TEXTURES];
    video::color Color(DefaultVertexColor_);
    f32 FogCoord(DefaultVertexFogCoord_);
    
    // Read position
    Position = File_->readVector<f32>();
    
    // Read texture coordinates
    dim::vector3df TexCoord;
    
    for (u8 i = 0, j; i < MAX_COUNT_OF_TEXTURES; ++i)
    {
        if (TexCoordsDimensions_[i] > 0)
        {
            for (j = 0; j < TexCoordsDimensions_[i]; ++j)
                (TexCoords[i])[j] = File_->readValue<f32>();
        }
    }
    
    // Read color
    if (hasVertexColors_)
        Color = File_->readColor();
    
    // Read fog coordinate
    if (hasVertexFogCoords_)
        FogCoord = File_->readValue<f32>();
    
    // Read normal
    if (hasVertexNormals_)
        Normal = File_->readVector<f32>();
    
    // Create the vertex
    const u32 Vertex = Surface_->addVertex(
        Position, Normal, TexCoords[0], Color, FogCoord
    );
    
    for (u8 i = 0; i < MAX_COUNT_OF_TEXTURES; ++i)
    {
        if (TexCoordsDimensions_[i] > 0)
            Surface_->setVertexTexCoord(Vertex, TexCoords[i], i);
    }
}

void MeshLoaderSPM::readChunkTriangle()
{
    // Read the indices (32 bits or 16 bits)
    u32 v0, v1, v2;
    
    if (has32BitIndices_)
    {
        v0 = File_->readValue<u32>();
        v1 = File_->readValue<u32>();
        v2 = File_->readValue<u32>();
    }
    else
    {
        v0 = (u32)File_->readValue<u16>();
        v1 = (u32)File_->readValue<u16>();
        v2 = (u32)File_->readValue<u16>();
    }
    
    // Create the triangle
    Surface_->addTriangle(v0, v1, v2);
}

void MeshLoaderSPM::readChunkTexture()
{
    // Read texture basic information
    bool isTexValid = (bool)File_->readValue<s8>();
    
    if (isTexValid)
    {
        const u8 Layer = CurMesh_->getTextureCount();
        
        // Read the texture resource information
        const io::stringc Filename(File_->readStringData());
        
        if (SceneGraph::getTextureLoadingState())
        {
            video::Texture* Tex = 0;
            
            if (FileSys_->findFile(TexturePath_ + Filename))
                Tex = __spVideoDriver->loadTexture(TexturePath_ + Filename);
            else
                Tex = __spVideoDriver->loadTexture(Filename);
            
            Surface_->addTexture(Tex);
        }
        
        // Get texture flags: environment mapping, matrix etc.
        u16 TextureFlags = File_->readValue<u16>();
        
        // Read the flags data
        if (TextureFlags & MDLSPM_CHUNK_TEXTUREMATRIX)
            Surface_->setTextureMatrix(Layer, File_->readMatrix<f32>());
        
        Surface_->setTextureEnv(Layer, File_->readValue<video::ETextureEnvTypes>());
        Surface_->setMappingGen(Layer, File_->readValue<video::EMappingGenTypes>());
        Surface_->setMappingGenCoords(Layer, File_->readValue<s32>());
    }
}

void MeshLoaderSPM::readChunkAnimationNode()
{
    // !TODO!
}

void MeshLoaderSPM::readChunkAnimationMorphTarget()
{
    // !TODO!
}

void MeshLoaderSPM::readChunkAnimationSkeletal()
{
    // !TODO!
}

void MeshLoaderSPM::readChunkAnimationBone()
{
    // !TODO!
}


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
