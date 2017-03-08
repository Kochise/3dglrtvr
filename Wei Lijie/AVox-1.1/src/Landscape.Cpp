#include "Landscape.H"

#include <Math.H>
#include <stdlib.h>

#include "Textures.H"
#include "Common.h"

Voxel::Voxel()
{
	FreeFly=false;
	PixelSkip = 8;
	EyeShot = 2048;

	Wireframe= false;
	
	Xo = 0;
	Yo = 0;   
	Zo = 285;

	zsVox=1;

	HorizAngle = 0;
	FieldView = PI / 8;

	ViewScale = 32000;
	//HeightPrjScale = 256;
	HeightPrjScale = 2048;

	LoadMap("Terrain.Raw",HMap);

	Image.Load("Color.Tga");

	for(int i = 0; i< 6284; i++) {
		sin_t[i] = sin( i/1000.0 );
		cos_t[i] = cos( i/1000.0 );
	}

}

Voxel::~Voxel()   
{
}

void Voxel::Init(int width , int height)
{	
	//Width = GetSystemMetrics( SM_CXSCREEN );
	//Height = GetSystemMetrics( SM_CYSCREEN );
	Width = width;
	Height = height;

	
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D,  mTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, Image.Components, Image.Width, Image.Height, Image.Format, GL_UNSIGNED_BYTE, Image.cMap);

	//float fogColor[4] = {.11f, .11f, .11f, .25f};//
	float fogColor[4] = {0.8, 0.7, 1.0, 1.0};// fog color
    glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);

	//glFogf(GL_FOG_DENSITY, 1);
	//glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_START, EyeShot-EyeShot/3);
	glFogf(GL_FOG_END, EyeShot);
	glEnable(GL_FOG);


}

void Voxel::vLine(int x0, int y0, int x1, int y1, int z,  int s, int Distance)
{
	register float Texel = (float)( (ViewScale<<1) << MAP_SHIFT );

	register int State = 0, zs = z * s;

	// Numbers of voxel in one row 
	int vColNum = Width/(PixelSkip) + 1;

	int dx=(x1 - x0) / vColNum;
	int  dy=(y1 - y0) / vColNum;
	
	int u0, v0, u1, v1 ,a , b, h0, h1, h2, h3, h4, vHeight;
        
    // Precalculation value
	int pcShit = (1<<MAP_SHIFT)-1;
	int pcHeight = Height>>1;
	int Correction = (int)(Distance*SLOPE)<<1;

	for (int i=0; i<=vColNum; i++)
	//for (int i=0; i<= 1; i++)

	{
		
		b  = (y0 >> 8) & 255;
		a  = (x0 >>  8) & 255;
		
		u0 = (x0 >> 16) & pcShit;
		v0 = (y0 >> 16) & pcShit;
		u1 = (u0  +  1 ) & pcShit;
		v1 = (v0  +  1 ) & pcShit;

		v0 <<= (MAP_SHIFT);
		v1 <<= (MAP_SHIFT);
				
		h0 = HMap[u0+v0];
		h2 = HMap[u0+v1];
        h1 = HMap[u1+v0];
        h3 = HMap[u1+v1];

		// Perform bilinear filtering
		h0 = ((h0 << 8)-Correction) + a * (h1 - h0);
		h2 = ((h2 << 8)-Correction) + a * (h3 - h2);
		h4 = (h0 << 8) + b * (h2 - h0);

		// Calculate voxel projection
		vHeight = (((__int64)h4 * (__int64)s) >> 16);
		vHeight -= zs ;
		vHeight >>= 11;
		vHeight  += pcHeight;

		// Voxel above horizon?
		if (vHeight>Horizon[i])
		{
				if (State == 0)
				{
					glBegin( Wireframe? GL_LINE_STRIP : GL_TRIANGLE_STRIP);
                        {
							glTexCoord2f((float)LastYo[i-1] / Texel, (float)LastXo[i-1] / Texel);
                            glVertex3f( (i - 1) * PixelSkip,  Horizon[i-1],  -Distance);
							//glVertex3f( (i - 1) * PixelSkip,  Horizon[i-1],  -Distance);

							PolygonsInUse++;
                            }
                            State = 1;
		                }
						PolygonsInUse+=2;
						
						glTexCoord2f((float)y0/ Texel, (float)x0 / Texel);
						glVertex3f(i * PixelSkip, vHeight, -Distance);
						
						glTexCoord2f((float)LastYo[i] / Texel, (float)LastXo[i] / Texel);
						glVertex3f(i * PixelSkip, Horizon[i], -Distance);
                        
                        Horizon[i] = vHeight;
                }
                else
                {
                        if (State == 1)
                        {								
								glTexCoord2f((float)LastYo[i] / Texel, (float)LastXo[i] / Texel);
								glVertex3f(i * PixelSkip, Horizon[i], -Distance);
                                glEnd();
                                State = 0;
                        }
                }
        //  Log.Write("Black","Horizon[%i] = %i",i, Horizon[i]);
		/*if ( vHeight <= Horizon[i])
		{
          Log.Write("Black","vHeight <= Horizon[%i] ",i);
		};*/

		LastXo[i] = x0;
		LastYo[i] = y0;

		x0+=dx;
		y0+=dy;

	};


	if (State == 1)
        {
                glEnd();   
        }
};


// Angle a from point(X0, Y0)

void Voxel::RayCasting()
{	
	PolygonsInUse = 0;

	int Distance;

	for (Distance=0;Distance<MaxVoxels; Distance++)	 
	{
		Horizon[Distance]=0;
		if (Distance<MapSize)
		{
			LastXo[Distance] = 0;
			LastYo[Distance] = 0;
		}
	
	}

	int u0, v0, u1, v1 ,a , b, h0, h1, h2, h3, h4, vHeight;

	int x0=Xo, y0=Yo;

	b  = (y0 >> 8) & 255;
	a  = (x0 >>  8) & 255;
		
	u0 = (x0 >> 16) & ((1<<MAP_SHIFT)-1);
	v0 = (y0 >> 16) & ((1<<MAP_SHIFT)-1);
	u1 = (u0  +  1 ) & ((1<<MAP_SHIFT)-1);
	v1 = (v0  +  1 ) & ((1<<MAP_SHIFT)-1);

	v0 <<= (MAP_SHIFT);
	v1 <<= (MAP_SHIFT);
		
	h0 = HMap[u0 + v0];
	h2 = HMap[u0 + v1];
	h1 = HMap[u1 + v0];
	h3 = HMap[u1 + v1];

	// Perform bilinear filtering
	h0 = (h0 << 8) + a * (h1 - h0);
	h2 = (h2 << 8) + a * (h3 - h2);
	h4 = ((h0 << 8) + b * (h2 - h0))>>16;

	if (FreeFly==true)
	{
		Zo=h4;
	}

    t_zo = Zo + 50 ;

	glBindTexture(GL_TEXTURE_2D, mTexture);

	//#define Scale	ViewScale
	
	hf_s = int((HorizAngle-FieldView)*1000) % PI_2K, hf_a = int((HorizAngle+FieldView)*1000) % PI_2K;
		            
    h_h = Height*HeightPrjScale;

	for (Distance = 1; Distance < EyeShot; Distance+=zsVox+(Distance >> 6))
	{
		/*vLine( Xo+int(Distance*Scale*cos(HorizAngle-FieldView)),  Yo+int(Distance*Scale*sin(HorizAngle	- FieldView)),
					 Xo+int(Distance*Scale*cos(HorizAngle+FieldView)),  Yo+int(Distance*Scale*sin(HorizAngle+FieldView)), 
					 Zo-2*Distance+150,	 Height*HeightPrjScale/Distance,	Distance);*/

		d_s = Distance*ViewScale;
		
		//Position, directon , angle and distance
		//Restricted in viewport.
		
		vLine( Xo + int(d_s*cos_t[abs(hf_s)]),  Yo + int(d_s*( hf_s >= 0 ? sin_t[hf_s] :-sin_t[-hf_s] ) ),
					 Xo + int(d_s*cos_t[abs(hf_a)]),  Yo + int(d_s*( hf_a >= 0 ? sin_t[hf_a] :-sin_t[-hf_a] )), 
					 t_zo - ( Distance << 1),	 h_h/Distance,	Distance);

		/*vLine( Xo+int(d_s*cos_t[hf_s]),  Yo+int(d_s*sin_t[hf_s] ),
					 Xo+int(d_s*cos_t[hf_a]),  Yo+int(d_s*sin_t[hf_a] ), 
					 t_zo - ( Distance << 1),	 h_h/Distance,	Distance);*/
	}

}     
