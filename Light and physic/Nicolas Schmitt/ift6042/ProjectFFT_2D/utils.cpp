/*
 *   Copyright (c) 2002 Nicolas Schmitt
 *
 *   This file is part of (s)Smoke.
 *
 *   (s)Smoke is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   (s)Smoke is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details (gpl.txt).
 *
 *   You should have received a copy of the GNU General Public License
 *   along with (s)Smoke; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "define.h"

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
	if ( globals.bUseConfinement ) {
		CheckMenuItem ( hMenu, IDM_SIMUL_VORTCONF, MF_CHECKED );
	} else {
		CheckMenuItem ( hMenu, IDM_SIMUL_VORTCONF, MF_UNCHECKED );
	}
	if ( globals.bLine ) {
		CheckMenuItem ( hMenu, IDM_LINE, MF_CHECKED );
	} else {
		CheckMenuItem ( hMenu, IDM_LINE, MF_UNCHECKED );
	}
	if ( globals.bFiltre ) {
		CheckMenuItem ( hMenu, IDM_SIMUL_FILTRE, MF_CHECKED );
	} else {
		CheckMenuItem ( hMenu, IDM_SIMUL_FILTRE, MF_UNCHECKED );
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
		return true;
	}
}

int GetFPS ( void )
{
	static LARGE_INTEGER lastTime = globals.startTime;
	LARGE_INTEGER		 currentTime;
	float				 fps;

	QueryPerformanceCounter ( &currentTime );

	fps = 1.0f*(float)globals.ticksPerSecond.QuadPart/((float)currentTime.QuadPart-(float)lastTime.QuadPart);

	lastTime = currentTime;
	return (int)fps;
}

void SaveGrids ( char *fileName )
{
	DWORD res, size;
	int i;
	HANDLE hFile = CreateFile ( fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL, 0 );
	if ( hFile == INVALID_HANDLE_VALUE ) {
		AddLine ( "Unable to save simulation.\r\n" );
		return;
	}

	WriteFile ( hFile, "(s)SmokeState", strlen ( "(s)SmokeState" ), &res, NULL );
	WriteFile ( hFile, &calculs.length[0], sizeof ( calculs.length[0] ), &res, NULL );
	WriteFile ( hFile, &calculs.length[1], sizeof ( calculs.length[0] ), &res, NULL );
	size = calculs.length[0] * calculs.length[1] * sizeof ( double );
	for ( i = 0; i < NDIM; i++ ) {
		WriteFile ( hFile, calculs.u1[i], size, &res, NULL );
		WriteFile ( hFile, calculs.f[i], size, &res, NULL );
	}
	WriteFile ( hFile, calculs.s1, size, &res, NULL );
	WriteFile ( hFile, calculs.t1, size, &res, NULL );
	WriteFile ( hFile, calculs.source, size, &res, NULL );
	CloseHandle ( hFile );
	AddLine ( "Simulation saved.\r\n" );
}

void LoadGrids ( char *fileName )
{
	DWORD res, size;
	int i; char buffer[512];
	int lg[2];

	HANDLE hFile = CreateFile ( fileName, GENERIC_READ, 0, 0, OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL, 0 );
	if ( hFile == INVALID_HANDLE_VALUE ) {
		AddLine ( "Unable to load simulation.\r\n" );
		return;
	}
	ReadFile ( hFile, buffer, strlen ( "(s)SmokeState" ), &res, NULL );
	if ( strncmp ( buffer, "(s)SmokeState", strlen ( "(s)SmokeState" ) ) != 0 ) {
		AddLine ( "Bad file format." );
		return;
	}
	ReadFile ( hFile, &lg[0], sizeof ( int ), &res, NULL );
	ReadFile ( hFile, &lg[1], sizeof ( int ), &res, NULL );
	if ( lg[0] != calculs.length[0] || lg[1] != calculs.length[1] ) {
		AddLine ( "Dimension non compatible\r\n" );
		return;
	}
	size = calculs.length[0] * calculs.length[1] * sizeof ( double );
	for ( i = 0; i < NDIM; i++ ) {
		ReadFile ( hFile, calculs.u1[i], size, &res, NULL );
		ReadFile ( hFile, calculs.f[i], size, &res, NULL );
	}
	ReadFile ( hFile, calculs.s1, size, &res, NULL );
	ReadFile ( hFile, calculs.t1, size, &res, NULL );
	ReadFile ( hFile, calculs.source, size, &res, NULL );
	CloseHandle ( hFile );
	AddLine ( "Simulation loaded.\r\n" );
}

int FileExists ( const char *filename )
{
	FILE *fp = fopen ( filename, "rb" );
	if ( !fp ) {
		return FALSE;
	}
	fclose ( fp );
	return TRUE;
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

void ExportAsRTFile ( char *fileName )
{
	FILE *fp = fopen ( fileName, "w" );
	fprintf ( fp, "GenImage 640 480\n" );
	fprintf ( fp, "AutoPerspective\n" );
	fprintf ( fp, "AutoLookAt\n\n" );
	fprintf ( fp, "BeginScene\n\n" );
	fprintf ( fp, "ClearColor 0 0 0\n" );
	fprintf ( fp, "AddAmbientLight .2 .2 .2\n" );
	fprintf ( fp, "Translate 0 0 -10\n" );
	for ( int i = 0; i < calculs.length[1]; i++ ) {
		for ( int j = 0; j < calculs.length[0]; j++ ) {
			float density = (float)calculs.s1[ARR(i,j)] > 1.0f ? 1.0f : (float)calculs.s1[ARR(i,j)];
			fprintf ( fp, "AddSmokeBox %d %d %d %d %d %d %f\n", i, j, 0, i+1, j+1, -1, density );
		}
	}
	fclose ( fp );
}
