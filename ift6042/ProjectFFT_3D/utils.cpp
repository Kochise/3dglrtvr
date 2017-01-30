#include "define.h"

void InverseMatrix ( float mat[16], float matout[16] )
{
	int i, ipivot, isave;
	int j, jrow, kcol, krow;
	int perm[4];
	float a, result[16];

	for ( i = 0; i < 4; i++ ) {
	    perm[i] = i;
		for ( j = 0; j < 4; j++ ) {
			if ( i == j ) {
				result[ i*4 + j ] = 1.0 ;
			} else {
				result[ i*4+ j ] = 0.0 ;
			}
		}
	}
    
	for ( j = 0; j < 4; j++ ) {
	    ipivot = perm[j];
	    for ( i = j + 1; i < 4; i++ ) {
			jrow = perm[i] ;
			a = (float) fabs ( mat[ ipivot*4 + j ] );
			if ( a < fabs ( mat[ jrow*4 + j ]) ) {
				ipivot = jrow ;
				isave = i ;
			}
	    }
    
	    if ( ipivot != perm[j] ) {
			perm[isave] = perm[j];
			perm[j] = ipivot;
	    }
    
		for ( kcol = j+1; kcol < 4; kcol++ ) {
			mat[ipivot*4+kcol] /= mat[ipivot*4+j];
		}
		for ( kcol=0; kcol < 4; kcol++ ) {
			result[ipivot*4+kcol] /= mat[ipivot*4+j];
		}
    
	    for ( i=j+1; i<4; i++ ) {
			jrow = perm[i] ;
    
			for ( kcol=j+1; kcol < 4; kcol++ ) {
			    mat[jrow*4+kcol] -= mat[ipivot*4+kcol] * mat[jrow*4+j] ;
			}
			for ( kcol=0; kcol < 4; kcol++ ) {
				result[jrow*4+kcol] -= result[ipivot*4+kcol] * mat[jrow*4+j] ;
			}
	    }
	}
    
	for ( i = 3; i >= 0; i-- ) {
	    jrow = perm[i];
		for ( kcol=0; kcol < 4; kcol++ ) {
			for ( j=i+1; j < 4; j++ ) {
				krow = perm[j] ;
				result[jrow*4+kcol] -= mat[jrow*4+j] * result[krow*4+kcol] ;
			}
		}
	}
    
	for ( i = 0; i < 4; i++ ) {
	    jrow = perm[i];
		for ( j=0; j<4; j++ ) {
			matout[i*4+j] = result[jrow*4+j];
		}
	}
    	
}

void MatxVect ( float mat[16], float vect[4], float res[4] )
{
	for ( int i = 0; i < 4; i++ ) {
		res[i] = 0.0;
		for ( int j = 0; j < 4; j++ ) {
			res[i] += mat[ i*16 + j ] * vect[j];
		}
	}
}

void GetTranslation ( float matrix[16], float v[4] )
{
	v[0] = matrix[0*16+3];
	v[1] = matrix[1*16+3];
	v[2] = matrix[2*16+3];
}

/*
void BuildLookAtMatrix ( float mat[16], float eye0, float eye1, float eye2,
						 float center0, float center1, float center2,
						 float up0, float up1, float up2 )
{
	float x[3], y[3], z[3], mag;

	z[0] = eye0 - center0;
	z[1] = eye1 - center1;
	z[2] = eye2 - center2;

	mag = (float) sqrt ( z[0] * z[0] + z[1] * z[1] + z[2] * z[2] );
	if ( mag != 0 ) {
		z[0] /= mag;
		z[1] /= mag;
		z[2] /= mag;
	}

	y[0] = up0; y[1] = up1; y[2] = up2;

	x[0] =  y[1] * z[2] - y[2] * z[1];
	x[1] = -y[0] * z[2] + y[2] * z[0];
	x[2] =  y[0] * z[1] - y[1] * z[0];

	y[0] =  z[1] * x[2] - z[2] * x[1];
	y[1] = -z[0] * x[2] + z[2] * x[0];
	y[2] =  z[0] * x[1] - z[1] * x[0];

	mag = (float) sqrt( x[0] * x[0] + x[1] * x[1] + x[2] * x[2] );
	if ( mag != 0 ) {
		x[0] /= mag;
		x[1] /= mag;
		x[2] /= mag;
	}

	mag = (float) sqrt( y[0] * y[0] + y[1] * y[1] + y[2] * y[2] );
	if ( mag != 0 ) {
		y[0] /= mag;
		y[1] /= mag;
		y[2] /= mag;
	}

#define M(row,col)  mat[col*4+row]
	M(0,0) = x[0];  M(0,1) = x[1];  M(0,2) = x[2];  M(0,3) = - x[0] * eye0 - x[1] * eye1 - x[2] * eye2;
	M(1,0) = y[0];  M(1,1) = y[1];  M(1,2) = y[2];  M(1,3) = - y[0] * eye0 - y[1] * eye1 - y[2] * eye2;
	M(2,0) = z[0];  M(2,1) = z[1];  M(2,2) = z[2];  M(2,3) = - z[0] * eye0 - z[1] * eye1 - z[2] * eye2;
	M(3,0) = 0.0;   M(3,1) = 0.0;   M(3,2) = 0.0;   M(3,3) = 1.0;
#undef M

}
*/

void BuildFrustumMatrix ( float m[16], float l, float r, float b, float t, float n, float f)
{
	m[0] = ( 2.0f * n ) / ( r - l );
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = ( 2.0f * n ) / ( t - b );
	m[6] = 0.0f;
	m[7] = 0.0f;

	m[8] = ( r + l ) / ( r - l );
	m[9] = ( t + b ) / ( t - b );
	m[10] = - ( f + n ) / ( f - n );
	m[11] = - 1.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = - ( 2.0f * f * n) / ( f - n );
	m[15] = 0.0f;
}

void BuildPerspectiveMatrix ( float m[16], float fovy, float aspect, float zNear, float zFar)
{
	float xmin, xmax, ymin, ymax;

	ymax = zNear * (float) tan ( fovy * M_PI / 360.0 );
	ymin = -ymax;

	xmin = ymin * aspect;
	xmax = ymax * aspect;

	BuildFrustumMatrix ( m, xmin, xmax, ymin, ymax, zNear, zFar );
}

/*
double ProduitScalaire ( double vect1[3], double vect2[3] )
{
	return ( vect1[0] * vect2[0] + vect1[1] * vect2[1] + vect1[2] * vect2[2] );
}
*/

void ErrorBox ( const char *format, ... )
{
	va_list ap;
	char buffer[2048];
  
	va_start ( ap, format );
	vsprintf ( buffer, format, ap );
	MessageBox ( globals.hwnd, buffer, "Error", MB_ICONERROR | MB_OK );
	va_end ( ap );
}

void AddLine (  char *line )
{
	int taille;
	taille = GetWindowTextLength ( globals.hEdit );
	SendMessage ( globals.hEdit, EM_SETSEL, taille, taille );
	SendMessage ( globals.hEdit, EM_REPLACESEL, FALSE, (LPARAM)line );
	SendMessage ( globals.hEdit, EM_SCROLL, SB_PAGEDOWN, 0 );
}

void UpdateMyMenus ( void )
{
	static HMENU hMenu = NULL;
	if ( hMenu == NULL ) {
		hMenu = GetMenu ( globals.hwnd );
	}
	if ( globals.bSimulate ) {
		EnableMenuItem ( hMenu, IDM_START, MF_GRAYED );
		EnableMenuItem ( hMenu, IDM_STOP, MF_ENABLED );
		EnableMenuItem ( hMenu, IDM_SIMUL_FPS, MF_ENABLED );
	} else {
		EnableMenuItem ( hMenu, IDM_START, MF_ENABLED );
		EnableMenuItem ( hMenu, IDM_STOP, MF_GRAYED );
		EnableMenuItem ( hMenu, IDM_SIMUL_FPS, MF_GRAYED );
	}
	if ( globals.bVelocity ) {
		CheckMenuItem ( hMenu, IDM_SIMUL_VIT, MF_CHECKED );
	} else {
		CheckMenuItem ( hMenu, IDM_SIMUL_VIT, MF_UNCHECKED );
	}
	if ( globals.bUseTexture ) {
		CheckMenuItem ( hMenu, IDM_SIMUL_TEXT, MF_CHECKED );
	} else {
		CheckMenuItem ( hMenu, IDM_SIMUL_TEXT, MF_UNCHECKED );
	}
	if ( globals.bLine ) {
		CheckMenuItem ( hMenu, IDM_LINE, MF_CHECKED );
	} else {
		CheckMenuItem ( hMenu, IDM_LINE, MF_UNCHECKED );
	}
	if ( globals.bDrawBox ) {
		CheckMenuItem ( hMenu, IDM_DRAWBOX, MF_CHECKED );
	} else {
		CheckMenuItem ( hMenu, IDM_DRAWBOX, MF_UNCHECKED );
	}
}

int TimerInit ( void )
{
	// Est-ce que le system possede un timer
	// haute resolution
	if ( !QueryPerformanceFrequency ( &globals.ticksPerSecond ) ) {
		return FALSE;
	} else 	{
		// Oui ? Et ben on va s'en servir pour savoir
		// le nombre d'image qu'on affiche par seconde
		QueryPerformanceCounter ( &globals.startTime );
		return TRUE;
	}
}

float GetFPS ( void )
{
	static LARGE_INTEGER lastTime = globals.startTime;
	LARGE_INTEGER		 currentTime;
	float				 fps;

	QueryPerformanceCounter ( &currentTime );

	fps = 1.0f*(float)globals.ticksPerSecond.QuadPart/((float)currentTime.QuadPart-(float)lastTime.QuadPart);

	lastTime = currentTime;
	return fps;
}

void ExportAsDevRT ( void )
{
	char buffer[1024];
	DWORD res;
	
	HANDLE hFile = CreateFile ( "smoke.ray", GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL, 0 );

	if ( hFile == INVALID_HANDLE_VALUE ) {
		AddLine ( "Unable to export simulation.\r\n" );
		return;
	}

	strcpy ( buffer, "/* Exported by (s)Smoke */\r\n\r\n" );
	WriteFile ( hFile, buffer, strlen ( buffer ), &res, NULL );
	sprintf ( buffer, "BeginSmoke %d %d\r\n", 1, calculs.size );
	WriteFile ( hFile, buffer, strlen ( buffer ), &res, NULL );

	double density;
	
	for ( int i = 0; i < calculs.size; i++ ) {
		for ( int j = 0; j < calculs.size; j++ ) {
			for ( int k = 0; k < calculs.size; k++ ) {
				density = calculs.s1[ARR(i,j,k)];
				if ( density > 1.0 ) {
					density = 1.0;
				}
				sprintf ( buffer, "  AddVoxel %lf\r\n", density );
				WriteFile ( hFile, buffer, strlen ( buffer ), &res, NULL );
			}
		}
	}

	WriteFile ( hFile, "EndSmoke\n", strlen ( "EndSmoke\r\n" ), &res, NULL );

	CloseHandle ( hFile );
	AddLine ( "Simulation exported.\r\n" );
}

void SaveGrids ( const char *fileName )
{
	DWORD res, size;
	int i;
	HANDLE hFile = CreateFile ( fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL, 0 );
	if ( hFile == INVALID_HANDLE_VALUE ) {
		AddLine ( "Unable to save simulation.\r\n" );
		return;
	}

	WriteFile ( hFile, "(s)SmokeState3D", strlen ( "(s)SmokeState3D" ), &res, NULL );
	WriteFile ( hFile, &calculs.size, sizeof ( calculs.size ), &res, NULL );
	size = calculs.size * calculs.size * calculs.size * sizeof ( double );
	for ( i = 0; i < NDIM; i++ ) {
		WriteFile ( hFile, calculs.u1[i], size, &res, NULL );
		WriteFile ( hFile, calculs.f[i], size, &res, NULL );
	}
	WriteFile ( hFile, calculs.s1, size, &res, NULL );
	WriteFile ( hFile, calculs.source, size, &res, NULL );
	CloseHandle ( hFile );
	AddLine ( "Simulation saved.\r\n" );
}

void LoadGrids ( const char *fileName )
{
	DWORD res, size;
	int i; char buffer[512];
	int lg;

	HANDLE hFile = CreateFile ( fileName, GENERIC_READ, 0, 0, OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL, 0 );
	if ( hFile == INVALID_HANDLE_VALUE ) {
		AddLine ( "Unable to load simulation.\r\n" );
		return;
	}
	ReadFile ( hFile, buffer, strlen ( "(s)SmokeState3D" ), &res, NULL );
	if ( strncmp ( buffer, "(s)SmokeState3D", strlen ( "(s)SmokeState3D" ) ) != 0 ) {
		AddLine ( "Bad file format." );
		return;
	}
	ReadFile ( hFile, &lg, sizeof ( int ), &res, NULL );
	if ( lg != calculs.size ) {
		AddLine ( "Dimension non compatible\r\n" );
		return;
	}
	size = calculs.size * calculs.size * calculs.size * sizeof ( double );
	for ( i = 0; i < NDIM; i++ ) {
		ReadFile ( hFile, calculs.u1[i], size, &res, NULL );
		ReadFile ( hFile, calculs.f[i], size, &res, NULL );
	}
	ReadFile ( hFile, calculs.s1, size, &res, NULL );
	ReadFile ( hFile, calculs.source, size, &res, NULL );
	CloseHandle ( hFile );
	AddLine ( "Simulation loaded.\r\n" );
}

inline int FileExists ( const char *filename )
{
	FILE *fp = fopen ( filename, "rb" );
	if ( !fp ) {
		return FALSE;
	}
	fclose ( fp );
	return TRUE;
}

void TakeMovieShot ( void )
{
	static int cur_img = 0;
	char filename[MAX_PATH];

	CreateDirectory ( "movie", NULL );
	sprintf ( filename, "movie/frame%03i.bmp", cur_img );

	UINT img;
	ilGenImages ( 1, &img );
	ilBindImage ( img );
	ilutGLScreen();
	ilSaveImage ( filename );
	ilDeleteImages ( 1, &img );

	cur_img++;
}

void TakeScreenshot ( void )
{
	static int cur_shot = 0;
	char filename[MAX_PATH];

	CreateDirectory ( "screenshot", NULL );
	sprintf ( filename, "screenshot/screen%03i.jpg", cur_shot );

	while ( FileExists ( filename ) ) {
		sprintf ( filename, "screenshot/screen%03i.jpg", ++cur_shot );
	}

	UINT img;
	ilGenImages ( 1, &img );
	ilBindImage ( img );
	ilutGLScreen();
	ilSaveImage ( filename );
	ilDeleteImages ( 1, &img );

	char buffer[512];
	sprintf ( buffer, "Screenshot : %s\r\n", filename );
	AddLine ( buffer );
	
	cur_shot++;
}

void CreateRandomFields ( void )
{
	int randx = rand() % calculs.size;
	int randy = rand() % calculs.size;
	int randz = rand() % calculs.size;
	calculs.source[ARR(randx, randy, randz)] = 15.0;
	calculs.f[0][ARR(randx, randy, randz)] = 3 * ( (double)rand() / (double)RAND_MAX - 0.5 );
	calculs.f[1][ARR(randx, randy, randz)] = 3 * ( (double)rand() / (double)RAND_MAX - 0.5 );
	calculs.f[2][ARR(randx, randy, randz)] = 3 * ( (double)rand() / (double)RAND_MAX - 0.5 );
}
