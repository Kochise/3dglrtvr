/**
 *  Marching Cubes Demo, by glUser3f [gluser3f@gdnmail.net]
 *  Feel free to use this code in your own projects.
 *  If you do something cool with it, please email me so I can see it!
 *  
 *  Credits:
 *  Theory by Paul Bourke        [http://astronomy.swin.edu.au/~pbourke/modelling/polygonise/]
 *  OpenGL basecode by NeHe      [nehe.gamedev.net]
 *  Lookup Tables by Paul Bourke [http://astronomy.swin.edu.au/~pbourke/modelling/polygonise/]
 *  The rest is done by me, glUser3f ;)
 *
 */

#ifndef _GRID_H_

#define _GRID_H_

#include <list>

struct GridPoint {
	float x, y, z;
	float val;
};

struct Point {
	float x, y, z;
};

struct Triangle {
	Point point[3];
	Point normal;
};

struct GridCell {
	GridPoint* point[8];
};


class Grid {

public:
	Grid(int size, float isoLevel, float (*eval)(float, float, float));
	~Grid(void);

	int go(void);
	std::list<Triangle>& getTriangles(void);

	void calculateGrids(void);

	void draw(void);

private:
	int polygoniseAndInsert(const GridCell* grid, float isoLevel);
	Point interpolate(float isoLevel, const GridPoint* gp1, const GridPoint* gp2);

	void connectGrids(void);

	void allocateTables(void);
	void deallocateTables(void);

	void calculateNormal(Triangle& tri);

	int mSize;
	float mIsoLevel;
	GridPoint*** mGridPoints;
	GridCell***  mGridCells;
	float (*mEval)(float, float, float);
	std::list<Triangle> mTriangles;

};

#endif