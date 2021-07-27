
#include <iostream>
#include "logconsole.h"

LogConsole::LogConsole(LogFlags flags)
: Log(flags)
{
}

LogConsole::~LogConsole()
{
}

void LogConsole::dbg(const std::string& msg)
{
	if (showdebug)
		std::cout << "Debug : " << msg << std::endl;
}

void LogConsole::msg(const std::string& msg)
{
	if (showmessages)
		std::cout << "Message : " << msg << std::endl;
}

void LogConsole::err(const std::string& msg)
{
	if (showerrors)
		std::cerr << "Error : " << msg << std::endl;
}

