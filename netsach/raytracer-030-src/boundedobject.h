#ifndef BOUNDEDOBJECT_H
#define BOUNDEDOBJECT_H

#include "object.h"

class BoundedObject : public Object
{
    public :
        double refractionindice;

        BoundedObject(const Point3d& _point, const Base3d& _base, const std::string& _matname, double _indice);
        virtual ~BoundedObject();
};

#endif

