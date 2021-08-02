/*
  XCruiser(OpenGL) - a filesystem visualization utility
  main.c: main routine
    
  Copyright (C) 2013 Kei Taneishi

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "xcruiser.h"
#define SPEED 3

star Universe;

// Exit with message describing command line format
static void usage()
{
    printf("xcruiser - version 0.1\n");
    printf("usage: xcruiser [-options ...]\n");
    printf("    -background color    background color\n");
    printf("    -geometry geom       window geometry\n");
    printf("    -titlemode           informative title bar\n"); // false
    printf("    -viewall             view all files\n"); // false
    printf("    -delayticks n        adjust delays (milliseconds)\n"); // 50
    exit(1);
}

void display()
{
    render();

    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (double)width/height, 0.001, 30.0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key){
    case ' ': add_velocity(0.05*SPEED); break;
    case 'B': add_velocity(-0.1*SPEED); break;
#if DEBUG_DRAW
    case 'd': debug(); break;
#endif
    case 'f': freeze(); break;
    case 'h': autopilot("/home/user"); break;
    case 'i': get_info(); break;
    case 'm': toggle_ui("measure"); break;
    case 'n': autopilot(""); break;
    case  13: // [Enter]
        if (glutGetModifiers() & GLUT_ACTIVE_ALT){
            glutFullScreenToggle();
            break;
        }
    case 'o': open_file(); break;
    case 'v':
        toggle_ui("info");
        toggle_ui("measure");
        toggle_ui("label"); break;
    case 'x': flyingmode(); break;
    case 'z': reposition(); break;
    case  27: // [ESC]
    case 'q': exit(0);
    }
}

void keyboard_up(unsigned char key, int x, int y)
{
    move_cursor_to(0,0);
    brake(0.1);
}

void special(int key, int x, int y)
{
    switch(key){
    case GLUT_KEY_LEFT: move_cursor(-0.2*SPEED,0); break;
    case GLUT_KEY_RIGHT: move_cursor(0.2*SPEED,0); break;
    case GLUT_KEY_UP: move_cursor(0,-0.2*SPEED); break;
    case GLUT_KEY_DOWN: move_cursor(0,0.2*SPEED); break;
    }
}

void special_up(int key, int x, int y)
{
    move_cursor_to(0,0);
    brake(0.1);
}

void mouse(int button, int state, int x, int y)
{
    switch(button){
    case GLUT_LEFT_BUTTON:
        switch (state){
        case GLUT_UP: brake(0.1); break;
        case GLUT_DOWN: 
            if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) add_velocity(0.1*SPEED);
            else add_velocity(0.05*SPEED);
            break;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        switch (state){
        case GLUT_UP: brake(0.1); break;
        case GLUT_DOWN:
            if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) add_velocity(-0.1*SPEED);
            else add_velocity(-0.05*SPEED);
            break;
        }
        break;
    case GLUT_MIDDLE_BUTTON:
        switch (state){
        case GLUT_DOWN: open_file(); break;
        }
        break;
    }
}

void motion(int x, int y)
{
    move_cursor(x,y);
}

int main(int argc, char *argv[])
{
    int width = 900, height = 900;
    if (argc != 1) usage();
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 50);
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    glutCreateWindow("XCruiser - filesystem visualization utility");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);
    glutSpecialFunc(special);
    glutSpecialUpFunc(special_up);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);

    init_screen();
    init_universe();

    reshape(width, height);
    glutMainLoop();
}
