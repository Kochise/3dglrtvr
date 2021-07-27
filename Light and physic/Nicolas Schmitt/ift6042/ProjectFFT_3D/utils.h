#ifndef _UTILS_H_
#define _UTILS_H_

extern void ErrorBox ( const char *format, ... );
extern void AddLine ( char *line );
extern void UpdateMyMenus ( void );
extern float GetFPS ( void );
extern int TimerInit ( void );
extern void LoadGrids ( const char *fileName );
extern void SaveGrids ( const char *fileName );
extern void TakeScreenshot ( void );
extern void TakeMovieShot ( void );
extern void ExportAsDevRT ( void );
extern void CreateRandomFields ( void );
extern inline int FileExists ( const char *filename );
extern void InverseMatrix ( float mat[16], float matout[16] );
extern void MatxVect ( float mat[16], float vect[4], float res[4] );
extern void GetTranslation ( float matrix[16], float v[4] );
extern void BuildLookAtMatrix ( float mat[16], float eye0, float eye1, float eye2,
							    float center0, float center1, float center2,
							    float up0, float up1, float up2 );
extern void BuildFrustumMatrix ( float m[16], float l, float r, float b, float t, float n, float f);
extern void BuildPerspectiveMatrix ( float m[16], float fovy, float aspect, float zNear, float zFar);

#endif // _UTILS_H_
