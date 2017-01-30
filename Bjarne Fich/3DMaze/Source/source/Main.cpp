#include <windows.h>
#include <time.h>

#include "Macros.hpp"
#include "Globals.hpp"
#include "Log.hpp"
#include "GLWindow.hpp"
#include "Texture.hpp"
#include "Maze.hpp"
#include "Position.hpp"
#include "VectorText.hpp"

// Globals:
OPENGLWINDOW *glWindow = 0;
TEXTURELIST *TextureList = 0;
bool GlobalMazeFlags[FLAGS_END];

class Keyboard {
  public:
    bool up, down, left, right, forward, back, rollright, rollleft;
    Keyboard() { up=false; down=false; left=false; right=false; forward=false; back=false; rollleft=false; rollright=false; };
    ~Keyboard() {};
};

class Mouse {
  public:
  	bool lbutton, rbutton;
  	int x,y;
  	Mouse() { lbutton=false; rbutton=false; }
    ~Mouse() {};
};

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil) {
	// Variables
	bool running = true;
	// Timing variables
  float fps = 0;
  clock_t time_now, last_time = 0;
  long int frames, ticks = 0;
  clock_t movement_timer = 0;
  // Input variables
  bool keypress;
  Keyboard keys;
  Mouse mouse;
	MSG messages;
	// Score
  int mic, sec, min;
  long int play_time, help_time = 0;
  // Other
	MOVEMENT mymove;
	int size = 9;
	int seed = 0;
	VECTOR3D vec;

  ResetLog("Log.txt");

  GlobalMazeFlags[FLAG_WALL_SOLID] = false;
  GlobalMazeFlags[FLAG_WALL_LINES] = true;
  GlobalMazeFlags[FLAG_HELP_ARROWS] = false;
  GlobalMazeFlags[FLAG_WALL_TEXTURES] = false;
  GlobalMazeFlags[FLAG_AUTOLEVEL] = true;
  GlobalMazeFlags[FLAG_DEBUG] = false;  
  
  srand(10);

  Log("Open Window...");
	glWindow = new OPENGLWINDOW("data/setup.xml", hThisInstance, nFunsterStil);

  Log("Setup TextureList...");
  TextureList = new TEXTURELIST();

  Log("Setup Maze...");
  MAZE3D *Maze = new MAZE3D();
	//Maze->Build();
	Maze->Load("data/Maze1.xml");
  
 	int pos_height = Maze->SizeY/2;
 	int pos_front = Maze->SizeX + MAX((int)((float)(Maze->SizeY)*1),(int)((float)(Maze->SizeZ)*0.75));
 	if (pos_height<3) {
 		mymove.Pos.Set(pos_front*10,5+80,5+(Maze->SizeZ/2)*10,90,-15,0);
  } else {
  	mymove.Pos.Set(pos_front*10,5+pos_height*10,5+(Maze->SizeZ/2)*10,90,0,0);
 	}
 	
  Log("Wait for 'press space'...");
	while (running) {
		// Message handling
		if (PeekMessage(&messages, NULL, 0, 0, PM_REMOVE)) {
			switch(messages.message) {
        case WM_MOUSEMOVE:
          mouse.x = LOWORD(messages.lParam);
          mouse.y = HIWORD(messages.lParam);
          break;
        case WM_QUIT:
        	running = false;
          break;
        case WM_KEYDOWN:
        case WM_KEYUP:
          if (messages.message==WM_KEYDOWN) { keypress = true; } else { keypress = false; }
          switch (messages.wParam) {  // Keyboard keys
            case VK_ESCAPE:
              running = false;
              PostQuitMessage(0);
              break;
            case ' ':
              running = false;
              break;
            case 's':
            case 'S':
            	if (!keypress) GlobalMazeFlags[FLAG_WALL_SOLID] = !GlobalMazeFlags[FLAG_WALL_SOLID];
              break;
            case 'l':
            case 'L':
            	if (!keypress) GlobalMazeFlags[FLAG_WALL_LINES] = !GlobalMazeFlags[FLAG_WALL_LINES];
              break;
            case 't':
            case 'T':
            	if (!keypress) GlobalMazeFlags[FLAG_WALL_TEXTURES] = !GlobalMazeFlags[FLAG_WALL_TEXTURES];
              break;
            case 'p':
            case 'P':
            	if (!keypress) GlobalMazeFlags[FLAG_AUTOLEVEL] = !GlobalMazeFlags[FLAG_AUTOLEVEL];
              break;
            case 'd':
            case 'D':
            	if (!keypress) GlobalMazeFlags[FLAG_DEBUG] = !GlobalMazeFlags[FLAG_DEBUG];
              break;
            case 'g':
            case 'G':
            	if (!keypress) {
            		if ((GlobalMazeFlags[FLAG_GROUND]) && (GlobalMazeFlags[FLAG_SKY])) {
            			GlobalMazeFlags[FLAG_GROUND] = false;
            			GlobalMazeFlags[FLAG_SKY] = true;
            		} else if ((!GlobalMazeFlags[FLAG_GROUND]) && (GlobalMazeFlags[FLAG_SKY])) {
            			GlobalMazeFlags[FLAG_GROUND] = false;
            			GlobalMazeFlags[FLAG_SKY] = false;
            		} else if ((!GlobalMazeFlags[FLAG_GROUND]) && (!GlobalMazeFlags[FLAG_SKY])) {
            			GlobalMazeFlags[FLAG_GROUND] = true;
            			GlobalMazeFlags[FLAG_SKY] = false;
            		} else if ((GlobalMazeFlags[FLAG_GROUND]) && (!GlobalMazeFlags[FLAG_SKY])) {
            			GlobalMazeFlags[FLAG_GROUND] = true;
            			GlobalMazeFlags[FLAG_SKY] = true;
            		}
            	}
              break;
            case 'h':
            case 'H':
            	GlobalMazeFlags[FLAG_HELP_ARROWS] = keypress;
              break;
            case '1': if (!keypress) { Maze->Load("data/Maze1.xml"); ticks=9; } break;
            case '2': if (!keypress) { Maze->Load("data/Maze2.xml"); ticks=9; } break;
            case '3': if (!keypress) { Maze->Load("data/Maze3.xml"); ticks=9; } break;
            case '4': if (!keypress) { Maze->Load("data/Maze4.xml"); ticks=9; } break;
            case '5': if (!keypress) { Maze->Load("data/Maze5.xml"); ticks=9; } break;
            case '6': if (!keypress) { Maze->Load("data/Maze6.xml"); ticks=9; } break;
            case '7': if (!keypress) { Maze->Load("data/Maze7.xml"); ticks=9; } break;
            case '8': if (!keypress) { Maze->Load("data/Maze8.xml"); ticks=9; } break;
            case '9': if (!keypress) { Maze->Load("data/Maze9.xml"); ticks=9; } break;
            case '0': if (!keypress) { Maze->Seed=time_now; Maze->Build(); } break;
          }
          break;
      }
    }

    // Update the frame rate once per 1 sec second (CLK_TCK is clock ticks pr second)
    time_now = clock();
    ++frames;
    if(time_now - last_time > CLK_TCK*2) {
      fps = (frames*CLK_TCK)/(time_now-last_time);
      last_time = time_now;
      frames   = 0L;
    }

    // Movement and timer ticks (Updated 30 times pr second)
    if(time_now - movement_timer > CLK_TCK/30) {
    	ticks++;

    	if (ticks==10) {
    		if (Maze->SizeY<=3) {
    			mymove.Pos.Set(MAX(Maze->SizeZ*7.5,Maze->SizeY*10) + Maze->SizeX*10,85,5+(Maze->SizeZ/2)*10,90,-15,0);
   			} else {
   				mymove.Pos.Set(MAX(Maze->SizeZ*7.5,Maze->SizeY*10) + Maze->SizeX*10,5+(Maze->SizeY/2)*10,5+(Maze->SizeZ/2)*10,90,0,0);
   			}
   		}

    	movement_timer = time_now;

    	mymove.Turning = ((float)(mouse.x) - (float)(glWindow->SizeX) / 2.0) / 10000.0;
    	mymove.Tilting = -((float)(mouse.y) - (float)(glWindow->SizeY) / 2.0) / 10000.0;
    	
    	//mymove.Level(0.01);
    	if (GlobalMazeFlags[FLAG_AUTOLEVEL]) {
    		mymove.Pos.rotation.GetRightVector(&vec);
    		vec.Normalize();
    		if (vec.y < -0.01) mymove.Rolling -= vec.y*vec.y;
    		if (vec.y > 0.01)  mymove.Rolling += vec.y*vec.y;
    		mymove.Rolling *= 0.9;
   		}

    	//mymove.Update();
    	mymove.Pos.Turn(mymove.Turning);
     	mymove.Pos.Tilt(mymove.Tilting);
      mymove.Pos.Roll(mymove.Rolling);
    }

    // Draw startscreen
    glWindow->Clear();
    mymove.LoadAsCamera();
    Maze->Draw(mymove.Pos.vector.x, mymove.Pos.vector.y, mymove.Pos.vector.z);

    VECTORTEXT VTEXT;
    POSITION Pos;
    glColor4f(1,1,0,1);
    Pos.Set(0,0,0,180,0,0);
    Pos.vector = mymove.Pos.vector;
    Pos.vector.x -= 150;
    Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "3DMaze."); Pos.vector.y -= 5;
    VTEXT.Draw(Pos, 0.5, "Press space to continue.");

    Pos.Set(0,0,0,270,0,0);
    Pos.vector = mymove.Pos.vector;
    Pos.vector.y += 10;
    Pos.vector.z -= 150;
    VTEXT.Draw(Pos, 0.5, "Movement:"); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "a or up: Accelerate forward."); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "z or down: Decelerate."); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "q or left: Roll left (See also auto plane)."); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "w or right: Roll right (See also auto plane)."); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "space: Stop all movement."); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "Mouse up/down/left/right: Turn and tilt."); Pos.vector.y -= 6;
    VTEXT.Draw(Pos, 0.5, "Misc:"); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "Esc: Quit program."); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "1-9: Load mazes."); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "0 (Zero): Randomise current maze."); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "h: Show help arrows in maze (cost score)."); Pos.vector.y -= 6;
    VTEXT.Draw(Pos, 0.5, "Flags:"); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "s: Toggle solid/transparent walls."); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "p: Toggle auto plane. (Level to horizon)."); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "l: Toggle Lines on walls (edges, and cross)."); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "t: Toggle textures."); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "d: Toggle debug mode (show FPS and maze seed)."); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "g: Toggle ground and sky display."); Pos.vector.y -= 3;

    Pos.Set(0,0,0,90,0,0);
    Pos.vector = mymove.Pos.vector;
    Pos.vector.z += 150;
    Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "e-mail:"); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "bjarne.fich @ dragonslayer.dk"); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "http:"); Pos.vector.y -= 3;
    VTEXT.Draw(Pos, 0.5, "www.dragonslayer.dk"); Pos.vector.y -= 3;

   	// Cursor
    glColor4f(1,1,1,1);
    glWindow->printAt(mouse.x-3,mouse.y+8,"+");

    if (GlobalMazeFlags[FLAG_DEBUG]) {
    	glWindow->printAt(10,100,"FPS: %1.1f", fps);
    	glWindow->printAt(10,115,"Seed: %i", Maze->Seed);
   	}

    glWindow->Swap();
  }
  
  running = true;
  play_time = 0;
  help_time = 0;
  
  Log("Enter main loop...");
	while (running) {
		// Message handling
		if (PeekMessage(&messages, NULL, 0, 0, PM_REMOVE)) {
			switch(messages.message) {
        case WM_LBUTTONDOWN:
        	mouse.lbutton = true;
          break;
        case WM_LBUTTONUP:
        	mouse.lbutton = false;
          break;
        case WM_RBUTTONDOWN:
        	mouse.rbutton = true;
          break;
        case WM_RBUTTONUP:
        	mouse.rbutton = false;
          break;
        case WM_MOUSEMOVE:
          mouse.x = LOWORD(messages.lParam);
          mouse.y = HIWORD(messages.lParam);
          break;
        case WM_KEYDOWN:
        case WM_KEYUP:
          if (messages.message==WM_KEYDOWN) { keypress = true; } else { keypress = false; }
          switch (messages.wParam) {  // Keyboard keys
            case VK_ESCAPE:
              running = false;
              PostQuitMessage(0);
              break;
            case VK_UP:
              keys.up = keypress;
              break;
            case VK_DOWN:
              keys.down = keypress;
              break;
            case VK_LEFT:
              keys.left = keypress;
              break;
            case VK_RIGHT:
              keys.right = keypress;
              break;
            case 'a':
            case 'A':
              keys.forward = keypress;
              break;
            case 'z':
            case 'Z':
              keys.back = keypress;
              break;
            case 'q':
            case 'Q':
              keys.rollleft = keypress;
              break;
            case 'w':
            case 'W':
              keys.rollright = keypress;
              break;
            case ' ':
     					mymove.Moveing = 0;
							mymove.Rolling = 0;
              break;
            case 's':
            case 'S':
            	if (!keypress) GlobalMazeFlags[FLAG_WALL_SOLID] = !GlobalMazeFlags[FLAG_WALL_SOLID];
              break;
            case 'l':
            case 'L':
            	if (!keypress) GlobalMazeFlags[FLAG_WALL_LINES] = !GlobalMazeFlags[FLAG_WALL_LINES];
              break;
            case 't':
            case 'T':
            	if (!keypress) GlobalMazeFlags[FLAG_WALL_TEXTURES] = !GlobalMazeFlags[FLAG_WALL_TEXTURES];
              break;
            case 'p':
            case 'P':
            	if (!keypress) GlobalMazeFlags[FLAG_AUTOLEVEL] = !GlobalMazeFlags[FLAG_AUTOLEVEL];
              break;
            case 'd':
            case 'D':
            	if (!keypress) GlobalMazeFlags[FLAG_DEBUG] = !GlobalMazeFlags[FLAG_DEBUG];
              break;
            case 'g':
            case 'G':
            	if (!keypress) {
            		if ((GlobalMazeFlags[FLAG_GROUND]) && (GlobalMazeFlags[FLAG_SKY])) {
            			GlobalMazeFlags[FLAG_GROUND] = false;
            			GlobalMazeFlags[FLAG_SKY] = true;
            		} else if ((!GlobalMazeFlags[FLAG_GROUND]) && (GlobalMazeFlags[FLAG_SKY])) {
            			GlobalMazeFlags[FLAG_GROUND] = false;
            			GlobalMazeFlags[FLAG_SKY] = false;
            		} else if ((!GlobalMazeFlags[FLAG_GROUND]) && (!GlobalMazeFlags[FLAG_SKY])) {
            			GlobalMazeFlags[FLAG_GROUND] = true;
            			GlobalMazeFlags[FLAG_SKY] = false;
            		} else if ((GlobalMazeFlags[FLAG_GROUND]) && (!GlobalMazeFlags[FLAG_SKY])) {
            			GlobalMazeFlags[FLAG_GROUND] = true;
            			GlobalMazeFlags[FLAG_SKY] = true;
            		}
            	}
              break;
            case 'h':
            case 'H':
            	GlobalMazeFlags[FLAG_HELP_ARROWS] = keypress;
              break;
            case '1': if (!keypress) { Maze->Load("data/Maze1.xml"); ticks=9; } break;
            case '2': if (!keypress) { Maze->Load("data/Maze2.xml"); ticks=9; } break;
            case '3': if (!keypress) { Maze->Load("data/Maze3.xml"); ticks=9; } break;
            case '4': if (!keypress) { Maze->Load("data/Maze4.xml"); ticks=9; } break;
            case '5': if (!keypress) { Maze->Load("data/Maze5.xml"); ticks=9; } break;
            case '6': if (!keypress) { Maze->Load("data/Maze6.xml"); ticks=9; } break;
            case '7': if (!keypress) { Maze->Load("data/Maze7.xml"); ticks=9; } break;
            case '8': if (!keypress) { Maze->Load("data/Maze8.xml"); ticks=9; } break;
            case '9': if (!keypress) { Maze->Load("data/Maze9.xml"); ticks=9; } break;
            case '0': if (!keypress) { Maze->Seed=time_now; Maze->Build(); } break;
          } // Keyboard keys
          break;
        case WM_QUIT:
        	running = false;
          break;
        default:
          TranslateMessage(&messages);
          DispatchMessage(&messages);
          break;
      }
    }

    // Update the frame rate once per 1 sec second (CLK_TCK is clock ticks pr second)
    time_now = clock();
    ++frames;
    if(time_now - last_time > CLK_TCK*2) {
      fps = (frames*CLK_TCK)/(time_now-last_time);
      last_time = time_now;
      frames   = 0L;
    }

    // Movement and timer ticks (Updated 30 times pr second)
    if(time_now - movement_timer > CLK_TCK/30) {
    	ticks++;

    	if (ticks==10) {
    		if (Maze->SizeY<=3) {
    			mymove.Pos.Set(MAX(Maze->SizeZ*7.5,Maze->SizeY*10) + Maze->SizeX*10,85,5+(Maze->SizeZ/2)*10,90,-15,0);
   			} else {
   				mymove.Pos.Set(MAX(Maze->SizeZ*7.5,Maze->SizeY*10) + Maze->SizeX*10,5+(Maze->SizeY/2)*10,5+(Maze->SizeZ/2)*10,90,0,0);
   			}
   			play_time = 0;
   			help_time = 0;
   		}

    	movement_timer = time_now;

    	if ((keys.forward) || (keys.up)) mymove.Moveing += 0.05;
    	if ((keys.back) || (keys.down)) mymove.Moveing -= 0.05;
    	if ((keys.rollright) || (keys.right)) mymove.Rolling += 0.002;
    	if ((keys.rollleft) || (keys.left)) mymove.Rolling -= 0.002;

    	mymove.Turning = ((float)(mouse.x) - (float)(glWindow->SizeX) / 2.0) / 10000.0;
    	mymove.Tilting = -((float)(mouse.y) - (float)(glWindow->SizeY) / 2.0) / 10000.0;
    	
    	//mymove.Level(0.01);
    	if (GlobalMazeFlags[FLAG_AUTOLEVEL]) {
    		mymove.Pos.rotation.GetRightVector(&vec);
    		vec.Normalize();
    		if (vec.y < -0.01) mymove.Rolling -= vec.y*vec.y;
    		if (vec.y > 0.01)  mymove.Rolling += vec.y*vec.y;
    		mymove.Rolling *= 0.9;
   		}

    	//mymove.Update();
    	mymove.Pos.Turn(mymove.Turning);
     	mymove.Pos.Tilt(mymove.Tilting);
      mymove.Pos.Roll(mymove.Rolling);
      if (mymove.Moveing<-1) mymove.Moveing = -1;
      if (mymove.Moveing>3) mymove.Moveing = 3;
    	mymove.Pos.rotation.GetDirectionVector(&vec);
    	vec *= mymove.Moveing;
      vec = Maze->Collision(mymove.Pos.vector,vec);
      mymove.Pos.vector.x += vec.x;
      mymove.Pos.vector.y += vec.y;
      mymove.Pos.vector.z += vec.z;
      
      if ((mymove.Pos.vector.x>10) && (mymove.Pos.vector.x<((Maze->SizeX-1)*10)) && (mymove.Pos.vector.y>10) && (mymove.Pos.vector.y<((Maze->SizeY-1)*10)) && (mymove.Pos.vector.z>10) && (mymove.Pos.vector.z<((Maze->SizeZ-1)*10))) {
      	play_time++;
     	}
    	if (GlobalMazeFlags[FLAG_HELP_ARROWS]) help_time++;
    }

    // Drawing
    glWindow->Clear();
    mymove.LoadAsCamera();
    Maze->Draw(mymove.Pos.vector.x, mymove.Pos.vector.y, mymove.Pos.vector.z);

    // Score    
    glColor4f(1,1,1,1);
    min = (play_time)/(30*60);
    sec = ((play_time)/30) % 60;
    mic = ((play_time)/3) % 10;
   	glWindow->printAt(10,30,"Time: %2i:%02i.%i", min, sec, mic);
    min = (help_time)/(30*60);
    sec = ((help_time)/30) % 60;
    mic = ((help_time)/3) % 10;
   	glWindow->printAt(10,45,"Help: %2i:%02i.%i", min, sec, mic);
   	glWindow->printAt(10,60,"Used: %07i", (play_time+help_time*30) / 3);

   	// Cursor
    glColor4f(1,1,1,1);
    glWindow->printAt(mouse.x-3,mouse.y+8,"+");

    if (GlobalMazeFlags[FLAG_DEBUG]) {
    	glWindow->printAt(10,100,"FPS: %1.1f", fps);
    	glWindow->printAt(10,115,"Seed: %i", Maze->Seed);
   	}

   	glWindow->Swap();
  }

  Log("Cleanup...");
  delete Maze;
  delete TextureList;
  delete glWindow;

  Log("Exit...");
	return messages.wParam;
}

