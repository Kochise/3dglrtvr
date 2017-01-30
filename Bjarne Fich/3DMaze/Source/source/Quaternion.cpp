#include <Math.h>
#include <memory.h>

#include "Macros.hpp"
#include "Quaternion.hpp"

QUATERNION::QUATERNION(void) {
  Reset();
}

QUATERNION::QUATERNION(const QUATERNION &copyFrom) {
  Set(copyFrom);
}

QUATERNION::QUATERNION(float xT, float yT, float zT) {
  Set(xT, yT, zT);
}

QUATERNION::QUATERNION(float angle, float xT, float yT, float zT) {
  Set(angle, xT, yT, zT);
}

QUATERNION &QUATERNION::Reset(void) {
  q[0] = 1.0f;
	q[1] = 0.0f;
	q[2] = 0.0f;
	q[3] = 0.0f;
	return *this;
}

QUATERNION &QUATERNION::Set(const QUATERNION &copyFrom) {
	q[0] = copyFrom.q[0];
	q[1] = copyFrom.q[1];
	q[2] = copyFrom.q[2];
	q[3] = copyFrom.q[3];
	return *this;
}

QUATERNION &QUATERNION::Set(float xT, float yT, float zT) {
	QUATERNION xQ(xT, 1.0f, 0.0f, 0.0f);
	QUATERNION yQ(yT, 0.0f, 1.0f, 0.0f);
	QUATERNION zQ(zT, 0.0f, 0.0f, 1.0f);
	Set(xQ);
	PostMult(yQ);
	PostMult(zQ);
	return *this;
}

QUATERNION &QUATERNION::Set(float angle, float xT, float yT, float zT) {
	float factor = xT*xT+yT*yT+zT*zT;
	if(!factor)
		factor = (float) TINY;
	float scaleBy = (float) (1.0/sqrt(factor));
	q[0] = (float) cos(angle/2.0f);
	float sinHalfAngle = (float) sin(angle/2.0f);
	q[1] = xT*scaleBy*sinHalfAngle;
	q[2] = yT*scaleBy*sinHalfAngle;
	q[3] = zT*scaleBy*sinHalfAngle;
	return *this;
}

QUATERNION &QUATERNION::PostMult(const QUATERNION &quat) {
	QUATERNION tempQ(*this);
	MultAndSet(tempQ, quat);
	return *this;
}

QUATERNION &QUATERNION::MultAndSet(const QUATERNION &quat1, const QUATERNION &quat2) {
  q[0] = quat2.q[0]*quat1.q[0]
		   -quat2.q[1]*quat1.q[1]
		   -quat2.q[2]*quat1.q[2]
		   -quat2.q[3]*quat1.q[3];

	q[1] = quat2.q[0]*quat1.q[1]
		   +quat2.q[1]*quat1.q[0]
		   +quat2.q[2]*quat1.q[3]
		   -quat2.q[3]*quat1.q[2];

	q[2] = quat2.q[0]*quat1.q[2]
		   -quat2.q[1]*quat1.q[3]
		   +quat2.q[2]*quat1.q[0]
		   +quat2.q[3]*quat1.q[1];

	q[3] = quat2.q[0]*quat1.q[3]
		   +quat2.q[1]*quat1.q[2]
		   -quat2.q[2]*quat1.q[1]
		   +quat2.q[3]*quat1.q[0];
	return *this;
}

QUATERNION &QUATERNION::Normalize(void) {
	float factor = q[0]*q[0]+
				   q[1]*q[1]+
				   q[2]*q[2]+
				   q[3]*q[3];
	float scaleBy = (float) (1.0/sqrt(factor));
	q[0] = q[0]*scaleBy;
	q[1] = q[1]*scaleBy;
	q[2] = q[2]*scaleBy;
	q[3] = q[3]*scaleBy;
	return *this;
}

QUATERNION QUATERNION::Inversed(void) {
	QUATERNION qT;
	qT.q[1] = -q[1];
	qT.q[2] = -q[2];
	qT.q[3] = -q[3];
	return qT;
}

float QUATERNION::DotProduct(void) {
  return x*x+y*y+z*z+w*w;
}

float QUATERNION::DotProduct(QUATERNION qT) {
  return x*qT.x+y*qT.y+z*qT.z+w*qT.w;
}

float QUATERNION::GetLength(void) {
  return (float) sqrt(x*x+y*y+z*z+w*w);
}

void QUATERNION::GetMatrix(MATRIX4X4 *mT) {
	Normalize();
	float xx = x*x;
	float yy = y*y;
	float zz = z*z;

	mT->xx = 1.0f-2.0f*(yy+zz);
	mT->yx = 2.0f*(x*y+w*z);
	mT->zx = 2.0f*(x*z-w*y);
	mT->wx = 0.0f;

	mT->xy = 2.0f*(x*y-w*z);
	mT->yy = 1.0f-2.0f*(xx+zz);
	mT->zy = 2.0f*(y*z+w*x);
	mT->wy= 0.0f;

	mT->xz = 2.0f*(x*z+w*y);
	mT->yz = 2.0f*(y*z-w*x);
	mT->zz = 1.0f-2.0f*(xx+yy);
	mT->wz = 0.0f;

	mT->xw = 0.0f;
	mT->yw = 0.0f;
	mT->zw = 0.0f;
	mT->ww = 1.0f;
}

void QUATERNION::GetInvertedMatrix(MATRIX4X4 *mT) {
	Normalize();
	float xx = x*x;
	float yy = y*y;
	float zz = z*z;

	mT->xx = -(1.0f-2.0f*(yy+zz));
	mT->yx = -(2.0f*(x*y+w*z));
	mT->zx = -(2.0f*(x*z-w*y));
	mT->wx = 0.0f;

	mT->xy = 2.0f*(x*y-w*z);
	mT->yy = 1.0f-2.0f*(xx+zz);
	mT->yz = 2.0f*(y*z+w*x);
	mT->yw = 0.0f;

	mT->zx = 2.0f*(x*z+w*y);
	mT->zy = 2.0f*(y*z-w*x);
	mT->zz = 1.0f-2.0f*(xx+yy);
	mT->zw = 0.0f;

	mT->wx = 0.0f;
	mT->wy = 0.0f;
	mT->wz = 0.0f;
	mT->ww = 1.0f;
}

void QUATERNION::SetMatrix(MATRIX4X4 mT) {
	float tr, s, q[4];
  int i, j, k;
	int nxt[3] = {1, 2, 0};

	tr = mT.xx+mT.yy+mT.zz;

	//Check the diagonal
	if(tr > 0.0f) {
		s = (float) sqrt(tr+1.0f);
		w = s/2.0f;
		s = 0.5f/s;
		x = (mT.zy-mT.yz)*s;
		y = (mT.xz-mT.zx)*s;
		z = (mT.yx-mT.xy)*s;
  }	else {
		//Diagonal is negative
		i = 0;
		if(mT.yy > mT.xx)
			i = 1;
		if(mT.zz > mT.m44[i][i])
			i = 2;
		
		j = nxt[i];
		k = nxt[j];

		s = (float) sqrt((mT.m44[i][i]-(mT.m44[j][j]+mT.m44[k][k]))+1.0f);

		q[i] = s*0.5f;

		if(s) s = 0.5f/s;

		q[3] = (mT.m44[j][k] - mT.m44[k][j])*s;
		q[j] = (mT.m44[i][j] + mT.m44[j][i])*s;
		q[k] = (mT.m44[i][k] + mT.m44[k][i])*s;

		x = q[0];
		y = q[1];
		z = q[2];
		w = q[3];
	}
}

void QUATERNION::GetAxisAngle(float &axisX, float &axisY, float &axisZ, float &rotAngle) {
	float lenOfVector = q[1]*q[1]+q[2]*q[2]+q[3]*q[3];

	if(lenOfVector < TINY) {
		axisX = 1.0f;
		axisY = 0.0f;
		axisZ = 0.0f;
		rotAngle = 0.0f;
	}	else {
		float invLen = (float) (1.0/sqrt(lenOfVector));
		axisX = q[1]*invLen;
		axisY = q[2]*invLen;
		axisZ = q[3]*invLen;
		rotAngle = (float) (2.0f*acos(q[0]));
	}
}

void QUATERNION::GetDirectionVector(VECTOR3D *dir) {
	Normalize();
	(*dir).x = 2.0f*(x*z-y*w);
	(*dir).y = 2.0f*(y*z+x*w);
	(*dir).z = 1.0f-2.0f*(x*x+y*y);
}

void QUATERNION::GetRightVector(VECTOR3D *dir) {
	Normalize();
	(*dir).x = 1.0f-2.0f*(y*y+z*z);
	(*dir).y = 2.0f*(x*y-z*w);
	(*dir).z = 2.0f*(x*z+y*w);
}

void QUATERNION::GetUpVector(VECTOR3D *dir) {
	Normalize();
  (*dir).x = 2.0f*(x*y+z*w);
	(*dir).y = 1.0f-2.0f*(x*x+z*z);
	(*dir).z = 2.0f*(y*z-x*w);
}

void QUATERNION::EulerToQuat(const float exT, const float eyT, const float ezT) {
	double ex, ey, ez; //Temp half euler angles
	double cr, cp, cy, sr, sp, sy, cpcy, spsy; //Temp vars in roll, pitch and yaw

	//Convert to rads and half them
	ex = PIdiv180*exT/2.0f;
	ey = PIdiv180*eyT/2.0f;
	ez = PIdiv180*ezT/2.0f;

	cr = cos(ex);
	cp = cos(ey);
	cy = cos(ez);

	sr = sin(ex);
	sp = sin(ey);
	sy = sin(ez);

	cpcy = cp*cy;
	spsy = sp*sy;

	w = float(cr*cpcy + sr*spsy);

	x = float(sr*cpcy - cr*spsy);
	y = float(cr*sp*sy + sr*cp*sy);
	z = float(cr*cp*sy - sr*sp*cy);

	Normalize();
}

void QUATERNION::GetEulerAngles(float &xT, float &yT, float &zT) {
	MATRIX4X4 mT;
	GetMatrix(&mT);
	mT.GetEulerAngles(xT, yT, zT);
}

//	fTime = -2 * (x*x*x) + 3 * (x*x);
//	t = 0.5 - 0.5 * cos(sin(x*PI)*PI);
// Computes spherical linear interpolation between q1 and q2 with time 0-1
void QUATERNION::Slerp(const QUATERNION q1, const QUATERNION q2, const float fTime) {
	float omega, cosom, sinom, scale0, scale1;
	float from[4], to[4], to1[4];

	memcpy(&from, q1.q, sizeof(float)*4);
	memcpy(&to, q2.q, sizeof(float)*4);

	enum {W, X, Y, Z};

	//Calc cosine
	cosom = from[X]*to[X] + from[Y]*to[Y] + from[Z]*to[Z] + from[W]*to[W];

	//Adjust signs (if necessary)
	if(cosom < 0.0f) {
    cosom = -cosom;
    to1[X] = -to[X];
    to1[Y] = -to[Y];
    to1[Z] = -to[Z];
    to1[W] = -to[W];
	} else {
    to1[X] = to[X];
    to1[Y] = to[Y];
    to1[Z] = to[Z];
    to1[W] = to[W];
  }

	//Calculate coefficients
	if((1.0f-cosom) > TINY) {
		//Standard case (slerp)
    omega = (float) acos(cosom);
    sinom = (float) sin(omega);
    scale0 = (float) sin((1.0f-fTime)*omega)/sinom;
    scale1 = (float) sin(fTime*omega)/sinom;
  } else {
		//"from" and "to" quaternions are very close 
		//... so we can do a linear interpolation
    scale0 = 1.0f-fTime;
    scale1 = fTime;
  }

	//Calculate final values
	q[X] = scale0*from[X] + scale1*to1[X];
	q[Y] = scale0*from[Y] + scale1*to1[Y];
	q[Z] = scale0*from[Z] + scale1*to1[Z];
	q[W] = scale0*from[W] + scale1*to1[W];
}

void QUATERNION::SquadSlerp(QUATERNION aT, QUATERNION pT, QUATERNION qT, QUATERNION bT, float t) {
	QUATERNION ab;
	QUATERNION pq;

	ab.Slerp(aT, bT, t);
	pq.Slerp(pT, qT, t);
	Slerp(ab, pq, 2*t*(1-t));
}

void QUATERNION::Invert(void) {
	float l, m;

	l = GetLength();
	if(fabs(l) < TINY) {
    x = y = z = 0.0f;
    w = 1.0f;
  } else {
    m = 1.0f/l;
    x = -x*m;
    y = -y*m;
    z = -z*m;
    w = w*m;
  }
}

void QUATERNION::Mult(QUATERNION a, QUATERNION b) {
  x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
  y = a.w*b.y + a.y*b.w + a.z*b.x - a.x*b.z;
  z = a.w*b.z + a.z*b.w + a.x*b.y - a.y*b.x;
  w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
}

void QUATERNION::Ln(void) {
	float om, s, t;

	s = (float) sqrt(x*x + y*y + z*z);
	om = (float) atan2(s, w);
	if(fabs(s) < TINY)
		t = 0.0f;
	else
		t = om/s;
	for(int i = 1; i < 4; ++i) q[i] = q[i]*t;
	w = 0.0f;
}

void QUATERNION::LnDif(QUATERNION a, QUATERNION b) {
	QUATERNION invp;
	for(int i = 0; i < 4; i++) invp.q[i] = a.q[i];
	invp.Invert();
	Mult(invp, b);
	Ln();
}

void QUATERNION::Exp(void) {
	float om, sinom;
	om = (float) sqrt(x*x + y*y + z*z);
	if(fabs(om) < TINY)
		sinom = 1.0f;
	else
		sinom = (float) sin(om)/om;
	for(int i = 1; i < 4; ++i) q[i] = q[i]*sinom;
	w = (float) cos(om);
}

void QUATERNION::InnerPoint(QUATERNION pT, QUATERNION qT, QUATERNION nT) {
	QUATERNION dnT, dpT, xT;

	dnT.LnDif(qT, nT);
	dpT.LnDif(qT, pT);
	for(int i = 0; i < 4; i++) xT.q[i] = -1.0f/4.0f*(dnT.q[i]+dpT.q[i]);
	xT.Exp();
	Mult(qT, xT);
}

