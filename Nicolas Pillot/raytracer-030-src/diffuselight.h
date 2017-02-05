#ifndef DIFFUSELIGHT_H
#define DIFFUSELIGHT_H

#include "light.h"

class DiffuseLight : public Light
{
    public:
        DiffuseLight(const Point3d& _point, const Color& _color);
        virtual ~DiffuseLight();
        
        virtual std::string toString() const;
        virtual double getVisibilityFromPoint(const Point3d& _point, const Vector3d& _normal, const Scene* const scene) const;

        static DiffuseLight instanciateFromStream(const std::istream& is);
};

#endif
