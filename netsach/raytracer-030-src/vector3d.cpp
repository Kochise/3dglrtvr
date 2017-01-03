
#include <cmath>
#include "point3d.h"
#include "row3d.h"
#include "vector3d.h"
#include "matrix3d.h"
#include "exceptions.h"

Vector3d::~Vector3d()
{
}

Vector3d::Vector3d()
: dx(0), dy(0), dz(0)
{
}

Vector3d::Vector3d(double _dx, double _dy, double _dz)
: dx(_dx), dy(_dy), dz(_dz)
{
}

Vector3d::Vector3d( const Point3d& base, const Point3d& extrem )
: dx(extrem.x - base.x), dy(extrem.y - base.y), dz(extrem.z - base.z)
{
}
 
std::string Vector3d::toString() const
{
    return "Vector3d { "
        + Utils::value2string("DX", dx) + ' '
		+ Utils::value2string("DY", dy) + ' '
        + Utils::value2string("DZ", dz) + " }";
}

void Vector3d::normalize()
{
    double l= length();
    if (l == 0.0)
        throw MathException("Unable to normalze a zero-length Vector3d");
    dx /= l;
    dy /= l;
    dz /= l;
}

Vector3d Vector3d::normalized() const
{
    double l = length();
    if (l == 0.0)
        throw MathException("Unable to normalze a zero-length Vector3d");
    return Vector3d(dx / l, dy / l, dz / l);
}

Vector3d Vector3d::operator ^ (const Vector3d& v) const
{
    return Vector3d( dy*v.dz - v.dy*dz, dz*v.dx - v.dz*dx, dx*v.dy - v.dx*dy ) ;                     
}

Vector3d Vector3d::operator - (const Vector3d& v) const
{
    return Vector3d( dx - v.dx, dy - v.dy, dz - v.dz );
}

Vector3d Vector3d::operator + (const Vector3d& v) const
{
    return Vector3d( dx + v.dx, dy + v.dy, dz + v.dz );
}

Vector3d Vector3d::operator * (double lambda) const
{
    return Vector3d( dx * lambda, dy * lambda, dz * lambda );
}

Matrix3d Vector3d::operator * (const Row3d& r) const
{
	return Matrix3d(dx * r.dx, dx * r.dy, dx * r.dz,
					dy * r.dx, dy * r.dy, dy * r.dz,
					dz * r.dx, dz * r.dy, dz * r.dz);
}

Vector3d Vector3d::operator / (double lambda) const
{
    if (lambda == 0.0)
        throw MathException("Vector3d divided by zero");
    return Vector3d( dx / lambda, dy / lambda, dz / lambda );
}

Vector3d Vector3d::operator - () const
{
    return Vector3d( -dx, -dy, -dz );
}

double Vector3d::operator | (const Vector3d& v) const
{
    return (dx * v.dx + dy * v.dy + dz * v.dz );
}

double Vector3d::length() const
{
	return sqrt(dx * dx + dy * dy + dz * dz);
}

Point3d Vector3d::operator + (const Point3d& p) const
{
    return Point3d(p.x + dx, p.y + dy, p.z + dz);
}

Point3d Vector3d::operator - (const Point3d& p) const
{
	return Point3d(p.x - dx, p.y - dy, p.z - dz);
}

Row3d Vector3d::transpose() const
{
	return Row3d(dx, dy, dz);
}

void Vector3d::operator += (const Vector3d& v)
{
	dx += v.dx;
	dy += v.dy;
	dz += v.dz;
}

void Vector3d::operator -= (const Vector3d& v)
{
	dx -= v.dx;
	dy -= v.dy;
	dz -= v.dz;
}

void Vector3d::operator *= (double lambda)
{
	dx *= lambda;
	dy *= lambda;
	dz *= lambda;
}

void Vector3d::operator /= (double lambda)
{
    if (lambda == 0.0)
        throw MathException("Vector3d divided by zero");
	dx /= lambda;
	dy /= lambda;
	dz /= lambda;
}

Vector3d Vector3d::instanciateFromStream(const std::istream& is)
{
    


	return Vector3d(0,0,0);
}

