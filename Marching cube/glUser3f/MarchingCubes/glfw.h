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

#ifndef _GLFW_H_

#define _GLFW_H_

BOOL glfwInit(HINSTANCE hInstance, HWND hWnd);

GLvoid glfwDeInit(void);

GLvoid glfwRender(void);

GLvoid glfwUpdate(bool keys[256]);

GLvoid glfwActivate(void);

GLvoid glfwDeActivate(void);

GLvoid Terminate(void);

#endif