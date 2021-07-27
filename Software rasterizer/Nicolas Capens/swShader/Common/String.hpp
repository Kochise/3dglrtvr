#ifndef String_hpp
#define String_hpp

namespace swShader
{
	class String
	{
	public:
		String();
		String(const String &string);
		String(const char *text);

		~String();

		String &operator<<(String &(__cdecl*)(String&));
		String &operator<<(const char*);
		String &operator<<(const unsigned char*);
		String &operator<<(const signed char*);
		String &operator<<(const char);
		String &operator<<(const unsigned char);
		String &operator<<(const signed char);
      	String &operator<<(const short);
      	String &operator<<(const unsigned short);
      	String &operator<<(const int);
      	String &operator<<(const unsigned int);
     	String &operator<<(const float);

		void operator=(const String &string);
		
		String operator+(const String &string) const;
		String operator+(const char *text) const;
		String operator+(char c) const;
		String &operator+=(const String &string);
		String &operator+=(const char *text);
		String &operator+=(char c);
		bool operator==(const String &string) const;
		bool operator==(const char *text) const;
		bool operator!=(const String &string) const;
		bool operator!=(const char *text) const;
		char &operator[](int index);
		const char &operator[](int index) const;
		char *operator*();
		const char *operator*() const;
		operator char*();
		operator const char*() const;

		int	getLength() const;
		void printText(const char *string, ...);

	private:
		static int String::inlStrlen(const char *text);
		static void String::proStrcpy(char *dest, const String &string, const char *s2, int length2);

		char *data;
		int	length;
	};
}

#include "Error.hpp"

namespace swShader
{
	inline String &String::operator<<(const char *c)
	{
		if(!c)
		{
			throw 0;
		}

		return *this += (const char*)c;
	}

	inline String &String::operator<<(const unsigned char c)
	{
		char t[2] = {c, 0};
		return *this += (const char*)t;
	}

	inline String &String::operator<<(String&(__cdecl *_f)(String&))
	{ 
		return _f(*this); 
	}

	inline String &String::operator<<(const char _c)
	{ 
		return *this << (const unsigned char)_c; 
	}

	inline String &String::operator<<(const signed char _c)
	{ 
		return *this << (const unsigned char)_c; 
	}

	inline String &String::operator<<(const unsigned char *_s)
	{ 
		return *this << (const char *)_s; 
	}

	inline String &String::operator<<(const signed char *_s)
	{ 
		return *this << (const char *)_s; 
	}

	inline char &String::operator[](int index)
	{
		if(!(index >= 0 && index < length))
		{
			throw Error("Index out of bounds");
		}

		return data[index];
	}

	inline const char &String::operator[](int index) const
	{
		if(!(index >= 0 && index < length))
		{
			throw Error("Index out of bounds");
		}

		return data[index];
	}

	inline char *String::operator*()
	{
		return data;
	}

	inline const char *String::operator*() const
	{
		return data;
	}

	inline String::operator char*()
	{ 
		return data; 
	}

	inline String::operator const char*() const
	{ 
		return data; 
	}

	inline int String::getLength() const
	{
		return length;
	}
}


#endif   // String_hpp