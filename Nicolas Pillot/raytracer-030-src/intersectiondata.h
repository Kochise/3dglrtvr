#ifndef INTERSECTIONDATA_H
#define INTERSECTIONDATA_H

#include "printable.h"
#include "point3d.h"

class Object;

class IntersectionData : public Printable
{
    public:
        IntersectionData(double _lambda, const Object * _object, const Point3d& _localpoint);
        virtual ~IntersectionData();

        const double lambda;
		const Object * object;
		const Point3d localpoint;

        std::string toString () const;
};

#endif
