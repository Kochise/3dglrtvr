
#include "point3d.h"
#include "vector3d.h"

Point3d::~Point3d()
{
}

Point3d::Point3d( double _x, double _y, double _z)
: x(_x), y(_y), z(_z)
{
}
 
void Point3d::MoveTo(double _x, double _y, double _z)
{
    x = _x;
    y = _y;
    z = _z;
}

void Point3d::MoveBy(double dx, double dy, double dz)
{
    x += dx;
    y += dy;
    z += dz;
}

std::string Point3d::toString() const
{
    return "Point3d { "
		+ Utils::value2string("X",x) + ' '
		+ Utils::value2string("Y",y) + ' '
		+ Utils::value2string("Z",z) + " }";
}

Point3d Point3d::operator + (const Vector3d& v) const
{
    return Point3d(x + v.dx, y + v.dy, z + v.dz);
}

Point3d Point3d::operator - (const Vector3d& v) const
{
	return Point3d(x - v.dx, y - v.dy, z - v.dz);
}

Point3d Point3d::instanciateFromStream(const std::istream& is)
{
    


	return Point3d(0,0,0);
}
