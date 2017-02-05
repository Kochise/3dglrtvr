
#include "scene.h"
#include "exceptions.h"

Scene::Scene(double _ambientrefraction)
: ambientrefraction(_ambientrefraction)
{
}

Scene::~Scene()
{
    // Clears every object 
    for (MyObjects::iterator ito = objects.begin(); ito != objects.end(); ito++)
        if ((*ito) != NULL)
        {
            delete *ito;
            *ito = NULL;
        }
    // Clears every light
    for (MyLights::iterator itl = lights.begin(); itl != lights.end(); itl++)
        if ((*itl) != NULL)
        {
            delete *itl;
            *itl = NULL;
        }
    // Clears every material
    for (MyMaterials::iterator itm = materials.begin(); itm != materials.end(); itm++)
        if ((*itm).second != NULL)
        {
            delete (*itm).second;
            (*itm).second = NULL;
        }
}

int Scene::getLightNumber() const
{
    return (int) lights.size();
}

int Scene::getObjectNumber() const
{
    return (int) objects.size();
}

int Scene::getMaterialNumber()  const
{
    return (int) materials.size();
}

Light * Scene::getLight (int lightnum)  const
{
    if ((lightnum < 0) || (lightnum >= getLightNumber()))
        throw InvalidParameterException("Scene::getLight : index out of bound");
    if (lights[lightnum] == NULL)
        throw NullPointerException("Scene::getLight : null pointer detected");
    return lights[lightnum];
}

Object * Scene::getObject (int objectnum) const
{
    if ((objectnum < 0) || (objectnum >= getObjectNumber()))
        throw InvalidParameterException("Scene::getObjectt : index out of bound");
    if (objects[objectnum] == NULL)
        throw NullPointerException("Scene::getObject : null pointer detected");
    return objects[objectnum];
}

Material * Scene::getMaterial (const std::string& matname) const
{
    MyMaterials::const_iterator it = materials.find(matname);
    if (it == materials.end())
        throw NotFoundException("Scene::getMaterial : material '" + matname + "' not found");
    if ((*it).second == NULL)
        throw NullPointerException("Scene::getMaterial : null pointer detected");
    return (*it).second;
}
        
void Scene::addLight (Light * light)
{
    if (light == NULL)
        throw NullPointerException("Scene::addLight : null pointer detected");
    lights.push_back(light);
}

void Scene::addObject (Object * object)
{
    if (object == NULL)
        throw NullPointerException("Scene::addObject : null pointer detected");
    objects.push_back(object);
}

void Scene::addMaterial (Material * material)
{
    if (material == NULL)
        throw NullPointerException("Scene::addMaterial : null pointer detected");
    std::string matname = material->getMaterialName();
    if (matname.size() == 0)
        throw InvalidParameterException("Scene::addMaterial : unnamed material detected");
    MyMaterials::const_iterator it = materials.find(matname);
    if (it == materials.end())
        materials.insert(MyMaterials::value_type(matname,material));
}

void Scene::removeLight (int lightnum)
{
    if ((lightnum < 0) || (lightnum >= getLightNumber()))
        throw InvalidParameterException("Scene::getLight : index out of bound");
    if (lights[lightnum] != NULL)
    {
        delete lights[lightnum];
        lights[lightnum] = NULL;
    }
    lights.erase(lights.begin() + lightnum);
}

void Scene::removeObject (int objectnum)
{
    if ((objectnum < 0) || (objectnum >= getObjectNumber()))
        throw InvalidParameterException("Scene::getObject : index out of bound");
    if (objects[objectnum] != NULL)
    {
        delete objects[objectnum];
        objects[objectnum] = NULL;
    }
    objects.erase(objects.begin() + objectnum);
}

void Scene::removeMaterial (const std::string& matname)
{
    MyMaterials::iterator it = materials.find(matname);
    if (it == materials.end())
        throw NotFoundException("Scene::removeMaterial : material '" + matname + "' not found");
    
    if ((*it).second != NULL)
    {
        delete (*it).second;
        (*it).second = NULL;
    }
    materials.erase(it);
}

std::string Scene::toString() const
{
    return "Scene { " + Utils::value2string(getObjectNumber()) + ' '
                      + Utils::value2string(getLightNumber()) + ' '
                      + Utils::value2string(getMaterialNumber()) + " }";
}


