/*
 * Mesh scene node header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_SCENE_MESH_H__
#define __SP_SCENE_MESH_H__


#include "Base/spStandard.hpp"
#include "Base/spInputOutputString.hpp"
#include "Base/spMaterialStates.hpp"
#include "Base/spMeshBuffer.hpp"
#include "Base/spMathViewFrustum.hpp"
#include "Base/spTreeNodeQuad.hpp"
#include "Base/spTreeNodeOct.hpp"
#include "Base/spTreeNodeBSP.hpp"
#include "SceneGraph/spMaterialNode.hpp"
#include "RenderSystem/spTextureBase.hpp"

#include <limits.h>
#include <vector>
#include <list>
#include <map>


namespace sp
{
namespace scene
{


/*
 * Macros
 */

static const u32 MESHBUFFER_IGNORE = UINT_MAX; //!< General value to use each Mesh's surface.


/*
 * Pre-declarations
 */

class Mesh;
class Camera;
class SceneGraph;


/*!
 * Meshes represents the main 3D objects which are also called "Entity", "Model" or just "3D Object".
 * A Mesh consists basically of information about its location (position, rotation, scaling),
 * a material (see video::SMeshMaterial) with its color, shader etc. and multiple surfaces which form
 * the 3D data (vertices, triangles, textures). The way to create a 3D model is the following:
 * create a Mesh, add one or more surfaces, add vertices to the surface, add triangles to the
 * surface and finish! Don't forget to update the mesh buffer using the "updateMeshBuffer" function.
 * Then you can add textures on it. Actually there are only surfaces to use multiple textures which shall not
 * lie one upon the other (that would be multi-texturing). But each surface uses the same Mesh's material.
 * Consider that not only a large count of triangle can be a performance problem but also the count of surfaces and entities!
 * 
 * Example code:
 * \code
 * // Create the mesh
 * scene::Mesh* Obj = spSmngr->createMesh();
 * 
 * // Create a new mesh buffer
 * video::MeshBuffer* Shape = Obj->createMeshBuffer();
 * 
 * // Add a texture
 * Shape->addTexture(spDriver->loadTexture("ExampleTexture.jpg"));
 * 
 * // Add three vertices
 * Shape->addVertex(dim::vector3df( 0.0f,  1.0f, 0.0f), dim::point2df(0.5f, 0.0f), video::color(255, 0, 0)); // Vertex 0
 * Shape->addVertex(dim::vector3df( 1.0f, -1.0f, 0.0f), dim::point2df(1.0f, 1.0f), video::color(0, 255, 0)); // Vertex 1
 * Shape->addVertex(dim::vector3df(-1.0f, -1.0f, 0.0f), dim::point2df(0.0f, 1.0f), video::color(0, 0, 255)); // Vertex 2
 * 
 * // Add a triangle
 * Shape->addTriangle(0, 1, 2);
 * 
 * // Update vertex- and index buffer after creating the geometry
 * Shape->updateMeshBuffer();
 * \endcode
 */
class SP_EXPORT Mesh : public MaterialNode
{
    
    public:
        
        Mesh();
        virtual ~Mesh();
        
        /* === Texturing === */
        
        //! Adds the specifies texture to all mesh buffers.
        void addTexture(video::Texture* Tex, const u8 Layer = video::TEXTURE_IGNORE);
        
        /**
        Computes the texture coordinates automatically by the triangles' normal (typically used for lightmap texturing).
        \param Layer: Layer/ level which shall be used.
        \param Density: Factor which will be multiplied with the space coordinates to change it into texture coordinates.
        \param MeshBufferIndex: Specifies the index for the mesh buffer which is to be used. By default MESHBUFFER_IGNORE to use all.
        */
        void textureAutoMap(
            const u8 Layer, const f32 Density = 1.0, const u32 MeshBufferIndex = MESHBUFFER_IGNORE, bool isGlobal = true
        );
        
        //! Returns a list with all textures of all mesh buffers.
        std::list<video::Texture*> getTextureList() const;
        //! Returns count of textures of the specified surface.
        u32 getTextureCount() const;
        
        /* === Mesh buidling === */
        
        //! Updates the normal vectors for each mesh buffer.
        virtual void updateNormals();
        
        /**
        Updates the tangent space (i.e. tangent and binormal vectors for each vertex).
        This function stores the computed vectors in the specified texture coordinates to use them later
        in your Shader programs for OpenGL and Direct3D9. Direct3D11 can use the special tangent and binormal vector
        members of the SMeshVertex3D structure.
        \param TangentLayer: Texture layer for the tangent vector.
        \param BinormalLayer: Texture layer for the binormal vector.
        */
        void updateTangentSpace(const u8 TangentLayer = video::TEXTURE_IGNORE, const u8 BinormalLayer = video::TEXTURE_IGNORE);
        
        //! Updates each vertex buffer.
        void updateVertexBuffer();
        //! Updates each index buffer.
        void updateIndexBuffer();
        //! Updates each vertex- and index buffer.
        void updateMeshBuffer();
        
        /**
        Translates each vertex to the specified direction.
        \param Direction: Direction in which the mesh shall be moved.
        \param Surface: Affected surface(s).
        */
        void meshTranslate(const dim::vector3df &Direction);
        void meshTransform(const dim::vector3df &Size);
        void meshTransform(const dim::matrix4f &Matrix);
        void meshTurn(const dim::vector3df &Rotation);
        
        //! Flips the mesh. i.e. each vertex's coordinate will be inverted.
        void meshFlip();
        void meshFlip(bool isXAxis, bool isYAxis, bool isZAxis);
        
        /**
        Fits the mesh. i.e. each vertex's coordinate will be transformed to the specified box.
        \param Position: Specifies the left-bottom-front position of the transformation box.
        \param Size: Specifies the size of the transformation box.
        */
        void meshFit(const dim::vector3df &Position, const dim::vector3df &Size);
        
        /**
        Spherifies the whole mesh. Best primitive to get a nice sphere is the cube (but use more than 1 segment, e.g. 10 or more).
        \param Factor: Transformation factor in the range from [-1.0 .. 1.0].
        */
        void meshSpherify(f32 Factor);
        
        /**
        Twists the whole mesh. Can be used to create an other kind of a spiral when using a cube
        (with more than 1 segment, e.g. 10 or more).
        \param Rotation: Rotation degree for the twist transformation.
        */
        void meshTwist(f32 Rotation);
        
        /**
        Merges each child mesh as a new surface to this mesh object. This only works if the children are specified
        in the node's children list. Use "Node::addChild" or "Node::addChildren".
        \param isDeleteChildren: If true each child mesh will be deleted after the merging process (by default true).
        */
        void mergeFamily(bool isDeleteChildren = true);
        
        //! Centers the mesh's origin. After this operation the mesh's origin (vector [0|0|0]) is in the middle of the model.
        void centerOrigin();
        
        //! Clips (or rather seperates) concatenated triangles for each mesh buffer. After calling this function each triangle has its own vertices.
        void clipConcatenatedTriangles();
        
        /**
        Flips each mesh's triangle. Each triangle's indices A and C are swapping their value.
        e.g. when a triangle has the indices (0, 1, 2) after flipping it has the indices (2, 1, 0).
        */
        void flipTriangles();
        
        /**
        Gives you the mesh's bounding box which is formed by each vertex-coordinate. There is still a function called
        "getBoundingBox" in the Node class which gives you user defined bounding box for frustum-culling.
        \param Min: Vector which will receive the minimal point of the bounding box.
        \param Max: Vector which will receive the maximal point of the bounding box.
        */
        void getMeshBoundingBox(dim::vector3df &Min, dim::vector3df &Max) const;
        //! Returns dimension of the mesh AABB (axis-aliend-bounding-box).
        dim::aabbox3df getMeshBoundingBox() const;
        //! Returns radius of the mesh bounding sphere.
        f32 getMeshBoundingSphere() const;
        
        /**
        Proceeds frustum-culling for more optimization. This function is used in the mesh's "render" function.
        \param Frustum: ViewFrustum of the active camera. Camera objects can be activated by the
        SceneManager using "SceneManager::setActiveCamera".
        \param Transformation: Frustum's matrix transformation. Use the mesh's global location.
        \return True if the mesh is inside the frustum otherwise false.
        */
        bool checkFrustumCulling(const math::ViewFrustum &Frustum, const dim::matrix4f &Transformation) const;
        
        /* === LOD sub meshes (level-of-detail optimization) === */
        
        /**
        Adds a LOD (level-of-detail) sub mesh.
        \param LODSubMesh: Mesh object which shall be added to the LOD list.
        When LOD is enabled the base mesh (this Mesh object) is the first object and has to be the best detailed mesh.
        \param isCopyMaterials: If true the "LODSubMesh" object's materials are copied to these of this Mesh object.
        */
        void addLODSubMesh(Mesh* LODSubMesh, bool isCopyMaterials = false);
        //! Clears the LOD (level-of-detail) sub mesh list.
        void clearLODSubMeshes();
        
        //! Sets the LOD (level-of-detail) sub meshes list directly.
        void setLODSubMeshList(const std::vector<Mesh*> &LODSubMeshList);
        //! Returns list of the LOD (level-of-detail) sub meshes.
        inline std::vector<Mesh*> getLODSubMeshList() const
        {
            return LODSubMeshList_;
        }
        
        /**
        Sets the LOD distance.
        \param Distance: Specifies the distance which shall be used for LOD computing. Only linear and not exponentially yet.
        */
        void setLODDistance(f32 Distance);
        //! Returns LOD distance.
        inline bool getLODDistance() const
        {
            return LODSubMeshDistance_;
        }
        
        //! Enables or disables the LOD (level-of-detail) management.
        void setLOD(bool Enable);
        //! Returns status of the LOD management.
        inline bool getLOD() const
        {
            return UseLODSubMeshes_;
        }
        
        /* === Mesh buffers === */
        
        /**
        Creates a new video::MeshBuffer object. This object can hold 3D geometry data.
        \param VertexFormat: Specifies the initial vertex format. By default video::VertexFormatDefault (can be used with RenderSystem::getVertexFormatDefault()).
        \param IndexFormat: Specifies the initial index format. By default ATTRIBUTE_USNIGNED_SHORT.
        \return Pointer to the new video::MeshBuffer object.
        */
        video::MeshBuffer* createMeshBuffer(
            video::VertexFormat* VertexFormat = 0,
            const video::ERendererDataTypes IndexFormat = video::DATATYPE_UNSIGNED_SHORT
        );
        
        //! Deletes the specified mesh buffer.
        void deleteMeshBuffer(const u32 Index);
        
        //! Deletes all mesh buffers.
        void deleteMeshBuffers();
        
        /**
        Optimizes the count of surfaces. This optimization is dependent of the textures used by the surfaces.
        e.g. you have mesh with multiple surface where some surfaces have an absolut equal texture list these surfaces
        are reduces to one.
        */
        void optimizeMeshBuffers();
        
        /**
        Optimizes the mesh vertices order depending on their transparency. Most transparent vertices will be placed at the end
        and less transparent vertices at the begin. This function can be used to avoid the overlapping problem with alpha-blending.
        */
        void optimizeTransparency(); // !!! TODO !!!
        
        //! Returns the specified video::MeshBuffer object.
        inline video::MeshBuffer* getMeshBuffer(const u32 Index)
        {
            return Index < SurfaceList_->size() ? (*SurfaceList_)[Index] : 0;
        }
        //! Returns the specified constant video::MeshBuffer object.
        inline const video::MeshBuffer* getMeshBuffer(const u32 Index) const
        {
            return Index < SurfaceList_->size() ? (*SurfaceList_)[Index] : 0;
        }
        
        //! Returns the last video::MeshBuffer object.
        inline video::MeshBuffer* getLastMeshBuffer()
        {
            return !SurfaceList_->empty() ? (*SurfaceList_)[SurfaceList_->size() - 1] : 0;
        }
        //! Returns the last constant video::MeshBuffer object.
        inline const video::MeshBuffer* getLastMeshBuffer() const
        {
            return !SurfaceList_->empty() ? (*SurfaceList_)[SurfaceList_->size() - 1] : 0;
        }
        
        //! Returns cound of mesh buffers.
        inline u32 getMeshBufferCount() const
        {
            return SurfaceList_->size();
        }
        
        /* === Vertices === */
        
        //! Returns count of vertices for the specified surface.
        u32 getVertexCount() const;
        
        //! Returns count of triangles for the specified surface.
        u32 getTriangleCount() const;
        
        /**
        Sets a mesh reference. This feature is very usfull when rendering multiple equal 3D models.
        This saves much memory and for animation much performance costs. e.g. very good for animated grass when
        you have thousands of Mesh objects which all refers to only one animated mesh. Each of these objecsts
        uses the same mesh buffer. Small memory costs and small animation performance costs but many movement in the scene.
        \param ReferenceMesh: Mesh object which holds the mesh buffer which is to be used.
        \param CopyLocation: If true the location is copied from the "ReferenceEntity" object.
        \param CopyMaterial: If true the material settings are copied from the "ReferenceEntity" object.
        */
        void setReference(Mesh* ReferenceMesh, bool CopyLocation = false, bool CopyMaterial = false);
        
        /**
        Saves the basic vertex data (Vertices' position and normal).
        This feature can be used when animating the object to reload the old vertices data.
        */
        /*void saveVertexBasicData();
        void loadVertexBasicData();
        void clearVertexBasicData();*/
        
        /* === Oct-tree optimization === */
        
        /**
        Creates an OctTree for optimization in CollisionDetection, PickingSystem and other possible usage.
        \param ForksCount: Specifies how many forks the hirarchical tree shall has. The OctTree creation
        now is really fast but you should not set the ForksCount to high (Should be less than 10) because
        (8 ^ ForksCount) tree-nodes are created ;-).
        */
        void createOctTree(s8 ForksCount = DEF_TREENODE_FORKSCOUNT);
        //! Deletes the OctTree.
        void deleteOctTree();
        
        //! \return Pointer to the OctTree root node. If currently no OctTree has been created for this mesh the return value is 0.
        inline TreeNodeOct* getOctTreeRoot() const
        {
            return OctTreeRoot_;
        }
        
        /* === Animation === */
        
        /**
        Adds a new animation. Only in this class MorphTarget- and SkeletalAnimations can be created.
        \param Type: Animation type. Three types are supported: Node-, MorphTarget- and SkleletalAnimation.
        \param Name: Animation's name. You can search the animation list by using the name.
        \return Pointer to an animation object. Cast this to the specified animation type.
        When adding a type of "scene::ANIMATION_NODE" cast the pointer to "scene::AnimationNode*".
        */
        Animation* addAnimation(const EAnimationTypes Type, const io::stringc &Name = "");
        
        /**
        Paints the whole mesh witht he specified color.
        \param Color: Specifies the color which is to be set.
        \param CombineColors: Specifies whether the color is to be multiplied with each vertex color or not.
        */
        void paint(const video::color &Color, bool CombineColors = false);
        
        /**
        Sets the shading type.
        \param Type: Type of shading (flat, gouraud, phong, per-pixel).
        \param UpdateImmediate: If true the normals will be updated immediately (updateNormals).
        \see MaterialNode::setShading
        */
        void setShading(const video::EShadingTypes Type, bool UpdateImmediate = false);
        
        /* === Rendering & copying === */
        
        inline void setRenderCallback(const video::UserRenderCallback &RenderCallback)
        {
            UserRenderProc_ = RenderCallback;
        }
        
        /**
        Copies the mesh. Normally used inside the SceneManager but can also be used manual.
        \param other: Specifies the mesh where the data of this mesh shall be stored.
        */
        void copy(const Mesh* other);
        
        //! \return Pointer to a new Mesh object which has been copied by this Mesh.
        Mesh* copy() const;
        
        /**
        Renders the whole mesh. This function is called in the "renderScene" function of the SceneManager.
        Here the object-transformation, frustum-culling, level of detail and drawing the surfaces' mesh buffers are proceeded.
        */
        virtual void render();
        
    protected:
        
        friend class SceneGraph;
        friend class SimpleSceneManager;
        friend class CollisionDetector;
        friend bool cmpObjectMeshes(Mesh* &obj1, Mesh* &obj2);
        
        /* === Functions === */
        
        void init();
        void clear();
        
        u32 updateLevelOfDetail();
        
        void copyMesh(Mesh* NewMesh) const;
        
        /* === Members === */
        
        std::vector<video::MeshBuffer*> * OrigSurfaceList_, * SurfaceList_, * LODSurfaceList_;
        
        bool UseLODSubMeshes_;
        f32 LODSubMeshDistance_;
        std::vector<Mesh*> LODSubMeshList_;
        
        TreeNodeOct* OctTreeRoot_;
        
        video::UserRenderCallback UserRenderProc_;
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
