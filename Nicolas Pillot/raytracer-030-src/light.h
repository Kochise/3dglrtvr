#ifndef LIGHT_H
#define LIGHT_H

#include "movable.h"
#include "color.h"

class Scene;

class Light : public Movable
{
    public:
        Color color;

    public:
        Light(const Point3d& _point, const Color& _color);
        virtual ~Light();
        
        virtual double getVisibilityFromPoint(const Point3d& _point, const Vector3d& _normal, const Scene* const scene) const = 0;
};

#endif
