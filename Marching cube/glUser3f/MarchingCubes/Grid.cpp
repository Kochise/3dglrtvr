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

#include "Grid.h"

#include <windows.h>
#include <gl/gl.h>

#include <cmath>
#include "Tables.h"

#define SQR(X) ((X)*(X))

Grid::Grid(int size, float isoLevel, float (*eval)(float, float, float)) {
	mSize     = size;
	mIsoLevel = isoLevel;
	mEval     = eval;
	allocateTables();
	calculateGrids();
	connectGrids();
}

Grid::~Grid(void) {
	deallocateTables();
}

int Grid::go(void) {
	mTriangles.clear();
	int triCount = 0;
	for (int i = 0; i < mSize; i++) {
		for (int j = 0; j < mSize; j++) {
			for (int k = 0; k < mSize; k++) {
				triCount += polygoniseAndInsert(&mGridCells[i][j][k], mIsoLevel);
			}
		}
	}
	return triCount;
}

void Grid::draw(void) {
	glBegin(GL_TRIANGLES);
		std::list<Triangle>::iterator i;
		for (i = mTriangles.begin(); i != mTriangles.end(); i++) {
			glNormal3fv((float*)&(*i).normal);
			for (int j = 0; j < 3; j++) {
				float color = SQR((*i).point[j].x) + SQR((*i).point[j].y) + SQR((*i).point[j].z);
				glColor3f(color + 0.1f, color, color + 0.2f);
				glVertex3fv((float*)&(*i).point[j]);
			}
		}	
	glEnd();
}

std::list<Triangle>& Grid::getTriangles(void) {
	return mTriangles;
}

int Grid::polygoniseAndInsert(const GridCell* grid, float isoLevel) {
	int cubeIndex = 0;
	if (grid->point[0]->val < isoLevel) cubeIndex |= 1;
	if (grid->point[1]->val < isoLevel) cubeIndex |= 2;
	if (grid->point[2]->val < isoLevel) cubeIndex |= 4;
	if (grid->point[3]->val < isoLevel) cubeIndex |= 8;
	if (grid->point[4]->val < isoLevel) cubeIndex |= 16;
	if (grid->point[5]->val < isoLevel) cubeIndex |= 32;
	if (grid->point[6]->val < isoLevel) cubeIndex |= 64;
	if (grid->point[7]->val < isoLevel) cubeIndex |= 128;

	if (EdgeTable[cubeIndex] == 0) {
		return 0;
	}

	Point vertices[12];
	if (EdgeTable[cubeIndex] & 1) {
		vertices[0] = interpolate(isoLevel, grid->point[0], grid->point[1]);
	}
	if (EdgeTable[cubeIndex] & 2) {
		vertices[1] = interpolate(isoLevel, grid->point[1], grid->point[2]);
	}
	if (EdgeTable[cubeIndex] & 4) {
		vertices[2] = interpolate(isoLevel, grid->point[2], grid->point[3]);
	}
	if (EdgeTable[cubeIndex] & 8) {
		vertices[3] = interpolate(isoLevel, grid->point[3], grid->point[0]);
	}
	if (EdgeTable[cubeIndex] & 16) {
		vertices[4] = interpolate(isoLevel, grid->point[4], grid->point[5]);
	}
	if (EdgeTable[cubeIndex] & 32) {
		vertices[5] = interpolate(isoLevel, grid->point[5], grid->point[6]);
	}
	if (EdgeTable[cubeIndex] & 64) {
		vertices[6] = interpolate(isoLevel, grid->point[6], grid->point[7]);
	}
	if (EdgeTable[cubeIndex] & 128) {
		vertices[7] = interpolate(isoLevel, grid->point[7], grid->point[4]);
	}
	if (EdgeTable[cubeIndex] & 256) {
		vertices[8] = interpolate(isoLevel, grid->point[0], grid->point[4]);
	}
	if (EdgeTable[cubeIndex] & 512) {
		vertices[9] = interpolate(isoLevel, grid->point[1], grid->point[5]);
	}
	if (EdgeTable[cubeIndex] & 1024) {
		vertices[10] = interpolate(isoLevel, grid->point[2], grid->point[6]);
	}
	if (EdgeTable[cubeIndex] & 2048) {
		vertices[11] = interpolate(isoLevel, grid->point[3], grid->point[7]);
	}

	int triCount = 0;
	for (int i = 0; TriTable[cubeIndex][i] != -1; i+=3) {
		Triangle tri;
		tri.point[0] = vertices[TriTable[cubeIndex][i  ]];
		tri.point[1] = vertices[TriTable[cubeIndex][i+1]];
		tri.point[2] = vertices[TriTable[cubeIndex][i+2]];
		calculateNormal(tri);
		mTriangles.push_back(tri);
		triCount++;
	}
	return triCount;

}

Point Grid::interpolate(float isoLevel, const GridPoint* gp1, const GridPoint* gp2) {
	float mu;
	Point p;
	mu = (isoLevel - gp1->val) / (gp2->val - gp1->val);
	p.x = gp1->x + mu * (gp2->x - gp1->x);
	p.y = gp1->y + mu * (gp2->y - gp1->y);
	p.z = gp1->z + mu * (gp2->z - gp1->z);
	return p;
}

void Grid::allocateTables(void) {
	if (mGridPoints || mGridCells) {
		deallocateTables();
	}
	mGridPoints = new GridPoint**[mSize + 1];
	for (int i = 0; i < mSize + 1; i++) {
		mGridPoints[i] = new GridPoint*[mSize + 1];
		for (int j = 0; j < mSize + 1; j++) {
			mGridPoints[i][j] = new GridPoint[mSize + 1];
		}
	}
	mGridCells  = new GridCell**[mSize];
	for (int i = 0; i < mSize; i++) {
		mGridCells[i] = new GridCell*[mSize];
		for (int j = 0; j < mSize; j++) {
			mGridCells[i][j] = new GridCell[mSize];
		}
	}
}

void Grid::deallocateTables(void) {
	if (mGridPoints) {
		for (int i = 0; i < mSize + 1; i++) {
			for (int j = 0; j < mSize + 1; j++) {
				delete [] mGridPoints[i][j];
			}
			delete [] mGridPoints[i];
		}
		delete [] mGridPoints;
		mGridPoints = NULL;
	}
	if (mGridCells) {
		for (int i = 0; i < mSize; i++) {
			for (int j = 0; j < mSize; j++) {
				delete [] mGridCells[i][j];
			}
			delete [] mGridCells[i];
		}
		delete [] mGridCells;
		mGridCells = NULL;
	}
}

void Grid::calculateGrids(void) {
	if (!mGridPoints || !mGridCells) {
		return;
	}
	for (int i = 0; i < mSize + 1; i++) {
		for (int j = 0; j < mSize + 1; j++) {
			for (int k = 0; k < mSize + 1; k++) {
				GridPoint* gridPoint = &mGridPoints[i][j][k];
				gridPoint->x = (float)(2 * i) / (float)(mSize) - 1.0f;
				gridPoint->y = (float)(2 * j) / (float)(mSize) - 1.0f;
				gridPoint->z = (float)(2 * k) / (float)(mSize) - 1.0f;
				gridPoint->val = mEval(gridPoint->x, gridPoint->y, gridPoint->z);
			}
		}
	}
}

void Grid::connectGrids(void) {
	if (!mGridPoints || !mGridCells) {
		return;
	}
	for (int i = 0; i < mSize; i++) {
		for (int j = 0; j < mSize; j++) {
			for (int k = 0; k < mSize; k++) {
				GridCell* gridCell = &mGridCells[i][j][k];
				gridCell->point[0] = &mGridPoints[i  ][j  ][k  ];
				gridCell->point[1] = &mGridPoints[i+1][j  ][k  ];
				gridCell->point[2] = &mGridPoints[i+1][j  ][k+1];
				gridCell->point[3] = &mGridPoints[i  ][j  ][k+1];
				gridCell->point[4] = &mGridPoints[i  ][j+1][k  ];
				gridCell->point[5] = &mGridPoints[i+1][j+1][k  ];
				gridCell->point[6] = &mGridPoints[i+1][j+1][k+1];
				gridCell->point[7] = &mGridPoints[i  ][j+1][k+1];
			}
		}
	}	
}

void Grid::calculateNormal(Triangle& tri) {
	float a[3], b[3];
	float length;
	a[0] = tri.point[0].x - tri.point[1].x;
	a[1] = tri.point[0].y - tri.point[1].y;
	a[2] = tri.point[0].z - tri.point[1].z;
	b[0] = tri.point[0].x - tri.point[2].x;
	b[1] = tri.point[0].y - tri.point[2].y;
	b[2] = tri.point[0].z - tri.point[2].z;
	tri.normal.x = a[1] * b[2] - b[1] * a[2];
	tri.normal.y = b[0] * a[2] - a[0] * b[2];
	tri.normal.z = a[0] * b[1] - b[0] * a[1];
	length = sqrt(tri.normal.x*tri.normal.x + tri.normal.y*tri.normal.y + tri.normal.z*tri.normal.z);
	tri.normal.x /= length;
	tri.normal.y /= length;
	tri.normal.z /= length;
}