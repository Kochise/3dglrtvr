
#include "intersectiondata.h"
#include "object.h"

IntersectionData::IntersectionData(double _lambda,  const Object * _object, const Point3d& _localpoint)
: lambda(_lambda), object(_object), localpoint(_localpoint)
{
}

IntersectionData::~IntersectionData()
{
}

std::string IntersectionData::toString () const
{
    return "IntersectionData { "
        + Utils::value2string("Lambda", lambda) + ' '
		+ Utils::value2string("Object", object->toString()) + ' '
        + Utils::value2string("LocalPoint", localpoint) + " }";
}
