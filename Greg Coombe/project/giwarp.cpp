//============================================================================
// example_basic.cpp : most basic GLVU example
//============================================================================
#include <glvu.hpp>

//----------------------------------------------------------------------------
// USER-PROVIDED INCLUDES
//----------------------------------------------------------------------------
#include <GL/glut.h>
#include <objmodel/objmodel.hpp>
#include "illuminate.h"
#include "GLModel.h"
#include "ColorCycler.h"

//----------------------------------------------------------------------------
// GLOBALS
//----------------------------------------------------------------------------
GLVU glvu;
Object *Obj;
ColorCycler color_cycler;
//ColorCycler pos_cycler;

int frame_count = 0;
bool color_cycle = false;
bool light_cycle = false;
//bool light_cycle = true; // For disco demo
bool moving = false;
float move_speed = 5.0;
Vec3f dir( 1.0, 1.0, 1.0 );

extern int width_;
extern int height_;


//----------------------------------------------------------------------------
// USER-PROVIDED DRAWING ROUTINE
//----------------------------------------------------------------------------
void userDisplayFunc0()
{
  frame_count++;
  if ( frame_count == 20 ){
    float t = glvu.GetFPS();
    //printf( "Time: %f\n", t );
    frame_count = 0;
  }
  
  // If we are relighting the scene, get the next color
  if ( color_cycle )
    set_colors( color_cycler.getNextColor(0.1) );
  if ( light_cycle )
    set_lights();
  
  if ( moving )
    move_objects( Vec3f( move_speed*dir.x, 0.0, move_speed*dir.z) );

  glvu.BeginFrame();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Obj->Display();  

  glvu.EndFrame();
}


//----------------------------------------------------------------------------
// USER-PROVIDED MENU HANDLING ROUTINE
//----------------------------------------------------------------------------
void userMenuHandler(int value)
{
  GLModel* gm = (GLModel*)Obj;
  char filename[50];

  switch(value)
  {
    case 0:
      // Do work neccessary to compute the global illumination solution.
      printf("Computing Global Illumination Information...\n");

      glvu.BeginFrame();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// for demo purposes; clear screen
      glvu.EndFrame();

      //glvu.SelectCam( 2 ); // switch to alternate cam
      visibility();
      illuminate();
      //glvu.SelectCam( 0 ); // switch back to original cam
      //glvu.AllCamsResetToOrig();
      //light_cycle = true;
      userDisplayFunc0();
      break;
    case 1:
      // Reset all of the colors of the polygons to be random
      printf("Generating random colors\n");
      gm->generateRandomColors();
      userDisplayFunc0();
      break;
    case 2:
      // Step through each of the cameras at the sample points
      printf("Getting next camera...\n");
      getVertexCam();
      break;
    case 3:
      color_cycle = !color_cycle;
      break;
    case 4:
      light_cycle = !light_cycle;
      break;
    case 5:
      // darken lights
      gm->darken();
      illuminate();
      break;
    case 6:
      // brighten lights
      gm->brighten();
      illuminate();
      break;
    case 7:
      // Load form factors
      cout << "Load form factors from:";
      cin >> filename;
      gm->matrix_->load( filename );
      cout << "Loaded!" << endl;
      illuminate();
      break;
    case 8:
      // Save form factors
      cout << "Save form factors to:";
      cin >> filename;
      gm->matrix_->dump( filename );
      cout << "Saved!" << endl;
      break;
    case 9:
      moving = !moving;
      break;
      
  };
}


//----------------------------------------------------------------------------
// USER-PROVIDED KEYBOARD HANDLING ROUTINE
//----------------------------------------------------------------------------
void userKeyboardFunc( unsigned char Key, int x, int y)
{
  switch(Key)
  {
    case 'l':
      userMenuHandler( 0 );
      return;
      break;
    case 'k':
      userMenuHandler( 2 );
      return;
      break;
    case 'r':
      userMenuHandler( 3 );
      return;
      break;
    case 'n':
      userMenuHandler( 4 );
      return;
      break;
    case '<':
      userMenuHandler( 5 );
      return;
      break;
    case '>':
      userMenuHandler( 6 );
      return;
      break;
    case 'm':
      userMenuHandler( 9 );
      return;
      break;

    // Now make some control keys for moving around the occluder
    case 'w':
      dir = Vec3f(1.0, 0.0, 0.0);
      return;
      break;
    case 'a':
      dir = Vec3f(0.0, 0.0, -1.0);
      return;
      break;
    case 's':
      dir = Vec3f(0.0, 0.0, 1.0);
      return;
      break;
    case 'z':
      dir = Vec3f(-1.0, 0.0, 0.0);
      return;
      break;


  };

  // OPTIONAL: CALL THE DEFAULT SUPERCLASS GLVU KEYBOARD HANDLER
  GLVU::DefaultKeyboardFunc(Key, x, y);
}

void userIdleFunc(){
  glutPostRedisplay();
}

void userReshapeFunc( int WW, int WH ){
  cout << "Reshaping to " << WW << "x" << WH << endl;
  width_ = WW;
  height_ = WH;
  glViewport(0, 0, WW, WH);  
  glvu.AllCamsPerspectiveAspectChange((GLfloat)WW/(GLfloat)WH);
  glutPostRedisplay();
}


//----------------------------------------------------------------------------
// MAIN ROUTINE. INIT USER OBJECTS AND ACTIVATE VIEWER.
//----------------------------------------------------------------------------
void main()
{
  // initialize random number generator
  srand( (unsigned)time( NULL ));

  //--------------------------------------------------------------------------
  // TO USE THE VIEWER:
  // (1) Instantiate some global GLVUs (one for each window). See top of file.
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  // (2) Init each viewer by specifying menu string, visual mode mask, 
  //     and starting position and extents of the window.
  //     After Init, perform any OpenGL initializations and
  //     initialize the viewer cameras.
  //--------------------------------------------------------------------------

  glvu.Init("Quickie Radiosity",
            GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA,
            50,50,256,256);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);

  glutDisplayFunc(userDisplayFunc0);
  glutIdleFunc(userIdleFunc);
  glutKeyboardFunc(userKeyboardFunc);
  glutReshapeFunc(userReshapeFunc);

  //Obj = new GLModel( "models/occlude.tri" );
  //Obj = new GLModel( "models/tie.tri" );
  //Obj = new GLModel( "models/two.tri" );
  //Obj = new GLModel( "models/tris3.tri" );
  //Obj = new GLModel( "models/tris2.tri" );

  //Obj = new GLModel( "models/venus.tri" );
  //Obj = new GLModel( "models/tesselate2.tri" );  // a simple scene
  Obj = new GLModel( "models/cornell.tri" ); // Cornell box
  //Obj = new GLModel( "models/cornell_cranked.tri" ); // Cornell box (high rez)
  //Obj = new GLModel( "models/cornell_sphere.tri" ); // Cornell box with sphere
  //Obj = new GLModel( "models/box.tri" );   // hypersheet

  
  init( ((GLModel*)Obj)->slist_.size() );

  // CREATE THE NEW SUBMENU TO BE ADDED TO THE GLVU DEFAULT MENU
  int glUserMenu = glutCreateMenu(userMenuHandler);
  glutAddMenuEntry("Compute Global Illumination",0);
  glutAddMenuEntry("Random Colors",1);
  glutAddMenuEntry("Lookfrom Polygon",2);
  glutAddMenuEntry("Color Cycle",3);
  glutAddMenuEntry("Light Cycle",4);
  glutAddMenuEntry("Darken",5);
  glutAddMenuEntry("Brighten",6);
  glutAddMenuEntry("Load Form Factors",7);
  glutAddMenuEntry("Save Form Factors",8);
  
  // ADD USER SUBMENU TO GLVU MAIN MENU
  int MainMenuID = glvu.GetMainMenuID();
  glutSetMenu(MainMenuID);
  glutAddSubMenu("User Submenu",glUserMenu);

  /*
  Vec3f m=Obj->Min, M=Obj->Max, LookAtCntr=(m+M)*0.5;
  Vec3f Eye = (-m+M)*1.5;
  Vec3f Up(0,1,0);
  float Yfov = 45;
  float Aspect = 1;     // WIDTH OVER HEIGHT
  float Near = 0.1f;    // NEAR PLANE DISTANCE RELATIVE TO MODEL DIAGONAL LENGTH
  float Far = 10.0f;    // FAR PLANE DISTANCE (ALSO RELATIVE)
  glvu.SetAllCams(m,M, Eye,LookAtCntr,Up, Yfov,Aspect, Near,Far);
  */
  
  Vec3f m=Obj->Min, M=Obj->Max;
  Vec3f LookAtCntr(279.840, 280.579, -654.389);
  Vec3f Eye(279.846, 280.585, -655.389);
  Vec3f Up(0,1,0);
  float Yfov = 45;
  float Aspect = 1;     // WIDTH OVER HEIGHT
  float Near = 0.1f;    // NEAR PLANE DISTANCE RELATIVE TO MODEL DIAGONAL LENGTH
  float Far = 10.0f;    // FAR PLANE DISTANCE (ALSO RELATIVE)
  glvu.SetAllCams(m,M, Eye,LookAtCntr,Up, Yfov,Aspect, Near,Far);

  glDisable( GL_LIGHTING );
  // Set up a polygon offset for rendering the shadow
  glPolygonOffset(0.2, 0.1);

  //--------------------------------------------------------------------------
  // (3) start the viewer event loop.
  //--------------------------------------------------------------------------
  glvu.StartFPSClock();
  //glEnable(GL_CULL_FACE);
  glutMainLoop();
}
