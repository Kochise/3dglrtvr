/*
 * Material node header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_SCENE_MATERIALNODE_H__
#define __SP_SCENE_MATERIALNODE_H__


#include "Base/spStandard.hpp"
#include "Base/spInputOutputString.hpp"
#include "Base/spMaterialStates.hpp"
#include "SceneGraph/spRenderNode.hpp"


namespace sp
{
namespace scene
{


/**
MaterialNode objects are the root of Mesh, Billboard and Terrain objects. The main content of these objects
is the MaterialStates member with all its interface functions. It also has a shader object.
*/
class SP_EXPORT MaterialNode : public RenderNode
{
    
    public:
        
        virtual ~MaterialNode();
        
        virtual void render() = 0;
        
        virtual bool compare(MaterialNode* other);
        
        /* === Material === */
        
        /**
        Sets the mesh's material. In materials many configurations are stored.
        One of these are Diffuse or Ambient colors, ZBuffer- or Lighitng enabling and a Shader object.
        The most of these things can be set by special functions but you have more controll of the
        material by setting it directly. The material affects the whole mesh (each surface).
        If you want a mesh with multiple materials you have to created multiple Mesh object and one of them
        has to be the parent object of the others (use "setParent").
        \param Material: SMaterialStates structure which holds the whole material's data.
        */
        void setMaterial(const video::MaterialStates* Material);
        
        //! \return Pointer to the video::MaterialStates object.
        inline video::MaterialStates* getMaterial()
        {
            return Material_;
        }
        //! \return Constant pointer to the video::MaterialStates object.
        inline const video::MaterialStates* getMaterial() const
        {
            return Material_;
        }
        
        /**
        Sets the shader object. Shader can be used for high performance graphics effects.
        e.g. water reflection and refraction (more about shaders in the Shader class).
        \param ShaderObject: Pointer to a Shader object.
        */
        inline void setShader(video::Shader* ShaderObject)
        {
            ShaderObject_ = (ShaderObject ? ShaderObject->getShaderTable() : 0);
        }
        
        //! Sets the shader table directly.
        inline void setShaderTable(video::ShaderTable* ShaderObject)
        {
            ShaderObject_ = ShaderObject;
        }
        //! Returns pointer to the ShaderTable object.
        inline video::ShaderTable* getShaderTable() const
        {
            return ShaderObject_;
        }
        
        //! Enables or disables the material. If disabled the last - in renderer set - material will be used.
        inline void setMaterialEnable(bool Enable)
        {
            EnableMaterial_ = Enable;
        }
        //! Returns true if the material is enabled otherwise false. By default true.
        inline bool getMaterialEnable() const
        {
            return EnableMaterial_;
        }
        
    protected:
        
        friend class SceneGraph;
        friend class CollisionDetector;
        friend bool cmpMeshesDefault(Mesh* &obj1, Mesh* &obj2);
        friend bool cmpMeshesFast(Mesh* &obj1, Mesh* &obj2);
        
        /* Functions */
        
        MaterialNode(const ENodeTypes Type);
        
        /* Members */
        
        video::MaterialStates* Material_;
        video::ShaderTable* ShaderObject_;
        bool EnableMaterial_;
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
