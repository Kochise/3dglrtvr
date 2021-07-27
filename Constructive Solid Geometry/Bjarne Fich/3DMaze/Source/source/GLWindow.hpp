#ifndef __GLWINDOW_HPP_
#define __GLWINDOW_HPP_

#include <windows.h>
#include <gl\gl.h>

#define DEPTH_PLANE 16000

class FRUSTUM {
  private:
    float frustumdata[6][4];
  public:
    FRUSTUM::FRUSTUM();
    FRUSTUM::~FRUSTUM();

    void FRUSTUM::ExtractFrustum();
    bool FRUSTUM::PointInFrustum(float x, float y, float z);
    bool FRUSTUM::SphereInFrustum(float x, float y, float z, float radius);
    bool FRUSTUM::CubeInFrustum(float x, float y, float z, float size);
};

class OPENGLWINDOW {
  private:
  	// Window
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    // Fonts
    GLuint FontBaseList;
    void BuildFont();
    void DeleteFont();
  public:
    int SizeX, SizeY;
    FRUSTUM *Frustum; // Used for clipping

    OPENGLWINDOW(char *configfilename, HINSTANCE hInstance, int nFunsterStil);
    ~OPENGLWINDOW();

    void Clear();// Clear buffers
    void Swap(); // Swap buffers
    void printAt(int x, int y, const char *fmt, ...);
};

#endif /* __GLWINDOW_HPP_ */

