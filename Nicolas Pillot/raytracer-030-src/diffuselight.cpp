
#include "diffuselight.h"
#include "ray.h"
#include "intersections.h"

DiffuseLight::DiffuseLight(const Point3d& _point, const Color& _color)
: Light(_point, _color)
{
}

DiffuseLight::~DiffuseLight()
{
}

std::string DiffuseLight::toString() const
{
    return "DiffuseLight { "
        + Utils::value2string("Point", point) + ' '
		+ Utils::value2string("Color", color) + " }";
}

extern bool f; //debug

double DiffuseLight::getVisibilityFromPoint(const Point3d& _point, const Vector3d& _normal, const Scene* const scene) const
{
    // Computes th ray going to the light
    Ray ray(_point, Vector3d(_point, point).normalized());
	// Computes all intersection from current ray with objects
    Intersections intersections(&ray, scene);
	// Find the first object visible (from ray point of view)
	const IntersectionData * const pid = intersections.getFirstVisible();

	// If no object has been seen, then light can be seen
	if (pid == NULL)
	{
	  double lighting = ray.direction | _normal;
		if (lighting < 0)
			lighting = 0;
		return lighting;
	}
	else
    return 0.0;
}

DiffuseLight DiffuseLight::instanciateFromStream(const std::istream& is)
{
    


	return DiffuseLight(Point3d(0,0,0),Color(1,1,1));
}

