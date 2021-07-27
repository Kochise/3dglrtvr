#ifndef OBJECT_H
#define OBJECT_H

#include "base3d.h"
#include "point3d.h"
#include "ray.h"
#include "surfacedata.h"
#include "material.h"
#include "intersections.h"

class Object : public Movable
{
    public :
        Base3d localbase;
        std::string materialname;
        
        Object(const Point3d& _point, const Base3d& _base, const std::string& _matname);
        virtual ~Object();
        virtual void findIntersections(const Ray& ray, Intersections& i) const = 0;
        virtual SurfaceData getSurfaceData(const Point3d& localpoint) const = 0;

		Ray convertRayToLocal(const Ray& ray) const;
		Ray convertRayToGlobal(const Ray& ray) const;
		Vector3d convertVector3dToLocal(const Vector3d& v) const;
		Vector3d convertVector3dToGlobal(const Vector3d& v) const;
		Point3d convertPoint3dToLocal(const Point3d& p) const;
		Point3d convertPoint3dToGlobal(const Point3d& p) const;
};

#endif

