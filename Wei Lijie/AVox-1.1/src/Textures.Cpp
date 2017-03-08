#include <FStream.H>
#include "Textures.H"
#include "Log.H"

Tga::Tga()
{

}

Tga::~Tga()
{
	if (cMap!=NULL) free(cMap);
}


bool Tga::Load(const char *FileName)
{
	#define TGA_TYPE_COLOR 2
	#define TGA_TYPE_GRAY 3

	GLubyte GrayMask[12] = {0, 0, TGA_TYPE_GRAY, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	GLubyte RGBMask[12]={0, 0, TGA_TYPE_COLOR, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	// Headers
	GLubyte Header[18];

	FILE *File = fopen(FileName, "rb");
	
    if (File == NULL) 
	{
		return false;
	}

	// Read from file
	if (fseek(File, 0, SEEK_SET) ||fread(Header, sizeof(Header), 1, File)!=true)
	{
		Log.Write("Red","Error reading header");
		return 0;
	} 
	
	if (memcmp(RGBMask, Header, sizeof(RGBMask))&&memcmp(GrayMask, Header, sizeof(GrayMask))!=0)
	{
		Log.Write("Red", "File: %s : Unrecognized image type %d", FileName, Header[2]);
		return 0;
	}

	BPP= Header[16];

	switch(Header[2])
	{
		case TGA_TYPE_GRAY:
			Format=GL_LUMINANCE;
			Components=1;
		break;

		case TGA_TYPE_COLOR:
			if (BPP==32)
			{
			Format=GL_RGBA;
			Components=4;
			}	
			else
			{
			Format=GL_RGB;
			Components=3;
			}
		break;
	}

	// Check format
	 if ((Format == GL_RGBA&& BPP!= 32) ||(Format== GL_RGB&& BPP!= 24) ||(Format == GL_LUMINANCE&& BPP!= 8)) 
	{
		Log.Write("Red", "File: %s : Wrong color depth", FileName);	
		return 0;
	}

	BPP>>=3;
 	Width = (Header[13]<< 8) |Header[12];
	Height = (Header[15]<< 8) |Header[14];

	
	cMap = (unsigned char*) malloc(Width * Height * BPP);

	if(fread(cMap, BPP, Width*Height,  File)!=size_t(Width*Height))
	{
		return 0;
	}
	
	if(BPP>=3)

	for (GLuint Index=0; Index<Width*Height*BPP; Index+=BPP)
	{
		GLubyte Temp=cMap[Index];
		cMap[Index]=cMap[Index + 2];
		cMap[Index+2]= Temp;
	}

	Log.Write("Green","File: %s (Width: %i, Height: %i, BPP: %i)", FileName, Width, Height, BPP);

	return  1;
}