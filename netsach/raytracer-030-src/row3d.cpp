
#include <cmath>
#include "vector3d.h"
#include "matrix3d.h"
#include "row3d.h"
#include "exceptions.h"
    
Row3d::~Row3d()
{
}

Row3d::Row3d()
: dx(0), dy(0), dz(0)
{
}

Row3d::Row3d( double _dx, double _dy, double _dz)
: dx(_dx), dy(_dy), dz(_dz)
{
}

std::string Row3d::toString() const
{
	return "Row3d { "
		+ Utils::value2string("A1",dx) + ' '
		+ Utils::value2string("A2",dy) + ' '
		+ Utils::value2string("A3",dz) + " }";
}

void Row3d::normalize()
{
    double l= length();
    if (l == 0.0)
        throw MathException("Unable to normalze a zero-length Row3d");
    dx /= l;
    dy /= l;
    dz /= l;
}

Row3d Row3d::normalized() const
{
    double l = length();
    if (l == 0.0)
        throw MathException("Unable to normalze a zero-length Row3d");
    return Row3d(dx / l, dy / l, dz / l);
}

Row3d Row3d::operator - (const Row3d& v) const
{
    return Row3d( dx - v.dx, dy - v.dy, dz - v.dz );
}

Row3d Row3d::operator + (const Row3d& v) const
{
    return Row3d( dx + v.dx, dy + v.dy, dz + v.dz );
}

Row3d Row3d::operator * (double lambda) const
{
    return Row3d( dx * lambda, dy * lambda, dz * lambda );
}

Row3d Row3d::operator / (double lambda) const
{
    if (lambda == 0.0)
        throw MathException("Row3d divided by zero");
    return Row3d( dx / lambda, dy / lambda, dz / lambda );
}

Row3d Row3d::operator - () const
{
    return Row3d( -dx, -dy, -dz );
}

double Row3d::length() const
{
	return sqrt(dx * dx + dy * dy + dz * dz);
}

Vector3d Row3d::transpose() const
{
	return Vector3d(dx, dy, dz);
}

double Row3d::operator * (const Vector3d& v) const
{
	return (dx * v.dx + dy * v.dy + dz * v.dz);
}

Row3d Row3d::operator * (const Matrix3d& m) const
{
	return Row3d(m.a11 * dx + m.a21 * dy + m.a31 * dz,
		         m.a12 * dx + m.a22 * dy + m.a32 * dz,
				 m.a13 * dx + m.a23 * dy + m.a33 * dz);
}


void Row3d::operator += (const Row3d& v)
{
	dx += v.dx;
	dy += v.dy;
	dz += v.dz;
}

void Row3d::operator -= (const Row3d& v)
{
	dx -= v.dx;
	dy -= v.dy;
	dz -= v.dz;
}

void Row3d::operator *= (double lambda)
{
	dx *= lambda;
	dy *= lambda;
	dz *= lambda;
}

void Row3d::operator /= (double lambda)
{
    if (lambda == 0.0)
        throw MathException("Row3d divided by zero");
	dx /= lambda;
	dy /= lambda;
	dz /= lambda;
}

Row3d Row3d::instanciateFromStream(const std::istream& is)
{
    


	return Row3d(0,0,0);
}
