#include "Font.H"

Font::Font()
{
	xHeight=GetSystemMetrics(SM_CYSCREEN);
	xWidth=GetSystemMetrics(SM_CXSCREEN);
}

Font::~Font()
{
	glDeleteLists(FontBase,	256);				// delete display lists
	Log.Write("Black","Font remove: Successfully");
}

void Font::Init(int xScreen, int yScreen)
{
	xWidth = xScreen;
	xHeight = yScreen;
}

bool Font::BuildFont()
{
	float	Cx;
	float	Cy;
	GLuint	Loop;

	Tga Image;
	Image.Load("Font.Tga");

	// create textures
	glGenTextures(1, &FontTexture);
	glBindTexture(GL_TEXTURE_2D,  FontTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, Image.Components, Image.Width, Image.Height,0, Image.Format, GL_UNSIGNED_BYTE,  Image.cMap);
	// set texture params
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	FontBase=glGenLists(256);
	glBindTexture(GL_TEXTURE_2D, FontTexture);
	
	glPushMatrix();

	for (Loop=0; Loop<256; Loop++)
	{
		Cx=float(Loop%16)/16.0f;
		Cy=float(Loop/16)/16.0f;

		glNewList(FontBase+Loop,GL_COMPILE);
			glBegin(GL_QUADS);
				glTexCoord2f(Cx,1-Cy-0.0625f);
				glVertex2i(0,0);
				glTexCoord2f(Cx+0.0625f,1-Cy-0.0625f);
				glVertex2i(16,0);
				glTexCoord2f(Cx+0.0625f,1-Cy);
				glVertex2i(16,16);
				glTexCoord2f(Cx,1-Cy);
				glVertex2i(0,16);
			glEnd();
			
			glTranslated(12,0,0);
		glEndList();
	}
	glPopMatrix();
	
	Log.Write("Black","Bitmap font initialization: Successfully");
	return true;
}

void Font::glPrint(GLint Xp, GLint Yp,char Set, const char *String, ...)
{
	char	Text[256];
	va_list	aPrint;									

	if (String == NULL)	return;

	va_start(aPrint, String);
	    vsprintf(Text, String, aPrint);			
	va_end(aPrint);
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBindTexture(GL_TEXTURE_2D, FontTexture);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, xWidth, 0 ,xHeight, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(Xp, xHeight-Yp,0);
	glListBase(FontBase-32+128*Set);
	glCallLists(strlen(Text), GL_BYTE, Text);	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glPopAttrib();
}