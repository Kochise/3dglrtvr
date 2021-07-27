/*
 * Mesh loader MD3 file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "FileFormats/Mesh/spMeshLoaderMD3.hpp"

#ifdef SP_COMPILE_WITH_MESHLOADER_MD3


#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{
namespace scene
{


/*
 * Macros
 */

static const s32 MD3_IDENTITY   = (('3'<<24) + ('P'<<16) + ('D'<<8) + 'I'); // magic number must be "IDP2" or 844121161
static const s32 MD3_VERSION    = 15;


/*
 * MeshLoaderMD3 class
 */

MeshLoaderMD3::MeshLoaderMD3() : MeshLoader()
{
    init();
}
MeshLoaderMD3::~MeshLoaderMD3()
{
    clear();
}

Mesh* MeshLoaderMD3::loadMesh(const io::stringc &Filename, const io::stringc &TexturePath)
{
    if (!openLoadFile(Filename, TexturePath))
        return Mesh_;
    
    if (!loadModelData())
    {
        io::Log::error("Loading MD3 mesh failed");
        FileSys_->closeFile(File_);
        return Mesh_;
    }
    
    FileSys_->closeFile(File_);
    
    return buildModel();
}


/*
 * ======= Private: =======
 */

void MeshLoaderMD3::init()
{
    
}
void MeshLoaderMD3::clear()
{
    
}

bool MeshLoaderMD3::loadModelData()
{
    return loadHeader() && loadSurfaces() && loadKeyframes();
}

bool MeshLoaderMD3::loadHeader()
{
    /* Temporary variables */
    memset(Header_.PathName, 0, sizeof(Header_.PathName));
    
    File_->readBuffer(&Header_, sizeof(SHeaderMD3));
    
    if (Header_.ID != MD3_IDENTITY)
    {
        io::Log::error("MD3 model has wrong identity (must be \"IDP3\")");
        return false;
    }
    
    if (Header_.Version != MD3_VERSION)
    {
        io::Log::error("MD3 model has unsupported version (" + io::stringc(Header_.Version) + ")");
        return false;
    }
    
    return true;
}

bool MeshLoaderMD3::loadSurfaces()
{
    File_->setSeek(Header_.SurfaceOffset);
    
    SSurfaceMD3 Surface;
    SVertexMD3 Vertex;
    STexCoordMD3 TexCoord;
    STriangleMD3 Triangle;
    
    std::vector<dim::vector3df> Vertices;
    
    for (s32 s = 0; s < Header_.CountOfSurfaces; ++s)
    {
        /* Create new mesh buffer */
        video::MeshBuffer* Surf = Mesh_->createMeshBuffer();
        
        /* Read surface */
        s32 BaseOffset = File_->getSeek();
        
        memset(Surface.Name, 0, sizeof(Surface.Name));
        
        File_->readBuffer(&Surface, sizeof(SSurfaceMD3));
        
        /* Read vertices */
        File_->setSeek(BaseOffset + Surface.VertexOffset);
        
        Vertices.resize(Surface.CountOfVertices);
        
        for (s32 f = 0; f < Header_.CountOfKeyframes; ++f)
        {
            for (s32 i = 0; i < Surface.CountOfVertices; ++i)
            {
                File_->readBuffer(&Vertex, sizeof(SVertexMD3));
                
                /* Get vertex coordinate */
                Vertices[i] = dim::vector3df(
                    static_cast<f32>(Vertex.Coord[1]),
                    static_cast<f32>(Vertex.Coord[2]),
                    static_cast<f32>(Vertex.Coord[0])
                ) * (1.0f / 64.0f);
                
                if (!f)
                {
                    /* Compute vertex normal */
                    const f32 Latitude  = static_cast<f32>(Vertex.Normal[0]) * 2.0f * M_PI / 255;
                    const f32 Longitude = static_cast<f32>(Vertex.Normal[1]) * 2.0f * M_PI / 255;
                    
                    dim::vector3df Normal(
                        cos(Latitude) * sin(Longitude),
                        sin(Latitude) * sin(Longitude),
                        cos(Longitude)
                    );
                    
                    /* Add new vertex */
                    Surf->addVertex(Vertices[i], Normal, 0.0f, video::color(255));
                }
            }
            
            KeyframeList_.push_back(Vertices);
        }
        
        /* Read texture coordinates */
        File_->setSeek(BaseOffset + Surface.TexCoordOffset);
        
        for (s32 i = 0; i < Surface.CountOfVertices; ++i)
        {
            File_->readBuffer(&TexCoord, sizeof(STexCoordMD3));
            
            Surf->setVertexTexCoord(i, *(dim::point2df*)TexCoord.TexCoord);
        }
        
        /* Read triangles */
        File_->setSeek(BaseOffset + Surface.TriangleOffset);
        
        for (s32 i = 0; i < Surface.CountOfTriangles; ++i)
        {
            File_->readBuffer(&Triangle, sizeof(STriangleMD3));
            
            Surf->addTriangle(
                Triangle.Indices[2], Triangle.Indices[1], Triangle.Indices[0]
            );
        }
        
        File_->setSeek(BaseOffset + Surface.EndOffset);
    }
    
    return true;
}

bool MeshLoaderMD3::loadKeyframes()
{
    File_->setSeek(Header_.KeyframeOffset);
    
    SKeyFrameMD3 Keyframe;
    
    #if 1
    io::Log::message("Keyframes: " + io::stringc(Header_.CountOfKeyframes));
    #endif
    
    for (s32 i = 0; i < Header_.CountOfKeyframes; ++i)
    {
        /* Read keyframe */
        memset(Keyframe.Name, 0, sizeof(Keyframe.Name));
        
        File_->readBuffer(&Keyframe, sizeof(SKeyFrameMD3));
        
        #if 1
        io::Log::message("\tKeyframe #" + io::stringc(i) + " \"" + io::stringc(Keyframe.Name) + "\"");
        #endif
        
        
        
        
    }
    
    return true;
}

Mesh* MeshLoaderMD3::buildModel()
{
    Mesh_->updateMeshBuffer();
    Mesh_->updateNormals();
    
    buildAnimation();
    
    return Mesh_;
}

void MeshLoaderMD3::buildAnimation()
{
    Anim_ = Mesh_->addAnimation(ANIMATION_MORPHTARGET);
    
    AnimationMorphTarget* AnimMorphTarget = static_cast<AnimationMorphTarget*>(Anim_);
    
    for (std::list< std::vector<dim::vector3df> >::iterator it = KeyframeList_.begin(); it != KeyframeList_.end(); ++it)
        AnimMorphTarget->addSequence(*it);
    
    AnimMorphTarget->setVertexWeightsDefault();
}


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
