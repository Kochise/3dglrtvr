#include <gl\gl.h>

#include "Macros.hpp"
#include "Position.hpp"

POSITION::POSITION() {
	Reset();
};

POSITION::~POSITION() {
};

void POSITION::Reset() {
	vector.x=0;
	vector.y=0;
	vector.z=0;
	rotation.Reset();
	rotation.Normalize();
};

void POSITION::Set(float px, float py, float pz, float turn, float tilt, float roll) {
  vector.x=px; vector.y=py; vector.z=pz;
  rotation.Reset();
  Turn(DEG_TO_RAD(turn));
  Tilt(DEG_TO_RAD(tilt));
  Roll(DEG_TO_RAD(roll));
}

void POSITION::Copy(POSITION *from) {
	vector.x=from->vector.x;
	vector.y=from->vector.y;
	vector.z=from->vector.z;
	rotation.x=from->rotation.x;
	rotation.y=from->rotation.y;
	rotation.z=from->rotation.z;
	rotation.w=from->rotation.w;
};

void POSITION::LoadAsCamera() {
  // Set camera to user pos
 	float axisX, axisY, axisZ, rotAngle;
  QUATERNION quat = rotation;
	quat.PostMult(QUATERNION(0,PI,0));
  glLoadIdentity();
  quat.GetAxisAngle(axisX, axisY, axisZ, rotAngle);
	glRotatef((float)(rotAngle*_180divPI), axisX, axisY, axisZ);
  glTranslatef(-vector.x, -vector.y, -vector.z);
}

void POSITION::Translate() {
  // Translate to current position...
 	float axisX, axisY, axisZ, rotAngle;
  glTranslatef(vector.x, vector.y, vector.z);
  rotation.GetAxisAngle(axisX, axisY, axisZ, rotAngle);
	glRotatef((float)(rotAngle*_180divPI), -axisX, -axisY, -axisZ);
}

void POSITION::Translate(POSITION *p) {
	rotation.PostMult(p->rotation);
	vector += p->vector;
}

void POSITION::Turn(float angle) {
  // Heading, pitch, roll
	rotation.PostMult(QUATERNION(0,angle,0));
}

void POSITION::Tilt(float angle) {
  // Heading, pitch, roll
	rotation.PostMult(QUATERNION(angle,0,0));
}

void POSITION::Roll(float angle) {
  // Heading, pitch, roll
	rotation.PostMult(QUATERNION(0,0,angle));
}

void POSITION::Move(float speed) {
  VECTOR3D forward;
	rotation.GetDirectionVector(&forward);
  vector.x += forward.x * speed;
  vector.y += forward.y * speed;
  vector.z += forward.z * speed;
}

void MOVEMENT::Level(float speed) {
	VECTOR3D forward, right;
	Pos.rotation.GetDirectionVector(&forward);
	forward.Normalize();
	Pos.rotation.GetRightVector(&right);
	right.Normalize();
	if (forward.y < -speed) Tilting += forward.y*forward.y;
	if (forward.y > speed)  Tilting -= forward.y*forward.y;
	if (right.y < -speed) Rolling -= right.y*right.y;
	if (right.y > speed)  Rolling += right.y*right.y;
	Tilting *= 0.9;
	Rolling *= 0.9;
};

