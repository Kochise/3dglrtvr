#include "Engine.H"
#include "Textures.H"
#include "Log.H"

VoxelEngine::VoxelEngine()
{
	Speed=0;
	SLOPE=1;
}

VoxelEngine::~VoxelEngine()
{
}

void VoxelEngine::Render()
{
	glColor3f(1,1,1);
	register int h_k = HorizAngle*1000;
	// Movement around landscape
	Xo+=Speed*(cos_t[abs(h_k)]);
	Yo+=Speed*( HorizAngle >= 0 ? sin_t[h_k] : -sin_t[-h_k]);

	// Ray-racing, another words - landscape rendering
	RayCasting();
	
	// Debug information
	glColor3f(0,0.75f,0);
	Cout.glPrint(0, 16, 1, "Lijie Wei, RMIT, Mel Aus, based on Andrey Serba's 6dof Voxel landscape");
	glColor3f(0.75f,0,0);
	Cout.glPrint(0, 32, 1, "%3.1f fps",Perfomance.DoTimerStuff());
	glColor3f(1,1,1);
	Cout.glPrint(0, 48, 1, "Triangles: %i ",PolygonsInUse);
	switch (PixelSkip)
	{
		case 4: Cout.glPrint(0, 64, 1, "Detalization (1,2,3,4): Best"); break;
		case 8: Cout.glPrint(0, 64, 1, "Detalization (1,2,3,4): Normal");	break;
		case 16: Cout.glPrint(0, 64, 1, "Detalization (1,2,3,4): Low"); break;
		case 32: Cout.glPrint(0, 64, 1, "Detalization (1,2,3,4): Very Low");
	}
	Cout.glPrint(0, 80, 1, "Speed (mouse buttons): %2.2f", Speed/8192.f);
	Cout.glPrint(0, 96, 1, "Visible area (z,x): %i", EyeShot);
	if (Wireframe)	Cout.glPrint(0, 112, 1, "Mode (space): wire"); else Cout.glPrint(0, 112, 1, "Mode (space): fill");
	
	static int Time=195;
	static float Inc=0;
	
	if (FreeFly==true)
	{
	Time++;
	if (Time==200) 
	{
		Inc=sin(rand())/128.f;
		if (rand()<(RAND_MAX/2)) Inc=Inc*(-1);
		Time=0;
	}
	HorizAngle+=Inc;
	if ((Speed/8192.f>-5.f)&&(Speed/8192.f<5.f))	Speed+=Inc*12800;
	}


	glColor3f(1,1,0);
	if (FreeFly==true) Cout.glPrint(0, 128, 1, "Free fly mode (f): On"); 
	else 
	{
		Cout.glPrint(0, 128, 1, "Free fly mode (f): Off");
		Inc=0;
	}
//	Cout.glPrint(0, 192, 1, "Slope %2.2f",SLOPE);
}

void VoxelEngine::Controll()
{
	DI8->UpDate();

	if (DI8->Mouse.isPressed(1)) Speed+=512;
	if (DI8->Mouse.isPressed(0)) Speed-=512;

	//HorizAngle+=(DI8->Mouse.GetDeltaPos(xAxis)/512.0f);
    HorizAngle+=(DI8->Mouse.GetDeltaPos(xAxis)/512.0f);

	// Get Slope Angle in radians
	SLOPE-=DI8->Mouse.GetDeltaPos(yAxis)/1.0f;

	if (DI8->Keyboard.isButton(DIK_HOME)) Zo+=2;
	if (DI8->Keyboard.isButton(DIK_END)) Zo-=2;
	
//	if (DI8->Keyboard.isButton(DIK_Q))  if (ViewScale>8192) ViewScale-=128;
//	if (DI8->Keyboard.isButton(DIK_W))  if (ViewScale<256000) ViewScale+=128;
//	if (DI8->Keyboard.isButton(DIK_A))  if (HeightPrjScale>32) HeightPrjScale-=4;
//	if (DI8->Keyboard.isButton(DIK_S))  if (HeightPrjScale<1024) HeightPrjScale+=4;

	if (DI8->Keyboard.isButton(DIK_Z))  
		if (EyeShot>0) 
		{		
				EyeShot-=4;
				glFogf(GL_FOG_START, EyeShot-EyeShot/3);
				glFogf(GL_FOG_END, EyeShot);
			}

	if (DI8->Keyboard.isButton(DIK_X))  
		if (EyeShot<4096) 
		{
			EyeShot+=4;
			glFogf(GL_FOG_START, EyeShot-EyeShot/3);
			glFogf(GL_FOG_END, EyeShot);
		}
	
	if (DI8->Keyboard.isButton(DIK_1)) 
	{
		PixelSkip=4;
		zsVox=1;
	}

	if (DI8->Keyboard.isButton(DIK_2)) 
	{
		PixelSkip=8;
		zsVox=1;
	}

	if (DI8->Keyboard.isButton(DIK_3)) 
	{
		PixelSkip=16;
		zsVox=3;
	}

	if (DI8->Keyboard.isButton(DIK_4)) 
	{
		PixelSkip=32;
		zsVox=3;
	}

	if (DI8->Keyboard.isButton(DIK_5)) 
	{
		PixelSkip=64;
		zsVox=5;
	}

	if (DI8->Keyboard.isButton(DIK_SPACE))  Wireframe=!Wireframe;
	if (DI8->Keyboard.isButton(DIK_F))  FreeFly=!FreeFly;		
	if (DI8->Keyboard.isButton(DIK_TAB))  Speed=0;

}
