/*
 * Mesh loader B3D file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "FileFormats/Mesh/spMeshLoaderB3D.hpp"

#ifdef SP_COMPILE_WITH_MESHLOADER_B3D


#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;
extern scene::SceneGraph* __spSceneManager;

namespace scene
{


MeshLoaderB3D::MeshLoaderB3D() : MeshLoader()
{
    init();
}
MeshLoaderB3D::~MeshLoaderB3D()
{
    clear();
}

Mesh* MeshLoaderB3D::loadMesh(const io::stringc &Filename, const io::stringc &TexturePath)
{
    if (!openLoadFile(Filename, TexturePath))
        return Mesh_;
    
    if (!loadModelData())
        io::Log::error("Loading B3D mesh failed");
    
    FileSys_->closeFile(File_);
    
    return Mesh_;
}


/*
 * ======= Private: =======
 */

void MeshLoaderB3D::init()
{
    /* General settings */
    CurPos_     = 0;
    
    Stack_.resize(1);
    Stack_[0] = 0;
    
    /* Initialize building members */
    isExpactNewKeyframes_   = false;
    CurBone_                = -1;
    AnimKeyframeCount_      = 0;
    AnimFPS_                = 1000.0f;
}
void MeshLoaderB3D::clear()
{
    MemoryManager::deleteList(AnimBoneList_);
}

io::stringc MeshLoaderB3D::readChunk()
{
    // Read chunk name
    c8 ChunkName[5] = { 0 };
    File_->readBuffer(ChunkName, 4);
    
    // Read chunk size
    s32 Size = File_->readValue<s32>();
    
    ++CurPos_;
    
    if (CurPos_ >= Stack_.size())
        Stack_.resize(CurPos_ + 1);
    
    Stack_[CurPos_] = File_->getSeek() + Size;
    
    // Returns chunk name
    return io::stringc(ChunkName).upper();
}

void MeshLoaderB3D::breakChunk()
{
    File_->setSeek(Stack_[CurPos_]);
    --CurPos_;
}

s32 MeshLoaderB3D::getChunkSize()
{
    return Stack_[CurPos_] - File_->getSeek();
}

void MeshLoaderB3D::readChunkBlock(io::stringc Tab)
{
    // Temporary variables
    io::stringc ChunkName;
    Tab += io::stringc(' ');
    
    // Loop while there are chunk data
    while (getChunkSize())
    {
        ChunkName = readChunk();
        
        if (ChunkName == "ANIM") // Animation (general information)
            readChunkANIM();
        else if (ChunkName == "KEYS") // Keyframes (for bone information)
            readChunkKEYS();
        else if (ChunkName == "TEXS") // Textures
            readChunkTEXS();
        else if (ChunkName == "BRUS") // Brushes
            readChunkBRUS();
        else if (ChunkName == "VRTS") // Vertices
            readChunkVRTS();
        else if (ChunkName == "TRIS") // Triangles
            readChunkTRIS();
        else if (ChunkName == "MESH") // Mesh (texture information)
            readChunkMESH();
        else if (ChunkName == "BONE") // Bone (for skeletal animation)
            readChunkBONE(Tab);
        else if (ChunkName == "NODE") // Node (new mesh or new bone)
            readChunkNODE();
        else
            break;
        
        readChunkBlock(Tab);
        breakChunk();
    }
}


/*
 * ======= Chunk functions =======
 */

bool MeshLoaderB3D::readChunkANIM()
{
    s32 Flags           = File_->readValue<s32>();
    AnimKeyframeCount_  = File_->readValue<s32>();
    AnimFPS_            = File_->readValue<f32>();
    
    if (AnimFPS_ == 0.0f)
        AnimFPS_ = 1000.0f / 60.0f;
    
    return true;
}

bool MeshLoaderB3D::readChunkKEYS()
{
    // Get current skeletal bone
    if (CurBone_ < 0 || CurBone_ >= AnimBoneList_.size())
    {
        io::Log::error("Animation bone array index (" + io::stringc(CurBone_) + ") out of bounds");
        breakChunk();
        return false;
    }
    
    SSkeletonBoneB3D* AnimBone = AnimBoneList_[CurBone_];
    
    // Temporary variables
    s32 i, LastFrame, Frame = 0;
    s32 Size = 4;
    f32 Interpolation;
    
    dim::vector3df KeyP, KeyS;
    dim::quaternion KeyR;
    
    // Read flags and get chunk size
    const s32 Flags = File_->readValue<s32>();
    
    if (Flags & 1) Size += 12;
    if (Flags & 2) Size += 12;
    if (Flags & 4) Size += 16;
    
    const s32 KeysCount = getChunkSize() / Size;
    
    // Allocate new keyframe memory
    if (isExpactNewKeyframes_)
    {
        // Allocate new keyframes
        AnimBone->KeyframeCount = math::Max(KeysCount, AnimKeyframeCount_);
        AnimBone->KeyframeList  = new SSkeletonBoneB3D::SBoneKeyframeInfo[AnimBone->KeyframeCount];
        
        isExpactNewKeyframes_ = false;
    }
    
    if (KeysCount*Size == getChunkSize())
    {
        while (getChunkSize())
        {
            LastFrame = Frame;
            
            Frame = File_->readValue<s32>() - 1;
            
            if (Frame < 0 || Frame >= AnimBone->KeyframeCount)
            {
                io::Log::error("Keyframe index out of bounds");
                breakChunk();
                break;
            }
            
            /* === Keyframe === */
            
            // Keyframe - position
            if (Flags & 1)
            {
                KeyP.X = File_->readValue<f32>();
                KeyP.Y = File_->readValue<f32>();
                KeyP.Z = File_->readValue<f32>();
                
                AnimBone->KeyframeList[Frame].isUndefined   = false;
                AnimBone->KeyframeList[Frame].Position      = KeyP;
                
                for (i = LastFrame + 1; i < Frame; ++i)
                {
                    Interpolation = static_cast<f32>( i - LastFrame ) / ( Frame - LastFrame );
                    
                    AnimBone->KeyframeList[i].isUndefined   = false;
                    AnimBone->KeyframeList[i].Position      =
                        AnimBone->KeyframeList[LastFrame].Position +
                        ( KeyP - AnimBone->KeyframeList[LastFrame].Position ) * Interpolation;
                }
            }
            
            // Keyframe - scale
            if (Flags & 2)
            {
                KeyS.X = File_->readValue<f32>();
                KeyS.Y = File_->readValue<f32>();
                KeyS.Z = File_->readValue<f32>();
                
                AnimBone->KeyframeList[Frame].isUndefined   = false;
                AnimBone->KeyframeList[Frame].Scale         = KeyS;
                
                for (i = LastFrame + 1; i < Frame; ++i)
                {
                    Interpolation = static_cast<f32>( i - LastFrame ) / ( Frame - LastFrame );
                    
                    AnimBone->KeyframeList[i].isUndefined   = false;
                    AnimBone->KeyframeList[i].Scale         =
                        AnimBone->KeyframeList[LastFrame].Scale +
                        ( KeyS - AnimBone->KeyframeList[LastFrame].Scale ) * Interpolation;
                }
            }
            
            // Keyframe - rotation
            if (Flags & 4)
            {
                KeyR.W = File_->readValue<f32>();
                KeyR.X = -File_->readValue<f32>();
                KeyR.Y = -File_->readValue<f32>();
                KeyR.Z = -File_->readValue<f32>();
                
                AnimBone->KeyframeList[Frame].isUndefined   = false;
                AnimBone->KeyframeList[Frame].Quaternion    = KeyR;
                
                for (i = LastFrame + 1; i < Frame; ++i)
                {
                    Interpolation = static_cast<f32>( i - LastFrame ) / ( Frame - LastFrame );
                    
                    AnimBone->KeyframeList[i].isUndefined   = false;
                    AnimBone->KeyframeList[i].Quaternion    =
                        AnimBone->KeyframeList[LastFrame].Quaternion +
                        ( KeyR - AnimBone->KeyframeList[LastFrame].Quaternion ) * Interpolation;
                }
            }
        } // /while
    }
    else
    {
        io::Log::warning("Illegal number of keyframes in B3D model");
        return false;
    }
    
    return true;
}

bool MeshLoaderB3D::readChunkTEXS()
{
    // Temporary variables
    io::stringc TexFilename;
    s32 Flags, Blend;
    f32 Rotation;
    dim::point2df Pos;
    dim::point2df Scale;
    
    while (getChunkSize())
    {
        // Reading
        TexFilename = File_->readStringC();
        Flags       = File_->readValue<s32>();
        Blend       = File_->readValue<s32>();
        Pos.X       = File_->readValue<f32>();
        Pos.Y       = File_->readValue<f32>();
        Scale.X     = File_->readValue<f32>();
        Scale.Y     = File_->readValue<f32>();
        Rotation    = File_->readValue<f32>();
        
        // Loading
        STextureSurfaceB3D TextureSurfaceData;
        {
            // General settings
            TextureSurfaceData.hTexture         = (SceneGraph::getTextureLoadingState() ? loadChunkTexture(TexFilename) : 0);
            TextureSurfaceData.Pos              = Pos;
            TextureSurfaceData.Scale            = Scale;
            TextureSurfaceData.isSphereMapping  = (Flags & 64) ? true : false;
            
            /*
            Texture flags:
            1: Color
            2: Alpha
            4: Masked
            8: Mipmapped
            16: Clamp U
            32: Clamp V
            64: Spherical reflection map
            */
            // Flags
            if (Flags & 2) // Alpha
                TextureSurfaceData.hTexture->setColorKeyAlpha();
            else if (Flags & 4) // Masked
                TextureSurfaceData.hTexture->setColorKey(video::emptycolor);
        }
        TextureList_.push_back(TextureSurfaceData);
    }
    
    return true;
}

bool MeshLoaderB3D::readChunkBRUS()
{
    // Temporary variables
    io::stringc Name;
    f32 Red, Green, Blue, Alpha, Shininess;
    s32 Blend, Effects, TexID;
    s32 TexCount = File_->readValue<s32>();
    
    while (getChunkSize())
    {
        Name        = File_->readStringC();
        Red         = File_->readValue<f32>();
        Green       = File_->readValue<f32>();
        Blue        = File_->readValue<f32>();
        Alpha       = File_->readValue<f32>();
        Shininess   = File_->readValue<f32>();
        Blend       = File_->readValue<s32>();
        Effects     = File_->readValue<s32>();
        
        for (s32 i = 0; i < TexCount; ++i)
        {
            TexID = File_->readValue<s32>();
            BrushTextureList_.push_back(TexID);
        }
    }
    
    return true;
}

bool MeshLoaderB3D::readChunkVRTS()
{
    s32 Flags = File_->readValue<s32>();
    s32 tcSets = File_->readValue<s32>();
    s32 tcSize = File_->readValue<s32>();
    
    // Temporary variables
    s32 Size = 12 + tcSets*tcSize*4;
    f32 TexCoord[3];
    
    SVertexB3D Vertex;
    
    if (Flags & 1) Size += 12;
    if (Flags & 2) Size += 16;
    
    s32 VerticesCount = getChunkSize() / Size;
    
    if (VerticesCount*Size == getChunkSize())
    {
        
        while (getChunkSize())
        {
            // Standard values
            Vertex.Normal   = dim::vector3df(0, 0, 1);
            Vertex.Color    = video::color(255);
            
            // Reading
            Vertex.Coord.X  = File_->readValue<f32>();
            Vertex.Coord.Y  = File_->readValue<f32>();
            Vertex.Coord.Z  = File_->readValue<f32>();
            
            if (Flags & 1)
            {
                Vertex.Normal.X = File_->readValue<f32>();
                Vertex.Normal.Y = File_->readValue<f32>();
                Vertex.Normal.Z = File_->readValue<f32>();
            }
            if (Flags & 2)
            {
                Vertex.Color.Red    = (s32)(File_->readValue<f32>() * 255);
                Vertex.Color.Green  = (s32)(File_->readValue<f32>() * 255);
                Vertex.Color.Blue   = (s32)(File_->readValue<f32>() * 255);
                Vertex.Color.Alpha  = (s32)(File_->readValue<f32>() * 255);
            }
            for (s32 j = 1; j <= tcSets*tcSize; ++j)
            {
                TexCoord[j-1] = File_->readValue<f32>();
            }
            
            Vertex.TexCoord.X   = TexCoord[0];
            Vertex.TexCoord.Y   = TexCoord[1];
            
            Vertex.SurfVertexID = VerticesList_.size();
            
            // Add new vertex
            VerticesList_.push_back(Vertex);
        } // wend
        
    }
    else
    {
        io::Log::warning("Illegal number of vertices in B3D model");
        return false;
    }
    
    return true;
}

bool MeshLoaderB3D::readChunkTRIS()
{
    // Temporary vairables
    s32 BrushID = File_->readValue<s32>();
    s32 Size = 12;
    s32 TriangleCount = getChunkSize() / Size;
    s32 v0, v1, v2;
    
    std::list<s32> VertexIndices, VertexIndicesUnique;
    
    if (BrushID == -1)
        BrushID = CurBrushID_;
    
    if (BrushID >= 0 && BrushID < BrushTextureList_.size())
        BrushID = BrushTextureList_[BrushID];
    
    if (TriangleCount*Size == getChunkSize())
    {
        
        while (getChunkSize())
        {
            // Reading
            v0 = File_->readValue<s32>();
            v1 = File_->readValue<s32>();
            v2 = File_->readValue<s32>();
            
            // Save in list
            VertexIndices.push_back(v0);
            VertexIndices.push_back(v1);
            VertexIndices.push_back(v2);
        }
        
        // Copy the list
        VertexIndicesUnique = VertexIndices;
        
        // Remove multiple elements
        VertexIndicesUnique.sort();
        VertexIndicesUnique.unique();
        
        // Create a new surface
        Surface_ = Mesh_->createMeshBuffer(SceneGraph::getDefaultVertexFormat(), SceneGraph::getDefaultIndexFormat());
        Surface_->setName(CurName_);
        
        SBrushSurfaceB3D BrushSurfaceData;
        {
            BrushSurfaceData.BrushID    = BrushID;
            BrushSurfaceData.Surface    = Surface_;
        }
        BrushSurfaceList_.push_back(BrushSurfaceData);
        
        SVertexB3D Vertex;
        
        // Create all vertices
        for (std::list<s32>::iterator it = VertexIndicesUnique.begin(); it != VertexIndicesUnique.end(); ++it)
        {
            Vertex = VerticesList_[*it];
            
            VerticesList_[*it].SurfVertexID = Surface_->addVertex(
                CurTransformation_ * Vertex.Coord,
                CurRotation_ * Vertex.Normal,
                Vertex.TexCoord,
                Vertex.Color
            );
            
            VerticesList_[*it].Surface      = Surface_;
            VerticesList_[*it].SurfaceNr    = Mesh_->getMeshBufferCount() - 1;
        }
        
        // Create all triangles
        for (std::list<s32>::iterator it = VertexIndices.begin(); it != VertexIndices.end();)
        {
            // Get the indices
            v0 = VerticesList_[*it++].SurfVertexID;
            v1 = VerticesList_[*it++].SurfVertexID;
            v2 = VerticesList_[*it++].SurfVertexID;
            
            // Creating
            Surface_->addTriangle(v0, v1, v2);
        }
        
    }
    else
    {
        io::Log::warning("Illegal number of triangles in B3D model");
        return false;
    }
    
    return true;
}

bool MeshLoaderB3D::readChunkMESH()
{
    // Read the current brush
    CurBrushID_ = File_->readValue<s32>();
    
    // Clear the vertices list of the last mesh
    VerticesList_.clear();
    
    return true;
}

bool MeshLoaderB3D::readChunkBONE(io::stringc &Tab)
{
    s32 Size = 8;
    s32 WeightsCount = getChunkSize() / Size;
    s32 VertexID;
    f32 Weight;
    
    isExpactNewKeyframes_ = true;
    
    if (WeightsCount*Size == getChunkSize())
    {
        
        s32 i = 0;
        
        // Temporary bone memory
        SSkeletonBoneB3D* BoneData = new SSkeletonBoneB3D();
        {
            BoneData->TabSize       = Tab.size();
            BoneData->ParentBoneID  = 0;
            BoneData->Name          = CurName_;
            
            BoneData->Translation   = Position_;
            BoneData->Quaternion    = Quaternion_;
            BoneData->Scale         = Scale_;
            
            BoneData->WeightsCount  = WeightsCount;
            BoneData->VerticesList  = new SSkeletonBoneB3D::SBoneVertexInfo[ WeightsCount ];
        }
        
        // Read the vertex identities
        while (getChunkSize())
        {
            VertexID    = File_->readValue<s32>();
            Weight      = File_->readValue<f32>();
            
            BoneData->VerticesList[i].Surface   = VerticesList_[VertexID].SurfaceNr;
            BoneData->VerticesList[i].Index     = VerticesList_[VertexID].SurfVertexID;
            BoneData->VerticesList[i].Weight    = Weight;
            
            ++i;
        }
        
        // Add a new bone
        AnimBoneList_.push_back(BoneData);
        
        CurBone_ = AnimBoneList_.size() - 1;
        
    }
    else
    {
        io::Log::warning("Illegal number of bone weights in B3D model");
        return false;
    }
    
    return true;
}

bool MeshLoaderB3D::readChunkNODE()
{
    // Read name
    CurName_        = File_->readStringC();
    
    // Read position
    Position_.X      = File_->readValue<f32>();
    Position_.Y      = File_->readValue<f32>();
    Position_.Z      = File_->readValue<f32>();
    
    // Read scaling
    Scale_.X         = File_->readValue<f32>();
    Scale_.Y         = File_->readValue<f32>();
    Scale_.Z         = File_->readValue<f32>();
    
    // Read rotation
    Quaternion_.W    = File_->readValue<f32>();
    Quaternion_.X    = File_->readValue<f32>();
    Quaternion_.Y    = File_->readValue<f32>();
    Quaternion_.Z    = File_->readValue<f32>();
    
    // Update current transformation
    CurRotation_ = Quaternion_.getMatrixTransposed();
    
    CurTransformation_.reset();
    CurTransformation_.translate(Position_);
    CurTransformation_ *= CurRotation_;
    CurTransformation_.scale(Scale_);
    
    return true;
}

video::Texture* MeshLoaderB3D::loadChunkTexture(io::stringc Filename)
{
    if (!io::FileSystem().findFile(TexturePath_ + Filename))
    {
        const io::stringc AlternativeFilename(
            Filename.right(Filename.size() - Filename.getPathPart().size())
        );
        
        if (AlternativeFilename != Filename)
        {
            io::Log::warning(
                io::stringc("Could not found texture file: \"") + TexturePath_ + Filename +
                io::stringc("\", trying alternative: \"") + TexturePath_ + AlternativeFilename + "\""
            );
            Filename = AlternativeFilename;
        }
    }
    
    return __spVideoDriver->loadTexture(TexturePath_ + Filename);
}


/*
 * ======= Building functions =======
 */

void MeshLoaderB3D::updateTexturing()
{
    for (std::vector<SBrushSurfaceB3D>::iterator it = BrushSurfaceList_.begin(); it != BrushSurfaceList_.end(); ++it)
    {
        if (it->Surface && it->BrushID < TextureList_.size() && it->BrushID >= 0)
        {
            if (TextureList_[it->BrushID].hTexture)
                it->Surface->addTexture(TextureList_[it->BrushID].hTexture);
            
            it->Surface->textureTransform(0, TextureList_[it->BrushID].Scale);
            it->Surface->textureTranslate(0, TextureList_[it->BrushID].Pos);
            
            if (TextureList_[it->BrushID].isSphereMapping)
                it->Surface->setMappingGen(0, video::MAPGEN_SPHERE_MAP);
        }
    }
}

void MeshLoaderB3D::buildAnimation()
{
    /* === Check if the object has a surface and a frame === */
    
    if (!Mesh_->getMeshBufferCount() || AnimBoneList_.empty())
        return;
    
    /* === Create a new animation === */
    
    Anim_ = Mesh_->addAnimation(ANIMATION_SKELETAL);
    
    /* === Temporary variables === */
    
    std::vector<SVertexWeight> VertexWeights;
    s32 i = 0, j;
    
    AnimationSkeletal* AnimSkeletal = (AnimationSkeletal*)Anim_;
    AnimationBone* AnimBone = 0;
    
    /* === Loop for the frame bone list (get the correct parent bones) === */
    
    for (i = 1; i < AnimBoneList_.size(); ++i)
    {
        for (j = i - 1; j >= 0; --j)
        {
            if (AnimBoneList_[i]->TabSize == AnimBoneList_[j]->TabSize + 1)
            {
                AnimBoneList_[i]->ParentBoneID = j + 1;
                break;
            }
        }
    }
    
    /* === Loop for each frame bone (creating the animation) === */
    
    for (std::vector<SSkeletonBoneB3D*>::iterator it = AnimBoneList_.begin(); it != AnimBoneList_.end(); ++it)
    {
        
        // Add a skeleton bone
        AnimBone = AnimSkeletal->addBone(
            (*it)->ParentBoneID ? AnimSkeletal->getBone((*it)->ParentBoneID) : 0,   // Parent bone
            (*it)->Translation,                                                     // Translation (location)
            (*it)->Quaternion,                                                      // Quaternion
            (*it)->Scale,                                                           // Scale
            (*it)->Name                                                             // Bone name
        );
        
        // Get the vertices list
        VertexWeights.resize((*it)->WeightsCount);
        
        if ((*it)->VerticesList)
        {
            for (i = 0; i < (*it)->WeightsCount; ++i)
            {
                VertexWeights[i].Surface    = Mesh_->getMeshBuffer((*it)->VerticesList[i].Surface);
                VertexWeights[i].Index      = (*it)->VerticesList[i].Index;
                VertexWeights[i].Weight     = (*it)->VerticesList[i].Weight;
            }
            
            // Add the vertices indices
            AnimBone->setupVertexWeights(VertexWeights);
        }
        else
        {
            io::Log::error(
                "Vertices list of bone " + io::stringc(AnimBone->getID()) + " is invalid"
            );
        }
        
        // Create the animation keyframes
        if ((*it)->KeyframeList)
        {
            SSkeletonBoneB3D::SBoneKeyframeInfo* LastDefinedKeyframe = 0;
            SSkeletonBoneB3D::SBoneKeyframeInfo* CurKeyframe = 0;
            
            for (i = 0; i < AnimKeyframeCount_; ++i)
            {
                CurKeyframe = &(*it)->KeyframeList[i];
                
                if (!CurKeyframe->isUndefined)
                    LastDefinedKeyframe = CurKeyframe;
                else if (CurKeyframe->isUndefined && LastDefinedKeyframe)
                    CurKeyframe = LastDefinedKeyframe;
                
                AnimBone->addKeyframe(
                    CurKeyframe->Position,      // Position keyframe
                    CurKeyframe->Quaternion,    // Quaternion keyframe
                    CurKeyframe->Scale,         // Scale keyframe
                    1000.0f / AnimFPS_          // Time (in ms.)
                );
            }
        }
        else
        {
            io::Log::error(
                "Keyframe list of bone " + io::stringc(AnimBone->getID()) + " is invalid"
            );
        }
    }
    
    /* Update the skeleton bones */
    AnimSkeletal->updateSkeleton();
}

bool MeshLoaderB3D::loadModelData()
{
    // Identity
    if (readChunk() != "BB3D")
    {
        io::Log::error("B3D model has invalid identity");
        return false;
    }
    
    // Version
    if (File_->readValue<s32>() / 100 > 0)
    {
        io::Log::error("B3D model has invalid version");
        return false;
    }
    
    // Read all chunks
    readChunkBlock();
    
    // Update texturing
    updateTexturing();
    
    // Update modeling
    Mesh_->updateIndexBuffer();
    Mesh_->updateNormals();
    
    // Create animation
    buildAnimation();
    
    return true;
}


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
