#ifndef __POSITION_HPP_
#define __POSITION_HPP_

#include "Vector3d.hpp"
#include "Quaternion.hpp"

class POSITION {
  public:
    VECTOR3D vector;
    QUATERNION rotation;

    POSITION();
    ~POSITION();

    void Reset();
    void Set(float px, float py, float pz, float turn, float tilt, float roll);
    void Copy(POSITION *from);
    void LoadAsCamera();
    void Translate();
    void Translate(POSITION *p);
    void Turn(float angle);
    void Tilt(float angle);
    void Roll(float angle);
    void Move(float speed);
};

class MOVEMENT {
  public:
    POSITION Pos;
    float Turning, Tilting, Rolling, Moveing, Falling;
    MOVEMENT() { Turning=0; Tilting=0; Rolling=0; Moveing=0; Falling=0; };
    ~MOVEMENT() {};

    void Update() { Pos.vector.y-=Falling; Pos.Turn(Turning); Pos.Tilt(Tilting); Pos.Roll(Rolling); Pos.Move(Moveing); };
    void LoadAsCamera() { Pos.LoadAsCamera(); };
    void Translate() { Pos.Translate(); };
    void Level(float speed);

};

#endif // __POSITION_HPP_

