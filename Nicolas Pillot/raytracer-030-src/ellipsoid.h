#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "boundedobject.h"

class Ellipsoid : public BoundedObject
{
    public:
        double xradius, yradius, zradius;
    
        Ellipsoid(const Point3d& _point, const Base3d& _base, const std::string& _matname, double _indice, double _xradius, double _yradius, double _zradius);
        virtual ~Ellipsoid();

        virtual void findIntersections(const Ray& ray, Intersections& i) const;
        virtual SurfaceData getSurfaceData(const Point3d& localpoint) const;

        virtual std::string toString() const;
        static Ellipsoid instanciateFromStream(const std::istream& is);

};

#endif
