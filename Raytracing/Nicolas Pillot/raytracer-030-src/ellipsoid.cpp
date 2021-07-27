
#include <cmath>
#include "ellipsoid.h"
#include "polynom.h"

Ellipsoid::Ellipsoid(const Point3d& _point, const Base3d& _base, const std::string& _matname, double _indice, double _xradius, double _yradius, double _zradius)
: BoundedObject(_point, _base, _matname, _indice), xradius(_xradius), yradius(_yradius), zradius(_zradius)
{
}

Ellipsoid::~Ellipsoid()
{
}

std::string Ellipsoid::toString() const
{
    return "Ellipsoid { "
        + Utils::value2string("Point", point) + ' '
		+ Utils::value2string("LocalBase", localbase) + ' '
        + Utils::value2string("RefractionIndex", refractionindice) + ' '
        + Utils::value2string("XRadius", xradius) + ' '
        + Utils::value2string("YRadius", yradius) + ' '
        + Utils::value2string("ZRadius", zradius) + " }";
}

void Ellipsoid::findIntersections(const Ray& ray, Intersections& i) const
{
	// find intersection points
	Polynom px; px[1] = ray.direction.dx; px[0] = ray.point.x;
	Polynom py; py[1] = ray.direction.dy; py[0] = ray.point.y;
	Polynom pz; pz[1] = ray.direction.dz; pz[0] = ray.point.z;
	Polynom eq = (px / xradius).pow(2) + (py / yradius).pow(2) + (pz / zradius).pow(2) - 1;
	Polynom::Zeros zeros = eq.getZeros();

	// fills an IntersectionData for each of them
	Polynom::Zeros::const_iterator it = zeros.begin();
	for (; it != zeros.end(); it++)
	{
		// compute the coordinates of the local point
		Point3d ip = ray.point + ray.direction * (*it);
		// add the information to the intersection array
		i.addIntersectionData(IntersectionData(*it, this, ip));
	}
}

SurfaceData Ellipsoid::getSurfaceData(const Point3d& localpoint) const
{
    // Bring poin back to a sphere's one
    Point3d localsphere (localpoint.x / xradius, localpoint.y / yradius, localpoint.z / zradius );

    // Computes the sphere normal
    Vector3d localnormal (localsphere.x, localsphere.y, localsphere.z);
    Vector3d globalnormal = convertVector3dToGlobal( localnormal );
    globalnormal.normalize();
    
    // Computes the U of the point
    double tetha;
    if (localsphere.x == 0.0)
    {
        if (localsphere.y > 0)
            tetha = + Utils::PI_2;
        else
            tetha = - Utils::PI_2;
    }
    else
    {
        tetha = atan( localsphere.y / localsphere.x );
        if (localsphere.x < 0)
        {
            if (localsphere.y > 0)
                tetha += Utils::PI;
            else
                tetha -= Utils::PI;
        }
    }
    double u = 0.5 * (tetha / Utils::PI + 1);
    
    // Computes the V of the point
    double v = acos(localsphere.z) / Utils::PI;

    return SurfaceData(globalnormal,u,v);
}

Ellipsoid Ellipsoid::instanciateFromStream(const std::istream& is)
{
    


	return Ellipsoid(Point3d(0,0,0),Base3d(Vector3d(1,0,0),Vector3d(0,1,0),Vector3d(0,0,1)),"matname",1,2,3,4);
}

