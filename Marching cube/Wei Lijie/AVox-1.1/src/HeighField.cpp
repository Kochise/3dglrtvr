#include "HeightField.H"

// restricted within 0..255
//
#define Clamp(x) ((x)<0 ? 0 : ((x)>255 ? 255 : (x)))
//
// mod 255
//
#define L(x) ((x)&0xff)

HeightField::HeightField()
{

	if ((HMap = (unsigned char*) malloc(square(MapSize)*2)) == NULL)
    {
    Log.Write("Red","Error: not enough memory");
    }
}

HeightField::~HeightField()
{
	free (HMap);
}

bool HeightField::LoadMap(const char *FileName, unsigned char *pHeightMap)
{
	FILE *File = NULL;	
	File = fopen( FileName, "rb" );
	if ( File == NULL )	
	{
		Log.Write("Red","Error: can't find file: %s",FileName);
		return false;
	}
	
	fread(pHeightMap, 1, MapSize*MapSize, File);
	
	int Result = ferror(File);
	if (Result)
	{
		Log.Write("Red", "Error: failed to get data from file: %s", FileName);
		return false;
	}
	fclose(File); 
	
	Log.Write("Black","HeigtMap file %s loading: Done",FileName);

	return true;
}
