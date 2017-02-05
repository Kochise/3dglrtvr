#ifndef LOGCONSOLE_H
#define LOGCONSOLE_H

#include "log.h"

class LogConsole : public Log
{
	public:
		LogConsole(LogFlags flags = LOG_STD);
		virtual ~LogConsole();

	public:
		virtual void dbg(const std::string& msg);
		virtual void msg(const std::string& msg);
		virtual void err(const std::string& msg);
};

#endif

