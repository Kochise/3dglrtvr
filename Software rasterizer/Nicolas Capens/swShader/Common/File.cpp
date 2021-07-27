#include "File.hpp"

#include "String.hpp"

#include <stdlib.h>
#include <io.h>
#include <memory.h>
#include <string.h>

namespace swShader
{
	File::File()
	{
		fileHandle = 0;
	}

	File::File(const char *fileName)
	{
		open(fileName);
	}

	File::~File()
	{
		close();
	}

	void File::create()
	{
		// TODO
	}

	void File::rename()
	{
		// TODO
	}

	void File::open(const char *fileName)
	{
		for(unsigned int i = 0; i < 512; i++)
		{
			fullPath[i] = '\0';
		}

		_fullpath(fullPath, fileName, 512);

		fileHandle = fopen(fullPath, "r+b");

		if(!fileHandle)
		{
			throw Error("Could not open file '%s'", fileName);
		}
	}

	void File::close()
	{
		if(fileHandle)
		{
			fclose(fileHandle);
		}

		fileHandle = 0;
	}

	bool File::changeSize(int Size)
	{
		return false;   // TODO
	}

	int File::length() const
	{
		return filelength(fileno(fileHandle));
	}

	int File::position() const
	{
		return ftell(fileHandle);
	}

	bool File::seekSet(int Offset)
	{
		return fseek(fileHandle, Offset, SEEK_SET) == 0;
	}

	bool File::seekCur(int Offset)
	{
		return fseek(fileHandle, Offset, SEEK_CUR) == 0;
	}

	bool File::seekEnd(int Offset)
	{
		return fseek(fileHandle, Offset, SEEK_END) == 0;
	}

	bool File::endOfFile() const
	{
		return feof(fileHandle) != 0;
	}

	int File::read(void *buffer, int size, int count) const
	{
		return fread(buffer, size, count, fileHandle);
	}

	char File::parseChar() const
	{
		return fgetc(fileHandle);
	}

	short File::parseShort() const
	{
		// TODO

		return 0;
	}

	int File::parseInt() const
	{
		int Int;

		fscanf(fileHandle, "%d", &Int);

		return Int;
	}

	float File::parseFloat() const
	{
		float Float;

		fscanf(fileHandle, "%f", &Float);

		return Float;
	}

	double File::parseDouble() const
	{
		double Double;

		fscanf(fileHandle, "%f", &Double);

		return Double;
	}

	unsigned int File::readString(char *stringBuffer) const
	{
		unsigned int length = 0;

		while((*stringBuffer++ = parseChar()) != '\0')
			length++;

		return length;
	}

	unsigned char File::readByte() const
	{
		unsigned char c;

		read(&c, sizeof(c), 1);

		return c;
	}

	char File::readChar() const
	{
		char c;

		read(&c, sizeof(c), 1);

		return c;
	}

	unsigned short File::readWord() const
	{
		unsigned short s;

		read(&s, sizeof(s), 1);

		return s;
	}

	unsigned int File::readDWord() const
	{
		unsigned int i;

		read(&i, sizeof(i), 1);

		return i;
	}

	float File::readFloat() const
	{
		float f;

		read(&f, sizeof(f), 1);

		return f;
	}

	double File::readDouble() const
	{
		double d;

		read(&d, sizeof(d), 1);

		return d;
	}

	bool File::scan(const char *sequence)
	{
		unsigned int i = 0;

		int originalPosition = position();

		while(!endOfFile())
		{
			char c = parseChar();

			if(c != sequence[i])
				i = 0;
			else
				i++;

			if(sequence[i] == '\0')
				return true;	
		}

		seekSet(originalPosition);

		return false;
	}

	String File::drive() const
	{
		return String();   // TODO
	}

	String File::path() const
	{
		return String();   // TODO
	}

	String File::directory() const
	{
		return String();   // TODO
	}

	String File::fileName() const
	{
		int i = 511;

		while(fullPath[i] != '\\')
			i -= 1;

		return String(&fullPath[i + 1]);
	}

	String File::extension() const
	{
		int i = 511;

		while(fullPath[i] != '.')
			i -= 1;

		return String(&fullPath[i + 1]);
	}

	bool File::hasExtension(const char *ext) const
	{
		int i = 511;

		while(fullPath[i] != '.')
			i -= 1;

		return stricmp(&fullPath[i + 1], ext) == 0;
	}

	String File::getFullPath() const
	{
		return fullPath;
	}

	void *File::getBuffer() const
	{
		byte *buffer = 0;
   
		int size = length();

		if(!size)
		{
			return 0;
		}
		
		if(!(buffer = new byte[size]))
		{
			return 0;
		}
 
		if(size != read(buffer, 1, size))
		{
			return 0;
		}

		return buffer;
	}

	char *File::getString() const
	{
		char *string = 0;
   
		int size = length();

		if(!size)
		{
			return 0;
		}
		
		if(!(string = new char[size + 1]))
		{
			return 0;
		}
 
		if(size != read(string, 1, size))
		{
			return 0;
		}

		string[size] = '\0';

		return string;
	}

	bool File::exists(const char *fileName)
	{
		if(fileName)
		{
			FILE *file;

			if(file = fopen(fileName, "r"))
			{
				fclose(file);

				return true;
			}
		}

		return false;
	}
}