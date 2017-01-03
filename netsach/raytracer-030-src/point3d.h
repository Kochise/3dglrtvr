#ifndef POINT3D_H
#define POINT3D_H

#include "printable.h"

#include "vector3d.h"

class Point3d : public Printable
{
    public:
        double x, y, z;

        Point3d( double _x, double _y, double _z);
        virtual ~Point3d();

        void MoveTo(double _x, double _y, double _z);
        void MoveBy(double dx, double dy, double dz);

        virtual std::string toString() const;

		Point3d operator + (const Vector3d& v) const;
		Point3d operator - (const Vector3d& v) const;

        static Point3d instanciateFromStream(const std::istream& is);
};

#endif

