
#include "boundedobject.h"

BoundedObject::BoundedObject(const Point3d& _point, const Base3d& _base, const std::string& _matname, double _indice)
: Object(_point, _base, _matname), refractionindice(_indice)
{
}

BoundedObject::~BoundedObject()
{
}

