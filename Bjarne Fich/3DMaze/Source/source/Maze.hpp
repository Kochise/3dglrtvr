#ifndef __Maze_HPP_
#define __Maze_HPP_

#include <gl\gl.h>
#include "Vector3d.hpp"

#define MAXSIZE 20000

class MAZE3D {
  private:
  	GLuint TexID[8];
    int data[MAXSIZE];
  public:
    MAZE3D();
    ~MAZE3D();

    int SizeX, SizeY, SizeZ, Walls, Seed, Style;
    int GetMaze(int x, int y, int z, int dir);
    void SetMaze(int x, int y, int z, int dir, int val);
    void BuildPlaneXY(int x, int y, int z);
    void BuildPlaneZY(int x, int y, int z);
    void BuildPlaneZX(int x, int y, int z);
    void MAZE3D::WormTunnel(int x, int y, int z, int dir);
    void GetEdges(int x, int y, int z, int dir, bool lines[4]);
    void DrawInterior(float x, float y, float z);
    void DrawWall(float x, float y, float z, int dir, bool lines[4]);

    void Load(char *filename);
    void Build();
    void Draw(float px, float py, float pz);
    void DrawGround();
    void DrawSky();
    void DrawEntry();
    void DrawExit();
    VECTOR3D Collision(VECTOR3D pos, VECTOR3D dir);
};

#endif // __Maze_HPP_
