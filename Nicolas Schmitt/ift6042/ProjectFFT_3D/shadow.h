#ifndef _SHADOW_H_
#define _DEFINE_H_

extern void RenderGround ( void );
extern int CgInit ( void );
extern void CgEnableProgram ( void );
extern void CgDisableProgram ( void );
extern void CgShutdown ( void );
extern void CgLoadUniformParameters ( const float lightPos[3], const float eyePos[3] );
extern void CgLoadVaryingParameters ( const float mesh[3], const float normal[3] );

#endif // _SHADOW_H_
