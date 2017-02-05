
#include <cmath>
#include "row3d.h"
#include "vector3d.h"
#include "matrix3d.h"
#include "exceptions.h"
    
Matrix3d::~Matrix3d()
{
}

Matrix3d::Matrix3d()
: a11(0), a12(0), a13(0),
  a21(0), a22(0), a23(0),
  a31(0), a32(0), a33(0)
{
}


Matrix3d::Matrix3d( double _a11, double _a12, double _a13,
					double _a21, double _a22, double _a23, 
					double _a31, double _a32, double _a33)
: a11(_a11), a12(_a12), a13(_a13),
  a21(_a21), a22(_a22), a23(_a23),
  a31(_a31), a32(_a32), a33(_a33)
{
}

Matrix3d::Matrix3d(const Vector3d& v1, const Vector3d& v2, const Vector3d& v3)
: a11(v1.dx), a12(v2.dx), a13(v3.dx),
  a21(v1.dy), a22(v2.dy), a23(v3.dy),
  a31(v1.dz), a32(v2.dz), a33(v3.dz)
{
}

Matrix3d::Matrix3d(const Row3d& r1, const Row3d& r2, const Row3d& r3)
: a11(r1.dx), a12(r1.dy), a13(r1.dz),
  a21(r2.dx), a22(r2.dy), a23(r2.dz),
  a31(r3.dx), a32(r3.dy), a33(r3.dz)
{
}

std::string Matrix3d::toString() const
{
    return "Matrix3d { "
		+ Utils::value2string("A11",a11) + ' '
		+ Utils::value2string("A12",a12) + ' '
		+ Utils::value2string("A13",a13) + ' '
		+ Utils::value2string("A21",a21) + ' '
		+ Utils::value2string("A22",a22) + ' '
		+ Utils::value2string("A23",a23) + ' '
		+ Utils::value2string("A31",a31) + ' '
		+ Utils::value2string("A32",a32) + ' '
		+ Utils::value2string("A33",a33) + " }";
}

Matrix3d Matrix3d::operator - () const
{
    return Matrix3d( -a11, -a12, -a13,
					 -a21, -a22, -a23,
					 -a31, -a32, -a33);
}

Matrix3d Matrix3d::operator - (const Matrix3d& m) const
{
    return Matrix3d( a11 - m.a11, a12 - m.a12, a13 - m.a13,
		             a21 - m.a21, a22 - m.a22, a23 - m.a23,
					 a31 - m.a31, a32 - m.a32, a33 - m.a33);
}

Matrix3d Matrix3d::operator + (const Matrix3d& m) const
{
    return Matrix3d( a11 + m.a11, a12 + m.a12, a13 + m.a13,
		             a21 + m.a21, a22 + m.a22, a23 + m.a23,
					 a31 + m.a31, a32 + m.a32, a33 + m.a33);
}

Matrix3d Matrix3d::operator * (double lambda) const
{
    return Matrix3d( a11 * lambda, a12 * lambda, a13 * lambda,
		             a21 * lambda, a22 * lambda, a23 * lambda,
					 a31 * lambda, a32 * lambda, a33 * lambda);
}

Matrix3d Matrix3d::operator / (double lambda) const
{
    if (lambda == 0.0)
        throw MathException("Matrix3d divided by zero");
    return Matrix3d( a11 / lambda, a12 / lambda, a13 / lambda,
		             a21 / lambda, a22 / lambda, a23 / lambda,
					 a31 / lambda, a32 / lambda, a33 / lambda);
}

Vector3d Matrix3d::operator * (const Vector3d& v) const
{
	return Vector3d(a11 * v.dx + a12 * v.dy + a13 * v.dz,
					a21 * v.dx + a22 * v.dy + a23 * v.dz,
					a31 * v.dx + a32 * v.dy + a33 * v.dz);
}

Matrix3d Matrix3d::operator * (const Matrix3d& m) const
{
	return Matrix3d(a11 * m.a11 + a12 * m.a21 + a13 * m.a31,
					a11 * m.a12 + a12 * m.a22 + a13 * m.a32,
					a11 * m.a13 + a12 * m.a23 + a13 * m.a33,
					a21 * m.a11 + a22 * m.a21 + a23 * m.a31,
					a21 * m.a12 + a22 * m.a22 + a23 * m.a32,
					a21 * m.a13 + a22 * m.a23 + a23 * m.a33,
					a31 * m.a11 + a32 * m.a21 + a33 * m.a31,
					a31 * m.a12 + a32 * m.a22 + a33 * m.a32,
					a31 * m.a13 + a32 * m.a23 + a33 * m.a33);
}

void Matrix3d::operator += (const Matrix3d& m)
{
	a11 += m.a11;
	a12 += m.a12;
	a13 += m.a13;
	a21 += m.a21;
	a22 += m.a22;
	a23 += m.a23;
	a31 += m.a31;
	a32 += m.a32;
	a33 += m.a33;
}

void Matrix3d::operator -= (const Matrix3d& m)
{
	a11 -= m.a11;
	a12 -= m.a12;
	a13 -= m.a13;
	a21 -= m.a21;
	a22 -= m.a22;
	a23 -= m.a23;
	a31 -= m.a31;
	a32 -= m.a32;
	a33 -= m.a33;
}

void Matrix3d::operator *= (double lambda)
{
	a11 *= lambda;
	a12 *= lambda;
	a13 *= lambda;
	a21 *= lambda;
	a22 *= lambda;
	a23 *= lambda;
	a31 *= lambda;
	a32 *= lambda;
	a33 *= lambda;
}

void Matrix3d::operator /= (double lambda)
{
    if (lambda == 0.0)
        throw MathException("Matrix3d divided by zero");
	a11 /= lambda;
	a12 /= lambda;
	a13 /= lambda;
	a21 /= lambda;
	a22 /= lambda;
	a23 /= lambda;
	a31 /= lambda;
	a32 /= lambda;
	a33 /= lambda;
}

Matrix3d Matrix3d::transposed() const
{
	return Matrix3d(a11, a21, a31, a12, a22, a32, a13, a23, a33);	
}

void Matrix3d::transpose()
{
	*this = this->transposed();
}

Matrix3d Matrix3d::inverted() const
{
	double d = det();
	if (d == 0.0)
        throw MathException("Matrix3d cannot be inverted");

	return Matrix3d((a22 * a33 - a32 * a23 )/d,
					(a13 * a32 - a12 * a33 )/d,
					(a12 * a23 - a22 * a13 )/d,
					(a31 * a23 - a21 * a33 )/d,
					(a11 * a33 - a31 * a13 )/d,
					(a13 * a21 - a11 * a23 )/d,
					(a21 * a32 - a31 * a22 )/d,
					(a31 * a12 - a11 * a32 )/d,
					(a11 * a22 - a21 * a12 )/d);
}

void Matrix3d::invert()
{
	*this = this->inverted();
}

double Matrix3d::det() const
{
	return a11 * a22 * a33 + a12 * a23 * a31 + a21 * a32 * a13
		 - a31 * a22 * a13 - a21 * a12 * a33 - a11 * a23 * a32;
}

Matrix3d Matrix3d::instanciateFromStream(const std::istream& is)
{
	return Matrix3d();
}
