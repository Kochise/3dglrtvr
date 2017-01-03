#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "vector3d.h"
#include "diffuselight.h"

class SpotLight : public DiffuseLight
{
    public:
        Vector3d direction;
    
        SpotLight(const Point3d& _point, const Color& _color, const Vector3d& _direction);
        virtual ~SpotLight();
};

#endif
