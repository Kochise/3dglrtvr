#include <Math.h>

#include "Macros.hpp"
#include "Vector3d.hpp"

VECTOR3D::VECTOR3D() {
	x = y = z = 0.0f;
};

VECTOR3D::VECTOR3D(float xx, float yy, float zz) {
	x = xx;
	y = yy;
	z = zz;
};

VECTOR3D::VECTOR3D(const float *vv) {
	*this = vv;
};

VECTOR3D::VECTOR3D(const VECTOR3D &vv) {
	*this = vv;
};

VECTOR3D &VECTOR3D::operator=(const VECTOR3D &vv) {
	x = vv.x;
	y = vv.y;
	z = vv.z;
};

VECTOR3D &VECTOR3D::operator=(const float *vv) {
	x = *vv++;
	y = *vv++;
	z = *vv;
};

bool VECTOR3D::operator==(const VECTOR3D &vv) {
	return (x == vv.x) && (y == vv.y) && (z == vv.z);
};

bool VECTOR3D::operator!=(const VECTOR3D vv) {
	return !(*this == vv);
};

VECTOR3D VECTOR3D::operator-() {
	return VECTOR3D(-x, -y, -z);
};

VECTOR3D &VECTOR3D::operator+=(const VECTOR3D vv) {
	x += vv.x;
	y += vv.y;
	z += vv.z;
	return *this;
};

VECTOR3D VECTOR3D::operator+(const VECTOR3D vv) {
	return VECTOR3D(x+vv.x, y+vv.y, z+vv.z);
};

VECTOR3D &VECTOR3D::operator-=(const VECTOR3D vv) {
	x -= vv.x;
	y -= vv.y;
	z -= vv.z;
	return *this;
};

VECTOR3D VECTOR3D::operator-(const VECTOR3D vv) {
	return VECTOR3D(x-vv.x, y-vv.y, z-vv.z);
};

VECTOR3D &VECTOR3D::operator*=(const VECTOR3D vv) {
	x *= vv.x;
	y *= vv.y;
	z *= vv.z;
	return *this;
};

VECTOR3D VECTOR3D::operator*(const VECTOR3D vv) {
	return VECTOR3D(x*vv.x, y*vv.y, z*vv.z);
};

VECTOR3D &VECTOR3D::operator/=(const VECTOR3D vv) {
	x /= vv.x;
	y /= vv.y;
	z /= vv.z;
	return *this;
};

VECTOR3D VECTOR3D::operator/(const VECTOR3D vv) {
	return VECTOR3D(x/vv.x, y/vv.y, z/vv.z);
};

VECTOR3D &VECTOR3D::operator*=(float f) {
	x *= f;
	y *= f;
	z *= f;
	return *this;
};

VECTOR3D VECTOR3D::operator*(float f) {
	return VECTOR3D(x*f, y*f, z*f);
};

VECTOR3D &VECTOR3D::operator/=(float f) {
	x /= f;
	y /= f;
	z /= f;
	return *this;
};

VECTOR3D VECTOR3D::operator/(float f) {
	return VECTOR3D(x/f, y/f, z/f);
};

bool VECTOR3D::IsNull() {
	return (!x) && (!y) && (!z);
};

float VECTOR3D::GetLen() {
	return (float) sqrt(x*x + y*y + z*z);
};

void VECTOR3D::SetLen(float f) {
	*this *= f/GetLen();
};

VECTOR3D &VECTOR3D::Normalize() {
	float f = x*x + y*y + z*z;
	if (fabs(f-1.0) < TINY) return *this;
	f = 1.0 / (float)sqrt(f);
	*this *= f;
	return *this;
};

VECTOR3D VECTOR3D::CrossProduct(const VECTOR3D vv) {
	return VECTOR3D((y*vv.z)-(z*vv.y), (z*vv.x)-(x*vv.z), (x*vv.y)-(y*vv.x));
};

float VECTOR3D::DotProduct(const VECTOR3D vv) {
	return x*vv.x + y*vv.y + z*vv.z;
};

float VECTOR3D::AngleBetween(VECTOR3D vv) {
	return (DotProduct(vv) / (GetLen() * vv.GetLen()));
};

void VECTOR3D::RotateX(float deg_angle) {
	float a = deg_angle * PIdiv180;
	float c = cos(a);
	float s = sin(a);
	float ty = c*y + s*z;
	z = c*z - s*y;
	y = ty;
};

void VECTOR3D::RotateY(float deg_angle) {
	float a = deg_angle * PIdiv180;
	float c = cos(a);
	float s = sin(a);
	float tx = c*x + s*z;
	z = c*z - s*x;
	x = tx;
};

void VECTOR3D::RotateZ(float deg_angle) {
	float a = deg_angle * PIdiv180;
	float c = cos(a);
	float s = sin(a);
	float ty = c*y + s*x;
	x = c*x - s*y;
	y = ty;
};

void VECTOR3D::Rotate(float x, float y, float z) {
	RotateX(x);
	RotateY(y);
	RotateZ(z);
};

