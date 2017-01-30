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

int StartRecording ( void )
{
	static int cur_mov = 0;
	char filename[MAX_PATH];

	CreateDirectory ( "movie", NULL );
	sprintf ( filename, "movie/movie%03i.avi", cur_mov );

	while ( FileExists ( filename ) ) {
		sprintf ( filename, "movie/movie%03i.avi", ++cur_mov );
	}

	globals.movie = new AVIMovie;

	return globals.movie->CreateMovie ( filename, 15 );
}

void StopRecording ( void )
{
	globals.movie->FinishMovie();
	delete globals.movie;
	globals.movie = NULL;
}

int AVIMovie::CreateMovie ( char *filename, int rate )
{
	this->curFrame = 0;

	AVIFileOpen ( &this->file, filename, OF_WRITE | OF_CREATE, NULL );

	UINT img;
	ilGenImages ( 1, &img );
	ilBindImage ( img );
	ilutGLScreen();
	this->height = ilGetInteger ( IL_IMAGE_HEIGHT );
	this->width  = ilGetInteger ( IL_IMAGE_WIDTH );
	this->frameSize = ilGetInteger ( IL_IMAGE_SIZE_OF_DATA );
	ilutGetBmpInfo ( (BITMAPINFO*)&this->bmpInfo );
	ilDeleteImages ( 1, &img );

	memset ( &this->asi, 0, sizeof ( asi ) );
	this->asi.fccType = streamtypeVIDEO;
	this->asi.fccHandler = 0;
	this->asi.dwScale = 1;
	this->asi.dwRate = rate;
	//this->asi.dwLength = number / g_rate; // <- ???
	this->asi.rcFrame.right = width;
	this->asi.rcFrame.bottom = height;
    this->asi.dwSuggestedBufferSize = this->frameSize;
	this->asi.dwQuality = -1;

	AVIFileCreateStream ( this->file, &this->stream, &this->asi );

	AVICOMPRESSOPTIONS *aopts[1];
	AVICOMPRESSOPTIONS opts;
	aopts[0] = &opts;
	if ( !AVISaveOptions ( NULL, 0, 1, &this->stream, (LPAVICOMPRESSOPTIONS FAR *) &aopts) ) {
		return FALSE;
	}

	AVIMakeCompressedStream ( &this->streamcmp, this->stream, &opts, NULL );

	AVIStreamSetFormat ( this->streamcmp, 0, &this->bmpInfo, sizeof ( this->bmpInfo ) );

	return TRUE;

}

void AVIMovie::FinishMovie ( void )
{
	AVIStreamRelease ( this->streamcmp );
	AVIStreamRelease ( this->stream );
	AVIFileRelease ( this->file );
	AVIFileExit();
}

void AVIMovie::TakeShot ( void )
{
	ILuint img;
	ilGenImages ( 1, &img );
	ilBindImage ( img );
	ilutGLScreen();
	ilConvertImage ( IL_BGR, IL_UNSIGNED_BYTE );
	AVIStreamWrite ( this->streamcmp, this->curFrame, 1, ilGetData(), this->frameSize, AVIIF_KEYFRAME, NULL, NULL );
	ilDeleteImages ( 1, &img );
	this->curFrame++;
}