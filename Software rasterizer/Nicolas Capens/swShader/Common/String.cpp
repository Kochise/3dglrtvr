#include "String.hpp"

#include <stdio.h>
#include <memory.h>
#include <stdarg.h>
#include <string.h>

namespace swShader
{
	inline int String::inlStrlen(const char *text)
	{
		const char *s;

		for(s = text; *s; s++);

		return (int)s -(int)text;
	}

	inline void String::proStrcpy(char *dest, const String &string, const char *s2, int length2)
	{
		const char *s1 = string;
		int length1 = string.getLength();

		int i;
		for(i = 0; i < length1; i++)
			*dest++ = *s1++;

		for(i = 0; i < length2; i++)
			*dest++ = *s2++;

		*dest = 0;
	}

	String::String()
	{
		data = new char[1];

		if(!data)
		{
			throw 0;
		}

		data[0] = 0;
		length = 0;
	}

	String::String(const String &str)
	{
		data = strdup(str);
		length = str.length;
	}

	String::String(const char *text)
	{
		if(!text)
		{
			data = new char[1];
			data[0] = 0;
			length = 0;
			return;
		}

		int newLength = inlStrlen(text);

		data = new char[newLength + 1];
		length = newLength;

		memcpy(data, text, newLength + 1);
	}

	String::~String()
	{
		delete[] data;
	}

	String &String::operator<<(const short s)
	{
		char n[8];
		sprintf(n, "%d", (int)s);
		return *this += (char*)n;
	}

	String &String::operator<<(const unsigned short s)
	{
		char n[8];
		sprintf(n, "%u", (unsigned int)s);
		return *this += (char*)n;
	}

	String &String::operator<<(const int i)
	{
		char n[16];
		sprintf(n, "%d", (int)i);
		return *this += (char*)n;
	}

	String &String::operator<<(const unsigned int i)
	{
		char n[16];
		sprintf(n, "%u", (unsigned int)i);
		return *this += (char*)n;
	}

	String &String::operator<<(const float f)
	{
		printText("%f.3", f);
		return *this;
	}

	void String::operator=(const String &str)
	{
		if(length != str.length)
		{
			delete[] data;
			data = new char[str.length + 1];
			length = str.length;
		}

		memcpy( data, str.data, str.length + 1);
	}

	String String::operator+(const String &string) const
	{
		int newLength = length + string.length;
		char *newData = new char[newLength + 1];

		proStrcpy(newData, *this, string.data, string.length);

		String s;
		s.length = newLength;
		s.data = newData;

		return s;
	}

	String String::operator+(const char *text) const
	{
		if(text)
		{
			int slen = inlStrlen(text);
			int newLength = length + slen;
			char *newData = new char[newLength + 1];

			proStrcpy(newData, *this, text, slen);

			String string;
			string.data = newData;
			string.length = newLength;

			return string;
		}

		return *this;
	}

	String String::operator+(char c) const
	{
		int newLength = length + 1;
		char *newData = new char[newLength + 1];

		proStrcpy(newData, *this, &c, 1);

		String string;
		string.data = newData;
		string.length = newLength;

		return string;
	}

	String &String::operator+=(const String &string)
	{
		int newLength = length + string.length;
		char *newData = new char[newLength + 1];

		proStrcpy(newData, *this, string.data, string.length);

		delete[] data;
		data = newData;
		length = newLength;

		return *this;
	}

	String &String::operator+=(const char *text)
	{
		if(text)
		{
			int slen = inlStrlen(text);
			int newLength = length + slen;
			char *newData = new char[newLength + 1];

			proStrcpy(newData, *this, text, slen);

			delete[] data;
			data = newData;
			length = newLength;
		}

		return *this;
	}

	String &String::operator+=(char c)
	{
		int newLength = length + 1;
		char *newData = new char[newLength + 1];

		proStrcpy(newData, *this, &c, 1);

		delete[] data;
		data = newData;
		length = newLength;

		return *this;
	}

	bool String::operator==(const String &string) const
	{
		if(length != string.length)
			return false;

		const char *s1 = data;
		const char *s2 = string.data;
		int count = string.length;

		while(count--)
		{
			if(*s1++ != *s2++)
			{
				return false;
			}
		}
		return true;
	}

	bool String::operator==(const char *text) const
	{
		if(text)
		{
			const char *s1 = data;

			while(*s1 == *text++)
			{
				if(*s1++ == 0)
				{
					return true;
				}
			}
		}

		return false;
	}

	bool String::operator!=(const String &string) const
	{
		if(length != string.length)
			return true;

		const char *s1 = data;
		const char *s2 = string.data;
		int count = string.length;

		while(count--)
		{
			if(*s1++ == *s2++)
			{
				return false;
			}
		}

		return true;
	}

	bool String::operator!=(const char *text) const
	{
		if(text)
		{
			const char *s1 = data;
			const char *s2 = text;

			while(*s1 == *s2++)
			{
				if(*s1++ == 0)
				{
					return false;
				}
			}
		}

		return true;
	}

	void String::printText(const char *text, ...)
	{
		if(text != 0)
		{
			throw 0;
		}

		va_list argList;
		char tBuf[256];

		va_start(argList, text) ;
		_vsnprintf(tBuf, 256, text, argList);
		va_end(argList);

		*this +=(char*)tBuf;
	}
}