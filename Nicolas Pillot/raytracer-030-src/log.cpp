#include "log.h"

Log::Log(LogFlags flags)
: showmessages ( (flags & LOG_MSG) == LOG_MSG ),
  showdebug    ( (flags & LOG_DBG) == LOG_DBG ),
  showerrors   ( (flags & LOG_ERR) == LOG_ERR )
{
}

Log::~Log()
{
}
