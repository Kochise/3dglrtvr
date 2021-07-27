#include <cstdlib>

#include "Globals.hpp"
#include "Maze.hpp"
#include "Macros.hpp"
#include "FileProcs.hpp"
#include "VectorText.hpp"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define FRONT 5
#define REAR 6
#define INTERIOR 7

#define WALL_UNDEFINED 0
#define WALL_SOLID 1
#define WALL_HOLE 2

#define INTERIOR_UNDEFINED 0
#define INTERIOR_BOX 1
#define INTERIOR_FLOOD 2

#define WALL_STYLE 0
#define WORM_STYLE 1

#define INDEX(x,y,z) ((x) + (y)*SizeX + (z)*SizeX*SizeY)

int MAZE3D::GetMaze(int x, int y, int z, int dir) {
	int res = 0;
	if ((x>=0) && (x<SizeX) && (y>=0) && (y<SizeY) && (z>=0) && (z<SizeZ)) {
		switch (dir) {
			case UP:    res = data[INDEX(x,y+1,z)]&7; break;
			case DOWN:  res = data[INDEX(x,y,z)]&7; break;
			case LEFT:  res = (data[INDEX(x,y,z)]>>3)&7; break;
			case RIGHT: res = (data[INDEX(x+1,y,z)]>>3)&7; break;
			case FRONT: res = (data[INDEX(x,y,z+1)]>>6)&7; break;
			case REAR:  res = (data[INDEX(x,y,z)]>>6)&7; break;
			case INTERIOR:  res = (data[INDEX(x,y,z)]>>9)&7; break;
		}
	}
	return res;
};

void MAZE3D::SetMaze(int x, int y, int z, int dir, int val) {
	int mask1 = 7;
	int mask2 = 7<<3;
	int mask3 = 7<<6;
	int mask4 = 7<<9;
	val &= 7;
	if ((x>=0) && (x<SizeX) && (y>=0) && (y<SizeY) && (z>=0) && (z<SizeZ)) {
		switch (dir) {
			case UP:    data[INDEX(x,y+1,z)] = (data[INDEX(x,y+1,z)]&(mask2+mask3+mask4)) | val; break;
			case DOWN:  data[INDEX(x,y,z)] = (data[INDEX(x,y,z)]&(mask2+mask3+mask4)) | val; break;
			case LEFT:  data[INDEX(x,y,z)] = (data[INDEX(x,y,z)]&(mask1+mask3+mask4)) | (val<<3); break;
			case RIGHT: data[INDEX(x+1,y,z)] = (data[INDEX(x+1,y,z)]&(mask1+mask3+mask4)) | (val<<3); break;
			case FRONT: data[INDEX(x,y,z+1)] = (data[INDEX(x,y,z+1)]&(mask1+mask2+mask4)) | (val<<6); break;
			case REAR:  data[INDEX(x,y,z)] = (data[INDEX(x,y,z)]&(mask1+mask2+mask4)) | (val<<6); break;
			case INTERIOR:  data[INDEX(x,y,z)] = (data[INDEX(x,y,z)]&(mask1+mask2+mask3)) | (val<<9); break;
		}
	}
};

void MAZE3D::BuildPlaneXY(int x, int y, int z) {
	int xx,yy,xmax,xmin,ymax,ymin;

	SET_RANGE(x,1,SizeX-2);
	SET_RANGE(y,1,SizeY-2);
	SET_RANGE(z,1,SizeZ-2);

	if (GetMaze(x,y,z,REAR)==WALL_UNDEFINED) {
		for (xmax=x; (GetMaze(xmax,y,z,RIGHT)==0) && (xmax<SizeX-2); xmax++);
		for (xmin=x; (GetMaze(xmin,y,z,LEFT)==0) && (xmin>0); xmin--);
		for (ymax=y; (GetMaze(x,ymax,z,UP)==0) && (ymax<SizeY-2); ymax++);
		for (ymin=y; (GetMaze(x,ymin,z,DOWN)==0) && (ymin>0); ymin--);

		for (xx=xmin; xx<=xmax; xx++)
			for (yy=ymin; yy<=ymax; yy++) {
				if ((yy!=y) || (xx!=x)) {
					SetMaze(xx,yy,z,REAR,WALL_SOLID);
				} else {
					SetMaze(xx,yy,z,REAR,WALL_HOLE);
				}
			}
  }
};

void MAZE3D::BuildPlaneZY(int x, int y, int z) {
	int yy,zz,ymax,ymin,zmax,zmin;

	SET_RANGE(x,1,SizeX-2);
	SET_RANGE(y,1,SizeY-2);
	SET_RANGE(z,1,SizeZ-2);

	if (GetMaze(x,y,z,LEFT)==WALL_UNDEFINED) {
		for (ymax=y; (GetMaze(x,ymax,z,UP)==0) && (ymax<SizeY-2); ymax++);
		for (ymin=y; (GetMaze(x,ymin,z,DOWN)==0) && (ymin>0); ymin--);
		for (zmax=z; (GetMaze(x,y,zmax,FRONT)==0) && (zmax<SizeZ-2); zmax++);
		for (zmin=z; (GetMaze(x,y,zmin,REAR)==0) && (zmin>0); zmin--);

		for (yy=ymin; yy<=ymax; yy++)
			for (zz=zmin; zz<=zmax; zz++) {
				if ((zz!=z) || (yy!=y)) {
					SetMaze(x,yy,zz,LEFT,WALL_SOLID);
				} else {
					SetMaze(x,yy,zz,LEFT,WALL_HOLE);
				}
			}
  }
};

void MAZE3D::BuildPlaneZX(int x, int y, int z) {
	int xx,zz,xmax,xmin,zmax,zmin;

	SET_RANGE(x,1,SizeX-2);
	SET_RANGE(y,1,SizeY-2);
	SET_RANGE(z,1,SizeZ-2);

	if (GetMaze(x,y,z,DOWN)==WALL_UNDEFINED) {
		for (xmax=x; (GetMaze(xmax,y,z,RIGHT)==0) && (xmax<SizeX-2); xmax++);
		for (xmin=x; (GetMaze(xmin,y,z,LEFT)==0) && (xmin>0); xmin--);
		for (zmax=z; (GetMaze(x,y,zmax,FRONT)==0) && (zmax<SizeZ-2); zmax++);
		for (zmin=z; (GetMaze(x,y,zmin,REAR)==0) && (zmin>0); zmin--);

		for (xx=xmin; xx<=xmax; xx++)
			for (zz=zmin; zz<=zmax; zz++) {
				if ((zz!=z) || (xx!=x)) {
					SetMaze(xx,y,zz,DOWN,WALL_SOLID);
				} else {
					SetMaze(xx,y,zz,DOWN,WALL_HOLE);
				}
			}
  }
};

void MAZE3D::WormTunnel(int x, int y, int z, int dir) {
  if (GetMaze(x,y,z,INTERIOR)==INTERIOR_BOX) {
  	switch (dir) {
  		case UP:
  			if ((GetMaze(x,y+1,z,INTERIOR)==INTERIOR_FLOOD) && (GetMaze(x,y+2,z,INTERIOR)==INTERIOR_FLOOD)) {
  				SetMaze(x,y,z,INTERIOR,INTERIOR_BOX);
  				SetMaze(x,y+1,z,INTERIOR,INTERIOR_BOX);
  				SetMaze(x,y+2,z,INTERIOR,INTERIOR_BOX);
  				SetMaze(x,y,z,UP,WALL_HOLE);
  				SetMaze(x,y+1,z,UP,WALL_HOLE);
  				SetMaze(x,y+1,z,DOWN,WALL_HOLE);
  				SetMaze(x,y+1,z,LEFT,WALL_SOLID);
  				SetMaze(x,y+1,z,RIGHT,WALL_SOLID);
  				SetMaze(x,y+1,z,FRONT,WALL_SOLID);
  				SetMaze(x,y+1,z,REAR,WALL_SOLID);
  				SetMaze(x,y+2,z,UP,WALL_SOLID);
  				SetMaze(x,y+2,z,DOWN,WALL_HOLE);
  				SetMaze(x,y+2,z,LEFT,WALL_SOLID);
  				SetMaze(x,y+2,z,RIGHT,WALL_SOLID);
  				SetMaze(x,y+2,z,FRONT,WALL_SOLID);
  				SetMaze(x,y+2,z,REAR,WALL_SOLID);
  				WormTunnel(x, y+2, z, dir);
 				} else {
 					WormTunnel(x, y, z, (int)(RANDOM_FLOAT*6.0)+1);
				}
				break;
			case DOWN:
  			if ((GetMaze(x,y-1,z,INTERIOR)==INTERIOR_FLOOD) && (GetMaze(x,y-2,z,INTERIOR)==INTERIOR_FLOOD)) {
					SetMaze(x,y,z,INTERIOR,INTERIOR_BOX);
					SetMaze(x,y-1,z,INTERIOR,INTERIOR_BOX);
					SetMaze(x,y-2,z,INTERIOR,INTERIOR_BOX);
					SetMaze(x,y,z,DOWN,WALL_HOLE);
					SetMaze(x,y-1,z,UP,WALL_HOLE);
					SetMaze(x,y-1,z,DOWN,WALL_HOLE);
					SetMaze(x,y-1,z,LEFT,WALL_SOLID);
					SetMaze(x,y-1,z,RIGHT,WALL_SOLID);
					SetMaze(x,y-1,z,FRONT,WALL_SOLID);
					SetMaze(x,y-1,z,REAR,WALL_SOLID);
					SetMaze(x,y-2,z,UP,WALL_HOLE);
					SetMaze(x,y-2,z,DOWN,WALL_SOLID);
					SetMaze(x,y-2,z,LEFT,WALL_SOLID);
					SetMaze(x,y-2,z,RIGHT,WALL_SOLID);
					SetMaze(x,y-2,z,FRONT,WALL_SOLID);
					SetMaze(x,y-2,z,REAR,WALL_SOLID);
					WormTunnel(x, y-2, z, dir);
				} else {
					WormTunnel(x, y, z, (int)(RANDOM_FLOAT*6.0)+1);
				}
				break;
			case FRONT:
  			if ((GetMaze(x-1,y,z,INTERIOR)==INTERIOR_FLOOD) && (GetMaze(x-2,y,z,INTERIOR)==INTERIOR_FLOOD)) {
					SetMaze(x,y,z,INTERIOR,INTERIOR_BOX);
					SetMaze(x-1,y,z,INTERIOR,INTERIOR_BOX);
					SetMaze(x-2,y,z,INTERIOR,INTERIOR_BOX);
					SetMaze(x,y,z,LEFT,WALL_HOLE);
					SetMaze(x-1,y,z,UP,WALL_SOLID);
					SetMaze(x-1,y,z,DOWN,WALL_SOLID);
					SetMaze(x-1,y,z,LEFT,WALL_HOLE);
					SetMaze(x-1,y,z,RIGHT,WALL_HOLE);
					SetMaze(x-1,y,z,FRONT,WALL_SOLID);
					SetMaze(x-1,y,z,REAR,WALL_SOLID);
					SetMaze(x-2,y,z,UP,WALL_SOLID);
					SetMaze(x-2,y,z,DOWN,WALL_SOLID);
					SetMaze(x-2,y,z,LEFT,WALL_SOLID);
					SetMaze(x-2,y,z,RIGHT,WALL_HOLE);
					SetMaze(x-2,y,z,FRONT,WALL_SOLID);
					SetMaze(x-2,y,z,REAR,WALL_SOLID);
					WormTunnel(x-2, y, z, dir);
				} else {
//					WormTunnel(x, y, z, (int)(RANDOM_FLOAT*6.0)+1);
				}
				break;
			case REAR:
  			if ((GetMaze(x+1,y,z,INTERIOR)==INTERIOR_FLOOD) && (GetMaze(x+2,y,z,INTERIOR)==INTERIOR_FLOOD)) {
					SetMaze(x,y,z,INTERIOR,INTERIOR_BOX);
					SetMaze(x+1,y,z,INTERIOR,INTERIOR_BOX);
					SetMaze(x+2,y,z,INTERIOR,INTERIOR_BOX);
					SetMaze(x,y,z,RIGHT,WALL_HOLE);
					SetMaze(x+1,y,z,UP,WALL_SOLID);
					SetMaze(x+1,y,z,DOWN,WALL_SOLID);
					SetMaze(x+1,y,z,LEFT,WALL_HOLE);
					SetMaze(x+1,y,z,RIGHT,WALL_HOLE);
					SetMaze(x+1,y,z,FRONT,WALL_SOLID);
					SetMaze(x+1,y,z,REAR,WALL_SOLID);
					SetMaze(x+2,y,z,UP,WALL_SOLID);
					SetMaze(x+2,y,z,DOWN,WALL_SOLID);
					SetMaze(x+2,y,z,LEFT,WALL_HOLE);
					SetMaze(x+2,y,z,RIGHT,WALL_SOLID);
					SetMaze(x+2,y,z,FRONT,WALL_SOLID);
					SetMaze(x+2,y,z,REAR,WALL_SOLID);
					WormTunnel(x+2, y, z, dir);
				} else {
					WormTunnel(x, y, z, (int)(RANDOM_FLOAT*6.0)+1);
				}
				break;
			case LEFT:
  			if ((GetMaze(x,y,z+1,INTERIOR)==INTERIOR_FLOOD) && (GetMaze(x,y,z+2,INTERIOR)==INTERIOR_FLOOD)) {
					SetMaze(x,y,z,INTERIOR,INTERIOR_BOX);
					SetMaze(x,y,z+1,INTERIOR,INTERIOR_BOX);
					SetMaze(x,y,z+2,INTERIOR,INTERIOR_BOX);
					SetMaze(x,y,z,FRONT,WALL_HOLE);
					SetMaze(x,y,z+1,UP,WALL_SOLID);
					SetMaze(x,y,z+1,DOWN,WALL_SOLID);
					SetMaze(x,y,z+1,LEFT,WALL_SOLID);
					SetMaze(x,y,z+1,RIGHT,WALL_SOLID);
					SetMaze(x,y,z+1,FRONT,WALL_HOLE);
					SetMaze(x,y,z+1,REAR,WALL_HOLE);
					SetMaze(x,y,z+2,UP,WALL_SOLID);
					SetMaze(x,y,z+2,DOWN,WALL_SOLID);
					SetMaze(x,y,z+2,LEFT,WALL_SOLID);
					SetMaze(x,y,z+2,RIGHT,WALL_SOLID);
					SetMaze(x,y,z+2,FRONT,WALL_SOLID);
					SetMaze(x,y,z+2,REAR,WALL_HOLE);
					WormTunnel(x, y, z+2, dir);
				} else {
					WormTunnel(x, y, z, (int)(RANDOM_FLOAT*6.0)+1);
				}
				break;
			case RIGHT:
  			if ((GetMaze(x,y,z-1,INTERIOR)==INTERIOR_FLOOD) && (GetMaze(x,y,z-2,INTERIOR)==INTERIOR_FLOOD)) {
					SetMaze(x,y,z,INTERIOR,INTERIOR_BOX);
					SetMaze(x,y,z-1,INTERIOR,INTERIOR_BOX);
					SetMaze(x,y,z-2,INTERIOR,INTERIOR_BOX);
					SetMaze(x,y,z,REAR,WALL_HOLE);
					SetMaze(x,y,z-1,UP,WALL_SOLID);
					SetMaze(x,y,z-1,DOWN,WALL_SOLID);
					SetMaze(x,y,z-1,LEFT,WALL_SOLID);
					SetMaze(x,y,z-1,RIGHT,WALL_SOLID);
					SetMaze(x,y,z-1,FRONT,WALL_HOLE);
					SetMaze(x,y,z-1,REAR,WALL_HOLE);
					SetMaze(x,y,z-2,UP,WALL_SOLID);
					SetMaze(x,y,z-2,DOWN,WALL_SOLID);
					SetMaze(x,y,z-2,LEFT,WALL_SOLID);
					SetMaze(x,y,z-2,RIGHT,WALL_SOLID);
					SetMaze(x,y,z-2,FRONT,WALL_HOLE);
					SetMaze(x,y,z-2,REAR,WALL_SOLID);
					WormTunnel(x, y, z-2, dir);
				} else {
					WormTunnel(x, y, z, (int)(RANDOM_FLOAT*6.0)+1);
				}
				break;
		}
  }
};

void MAZE3D::Build() {
	int t,c,cc;	
	int x,y,z;
  
  srand(Seed);

	for(t=0;t<MAXSIZE;t++) {
		data[t] = 0;
	}

	for(x=0;x<SizeX;x++)
		for(y=0;y<SizeY;y++) {
			data[INDEX(x,y,0)] = 0;
			data[INDEX(x,y,1)] |= 1<<6;
			data[INDEX(x,y,SizeZ-1)] = 1<<6;
			if ((x==0) || (x==(SizeX-1))) {
				data[INDEX(x,y,1)] &= (7<<3) + 7;;
				data[INDEX(x,y,SizeZ-1)] = 0;
			}
		}

	for(z=0;z<SizeZ;z++)
		for(y=0;y<SizeY;y++) {
			data[INDEX(0,y,z)] = 0;
			data[INDEX(1,y,z)] |= 1<<3;
			data[INDEX(SizeX-1,y,z)] = 1<<3;
			if ((z==0) || (z==(SizeZ-1))) {
				data[INDEX(1,y,z)] &= (7<<6) + 7;
				data[INDEX(SizeX-1,y,z)] = 0;
			}
		}

	for(x=0;x<SizeX;x++)
		for(z=0;z<SizeZ;z++) {
			data[INDEX(x,0,z)] = 0;
			data[INDEX(x,1,z)] |= 1;
			data[INDEX(x,SizeY-1,z)] = 1;
			if ((z==0) || (z==(SizeZ-1)) || (x==0) || (x==(SizeX-1))) {
				data[INDEX(x,1,z)] &= (7<<6) + (7<<3);
				data[INDEX(x,SizeY-1,z)] = 0;
			}
		}

  // Entry and exit
  SetMaze(0,SizeY/2,SizeZ/2,RIGHT,WALL_HOLE);
  SetMaze(SizeX-1,SizeY/2,SizeZ/2,LEFT,WALL_HOLE);

  if (Style==WORM_STYLE) {
  	for(x=1;x<SizeX-1;x++)
  		for(y=1;y<SizeY-1;y++)
  			for(z=1;z<SizeZ-1;z++)
  				SetMaze(x,y,z,INTERIOR,INTERIOR_FLOOD);

    for (x=SizeX-2, y=SizeY/2, z=SizeZ/2; x>SizeX-4; x--) {
    	SetMaze(x,y,z,INTERIOR,INTERIOR_BOX);
    	SetMaze(x,y,z,UP,WALL_SOLID);
    	SetMaze(x,y,z,DOWN,WALL_SOLID);
    	SetMaze(x,y,z,LEFT,WALL_HOLE);
    	SetMaze(x,y,z,RIGHT,WALL_HOLE);
    	SetMaze(x,y,z,FRONT,WALL_SOLID);
    	SetMaze(x,y,z,REAR,WALL_SOLID);
   	}
   	SetMaze(x,y,z,RIGHT,WALL_SOLID);
   	SetMaze(x-1,y,z,INTERIOR,INTERIOR_UNDEFINED);

   	for (t=(SizeX-2)*(SizeY-2)*(SizeZ-2), c=0; (t>0) && (c<Walls*100);t--) {
   		x = ((int)(RANDOM_FLOAT*(float)(SizeX/2))*2);
   		y = ((int)(RANDOM_FLOAT*(float)(SizeY/2))*2);
   		z = ((int)(RANDOM_FLOAT*(float)(SizeZ/2))*2);
   		if (((SizeY/2) & 1)==1) y++;
   		if (((SizeZ/2) & 1)==1) z++;
   		if (GetMaze(x,y,z,INTERIOR)==INTERIOR_BOX) {
   			WormTunnel(x, y, z, (int)(RANDOM_FLOAT*6.0)+1);
   			c++;
 			}
 			if ((t<=0) || (c>=Walls*100)) {
 				if (GetMaze(2,SizeY/2,SizeZ/2,INTERIOR)!=INTERIOR_BOX) {
 					t = 1;
 					c = (Walls*100) - 1;
 				}
 			}
    }

    x=1;
    y=SizeY/2;
    z=SizeZ/2;
    SetMaze(x,y,z,INTERIOR,INTERIOR_BOX);
    SetMaze(x,y,z,UP,WALL_SOLID);
    SetMaze(x,y,z,DOWN,WALL_SOLID);
    SetMaze(x,y,z,LEFT,WALL_HOLE);
    SetMaze(x,y,z,RIGHT,WALL_HOLE);
    SetMaze(x,y,z,FRONT,WALL_SOLID);
    SetMaze(x,y,z,REAR,WALL_SOLID);
  } else { // Style is Wall style
  	for (t=0; t<Walls; t++) {
  		BuildPlaneXY((int)(RANDOM_FLOAT*(float)(SizeX)), (int)(RANDOM_FLOAT*(float)(SizeY)), (int)(RANDOM_FLOAT*(float)(SizeZ)));
  		BuildPlaneZY((int)(RANDOM_FLOAT*(float)(SizeX)), (int)(RANDOM_FLOAT*(float)(SizeY)), (int)(RANDOM_FLOAT*(float)(SizeZ)));
  		BuildPlaneZX((int)(RANDOM_FLOAT*(float)(SizeX)), (int)(RANDOM_FLOAT*(float)(SizeY)), (int)(RANDOM_FLOAT*(float)(SizeZ)));
 		}
  }

	for(x=0;x<SizeX;x++)
		for(y=0;y<SizeY;y++)
			for(z=0;z<SizeZ;z++) {
				if (GetMaze(x,y,z,DOWN)==WALL_HOLE) SetMaze(x,y,z,DOWN,0);
				if (GetMaze(x,y,z,REAR)==WALL_HOLE) SetMaze(x,y,z,REAR,0);
				if (GetMaze(x,y,z,LEFT)==WALL_HOLE) SetMaze(x,y,z,LEFT,0);
			}

	for(x=0;x<SizeX-1;x++)
		for(y=0;y<SizeY-1;y++)
			for(z=0;z<SizeZ-1;z++)
				SetMaze(x,y,z,INTERIOR,INTERIOR_UNDEFINED);

  // Entry and exit
  SetMaze(0,SizeY/2,SizeZ/2,RIGHT,WALL_HOLE);
  SetMaze(SizeX-1,SizeY/2,SizeZ/2,LEFT,WALL_HOLE);

  SetMaze(1,SizeY/2,SizeZ/2,INTERIOR,RIGHT);
  SetMaze(SizeX-2,SizeY/2,SizeZ/2,INTERIOR,0);
  cc = -1;
  for(t=0;t<((SizeX-2)*(SizeY-2)*(SizeZ-2));t++) {
 	  c = 0;
	  for(x=0;x<SizeX;x++)
	  	for(y=0;y<SizeY;y++)
	  		for(z=0;z<SizeZ;z++) {
	  			if (GetMaze(x,y,z,INTERIOR) != 0) {
	  				c++;
	  				if ((GetMaze(x,y-1,z,INTERIOR)==0) && (GetMaze(x,y,z,DOWN)==0)) SetMaze(x,y-1,z,INTERIOR,UP);
	  				if ((GetMaze(x,y+1,z,INTERIOR)==0) && (GetMaze(x,y,z,UP)==0)) SetMaze(x,y+1,z,INTERIOR,DOWN);
	  				if ((GetMaze(x,y,z-1,INTERIOR)==0) && (GetMaze(x,y,z,REAR)==0)) SetMaze(x,y,z-1,INTERIOR,FRONT);
	  				if ((GetMaze(x,y,z+1,INTERIOR)==0) && (GetMaze(x,y,z,FRONT)==0)) SetMaze(x,y,z+1,INTERIOR,REAR);
	  				if ((GetMaze(x-1,y,z,INTERIOR)==0) && (GetMaze(x,y,z,LEFT)==0)) SetMaze(x-1,y,z,INTERIOR,LEFT);
	  				if ((GetMaze(x+1,y,z,INTERIOR)==0) && (GetMaze(x,y,z,RIGHT)==0)) SetMaze(x+1,y,z,INTERIOR,RIGHT);
  				}
	  		}
 		if (c==cc) {
   		t=SizeX*SizeY*SizeZ;
    } else {
    	cc = c;
   	}
  }

  SetMaze(SizeX-1,SizeY/2,SizeZ/2,INTERIOR,RIGHT);
  
  SetMaze(0,SizeY/2,SizeZ/2,RIGHT,0);
  SetMaze(SizeX-1,SizeY/2,SizeZ/2,LEFT,0);
};

#define BOOL(x) ((x)==1 ? true : false)

void MAZE3D::Load(char *filename) {
	int t;
  FileDecoder *fd = new FileDecoder(filename);
  SizeX = fd->ReadTagInt("sizex") + 2;
  SizeY = fd->ReadTagInt("sizey") + 2;
  SizeZ = fd->ReadTagInt("sizez") + 2;
  Walls = fd->ReadTagInt("walls");
  Style = fd->ReadTagInt("style");

  if (SizeX<3) SizeX = 3;
  if (SizeY<3) SizeY = 3;
  if (SizeZ<3) SizeZ = 3;
  while ((SizeX*SizeY*SizeZ) >= MAXSIZE) {
  	if (SizeX>3) SizeX--;
  	if ((SizeY>3) && ((SizeX*SizeY*SizeZ) >= MAXSIZE)) SizeY--;
  	if ((SizeZ>3) && ((SizeX*SizeY*SizeZ) >= MAXSIZE)) SizeZ--;
 	}

  Seed = fd->ReadTagInt("seed");

  t = fd->ReadTagInt("wall_solid");
  GlobalMazeFlags[FLAG_WALL_SOLID] = BOOL(t);
  t = fd->ReadTagInt("wall_lines");
  GlobalMazeFlags[FLAG_WALL_LINES] = BOOL(t);
  t = fd->ReadTagInt("ground");
  GlobalMazeFlags[FLAG_GROUND] = BOOL(t);
  t = fd->ReadTagInt("sky");
  GlobalMazeFlags[FLAG_SKY] = BOOL(t);
  t = fd->ReadTagInt("textures");
  GlobalMazeFlags[FLAG_WALL_TEXTURES] = BOOL(t);

  delete fd;
  
  Build();
}

MAZE3D::MAZE3D() {
	SizeX = 9;
	SizeY = 9;
	SizeZ = 9;
	Walls = SizeX*SizeY*SizeZ;
	Style = 0;
	TexID[0] = TextureList->Get("images/Wall.tga");
	TexID[1] = TextureList->Get("images/Floor.tga");
};

MAZE3D::~MAZE3D() {
};

void MAZE3D::DrawInterior(float x, float y, float z) {
	float Size = 10;
	float s = 0.25;
	float x0,y0,z0;
	int type = GetMaze((int)(x),(int)(y),(int)(z), INTERIOR);
	int t;

	if ((type==0) || (!GlobalMazeFlags[FLAG_HELP_ARROWS])) return;

	x0 = (x+0.5) * Size;
	y0 = (y+0.5) * Size;
	z0 = (z+0.5) * Size;

	for (t=0;t<8;t++) {
		if ((true) || (type==INTERIOR_BOX) || (type==INTERIOR_FLOOD)) {
			glBegin(GL_QUADS);
				if (GlobalMazeFlags[FLAG_WALL_SOLID]) {
					glColor4f(1,0,0,1);
				} else {
					glColor4f(1,0,0,0.5);
				}

			glTexCoord2f(0.0f, 0.0f); glVertex3f(x0+s, y0+s, z0-s);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x0+s, y0+s, z0+s);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x0+s, y0-s, z0+s);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x0+s, y0-s, z0-s);

			glTexCoord2f(0.0f, 0.0f); glVertex3f(x0-s, y0+s, z0-s);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x0-s, y0+s, z0+s);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x0-s, y0-s, z0+s);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x0-s, y0-s, z0-s);

			glTexCoord2f(0.0f, 0.0f); glVertex3f(x0+s, y0+s, z0-s);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x0+s, y0+s, z0+s);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x0-s, y0+s, z0+s);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x0-s, y0+s, z0-s);

			glTexCoord2f(0.0f, 0.0f); glVertex3f(x0+s, y0-s, z0-s);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x0+s, y0-s, z0+s);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x0-s, y0-s, z0+s);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x0-s, y0-s, z0-s);

			glTexCoord2f(0.0f, 0.0f); glVertex3f(x0+s, y0-s, z0+s);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x0+s, y0+s, z0+s);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x0-s, y0+s, z0+s);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x0-s, y0-s, z0+s);

			glTexCoord2f(0.0f, 0.0f); glVertex3f(x0+s, y0-s, z0-s);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x0+s, y0+s, z0-s);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x0-s, y0+s, z0-s);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x0-s, y0-s, z0-s);
			glEnd();
    }
    
    if (type==UP) y0 += 0.5;
    if (type==DOWN) y0 -= 0.5;
    if (type==RIGHT) x0 -= 0.5;
    if (type==LEFT) x0 += 0.5;
    if (type==FRONT) z0 += 0.5;
    if (type==REAR) z0 -= 0.5;
    
    s = s - 0.025;
	}
};

void MAZE3D::DrawWall(float x, float y, float z, int dir, bool lines[4]) {
	float Size = 10;
	float xx[4], yy[4], zz[4];
	int i;

	if (GlobalMazeFlags[FLAG_WALL_TEXTURES]) {
		glEnable(GL_TEXTURE_2D);
	} else {
		glDisable(GL_TEXTURE_2D);
	}

	int type = GetMaze((int)(x),(int)(y),(int)(z),dir);

	switch (dir) {
		case UP:
			glBindTexture(GL_TEXTURE_2D,TexID[1]);
			xx[0]=(x+0)*Size;  xx[1]=(x+1)*Size;  xx[2]=(x+1)*Size;  xx[3]=(x+0)*Size; 
			yy[0]=(y+1)*Size;  yy[1]=(y+1)*Size;  yy[2]=(y+1)*Size;  yy[3]=(y+1)*Size; 
			zz[0]=(z+0)*Size;  zz[1]=(z+0)*Size;  zz[2]=(z+1)*Size;  zz[3]=(z+1)*Size; 
			break;
		case DOWN:
			glBindTexture(GL_TEXTURE_2D,TexID[1]);
			xx[0]=(x+0)*Size;  xx[1]=(x+1)*Size;  xx[2]=(x+1)*Size;  xx[3]=(x+0)*Size; 
			yy[0]=(y+0)*Size;  yy[1]=(y+0)*Size;  yy[2]=(y+0)*Size;  yy[3]=(y+0)*Size; 
			zz[0]=(z+0)*Size;  zz[1]=(z+0)*Size;  zz[2]=(z+1)*Size;  zz[3]=(z+1)*Size; 
			break;
		case RIGHT:
			glBindTexture(GL_TEXTURE_2D,TexID[0]);
			xx[0]=(x+1)*Size;  xx[1]=(x+1)*Size;  xx[2]=(x+1)*Size;  xx[3]=(x+1)*Size; 
			yy[0]=(y+0)*Size;  yy[1]=(y+1)*Size;  yy[2]=(y+1)*Size;  yy[3]=(y+0)*Size; 
			zz[0]=(z+0)*Size;  zz[1]=(z+0)*Size;  zz[2]=(z+1)*Size;  zz[3]=(z+1)*Size; 
			break;
		case LEFT:
			glBindTexture(GL_TEXTURE_2D,TexID[0]);
			xx[0]=(x+0)*Size;  xx[1]=(x+0)*Size;  xx[2]=(x+0)*Size;  xx[3]=(x+0)*Size; 
			yy[0]=(y+0)*Size;  yy[1]=(y+1)*Size;  yy[2]=(y+1)*Size;  yy[3]=(y+0)*Size; 
			zz[0]=(z+0)*Size;  zz[1]=(z+0)*Size;  zz[2]=(z+1)*Size;  zz[3]=(z+1)*Size; 
			break;
		case FRONT:
			glBindTexture(GL_TEXTURE_2D,TexID[0]);
			xx[0]=(x+0)*Size;  xx[1]=(x+0)*Size;  xx[2]=(x+1)*Size;  xx[3]=(x+1)*Size; 
			yy[0]=(y+0)*Size;  yy[1]=(y+1)*Size;  yy[2]=(y+1)*Size;  yy[3]=(y+0)*Size; 
			zz[0]=(z+1)*Size;  zz[1]=(z+1)*Size;  zz[2]=(z+1)*Size;  zz[3]=(z+1)*Size; 
			break;
		case REAR:
			glBindTexture(GL_TEXTURE_2D,TexID[0]);
			xx[0]=(x+1)*Size;  xx[1]=(x+1)*Size;  xx[2]=(x+0)*Size;  xx[3]=(x+0)*Size; 
			yy[0]=(y+0)*Size;  yy[1]=(y+1)*Size;  yy[2]=(y+1)*Size;  yy[3]=(y+0)*Size; 
			zz[0]=(z+0)*Size;  zz[1]=(z+0)*Size;  zz[2]=(z+0)*Size;  zz[3]=(z+0)*Size; 
			break;
	};

	if (type==WALL_SOLID) {
		if ((GlobalMazeFlags[FLAG_WALL_SOLID]) || (GlobalMazeFlags[FLAG_WALL_TEXTURES])) {
			glColor4f(0,1,0,1);
		} else {
			glColor4f(0,1,0,0.2);
		}
		if ((!GlobalMazeFlags[FLAG_WALL_LINES]) && (GlobalMazeFlags[FLAG_WALL_SOLID])) {
			// If we don't use FLAG_WALL_LINES, we draw an edge with the wall color
			// to get sharp edges, preventing "pixels" to shine through from other lines.
			glLineWidth(1);
			glBegin(GL_LINE_LOOP);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(xx[0], yy[0], zz[0]);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(xx[3], yy[3], zz[3]);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(xx[2], yy[2], zz[2]);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(xx[1], yy[1], zz[1]);
			glEnd();
		}
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(xx[0], yy[0], zz[0]);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(xx[3], yy[3], zz[3]);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(xx[2], yy[2], zz[2]);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(xx[1], yy[1], zz[1]);
		glEnd();
		if (GlobalMazeFlags[FLAG_WALL_LINES]) {
			glLineWidth(1);
			glColor4f(0,0.5,0,1);
			glBegin(GL_LINE_LOOP);
				for (i=3;i>=0;i--) { glVertex3f(xx[i], yy[i], zz[i]); }
			glEnd();
			glBegin(GL_LINES); // Cross
				glVertex3f(xx[0], yy[0], zz[0]);
				glVertex3f(xx[2], yy[2], zz[2]);
				glVertex3f(xx[1], yy[1], zz[1]);
				glVertex3f(xx[3], yy[3], zz[3]);
			glEnd();
		}
	}	else if (type==WALL_HOLE) {
		glBegin(GL_QUADS);
			glColor4f(0,0,1,0.5); // Holes are always transperent
			glTexCoord2f(0.0f, 0.0f); glVertex3f(xx[0], yy[0], zz[0]);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(xx[3], yy[3], zz[3]);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(xx[2], yy[2], zz[2]);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(xx[1], yy[1], zz[1]);
		glEnd();
		if (GlobalMazeFlags[FLAG_WALL_LINES]) {
			glLineWidth(3);
			glColor4f(0,0,1,1);
			glBegin(GL_LINE_LOOP);
				for (i=3;i>=0;i--) { glVertex3f(xx[i], yy[i], zz[i]); }
			glEnd();
		}
	}

	// Edges!
	if (GlobalMazeFlags[FLAG_WALL_LINES]) {
		glLineWidth(1);
	} else {
		glLineWidth(1);
	}
	glBegin(GL_LINES);
		glColor4f(0,0,0,1);
		if (lines[0]) {
			glVertex3f(xx[0], yy[0], zz[0]);
			glVertex3f(xx[1], yy[1], zz[1]);
		}
		if (lines[1]) {
			glVertex3f(xx[1], yy[1], zz[1]);
			glVertex3f(xx[2], yy[2], zz[2]);
		}
		if (lines[2]) {
			glVertex3f(xx[2], yy[2], zz[2]);
			glVertex3f(xx[3], yy[3], zz[3]);
		}
		if (lines[3]) {
			glVertex3f(xx[3], yy[3], zz[3]);
			glVertex3f(xx[0], yy[0], zz[0]);
		}
	glEnd();
};

void MAZE3D::GetEdges(int x, int y, int z, int dir, bool lines[4]) {
	int i;
	for (i=0;i<4;i++) lines[i] = false;
	switch (dir) {
		case RIGHT:
			if (GetMaze(x+1,y,z,UP)!=0) lines[1] = true;	
			if (GetMaze(x+1,y,z,DOWN)!=0) lines[3] = true;	
			if (GetMaze(x+1,y,z,FRONT)!=0) lines[2] = true;	
			if (GetMaze(x+1,y,z,REAR)!=0) lines[0] = true;	
			if (!GlobalMazeFlags[FLAG_WALL_SOLID]) {
				if (GetMaze(x,y,z,UP)!=0) lines[1] = true;	
				if (GetMaze(x,y,z,DOWN)!=0) lines[3] = true;	
				if (GetMaze(x,y,z,FRONT)!=0) lines[2] = true;	
				if (GetMaze(x,y,z,REAR)!=0) lines[0] = true;	
			}
			if (GetMaze(x,y+1,z,RIGHT)==0) lines[1] = true;
			if (GetMaze(x,y-1,z,RIGHT)==0) lines[3] = true;
			if (GetMaze(x,y,z+1,RIGHT)==0) lines[2] = true;
			if (GetMaze(x,y,z-1,RIGHT)==0) lines[0] = true;
			break;
		case LEFT:
			if (GetMaze(x-1,y,z,UP)!=0) lines[1] = true;	
			if (GetMaze(x-1,y,z,DOWN)!=0) lines[3] = true;	
			if (GetMaze(x-1,y,z,FRONT)!=0) lines[2] = true;	
			if (GetMaze(x-1,y,z,REAR)!=0) lines[0] = true;
			if (!GlobalMazeFlags[FLAG_WALL_SOLID]) {
				if (GetMaze(x,y,z,UP)!=0) lines[1] = true;	
				if (GetMaze(x,y,z,DOWN)!=0) lines[3] = true;	
				if (GetMaze(x,y,z,FRONT)!=0) lines[2] = true;	
				if (GetMaze(x,y,z,REAR)!=0) lines[0] = true;	
			}
			if (GetMaze(x,y+1,z,LEFT)==0) lines[1] = true;
			if (GetMaze(x,y-1,z,LEFT)==0) lines[3] = true;
			if (GetMaze(x,y,z+1,LEFT)==0) lines[2] = true;
			if (GetMaze(x,y,z-1,LEFT)==0) lines[0] = true;
			break;
		case FRONT:
			if (GetMaze(x,y,z+1,UP)!=0) lines[1] = true;	
			if (GetMaze(x,y,z+1,DOWN)!=0) lines[3] = true;	
			if (GetMaze(x,y,z+1,LEFT)!=0) lines[0] = true;	
			if (GetMaze(x,y,z+1,RIGHT)!=0) lines[2] = true;	
			if (!GlobalMazeFlags[FLAG_WALL_SOLID]) {
				if (GetMaze(x,y,z,UP)!=0) lines[1] = true;	
				if (GetMaze(x,y,z,DOWN)!=0) lines[3] = true;	
				if (GetMaze(x,y,z,LEFT)!=0) lines[0] = true;	
				if (GetMaze(x,y,z,RIGHT)!=0) lines[2] = true;	
			}
			if (GetMaze(x,y+1,z,FRONT)==0) lines[1] = true;
			if (GetMaze(x,y-1,z,FRONT)==0) lines[3] = true;
			if (GetMaze(x+1,y,z,FRONT)==0) lines[2] = true;
			if (GetMaze(x-1,y,z,FRONT)==0) lines[0] = true;
			break;
		case REAR:
			if (GetMaze(x,y,z-1,UP)!=0) lines[1] = true;	
			if (GetMaze(x,y,z-1,DOWN)!=0) lines[3] = true;	
			if (GetMaze(x,y,z-1,LEFT)!=0) lines[2] = true;	
			if (GetMaze(x,y,z-1,RIGHT)!=0) lines[0] = true;	
			if (!GlobalMazeFlags[FLAG_WALL_SOLID]) {
				if (GetMaze(x,y,z,UP)!=0) lines[1] = true;	
				if (GetMaze(x,y,z,DOWN)!=0) lines[3] = true;	
				if (GetMaze(x,y,z,LEFT)!=0) lines[2] = true;	
				if (GetMaze(x,y,z,RIGHT)!=0) lines[0] = true;	
			}
			if (GetMaze(x,y+1,z,REAR)==0) lines[1] = true;
			if (GetMaze(x,y-1,z,REAR)==0) lines[3] = true;
			if (GetMaze(x+1,y,z,REAR)==0) lines[0] = true;
			if (GetMaze(x-1,y,z,REAR)==0) lines[2] = true;
			break;
		case DOWN:
			if (GetMaze(x,y-1,z,FRONT)!=0) lines[2] = true;	
			if (GetMaze(x,y-1,z,REAR)!=0) lines[0] = true;	
			if (GetMaze(x,y-1,z,LEFT)!=0) lines[3] = true;	
			if (GetMaze(x,y-1,z,RIGHT)!=0) lines[1] = true;	
			if (!GlobalMazeFlags[FLAG_WALL_SOLID]) {
				if (GetMaze(x,y,z,FRONT)!=0) lines[2] = true;	
				if (GetMaze(x,y,z,REAR)!=0) lines[0] = true;	
				if (GetMaze(x,y,z,LEFT)!=0) lines[3] = true;	
				if (GetMaze(x,y,z,RIGHT)!=0) lines[1] = true;	
			}
			if (GetMaze(x,y,z+1,DOWN)==0) lines[2] = true;
			if (GetMaze(x,y,z-1,DOWN)==0) lines[0] = true;
			if (GetMaze(x+1,y,z,DOWN)==0) lines[1] = true;
			if (GetMaze(x-1,y,z,DOWN)==0) lines[3] = true;
			break;
		case UP:
			if (GetMaze(x,y+1,z,FRONT)!=0) lines[2] = true;	
			if (GetMaze(x,y+1,z,REAR)!=0) lines[0] = true;	
			if (GetMaze(x,y+1,z,LEFT)!=0) lines[3] = true;	
			if (GetMaze(x,y+1,z,RIGHT)!=0) lines[1] = true;	
			if (!GlobalMazeFlags[FLAG_WALL_SOLID]) {
				if (GetMaze(x,y,z,FRONT)!=0) lines[2] = true;	
				if (GetMaze(x,y,z,REAR)!=0) lines[0] = true;	
				if (GetMaze(x,y,z,LEFT)!=0) lines[3] = true;	
				if (GetMaze(x,y,z,RIGHT)!=0) lines[1] = true;	
			}
			if (GetMaze(x,y,z+1,UP)==0) lines[2] = true;
			if (GetMaze(x,y,z-1,UP)==0) lines[0] = true;
			if (GetMaze(x+1,y,z,UP)==0) lines[1] = true;
			if (GetMaze(x-1,y,z,UP)==0) lines[3] = true;
			break;
	}
}

void MAZE3D::Draw(float px, float py, float pz) {
	int x,y,z,bx,by,bz,i;
	bool lines[4];

	DrawGround();
	DrawSky();
	DrawExit();
	DrawEntry();
	
	for (i=0;i<4;i++) lines[i] = false;

	bx = MAX(0,MIN(SizeX,(int)(px / 10)));
	by = MAX(0,MIN(SizeY,(int)(py / 10)));
	bz = MAX(0,MIN(SizeZ,(int)(pz / 10)));

	if (GlobalMazeFlags[FLAG_WALL_SOLID]) {
		glDisable(GL_BLEND);
	} else {
		glEnable(GL_BLEND);
	}
	glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

	glColor4f(1,1,1,1);
	for(x=0;x<bx;x++)
		for(y=0;y<by;y++)
			for(z=0;z<bz;z++) {
				DrawInterior(x,y,z);
				if (GetMaze(x,y,z,UP)!=0) {
					GetEdges(x,y,z,UP,lines);
					DrawWall(x,y,z,UP,lines);
				}
				if (GetMaze(x,y,z,RIGHT)!=0) {
					GetEdges(x,y,z,RIGHT,lines);
					DrawWall(x,y,z,RIGHT,lines);
				}
				if (GetMaze(x,y,z,FRONT)!=0) {
					GetEdges(x,y,z,FRONT,lines);
					DrawWall(x,y,z,FRONT,lines);
				}
			}

	for(x=SizeX-1;x>=bx;x--)
		for(y=0;y<by;y++)
			for(z=0;z<bz;z++) {
				DrawInterior(x,y,z);
				if (GetMaze(x,y,z,UP)!=0) {
					GetEdges(x,y,z,UP,lines);
					DrawWall(x,y,z,UP,lines);
				}
				if ((x!=bx) && (GetMaze(x,y,z,LEFT)!=0)) {
					GetEdges(x,y,z,LEFT,lines);
					DrawWall(x,y,z,LEFT,lines);
				}
				if (GetMaze(x,y,z,FRONT)!=0) {
					GetEdges(x,y,z,FRONT,lines);
					DrawWall(x,y,z,FRONT,lines);
				}
			}

	for(x=SizeX-1;x>bx;x--)
		for(y=0;y<by;y++)
			for(z=SizeZ-1;z>=bz;z--) {
				DrawInterior(x,y,z);
				if (GetMaze(x,y,z,UP)!=0) {
					GetEdges(x,y,z,UP,lines);
					DrawWall(x,y,z,UP,lines);
				}
				if (GetMaze(x,y,z,LEFT)!=0) {
					GetEdges(x,y,z,LEFT,lines);
					DrawWall(x,y,z,LEFT,lines);
				}
				if ((z!=bz) && (GetMaze(x,y,z,REAR)!=0)) {
					GetEdges(x,y,z,REAR,lines);
					DrawWall(x,y,z,REAR,lines);
				}
			}

	for(x=0;x<=bx;x++)
		for(y=0;y<by;y++)
			for(z=SizeZ-1;z>=bz;z--) {
				DrawInterior(x,y,z);
				if (GetMaze(x,y,z,UP)!=0) {
					GetEdges(x,y,z,UP,lines);
					DrawWall(x,y,z,UP,lines);
				}
				if ((x!=bx) && (GetMaze(x,y,z,RIGHT)!=0)) {
					GetEdges(x,y,z,RIGHT,lines);
					DrawWall(x,y,z,RIGHT,lines);
				}
				if ((z!=bz) && (GetMaze(x,y,z,REAR)!=0)) {
					GetEdges(x,y,z,REAR,lines);
					DrawWall(x,y,z,REAR,lines);
				}
			}

	for(x=0;x<bx;x++)
		for(y=SizeY-1;y>=by;y--)
			for(z=0;z<bz;z++) {
				DrawInterior(x,y,z);
				if ((y!=by) && (GetMaze(x,y,z,DOWN)!=0)) {
					GetEdges(x,y,z,DOWN,lines);
					DrawWall(x,y,z,DOWN,lines);
				}
				if (GetMaze(x,y,z,RIGHT)!=0) {
					GetEdges(x,y,z,RIGHT,lines);
					DrawWall(x,y,z,RIGHT,lines);
				}
				if (GetMaze(x,y,z,FRONT)!=0) {
					GetEdges(x,y,z,FRONT,lines);
					DrawWall(x,y,z,FRONT,lines);
				}
			}

	for(x=SizeX-1;x>=bx;x--)
		for(y=SizeY-1;y>=by;y--)
			for(z=0;z<bz;z++) {
				DrawInterior(x,y,z);
				if ((y!=by) && (GetMaze(x,y,z,DOWN)!=0)) {
					GetEdges(x,y,z,DOWN,lines);
					DrawWall(x,y,z,DOWN,lines);
				}
				if ((x!=bx) && (GetMaze(x,y,z,LEFT)!=0)) {
					GetEdges(x,y,z,LEFT,lines);
					DrawWall(x,y,z,LEFT,lines);
				}
				if (GetMaze(x,y,z,FRONT)!=0) {
					GetEdges(x,y,z,FRONT,lines);
					DrawWall(x,y,z,FRONT,lines);
				}
			}

	for(x=SizeX-1;x>bx;x--)
		for(y=SizeY-1;y>=by;y--)
			for(z=SizeZ-1;z>=bz;z--) {
				DrawInterior(x,y,z);
				if ((y!=by) && (GetMaze(x,y,z,DOWN)!=0)) {
					GetEdges(x,y,z,DOWN,lines);
					DrawWall(x,y,z,DOWN,lines);
				}
				if (GetMaze(x,y,z,LEFT)!=0) {
					GetEdges(x,y,z,LEFT,lines);
					DrawWall(x,y,z,LEFT,lines);
				}
				if ((z!=bz) && (GetMaze(x,y,z,REAR)!=0)) {
					GetEdges(x,y,z,REAR,lines);
					DrawWall(x,y,z,REAR,lines);
				}
			}

	for(x=0;x<=bx;x++)
		for(y=SizeY-1;y>=by;y--)
			for(z=SizeZ-1;z>=bz;z--) {
				DrawInterior(x,y,z);
				if ((y!=by) && (GetMaze(x,y,z,DOWN)!=0)) {
					GetEdges(x,y,z,DOWN,lines);
					DrawWall(x,y,z,DOWN,lines);
				}
				if ((x!=bx) && (GetMaze(x,y,z,RIGHT)!=0)) {
					GetEdges(x,y,z,RIGHT,lines);
					DrawWall(x,y,z,RIGHT,lines);
				}
				if ((z!=bz) && (GetMaze(x,y,z,REAR)!=0)) {
					GetEdges(x,y,z,REAR,lines);
					DrawWall(x,y,z,REAR,lines);
				}
			}

  glEnable(GL_CULL_FACE);

  if (px<5) DrawExit();
	if (px>(10*SizeX-5)) DrawEntry();
};



void MAZE3D::DrawGround() {
	int t;
	if (!GlobalMazeFlags[FLAG_GROUND]) return;
  glLineWidth(1);
  glBegin(GL_LINES);
  glColor4f(1,0,0,1);
	for (t=-40; t<=40; t++) {
		glVertex3f(t*25,-10,1000);
		glVertex3f(t*25,-10,-1000);
		glVertex3f(1000,-10,t*25);
		glVertex3f(-1000,-10,t*25);
	}
	glEnd();
}

void MAZE3D::DrawExit() {
	VECTORTEXT VTEXT;
	POSITION Pos;
 	Pos.Set(5, 10*(SizeY/2)+5, 10*(SizeZ/2)+5, 180,0,0);
  glColor4f(1,0,0,1);
	VTEXT.Draw(Pos, 0.5, "EXIT");

 	Pos.Set(522, -10, 507, 0,0,90);
  glColor4f(1,0,0,1);
	VTEXT.Draw(Pos, 0.2, "Oh, you found me...");
}

void MAZE3D::DrawEntry() {
	VECTORTEXT VTEXT;
	POSITION Pos;
 	Pos.Set(10*SizeX, 10*(SizeY/2)+5, 10*(SizeZ/2)+5, 180,0,0);
  glColor4f(0,0,1,1);
	VTEXT.Draw(Pos, 0.5, "ENTRY");
};

void MAZE3D::DrawSky() {
	int t;
	if (!GlobalMazeFlags[FLAG_SKY]) return;
  glLineWidth(1);
  glBegin(GL_LINES);
  glColor4f(0,0,1,1);
	for (t=-40; t<=40; t++) {
		glVertex3f(t*25,10*(SizeY+8),1000);
		glVertex3f(t*25,10*(SizeY+8),-1000);
		glVertex3f(1000,10*(SizeY+8),t*25);
		glVertex3f(-1000,10*(SizeY+8),t*25);
	}
	glEnd();
};

// Return the allowed move vector, checking for walls.
VECTOR3D MAZE3D::Collision(VECTOR3D pos, VECTOR3D dir) {
	int x,y,z,fx,fy,fz;
	VECTOR3D res;

	x = MAX(0,MIN(SizeX-1,(int)(pos.x / 10)));
	y = MAX(0,MIN(SizeY-1,(int)(pos.y / 10)));
	z = MAX(0,MIN(SizeZ-1,(int)(pos.z / 10)));
	
	fx = (int)(pos.x) - (x*10);
	fy = (int)(pos.y) - (y*10);
	fz = (int)(pos.z) - (z*10);
	
	res = dir;	
	
	if ((GetMaze(x,y,z,DOWN)==WALL_SOLID) && (dir.y<0) && (fy<=3)) res.y = 3-fy;
	if ((GetMaze(x,y,z,UP)==WALL_SOLID) && (dir.y>0) && (fy>=7)) res.y = 7-fy;
	if ((GetMaze(x,y,z,LEFT)==WALL_SOLID) && (dir.x<0) && (fx<=3)) res.x = 3-fx;
	if ((GetMaze(x,y,z,RIGHT)==WALL_SOLID) && (dir.x>0) && (fx>=7)) res.x = 7-fx;
	if ((GetMaze(x,y,z,REAR)==WALL_SOLID) && (dir.z<0) && (fz<=3)) res.z = 3-fz;
	if ((GetMaze(x,y,z,FRONT)==WALL_SOLID) && (dir.z>0) && (fz>=7)) res.z = 7-fz;
	
	return res;
};


