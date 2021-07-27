/*
 * Mesh loader SPM header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_MESHLOADER_SPM_H__
#define __SP_MESHLOADER_SPM_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_MESHLOADER_SPM


#include "Base/spInputOutputLog.hpp"
#include "Base/spDimension.hpp"
#include "RenderSystem/spTextureBase.hpp"
#include "FileFormats/Mesh/spMeshLoader.hpp"

#include <vector>
#include <string>


namespace sp
{
namespace scene
{


static const s32 SPM_MAGIC_NUMBER   = *((s32*)"SPMD");  // SoftPixelMoDel
static const u16 SPM_VERSION_MIN_NR = 0x2000;
static const u16 SPM_VERSION_NUMBER = 0x2100;           // v.2.1

enum EModelSPMChunkFlags
{
    MDLSPM_CHUNK_NONE               = 0x0000,
    
    // Objects flags
    MDLSPM_CHUNK_INDEX32BIT         = 0x0001,
    MDLSPM_CHUNK_VERTEXCOLOR        = 0x0002,
    MDLSPM_CHUNK_VERTEXFOG          = 0x0004,
    MDLSPM_CHUNK_VERTEXNORMAL       = 0x0008,
    
    // Mesh flags
    MDLSPM_CHUNK_GOURAUDSHADING     = 0x0008,
    MDLSPM_CHUNK_NODE_ANIM          = 0x0010,
    MDLSPM_CHUNK_MORPHTARGET_ANIM   = 0x0020,
    MDLSPM_CHUNK_SKELETAL_ANIM      = 0x0040,
    
    // Surface flags
    MDLSPM_CHUNK_TEXTUREINTERN      = 0x0010,
    MDLSPM_CHUNK_TEXTUREMATRIX      = 0x0020,
};


class SP_EXPORT MeshLoaderSPM : public MeshLoader
{
    
    public:
        
        MeshLoaderSPM();
        ~MeshLoaderSPM();
        
        Mesh* loadMesh(const io::stringc &Filename, const io::stringc &TexturePath);
        
    private:
        
        /* Functions */
        
        bool readHeader();
        
        void readChunkObject();
        void readChunkSubMesh(Mesh* SubMesh);
        void readChunkSurface();
        void readChunkVertex();
        void readChunkTriangle();
        void readChunkTexture();
        
        void readChunkAnimationNode();
        void readChunkAnimationMorphTarget();
        void readChunkAnimationSkeletal();
        void readChunkAnimationBone();
        
        /* Members */
        
        Mesh* CurMesh_;
        
        AnimationNode* AnimNode_;
        AnimationMorphTarget* AnimMorphTarget_;
        AnimationSkeletal* AnimSkeletal_;
        
        bool has32BitIndices_;
        bool hasVertexColors_;
        bool hasVertexFogCoords_;
        bool hasVertexNormals_;
        
        u8 TexCoordsDimensions_[MAX_COUNT_OF_TEXTURES];
        video::color DefaultVertexColor_;
        f32 DefaultVertexFogCoord_;
        
        u8 TexLayerCount_;
        
};


} // /namespace scene

} // /namespace sp


#endif

#endif



// ================================================================================
