#ifndef __Screamer
#define __Screamer

#include <StdIo.H>
#include <StdLib.H>
#include <StdArg.H>
#include <String.H>
#include <FStream.H>

#define	OutputFile	"Report.Html"					//
#define Log	Message()								// 

// 
class Screamer
{
private:
	ofstream LogFile;								// 
	char *HTML;
	char *PrevHTML;

	Screamer()										//
	{
	LogFile.open(OutputFile,ios::out);
	LogFile.close();

	HTML = (char*) calloc(256,sizeof(char));
	PrevHTML = (char*) calloc(256,sizeof(char));

	WriteCode("<Title>HTML Report vesion 2.4.1</Title>");
	WriteCode("<Html> <Body BgColor = Cornsilk>");
	WriteCode("<B><Font Size = 3 Color = DarkBlue Face = Verdana> HTML Report Version 2.4.1 </B>");
	WriteCode("<HR>");
	}

public:
	~Screamer()
	{
	WriteCode("</Font> </P> </Body> </Html>");
	free (PrevHTML);
	free (HTML);
	}
	
	void Write(const char* Color, const char *String, ...);
	void WriteBold(const char* Color, const char *String, ...);
	void WriteCode(const char *String);
	
	friend Screamer& Message();						// 
};

inline void Screamer::Write(const char* Color, const char *String, ...)
{
	if (!String) return;
	
	char LogText[1024];
	
	va_list	Print;									// 

	va_start(Print, String);
	    vsprintf(LogText, String, Print);			// 
	va_end(Print);
	
	LogFile.open(OutputFile,ios::app);

	strcat(HTML,"<Font Size = 2 Color = ");			// 
	strcat(HTML,Color);								// 
	strcat(HTML," Face = Verdana>");				// 

	if (strcmp(PrevHTML,HTML)!=0)					// 
	{
		strcpy(PrevHTML,HTML);						// 
		LogFile<<"</Font>"<<endl;					// 
		LogFile<<HTML<<endl;						// 
		*HTML=NULL;									// 
	}

	LogFile<<"<BR>"<<LogText<<endl;					// 
	LogFile.close();
}

inline void Screamer::WriteBold(const char* Color, const char *String, ...)
{
	if (!String) return;
	
	char LogText[1024];
	
	va_list	Print;									//

	va_start(Print, String);
	    vsprintf(LogText, String, Print);			//
	va_end(Print);
	
	WriteCode("<B>");
	Write(Color,String);
	WriteCode("</B>");
}


inline void Screamer::WriteCode(const char *String)
{
	LogFile.open(OutputFile,ios::app);
	LogFile<<String<<endl;
	LogFile.close();
}

inline Screamer& Message()
{
	static Screamer SingleObject;					//
	return SingleObject;
}

#endif