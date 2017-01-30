#include <Math.h>

#include "Macros.hpp"
#include "Matrix4x4.hpp"

MATRIX4X4::MATRIX4X4(void) {
	LoadIdentity();
};

MATRIX4X4::MATRIX4X4(const float *f) {
	*this = f;
};

MATRIX4X4::MATRIX4X4(const MATRIX4X4 &mT) {
	*this = mT;
};

MATRIX4X4 &MATRIX4X4::operator=(const MATRIX4X4 mT) {
	xx = mT.xx; xy = mT.xy; xz = mT.xz; xw = mT.xw;
	yx = mT.yx; yy = mT.yy; yz = mT.yz; yw = mT.yw;
	zx = mT.zx; zy = mT.zy; zz = mT.zz; zw = mT.zw;
	wx = mT.wx; wy = mT.wy; wz = mT.wz; ww = mT.ww;
	return *this;
};

MATRIX4X4 MATRIX4X4::operator*(const MATRIX4X4 m2) {
	MATRIX4X4 mT;
	mT.m[0] = m[0]*m2.m[0] + m[4]*m2.m[1] + m[8]*m2.m[2] + m[12]*m2.m[3];
	mT.m[1] = m[1]*m2.m[0] + m[5]*m2.m[1] + m[9]*m2.m[2] + m[13]*m2.m[3];
	mT.m[2] = m[2]*m2.m[0] + m[6]*m2.m[1] + m[10]*m2.m[2] + m[14]*m2.m[3];
	mT.m[3] = m[3]*m2.m[0] + m[7]*m2.m[1] + m[11]*m2.m[2] + m[15]*m2.m[3];
	mT.m[4] = m[0]*m2.m[4] + m[4]*m2.m[5] + m[8]*m2.m[6] + m[12]*m2.m[7];
	mT.m[5] = m[1]*m2.m[4] + m[5]*m2.m[5] + m[9]*m2.m[6] + m[13]*m2.m[7];
	mT.m[6] = m[2]*m2.m[4] + m[6]*m2.m[5] + m[10]*m2.m[6] + m[14]*m2.m[7];
	mT.m[7] = m[3]*m2.m[4] + m[7]*m2.m[5] + m[11]*m2.m[6] + m[15]*m2.m[7];
	mT.m[8] = m[0]*m2.m[8] + m[4]*m2.m[9] + m[8]*m2.m[10] + m[12]*m2.m[11];
	mT.m[9] = m[1]*m2.m[8] + m[5]*m2.m[9] + m[9]*m2.m[10] + m[13]*m2.m[11];
	mT.m[10] = m[2]*m2.m[8] + m[6]*m2.m[9] + m[10]*m2.m[10] + m[14]*m2.m[11];
	mT.m[11] = m[3]*m2.m[8] + m[7]*m2.m[9] + m[11]*m2.m[10] + m[15]*m2.m[11];
	mT.m[12] = m[0]*m2.m[12] + m[4]*m2.m[13] + m[8]*m2.m[14] + m[12]*m2.m[15];
	mT.m[13] = m[1]*m2.m[12] + m[5]*m2.m[13] + m[9]*m2.m[14] + m[13]*m2.m[15];
	mT.m[14] = m[2]*m2.m[12] + m[6]*m2.m[13] + m[10]*m2.m[14] + m[14]*m2.m[15];
	mT.m[15] = m[3]*m2.m[12] + m[7]*m2.m[13] + m[11]*m2.m[14] + m[15]*m2.m[15];
	return mT;
};

MATRIX4X4 &MATRIX4X4::operator*=(const MATRIX4X4 mm) {
	m[0] = m[0]*mm.m[0] + m[4]*mm.m[1] + m[8]*mm.m[2] + m[12]*mm.m[3];
	m[1] = m[1]*mm.m[0] + m[5]*mm.m[1] + m[9]*mm.m[2] + m[13]*mm.m[3];
	m[2] = m[2]*mm.m[0] + m[6]*mm.m[1] + m[10]*mm.m[2] + m[14]*mm.m[3];
	m[3] = m[3]*mm.m[0] + m[7]*mm.m[1] + m[11]*mm.m[2] + m[15]*mm.m[3];
	m[4] = m[0]*mm.m[4] + m[4]*mm.m[5] + m[8]*mm.m[6] + m[12]*mm.m[7];
	m[5] = m[1]*mm.m[4] + m[5]*mm.m[5] + m[9]*mm.m[6] + m[13]*mm.m[7];
	m[6] = m[2]*mm.m[4] + m[6]*mm.m[5] + m[10]*mm.m[6] + m[14]*mm.m[7];
	m[7] = m[3]*mm.m[4] + m[7]*mm.m[5] + m[11]*mm.m[6] + m[15]*mm.m[7];
	m[8] = m[0]*mm.m[8] + m[4]*mm.m[9] + m[8]*mm.m[10] + m[12]*mm.m[11];
	m[9] = m[1]*mm.m[8] + m[5]*mm.m[9] + m[9]*mm.m[10] + m[13]*mm.m[11];
	m[10] = m[2]*mm.m[8] + m[6]*mm.m[9] + m[10]*mm.m[10] + m[14]*mm.m[11];
	m[11] = m[3]*mm.m[8] + m[7]*mm.m[9] + m[11]*mm.m[10] + m[15]*mm.m[11];
	m[12] = m[0]*mm.m[12] + m[4]*mm.m[13] + m[8]*mm.m[14] + m[12]*mm.m[15];
	m[13] = m[1]*mm.m[12] + m[5]*mm.m[13] + m[9]*mm.m[14] + m[13]*mm.m[15];
	m[14] = m[2]*mm.m[12] + m[6]*mm.m[13] + m[10]*mm.m[14] + m[14]*mm.m[15];
	m[15] = m[3]*mm.m[12] + m[7]*mm.m[13] + m[11]*mm.m[14] + m[15]*mm.m[15];
	return *this;
};

VECTOR3D MATRIX4X4::operator*(const VECTOR3D vT) {
	return VECTOR3D(vT.x*xx + vT.y*xy + vT.z*xz+xw, vT.x*yx + vT.y*yy + vT.z*yz+yw, vT.x*zx + vT.y*zy + vT.z*zz+zw);
};

MATRIX4X4 MATRIX4X4::operator*(const float scalar) {
	int t;
	MATRIX4X4 mT;
	for (t=0; t<16; t++) mT.m[t]= m[t]*scalar;
	return mT;
}

MATRIX4X4 MATRIX4X4::operator+(const MATRIX4X4 &mm) {
	int t;
	MATRIX4X4 mT;
	for (t=0; t<16; t++) mT.m[t]= m[t] + mm.m[t];
	return mT;
}

MATRIX4X4 MATRIX4X4::operator-(const MATRIX4X4 &mm) {
	int t;
	MATRIX4X4 mT;
	for (t=0; t<16; t++) mT.m[t]= m[t] - mm.m[t];
	return mT;
}

MATRIX4X4 MATRIX4X4::operator*(const MATRIX4X4 &mm) {
	MATRIX4X4 mT;
	mT.m[0] = (m[0]*mm.m[0]) + (m[4]*mm.m[1]) + (m[8]*mm.m[2]) + (m[12]*mm.m[3]);
	mT.m[4] = (m[0]*mm.m[4]) + (m[4]*mm.m[5]) + (m[8]*mm.m[6]) + (m[12]*mm.m[7]);
	mT.m[8] = (m[0]*mm.m[8]) + (m[4]*mm.m[9]) + (m[8]*mm.m[10]) + (m[12]*mm.m[11]);
	mT.m[12] = (m[0]*mm.m[12]) + (m[4]*mm.m[13]) + (m[8]*mm.m[14]) + (m[12]*mm.m[15]);
	mT.m[1] = (m[1]*mm.m[0]) + (m[5]*mm.m[1]) + (m[9]*mm.m[2]) + (m[13]*mm.m[3]);
	mT.m[5] = (m[1]*mm.m[4]) + (m[5]*mm.m[5]) + (m[9]*mm.m[6]) + (m[13]*mm.m[7]);
	mT.m[9] = (m[1]*mm.m[8]) + (m[5]*mm.m[9]) + (m[9]*mm.m[10]) + (m[13]*mm.m[11]);
	mT.m[13] = (m[1]*mm.m[12]) + (m[5]*mm.m[13]) + (m[9]*mm.m[14]) + (m[13]*mm.m[15]);
	mT.m[2] = (m[2]*mm.m[0]) + (m[6]*mm.m[1]) + (m[10]*mm.m[2]) + (m[14]*mm.m[3]);
	mT.m[6] = (m[2]*mm.m[4]) + (m[6]*mm.m[5]) + (m[10]*mm.m[6]) + (m[14]*mm.m[7]);
	mT.m[10] = (m[2]*mm.m[8]) + (m[6]*mm.m[9]) + (m[10]*mm.m[10]) + (m[14]*mm.m[11]);
	mT.m[14] = (m[2]*mm.m[12]) + (m[6]*mm.m[13]) + (m[10]*mm.m[14]) + (m[14]*mm.m[15]);
	mT.m[3] = (m[3]*mm.m[0]) + (m[7]*mm.m[1]) + (m[11]*mm.m[2]) + (m[15]*mm.m[3]);
	mT.m[7] = (m[3]*mm.m[4]) + (m[7]*mm.m[5]) + (m[11]*mm.m[6]) + (m[15]*mm.m[7]);
	mT.m[11] = (m[3]*mm.m[8]) + (m[7]*mm.m[9]) + (m[11]*mm.m[10]) + (m[15]*mm.m[11]);
	mT.m[15] = (m[3]*mm.m[12]) + (m[7]*mm.m[13]) + (m[11]*mm.m[14]) + (m[15]*mm.m[15]);
	return mT;
}

void MATRIX4X4::LoadIdentity(void) {
	xx = 1.0f; xy = 0.0f; xz = 0.0f; xw = 0.0f;
	yx = 0.0f; yy = 1.0f; yz = 0.0f; yw = 0.0f;
	zx = 0.0f; zy = 0.0f; zz = 1.0f; zw = 0.0f;
	wx = 0.0f; wy = 0.0f; wz = 0.0f; ww = 1.0f;
};

void MATRIX4X4::LoadRotateX(const float angle) {
	const float c = (float) cos(angle);
	const float s = (float) sin(angle);
	yy = c;
	yz = s;
	zy = -s;
	zz = c;
};

void MATRIX4X4::LoadRotateY(const float angle) {
	const float c = (float) cos(angle);
	const float s = (float) sin(angle);
	xx = c;
	xz = -s;
	zx = s;
	zz = c;
};

void MATRIX4X4::LoadRotateZ(const float angle) {
	const float c = (float) cos(angle);
	const float s = (float) sin(angle);

	xx = c;
	xy = s;
	yx = -s;
	yy = c;
};

void MATRIX4X4::LoadScale(const float xT, const float yT, const float zT) {
	xx = xT;
	yy = yT;
	zz = zT;
};

void MATRIX4X4::LoadScale(const VECTOR3D vT) {
	LoadScale(vT.x, vT.y, vT.z);
};

void MATRIX4X4::LoadScale(const float *vT) {
	LoadScale(*vT++, *vT++, *vT);
};

inline void MATRIX4X4::Transpose(void) {
	FSWAP(xy, yx);
	FSWAP(xz, zx);
	FSWAP(xw, wx);
	FSWAP(yz, zy);
	FSWAP(yw, wy);
	FSWAP(zw, wz);
};

void MATRIX4X4::Translate(const float xT, const float yT, const float zT) {
	wx += xx*xT + yx*yT + zx*zT;
	wy += xy*xT + yy*yT + zy*zT;
	wz += xz*xT + yz*yT + zz*zT;
	ww += xw*xT + yw*yT + zw*zT;
};

void MATRIX4X4::Translate(const VECTOR3D vT) {
	Translate(vT.x, vT.y, vT.z);
};

void MATRIX4X4::Translate(const float *vT) {
	Translate(*vT++, *vT++, *vT);
};

void MATRIX4X4::GetEulerAngles(float &turn, float &tilt, float &roll) {
	double angleX, angleY, angleZ;
	angleY = atan2(zx, sqrt(zy*zy+zz*zz));
	double cosangleY = cos(angleY);
	if(fabs(cosangleY) > TINY) {
		angleZ = atan2(-yx/cosangleY, xx/cosangleY);
		angleX = atan2(-zy/cosangleY, zz/cosangleY);
	} else {
		if(fabs(PIdiv2-angleY) < TINY) {
			angleX = atan2(xy , yy);
			angleY = PIdiv2;
			angleZ = 0.0;
		} else {
			angleX = atan2(-xy, yy);
			angleY = -PIdiv2;
			angleZ = 0.0;
		}
	}
	turn = (float) (-angleX*_180divPI);
	tilt = (float) (-angleY*_180divPI);
	roll = (float) (-angleZ*_180divPI);
};


