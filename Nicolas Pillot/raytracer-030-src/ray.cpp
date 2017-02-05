
#include "ray.h"

Ray::Ray(const Point3d& _point, const Vector3d& _direction)
: Movable(_point), direction(_direction)
{
}

Ray::~Ray()
{
}

std::string Ray::toString() const
{
	return "Ray { "
		+ Utils::value2string("Point",point) + ' '
		+ Utils::value2string("Direction",direction) + " }";
}

