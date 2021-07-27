
#include "sphere.h"

Sphere::Sphere(const Point3d& _point, const Base3d& _base, const std::string& _matname, double _indice, double _radius)
: Ellipsoid(_point, _base, _matname, _indice, _radius, _radius, _radius), radius(_radius)
{
}

Sphere::~Sphere()
{
}

std::string Sphere::toString() const
{
    return "Sphere { "
        + Utils::value2string("Point", point) + ' '
		+ Utils::value2string("LocalBase", localbase) + ' '
        + Utils::value2string("RefractionIndex", refractionindice) + ' '
        + Utils::value2string("Radius", radius) + " }";
}

Sphere Sphere::instanciateFromStream(const std::istream& is)
{
    


	return Sphere(Point3d(0,0,0),Base3d(Vector3d(1,0,0),Vector3d(0,1,0),Vector3d(0,0,1)),"matname",1,2);
}

