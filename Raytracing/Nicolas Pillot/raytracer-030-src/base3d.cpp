
#include <cmath>
#include "base3d.h"
#include "matrix3d.h"
#include "exceptions.h"

Base3d::Base3d(const Vector3d& _v1, const Vector3d& _v2, const Vector3d& _v3)
: v1(_v1), v2(_v2), v3(_v3)
{
	checkValidBase();
	orthoNormalizeBase();
}

Base3d::~Base3d()
{
}
        
void Base3d::orthoNormalizeBase()
{
	v2 -= v1 * (v2 | v1);
	v3 = (v1 ^ v2);
    v1.normalize();
    v2.normalize();
    v3.normalize();
}

void Base3d::setVectors(const Vector3d& _v1, const Vector3d& _v2, const Vector3d& _v3)
{
	orthoNormalizeBase();
    v1 = _v1;
    v2 = _v2;
	v3 = _v3;
	checkValidBase();
}

std::string Base3d::toString() const
{
	return "Base3d { "
		+ Utils::value2string("V1", v1) + ' '
		+ Utils::value2string("V2", v2) + ' '
		+ Utils::value2string("V3", v3) + " }";
}

void Base3d::checkValidBase() const
{
	Matrix3d m(v1,v2,v3);
	if (m.det() == 0)
		throw MathException("Base3d::checkValidBase : linked-std::vector system");
}

void Base3d::rotateV1(double deg)
{
	deg = Utils::deg2rad(deg);
	double c = cos(deg), s = sin(deg);
	Vector3d v2p ( v2 * c + v3 * s );
	Vector3d v3p ( v3 * c - v2 * s );
	v2 = v2p;
	v3 = v3p;
}

void Base3d::rotateV2(double deg)
{
	deg = Utils::deg2rad(deg);
	double c = cos(deg), s = sin(deg);
	Vector3d v3p ( v3 * c + v1 * s );
	Vector3d v1p ( v1 * c - v3 * s );
	v1 = v1p;
	v3 = v3p;
}

void Base3d::rotateV3(double deg)
{
	deg = Utils::deg2rad(deg);
	double c = cos(deg), s = sin(deg);
	Vector3d v1p ( v1 * c + v2 * s );
	Vector3d v2p ( v2 * c - v1 * s );
	v2 = v2p;
	v1 = v1p;
}

Vector3d Base3d::getV1() const
{
	return v1;
}

Vector3d Base3d::getV2() const
{
	return v2;
}

Vector3d Base3d::getV3() const
{
	return v3;
}

Base3d Base3d::instanciateFromStream(const std::istream& is)
{
	return Base3d(Vector3d(1,0,0),Vector3d(0,1,0),Vector3d(0,0,1));
}

