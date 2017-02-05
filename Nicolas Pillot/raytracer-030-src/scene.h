#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>

#include "object.h"
#include "light.h"
#include "material.h"

class Scene : public Printable
{
    private:
        typedef std::vector<Light *> MyLights;
        typedef std::vector<Object *> MyObjects;
        typedef std::map<std::string, Material *> MyMaterials;
        MyObjects   objects;
        MyLights    lights;
        MyMaterials materials;

    public:
        Scene(double _ambientrefraction = 1.0);
        virtual ~Scene();
        
		double ambientrefraction;

        int getLightNumber() const;
        int getObjectNumber() const;
        int getMaterialNumber() const;

        Light    * getLight    (int lightnum)  const;        
        Object   * getObject   (int objectnum) const;        
        Material * getMaterial (const std::string& matname) const;        
        
        void addLight (Light * light);        
        void addObject (Object * object);        
        void addMaterial (Material * object);        
        
        void removeLight (int lightnum);        
        void removeObject (int objectnum);        
        void removeMaterial (const std::string& matname);
        
        std::string toString() const;
};

#endif
