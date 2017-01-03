
#include "unboundedobject.h"

UnboundedObject::UnboundedObject(const Point3d& _point, const Base3d& _base, const std::string& _matname)
: Object(_point, _base, _matname)
{
}

UnboundedObject::~UnboundedObject()
{
}

double UnboundedObject::getRefractionIndice(const Point3d& _point) const
{
    /* a negative value tells to use the ambient refraction incide */
    return -1;
}
