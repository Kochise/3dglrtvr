#ifndef UNBOUNDEDOBJECT_H
#define UNBOUNDEDOBJECT_H

#include "object.h"

class UnboundedObject : public Object
{
    public :
        UnboundedObject(const Point3d& _point, const Base3d& _base, const std::string& _matname);
        virtual ~UnboundedObject();
        virtual double getRefractionIndice(const Point3d& _point) const;
};

#endif

