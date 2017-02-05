#ifndef RAY_H
#define RAY_H

#include "movable.h"
#include "vector3d.h"

class Ray : public Movable
{
    public:
        Vector3d direction;

        Ray(const Point3d& _point, const Vector3d& _direction);
        virtual ~Ray();
        
        virtual std::string toString() const;
};

#endif

