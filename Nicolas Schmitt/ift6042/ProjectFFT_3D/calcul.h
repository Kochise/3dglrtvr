#ifndef _CALCUL_H_
#define _CALCUL_H_

extern void calcul ( void );
extern void RenderScene ( void );
extern float InitModel ( void );
extern void FreeModel ( void );
extern void ResetSimulation ( void );
extern void HitBox ( const POINT *p );
extern void DrawBox ( void );
extern void HandleKeyboard ( void );
extern void MakeSources();
extern void MakeBallSource ( void );

#endif // _CALCUL_H_
