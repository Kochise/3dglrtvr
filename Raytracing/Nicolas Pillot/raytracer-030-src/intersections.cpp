
#include "scene.h"
#include "ray.h"
#include "intersections.h"
#include "exceptions.h"

Intersections::Intersections(const Ray * const ray, const Scene * const scene)
{
	if (ray == NULL)
		throw NullPointerException("Intersections::Intersections : no Ray given");
	if (scene == NULL)
		throw NullPointerException("Intersections::Intersections : no Scene given");

    for (int i = 0; i < scene->getObjectNumber(); i++)
	{
		Object * o = scene->getObject(i);
		o->findIntersections(o->convertRayToLocal(*ray),*this);
	}
}

Intersections::~Intersections()
{
}

const IntersectionData * const Intersections::getFirstVisible() const
{
	IntersectionMap::const_iterator it = intersections.begin();
	for (; it != intersections.end(); it++)
		if ((*it).first > Utils::epsilon)
			return & (*it).second;
	return NULL;
}

std::string Intersections::toString() const
{
	std::string tmp ("Intersections { ");
    for (IntersectionMap::const_iterator it = intersections.begin(); it != intersections.end(); it++)
        tmp += ((*it).second).toString() + '\n';
    tmp += " }";
    return tmp;
}

void Intersections::addIntersectionData(const IntersectionData& id)
{
	intersections.insert(IntersectionMap::value_type(id.lambda, id));
}
