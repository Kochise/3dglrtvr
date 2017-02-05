#ifndef SPHERE_H
#define SPHERE_H

#include "ellipsoid.h"

class Sphere : public Ellipsoid
{
    public:
		double radius;

        Sphere(const Point3d& _point, const Base3d& _base, const std::string& _matname, double _indice, double _radius);
        virtual ~Sphere();

        virtual std::string toString() const;

        static Sphere instanciateFromStream(const std::istream& is);
};

#endif
