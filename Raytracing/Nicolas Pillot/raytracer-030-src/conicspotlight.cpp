
#include "conicspotlight.h"

ConicSpotLight::ConicSpotLight(const Point3d& _point, const Color& _color, const Vector3d& _direction, double _openingangle)
: SpotLight(_point,_color, _direction), openingangle(_openingangle)
{
}

ConicSpotLight::~ConicSpotLight()
{
}

std::string ConicSpotLight::toString() const
{
    return "ConicSpotLight { "
        + Utils::value2string("Point", point) + ' '
		+ Utils::value2string("Color", color) + ' '
        + Utils::value2string("Direction", direction) + ' '
		+ Utils::value2string("OpeningAngle", openingangle) + " }";
}

double ConicSpotLight::getVisibilityFromPoint(const Point3d& _point, const Vector3d& _normal, const Scene* const scene) const
{
/* TODO (#1#): action du cone du spot sur la vision normale */
    return 1.0;
}

ConicSpotLight ConicSpotLight::instanciateFromStream(const std::istream& is)
{
    


	return ConicSpotLight(Point3d(0,0,0),Color(1,1,1),Vector3d(0,0,1),1);
}

