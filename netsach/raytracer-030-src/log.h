#ifndef LOG_H
#define LOG_H

#include <string>

class Log
{
	public:
		bool showmessages;
		bool showdebug;
		bool showerrors;

		enum LogFlags
		{
			LOG_NONE= 0,
			LOG_MSG = 1,
			LOG_ERR = 2,
			LOG_DBG = 4,
			LOG_STD = LOG_ERR | LOG_MSG,
			LOG_ALL = LOG_ERR | LOG_MSG | LOG_DBG
		};
		Log(LogFlags flags = LOG_STD);
		virtual ~Log();

		virtual void dbg(const std::string& msg) = 0;
		virtual void msg(const std::string& msg) = 0;
		virtual void err(const std::string& msg) = 0;
};

#endif

