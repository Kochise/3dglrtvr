#include "Error.hpp"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

namespace swShader
{
	Error::Error(const char *format, ...)
	{
		string[0] = '\0';

		va_list argList;
		va_start(argList, format);
		_vsnprintf(string, 255, format, argList);
		va_end(argList);
	}

	const char *Error::getString() const
	{
		if(string[0] == '\0')
		{
			return "<Unknown>";
		}
		else
		{
			return string;
		}
	}

	Error &Error::operator<<(const Error &error)
	{
		_snprintf(string, 255, "%s\n%s", string, error.getString());

		return *this;
	}

	Error &Error::operator>>(const Error &error)
	{
		_snprintf(string, 255, "%s\n%s", error.getString(), string);

		return *this;
	}
}
